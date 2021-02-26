/**********************************************************************************/
/*                                                                                */
/*                              Copyright (c) 2021                                */
/*                           Rosetta H&S International                            */
/*                                                                                */
/*  Permission is hereby granted, free of charge, to any person obtaining a copy  */
/*  of this software and associated documentation files (the "Software"), to deal */
/*  in the Software without restriction, including without limitation the right   */
/*    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell   */
/*       copies of the Software, and to permit persons to whom the Software is    */
/*          furnished to do so, subject to the following conditions:              */
/*                                                                                */
/* The above copyright notice and this permission notice shall be included in all */
/*                copies or substantial portions of the Software.                 */
/*                                                                                */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE.                                                                      */
/*                                                                                */
/**********************************************************************************/

#define ORION_INTERNAL

#include <stdio.h>
#include <stdlib.h>
#include "../include/OSL/OString.hpp"
#include "../include/OSL/OFile.hpp"

#define _CONV(x) ((FILE*)x)

namespace Orion{
/* Internal */
	static char* concat(const char* directory, const char* file){
		char* dir=realpath(directory,0);
		if(dir){
			size_t l1=OStringLength(dir);
			size_t l2=OStringLength(file);

			char* path=(char*)malloc(sizeof(char)*(l1+l2)+2); /* You wouldn't know it, but this line caused days of headache... */
			/*
			 * Basically depending on how you inputted "directory", it would allocate less memory than it actually needed,
			 * so that later on in open(const char*, const char*) when it would call free() on the returned "path",
			 * it would fucking crash and give absolutely no hint at what was wrong. Apparently all I needed to do
			 * was change a "1" to a "2"... you gotta be fucking kidding me.
			 */
			sprintf(path,"%s/%s",dir,file);
			free(dir);
			return path;
		}else{ return 0; }
	}

	/* FIXME: This is incredibly intensive, and fucking stupid, so if you can find a better solution, please replace this. */
	static OFileType getTypeFromExtension(char* ext){
		/*
		 * This pains me to do but I can't really think of an easier solution that works as intended,
		 * so we're going to have to do this the YandereDev way... please forgive me for this.
		 */
	/* Initial fallback */
		if(!ext){ return OFT_UNKNOWN; }
	/* Text files */
		if     ( OStringFindFirst("txt text md",ext)!=OSTRING_NOTFOUND )
		{ return OFT_TEXT; }
	/* Config files */
		else if( OStringFindFirst("cfg conf json ini",ext)!=OSTRING_NOTFOUND )
		{ return OFT_CONFIG; }
	/* Image files */
		else if( OStringFindFirst("png jpg jpeg svg bmp xbm",ext)!=OSTRING_NOTFOUND )
		{ return OFT_IMAGE; }
	/* Video files */
		else if( OStringFindFirst("mpeg mp4 webm mkv mov avi",ext)!=OSTRING_NOTFOUND )
		{ return OFT_VIDEO; }
	/* Font files */
		else if( OStringFindFirst("ttf otf",ext)!=OSTRING_NOTFOUND )
		{ return OFT_FONT; }
	/* Binary files */
		else if( OStringFindFirst("out oapp bin exe",ext)!=OSTRING_NOTFOUND )
		{ return OFT_BINARY; }
	/* Fallback */
		else
		{return OFT_UNKNOWN; }
	}

/* Initialisation */

	OFile::~OFile(void){ close(); }

	OFile::OFile(void) : 
		action{OFILE_OPEN},
		FILEINF{0,0,0,0},misc{0,0,1},contents{0,0,0},
		type{OFT_ERROR}
		{}

	OFile::OFile(const char* file, OFileAction _action) : 
		action{OFILE_OPEN},
		FILEINF{0,0,0,0},misc{0,0,1},contents{0,0,0},
		type{OFT_ERROR}
		{ open(file,_action); }

	OFile::OFile(const char* directory, const char* file, OFileAction _action) : 
		action{OFILE_OPEN},
		FILEINF{0,0,0,0},misc{0,0,1},contents{0,0,0},
		type{OFT_ERROR}
		{ open(directory,file,_action); }

	void OFile::init(void){
	/* Initialise file content characteristics */
		contents.lineCount=1; /* All files have at least one line, although this makes iteration confusing. */
		contents.charCount=0;
		contents.lines=0;
		OFileHash tmpHash=0;
		int c;
		/*
		 * Sloppily generate a simplistic hash from the sum of the square of characters of this File in numeric form.
		 * It is quite sloppy, but it's really fast and only used for internal comparison between Files.
		 */
		while( (c=fgetc(_CONV(FILEINF.RAW)))!=EOF ){
			tmpHash+=(c*c);
			if((char)c=='\n'){ contents.lineCount++; }else{ contents.charCount++; }
		}
		FILEINF.HASH=tmpHash/2;

	/* If this File cares about misc data, allocate and initalise them. */
		if(misc.careAboutMisc){
		/* Store some data regarding the path, and keep some variables for use later in this scope. */
			size_t pathl=OStringLength(FILEINF.PATH);
			size_t optPos;
			size_t optl;
		/* Store the file's raw extension. */
			optPos=OStringFindLast(FILEINF.PATH,".")+1;
			optl=(pathl-optPos);
			if((optPos-1)!=OSTRING_NOTFOUND){
				if(optPos==OStringFindFirst(FILEINF.PATH,".")){ misc.ext=0;} /* Prevents weird issues with hidden files */
				else{
					misc.ext=(char*)malloc(sizeof(char)*(optl+1));
					for(size_t i=optPos;i<pathl;i++){ misc.ext[i-optPos]=FILEINF.PATH[i]; }
					misc.ext[optl]=0;
				}
			}else{ misc.ext=0; }
		/* Store the filename (with extension) */
			optPos=OStringFindLast(FILEINF.PATH,"/")+1;
			optl=(pathl-optPos);
			if((optPos-1)!=OSTRING_NOTFOUND){
				misc.name=(char*)malloc(sizeof(char)*(optl+1));
					for(size_t i=optPos;i<pathl;i++){ misc.name[i-optPos]=FILEINF.PATH[i]; }
					misc.name[optl]=0;
			}else{
				/*
				 * We still have to allocate a new block of memory for "name" even if "name" is the same as "path"
				 * because close() will free them both separately, and if "name" points to "path" this will cause a segfault.
				 */
				 misc.name=(char*)malloc(sizeof(char)*(pathl+1));
				 for(size_t i=0;i<pathl;i++){ misc.name[i]=FILEINF.PATH[i]; }
				 misc.name[pathl]=0;
			}
		/* Store the type of the file */
			type=getTypeFromExtension(misc.ext);
		}else{ misc.ext=0; misc.name=0; type=OFT_UNKNOWN; }
	}


	bool OFile::open(const char* file, OFileAction _action){
		if(!file){ OLog("ORIONAPI | WARNING! CANNOT PASS NULL WHEN OPENING A FILE!\n"); return false; }
		if(FILEINF.RAW){ close(); }
		action=_action;
		switch(action){
			case OFILE_OPEN:              { FILEINF.RAW=fopen(file,"r+"); break; }
			case OFILE_OPEN_READONLY:     { FILEINF.RAW=fopen(file,"r");  break; }
			case OFILE_NEW:               { FILEINF.RAW=fopen(file,"w+"); break; }
			case OFILE_NEW_WRITEONLY:     { FILEINF.RAW=fopen(file,"w");  break; }
		}

		if(FILEINF.RAW){
			FILEINF.DESC=fileno( _CONV(FILEINF.RAW) );
			FILEINF.PATH=realpath(file,0);
			init();
			return true;
		}else{ 
			FILEINF.PATH=0;
			FILEINF.RAW=0;
			FILEINF.DESC=0;
			FILEINF.HASH=0;
			misc.name=0;
			misc.ext=0;
			contents.lineCount=0;
			contents.charCount=0;
			contents.lines=0;
			type=OFT_ERROR;
			return false;
		}
	}

	bool OFile::open(const char* directory, const char* file, OFileAction _action){
		if(!directory || !file){ OLog("ORIONAPI | WARNING! CANNOT PASS NULL WHEN OPENING A FILE!\n"); return false; }
		char* dir=concat(directory,file);
		bool result=open(dir,_action);
		if(dir){ free(dir); }
		return result;
	}

	bool OFile::close(void){
		if(!FILEINF.RAW){ return false; }
		if( fclose( _CONV(FILEINF.RAW) ) ){ /* Oy Vey! */
			if(FILEINF.PATH){ free(FILEINF.PATH); }
			if(misc.name)   { free(misc.name); }
			if(misc.ext)    { free(misc.ext); }
			if(contents.lines){
				for(size_t i=0;i<contents.lineCount;i++){ free(contents.lines[i]); }
				free(contents.lines);
			}
			FILEINF.PATH=0;
			FILEINF.RAW=0;
			FILEINF.DESC=0;
			FILEINF.HASH=0;
			misc.name=0;
			misc.ext=0;
			contents.lineCount=0;
			contents.charCount=0;
			contents.lines=0;
			type=OFT_ERROR;
			return true;
		}else{ return false; }
	}

	void OFile::shouldInitMisc(bool v){ misc.careAboutMisc=v; }

/* File modifcation */

	/* TODO: Add abstracted functionality*/

/* Getters/misc ops */

	bool OFile::valid(void) const{ return ( FILEINF.RAW ? true : false ); }
	OFile::operator bool(void) const{ return (FILEINF.RAW ? true : false); }

	bool OFile::equalTo(OFile& other) const { return (other.getHash() == FILEINF.HASH); }
	bool OFile::operator==(OFile& other) const { return (other.getHash() == FILEINF.HASH); }

	const char* OFile::getFullPath(void) const { return (const char*) FILEINF.PATH; }
	const char* OFile::getName(void) const { return (const char*)misc.name; }
	const char* OFile::getExtension(void) const { return (const char*)misc.ext; }
	void* OFile::getCFile(void) const { return FILEINF.RAW; }
	OFileHash OFile::getHash(void) const { return FILEINF.HASH; }
	size_t OFile::getLineCount(void) const { return contents.lineCount; }
	size_t OFile::getCharCount(void) const { return contents.charCount; }
	const char** OFile::getLines(void) const { return (const char**)contents.lines; }

/* Generic */

	bool OFileExists(const char* file){
		FILE* f=fopen(file,"r");
		if(f){ fclose(f); return true; }
		else{ return false; }
	}

	bool OFileExists(const char* directory, const char* file){
		char* path=concat(directory,file);
		if(path){
			bool result=OFileExists(path);
			free(path);
			return result;
		}else{ return false; }
	}

}