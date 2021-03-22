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
#include <unistd.h>
#include "../include/OSL/OString.hpp"
#include "../include/OSL/OFile.hpp"

#define TOFILE(x) ((FILE*)x)

namespace Orion{
/*** Internal ***/
	static char* concat(const char* directory, const char* file){
		char* dir=realpath(directory,0);
		if(dir){
			size_t l1=OStringLength(dir);
			size_t l2=OStringLength(file);

			char* path=(char*)malloc(sizeof(char)*(l1+l2)+2);
			sprintf(path,"%s/%s",dir,file);
			free(dir);
			return path;
		}else{ return 0; }
	}
	/* FIXME: Maybe replace this with one that examines File signatures instead of the extensions? */
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

/*** File class definitions ***/
	/** Initialisation **/
	void OFile::init(bool skipEval){
	/* Since this can be called through some other methods, we need to be able to skip this intensive stuff if it's already been done. */
		if(!skipEval){
		/* Initialise file content characteristics */
			contents.lineCount=1; /* All files have at least one line, although this makes iteration confusing. */
			contents.charCount=0;
			contents.lines=0;
			contents.modified=0;
			OFileHash tmpHash=0;
			int c;
		/*
		 * Sloppily generate a simplistic hash from the sum of the square of bytes of this File in numeric form.
		 * It is quite sloppy, but it's really fast and only used for internal comparison between Files.
		 */
			while( (c=fgetc(TOFILE(CFILE.RAW)))!=EOF ){
				tmpHash+=((char)c*(char)c);
				if((char)c=='\n'){ contents.lineCount++; }else{ contents.charCount++; }
			}
			hash=tmpHash/2;
			size=contents.charCount+(contents.lineCount-1);
			rewind(TOFILE(CFILE.RAW));
		}
	/* If this File cares about misc data, allocate and initalise them. */
		if(flags.storeMisc){
		/* Store some data regarding the path, and keep some variables for use later in this scope. */
			size_t pathl=OStringLength(path);
			size_t optPos;
			size_t optl;
		/* Store the file's raw extension. */
			optPos=OStringFindLast(path,".")+1;
			optl=(pathl-optPos);
			if((optPos-1)!=OSTRING_NOTFOUND){
				if(optPos==OStringFindFirst(path,".")){ ext=0;} /* Prevents weird issues with hidden files */
				else{
					if(ext){ free(ext); ext=0; }
					else{ ext=(char*)malloc(sizeof(char)*(optl+1)); }
					for(size_t i=optPos;i<pathl;i++){ ext[i-optPos]=path[i]; }
					ext[optl]=0;
				}
			}else{ ext=0; }
		/* Store the filename (with extension) */
			optPos=OStringFindLast(path,"/")+1;
			optl=(pathl-optPos);
			if((optPos-1)!=OSTRING_NOTFOUND){
				if(name){ free(name); name=0; }
				else{ name=(char*)malloc(sizeof(char)*(optl+1)); }
				for(size_t i=optPos;i<pathl;i++){ name[i-optPos]=path[i]; }
				name[optl]=0;
			}else{
				/*
				 * We still have to allocate a new block of memory for "name" even if "name" is the same as "path"
				 * because close() will free them both separately, and if "name" points to "path" this will cause a segfault.
				 */
				if(name){ free(name); name=0; }
				else{ name=(char*)malloc(sizeof(char)*(pathl+1)); }
				for(size_t i=0;i<pathl;i++){ name[i]=path[i]; }
				name[pathl]=0;
			}
		/* Store the type of the File */
			type=getTypeFromExtension(ext);
		}else{ ext=0; name=0; type=OFT_UNKNOWN; }
	/* If contents have been evaluated and we are allowed to store contents to memory, go ahead and do it. */
		if(flags.storeToMem && flags.evalContents){
		/*
		 * Avoid storing the File's contents to memory by default if it's a heavy File such as an image, binary, or video.
		 * These three specifically due to what I plan to do with RenderKit.
		 */
			if(type!=OFT_BINARY && type!=OFT_IMAGE && type!=OFT_VIDEO){ storeToMem(); }
		}
	}

	/* Store File to memory. */
	bool OFile::storeToMem(void){
		if(CFILE.RAW && contents.lines==0){
		/* Store some variables we'll use later. */
			int c=0;
			size_t l=0,i=0;
		/* First check if the File's Line count has been determined, if not, determine it here. */
			if(!contents.lineCount){
				contents.lineCount=1;
				contents.charCount=0;
				OFileHash tmpHash=0;
				while( (c=fgetc(TOFILE(CFILE.RAW)))!=EOF ){
					tmpHash+=((char)c*(char)c);
					if((char)c=='\n'){ contents.lineCount++; }else{ contents.charCount++; }
				}
				hash=tmpHash/2;
				size=contents.charCount+(contents.lineCount-1);
				rewind(TOFILE(CFILE.RAW));
			}
		/* Allocate an array of strings so we can store our File's Lines to. */
			contents.lines=(OFileLine*)malloc(sizeof(OFileLine)*(contents.lineCount));
			for(size_t i=0;i<contents.lineCount;i++){ contents.lines[i]={0,0}; }
		/* Search and see the size of each line so we can allocate enough memory for each Line string. */
			while( (c=fgetc(TOFILE(CFILE.RAW))) ){
				if(c=='\n' || c==EOF){
					contents.lines[l]={ i, (unsigned char*)malloc(sizeof(unsigned char)*(i+1)) };
					i=0; l++;
					if(c==EOF){ break; }
				}else{  i++; }
			}
			rewind(TOFILE(CFILE.RAW));
		/* Go back and store the Line information. */
			c=0,l=0,i=0;
			while( (c=fgetc(TOFILE(CFILE.RAW))) ){
				if(c=='\n' || c==EOF){
					contents.lines[l].length=i;
					if(contents.lines[l].str){ contents.lines[l].str[i]=0; }
					i=0; l++;
					if(c==EOF){ break; }
				}else{
					if(contents.lines[l].str){
						contents.lines[l].str[i]=c;
						i++;
					}	
				}
			}
		/* Clean up and return. */
			rewind(TOFILE(CFILE.RAW));
			return true;
		}
		return false;
	}
	
	/** Constructors **/
	OFile::OFile(void) :
		type{OFT_ERROR},action{OFILE_AUTO},
		contents{0,0,0,0},hash{0},size{0},
		name{0},path{0},ext{0},CFILE{0,0},flags{1,1,1}
		{}
	OFile::OFile(const char* filename, OFileAction _action) :
		type{OFT_ERROR},action{OFILE_AUTO},
		contents{0,0,0,0},hash{0},size{0},
		name{0},path{0},ext{0},CFILE{0,0},flags{1,1,1}
		{ open(filename,_action); }
	OFile::OFile(const char* directory, const char* filename, OFileAction _action) :
		type{OFT_ERROR},action{OFILE_AUTO},
		contents{0,0,0,0},hash{0},size{0},
		name{0},path{0},ext{0},CFILE{0,0},flags{1,1,1}
		{ open(directory,filename,_action); }

	/** Opening **/
	bool OFile::open(const char* filename, OFileAction _action){
		if(!filename){ return false; }
		if(CFILE.RAW){ close(false); }
		action=_action;
	/* Determine how to open the File. */
		switch(action){
			case OFILE_OPEN:              { CFILE.RAW=fopen(filename,"r+"); break; }
			case OFILE_OPEN_READONLY:     { CFILE.RAW=fopen(filename,"r");  break; }
			case OFILE_NEW:               { CFILE.RAW=fopen(filename,"w"); break; }
			case OFILE_AUTO:{
				if(OFileExists(filename)){
					CFILE.RAW=fopen(filename,"r+");
					if(CFILE.RAW){ action=OFILE_OPEN; }
					else{ CFILE.RAW=fopen(filename,"r"); action=OFILE_OPEN_READONLY; }
				}else{
					CFILE.RAW=fopen(filename,"w");
					action=OFILE_NEW;
				}
			}
		}
	/* Store File information if opened successfully. */
		if(CFILE.RAW){
			CFILE.DESC=fileno( TOFILE(CFILE.RAW) );
			path=realpath(filename,0);
			init(false);
			return true;
		}else{ 
			type=OFT_ERROR;
			contents.lineCount=0;
			contents.charCount=0;
			contents.lines=0;
			contents.modified=false;
			hash=0;
			name=0;
			path=0;
			ext=0;
			CFILE.RAW=0;
			CFILE.DESC=0;
			return false;
		}
	}
	bool OFile::open(const char* directory, const char* filename, OFileAction _action){
		if(!directory || !filename){ return false; }
		char* dir=concat(directory,filename);
		bool result=open(dir,_action);
		if(dir){ free(dir); }
		return result;
	}

	/** Closing **/
	bool OFile::close(bool applyChanges){
		if(!CFILE.RAW){ return false; }
		// if(applyChanges){ save(); }
		(void)applyChanges;
		if( !fclose( TOFILE(CFILE.RAW) ) ){
			if(name) { free(name); }
			if(path) { free(path); }
			if(ext)  { free(ext); }
			if(contents.lines){
				for(size_t i=0;i<contents.lineCount;i++){ if(contents.lines[i].str){ free(contents.lines[i].str); } }
				free(contents.lines);
			}
			type=OFT_ERROR;
			contents.lineCount=0;
			contents.charCount=0;
			contents.lines=0;
			contents.modified=false;
			hash=0;
			name=0;
			path=0;
			ext=0;
			CFILE.RAW=0;
			CFILE.DESC=0;
			return false;
			return true;
		}else{ return false; }
	}

/*** Sub-struct definitions ***/
	OFileLine::operator bool(void) const { return (str ? true : false); }
	char OFileLine::operator [](size_t i) const { return str[i]; }
	OFileLine::operator char*(void) const { return (char*)str; }

	OFileContent::operator bool(void) const { return (lines ? true : false); }
	OFileLine OFileContent::operator [](size_t i) const { return lines[i]; }

/*** Generic File actions ***/
	bool OFileExists(const char* file){ return ( !access(file,F_OK) ? true : false ); }
	bool OFileExists(const char* directory, const char* file){
		char* path=concat(directory,file);
		if(path){
			bool result=OFileExists(path);
			free(path);
			return result;
		}else{ return false; }
	}

	bool OFileDelete(const char* file){ return ( (!remove(file) ) ? true : false ); }
	bool OFileDelete(const char* directory, const char* file){
		char* path=concat(directory,file);
		bool result=OFileDelete(path);
		if(path) { free(path); }
		return result;
	}

	bool OFileRename(const char* file, const char* newName){ return ( (!rename(file,newName)) ? true : false ); }
	bool OFileRename(const char* directory, const char* file, const char* newName){
		char* path=concat(directory,file);
		bool result=OFileRename(path,newName);
		if(path) { free(path); }
		return result;
	}

	OFileHash OFileGetHash(const char* file){
		FILE* f=fopen(file,"r");
		if(f){
			OFileHash r=0;
			int c;
			while( (c=fgetc(TOFILE(f)))!=EOF ){ r+=(c*c); }
			fclose(f);
			return r/2;
		}else{ return 0; }
	}
	OFileHash OFileGetHash(const char* directory, const char* file){
		char* path=concat(directory,file);
		if(path){
			bool result=OFileGetHash(path);
			free(path);
			return result;
		}else{ return 0; }
	}
}