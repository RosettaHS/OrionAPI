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
#include "../include/errdef.hpp"
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
	void OFile::init(bool skipGen){
		/* Since this can be called through some other methods, we need to be ablle to skip this intensive stuff if it's already been done. */
		if(!skipGen){
		/* Initialise file content characteristics */
			contents.lineCount=1; /* All files have at least one line, although this makes iteration confusing. */
			contents.charCount=0;
			contents.lines=0;
			contents.modified=0;
			OFileHash tmpHash=0;
			int c;
			/*
			 * Sloppily generate a simplistic hash from the sum of the square of characters of this File in numeric form.
			 * It is quite sloppy, but it's really fast and only used for internal comparison between Files.
			 */
			while( (c=fgetc(_CONV(FILEINF.RAW)))!=EOF ){
				tmpHash+=((char)c*(char)c);
				if((char)c=='\n'){ contents.lineCount++; }else{ contents.charCount++; }
			}
			FILEINF.HASH=tmpHash/2;
			FILEINF.SIZE=contents.charCount+(contents.lineCount-1);
			rewind(_CONV(FILEINF.RAW));
			if(careAboutMemStore){ storeToMem(); }
		}

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
					if(misc.ext){ free(misc.ext); misc.ext=0; }
					else{ misc.ext=(char*)malloc(sizeof(char)*(optl+1)); }
					for(size_t i=optPos;i<pathl;i++){ misc.ext[i-optPos]=FILEINF.PATH[i]; }
					misc.ext[optl]=0;
				}
			}else{ misc.ext=0; }
		/* Store the filename (with extension) */
			optPos=OStringFindLast(FILEINF.PATH,"/")+1;
			optl=(pathl-optPos);
			if((optPos-1)!=OSTRING_NOTFOUND){
				if(misc.name){ free(misc.name); misc.name=0; }
				else{ misc.name=(char*)malloc(sizeof(char)*(optl+1)); }
				for(size_t i=optPos;i<pathl;i++){ misc.name[i-optPos]=FILEINF.PATH[i]; }
				misc.name[optl]=0;
			}else{
				/*
				 * We still have to allocate a new block of memory for "name" even if "name" is the same as "path"
				 * because close() will free them both separately, and if "name" points to "path" this will cause a segfault.
				 */
				if(misc.name){ free(misc.name); misc.name=0; }
				else{ misc.name=(char*)malloc(sizeof(char)*(pathl+1)); }
				for(size_t i=0;i<pathl;i++){ misc.name[i]=FILEINF.PATH[i]; }
				misc.name[pathl]=0;
			}
		/* Store the type of the file */
			type=getTypeFromExtension(misc.ext);
		}else{ misc.ext=0; misc.name=0; type=OFT_UNKNOWN; }
	}

	/* Storing File to memory. */
	bool OFile::storeToMem(void){
		if(FILEINF.RAW && contents.lines==0){
		/* Allocate an array of strings so we can store our file's lines to. */
			contents.lines=(OFileLine*)malloc(sizeof(OFileLine)*(contents.lineCount));
			for(size_t i=0;i<contents.lineCount;i++){ contents.lines[i]={0,0}; }
		/* Search and see the size of each line so we can allocate enough memory for each line string. */
			int c=0;
			size_t l=0,i=0;
			while( (c=fgetc(_CONV(FILEINF.RAW))) ){
				if(c=='\n' || c==EOF){
					contents.lines[l]={ i, (char*)malloc(sizeof(char)*(i+1)) };
					i=0; l++;
					if(c==EOF){ break; }
				}else{  i++; }
			}
			rewind(_CONV(FILEINF.RAW));
		/* Go back and store the line information. */
			c=0,l=0,i=0;
			while( (c=fgetc(_CONV(FILEINF.RAW))) ){
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
		/* Clean up for the warcrime-level attrocity that was committed the past few lines */
			rewind(_CONV(FILEINF.RAW));
			return true;
		}
		return false;
	}

/* Constructors */
	OFile::~OFile(void){ close(false); }

	OFile::OFile(void) : 
		type{OFT_ERROR},
		action{OFILE_OPEN},
		FILEINF{0,0,0,0,0},misc{0,0,1},contents{0,0,0,0},careAboutMemStore{1}
		{}

	OFile::OFile(const char* file, OFileAction _action) : 
		type{OFT_ERROR},
		action{OFILE_OPEN},
		FILEINF{0,0,0,0,0},misc{0,0,1},contents{0,0,0,0},careAboutMemStore{1}
		{ open(file,_action); }

	OFile::OFile(const char* directory, const char* file, OFileAction _action) : 
		type{OFT_ERROR},
		action{OFILE_OPEN},
		FILEINF{0,0,0,0,0},misc{0,0,1},contents{0,0,0,0},careAboutMemStore{1}
		{ open(directory,file,_action); }

/* Management - Opening */
	bool OFile::open(const char* file, OFileAction _action){
		if(!file){ OLog("ORIONAPI | WARNING! CANNOT PASS NULL WHEN OPENING A FILE!\n"); return false; }
		if(FILEINF.RAW){ close(false); }
		action=_action;
		switch(action){
			case OFILE_OPEN:              { FILEINF.RAW=fopen(file,"r+"); break; }
			case OFILE_OPEN_READONLY:     { FILEINF.RAW=fopen(file,"r");  break; }
			case OFILE_NEW:               { FILEINF.RAW=fopen(file,"w"); break; }
			case OFILE_AUTO:{
				if(OFileExists(file)){
					FILEINF.RAW=fopen(file,"r+");
					if(FILEINF.RAW){ action=OFILE_OPEN; }
					else{ FILEINF.RAW=fopen(file,"r"); action=OFILE_OPEN_READONLY; }
				}else{
					FILEINF.RAW=fopen(file,"w");
					action=OFILE_NEW;
				}
			}
		}

		if(FILEINF.RAW){
			FILEINF.DESC=fileno( _CONV(FILEINF.RAW) );
			FILEINF.PATH=realpath(file,0);
			init(false);
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
			contents.modified=false;
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

/* Management - Closing */
	bool OFile::close(bool applyChanges){
		if(!FILEINF.RAW){ return false; }
		if(applyChanges){ save(); }
		if( fclose( _CONV(FILEINF.RAW) )==0 ){ /* Oy Vey! */
			if(FILEINF.PATH){ free(FILEINF.PATH); }
			if(misc.name)   { free(misc.name); }
			if(misc.ext)    { free(misc.ext); }
			if(contents.lines){
				for(size_t i=0;i<contents.lineCount;i++){ if(contents.lines[i].str){ free(contents.lines[i].str); } }
				free(contents.lines);
			}
			FILEINF.RAW=0;
			FILEINF.PATH=0;
			FILEINF.DESC=0;
			FILEINF.SIZE=0;
			FILEINF.HASH=0;
			misc.name=0;
			misc.ext=0;
			contents.lineCount=0;
			contents.charCount=0;
			contents.lines=0;
			contents.modified=false;
			type=OFT_ERROR;
			return true;
		}else{ return false; }
	}

/* File modifcation */

	/* Applying current fileContent */
	bool OFile::save(void){
		if(action!=OFILE_OPEN && action!=OFILE_NEW){ return false; }
		if(contents.modified){
			FILE* F=fopen(FILEINF.PATH,"w");
			if(F){
				for(size_t i=0;i<contents.lineCount;i++){
					for(size_t j=0;j<contents.lines[i].length;j++){
						fputc(contents.lines[i].str[j],F);
					}
					if(i!=contents.lineCount-1){ fputc('\n',F); }
				}
				fclose(F);
				contents.modified=false;
				return true;
			}
		}
		return false;
	}

	/* Saving File under different name */
	bool OFile::saveAs(const char* file){
		if(!file){ OLog("ORIONAPI | WARNING! CANNOT PASS NULL WHEN SAVING FILE UNDER ANOTHER NAME!\n"); return false; }
		if(FILEINF.RAW){
			FILE* F=fopen(file,"w");
			if(F){
				for(size_t i=0;i<contents.lineCount;i++){
					for(size_t j=0;j<contents.lines[i].length;j++){
						fputc(contents.lines[i].str[j],F);
					}
					if(i!=contents.lineCount-1){ fputc('\n',F); }
				}
				fclose(F);
				return true;
			}
		}
		return false;
	}
	bool OFile::saveAs(const char* directory, const char* file){
		if(!directory || !file){ OLog("ORIONAPI | WARNING! CANNOT PASS NULL WHEN SAVING FILE UNDER ANOTHER NAME!\n"); return false; }
		char* path=concat(directory,file);
		bool result=saveAs(path);
		if(path){ free(path); }
		return result;
	}

	/* Renaming the current File. */
	bool OFile::rename(const char* newName){
		if(FILEINF.RAW){
			fclose(_CONV(FILEINF.RAW));
			FILEINF.RAW=0;
			bool result=false;
			if(OFileRename(FILEINF.PATH,newName)){
				if(FILEINF.PATH){ free(FILEINF.PATH); }
				FILEINF.PATH=realpath(newName,0);
				init(true);
				result=true;
			}
			switch(action){
				case OFILE_OPEN:              { FILEINF.RAW=fopen(FILEINF.PATH,"r+"); break; }
				case OFILE_OPEN_READONLY:     { FILEINF.RAW=fopen(FILEINF.PATH,"r");  break; }
				case OFILE_NEW:               { FILEINF.RAW=fopen(FILEINF.PATH,"w+"); break; }
					/* THIS IS NOT A VALID TYPE! If this is detected, there is something very wrong. */
				case OFILE_AUTO:{
					OLog("ORIONAPI | WARNING! OFILE DETECTED WITH TYPE OFILE_AUTO! THIS IS A HELPER TYPE, IF IT'S ACTUALLY SET TO THIS THEN SOMETHING HAS GONE WRONG! GO GET MARK!\n");
					break;
				}
			}
			return result;
		}
		return false;
	}

	bool OFile::reset(void){
		if(FILEINF.RAW && contents.modified){
			if(contents.lines){
				for(size_t i=0;i<contents.lineCount;i++){ if(contents.lines[i].str){ free(contents.lines[i].str); } }
				free(contents.lines);
			}
			contents.lines=0;
			init(false);
			contents.modified=false;
			return true;
		}
		return false;
	}

	/* Deleting the current File. */
	bool OFile::deleteCurrent(void){
		if(FILEINF.RAW){
			if( !remove(FILEINF.PATH) ){
				if(FILEINF.PATH){ free(FILEINF.PATH); }
				if(misc.name)   { free(misc.name); }
				if(misc.ext)    { free(misc.ext); }
				if(contents.lines){
					for(size_t i=0;i<contents.lineCount;i++){ if(contents.lines[i].str){ free(contents.lines[i].str); } }
					free(contents.lines);
				}
				FILEINF.RAW=0;
				FILEINF.PATH=0;
				FILEINF.DESC=0;
				FILEINF.SIZE=0;
				FILEINF.HASH=0;
				misc.name=0;
				misc.ext=0;
				contents.lineCount=0;
				contents.charCount=0;
				contents.lines=0;
				contents.modified=false;
				type=OFT_ERROR;
				return true;
			}
		}
		return false;
	}

	/* Setting a specific line. */
	bool OFile::setLine(size_t line, const char* newText){
		if(FILEINF.RAW && contents.lines){
			if(action==OFILE_OPEN_READONLY){ return false; }
		/* Checks if the given Line is actually available, and if not, resize the File for it to be available. */
			if(line>=contents.lineCount){
				contents.lines=(OFileLine*)realloc(contents.lines,sizeof(OFileLine)*(line+1));
				if(!contents.lines){
					OLog("ORIONAPI | ERROR! COULD NOT ALLOCATE EXTRA LINES FOR FILE! CURRENT COUNT : %lu | ATTEMPTED ALLOCATION : %lu\n",contents.lineCount,line);
					exit(OERR_CANTMALLOC);
					return false;
				}
				for(size_t i=contents.lineCount;i<line+1;i++){
					contents.lines[i]={ 0,(char*)malloc(sizeof(char)) };
					contents.lines[i].str[0]=0;
				}
				contents.lineCount=(line+1);
			}
		/* Actually sets the Line data */
			OFileLine* lineToSet=&contents.lines[line];
			if(!lineToSet->str){ return false; }
			size_t l1=lineToSet->length;
			size_t l2=OStringLength(newText);
			if(l2>l1){ lineToSet->str=(char*)realloc(lineToSet->str,sizeof(char)*(l2+1)); }
			if(!lineToSet->str){
				OLog("ORIONAPI | ERROR! COULD NOT RESIZE FILE LINE %lu OF LENGTH %lu TO LENGTH %lu!\n",line,l1,l2);
				exit(OERR_CANTMALLOC);
			}
			for(size_t i=0;i<l2;i++){
				lineToSet->str[i]=newText[i];
			}
			lineToSet->length=l2;
			lineToSet->str[l2]=0;
			contents.modified=true;
			return true;
		}
		return false;
	}

/* Getters/misc ops */

	void OFile::shouldInitMisc(bool v) { misc.careAboutMisc=v; }
	void OFile::shouldStoreToMem(bool v){
		careAboutMemStore=v;
		if(careAboutMemStore && FILEINF.RAW && !contents.lines){ storeToMem(); }
	}
	bool OFile::valid(void) const { return ( FILEINF.RAW ? true : false ); }
	bool OFile::hasBeenModified(void) const { return contents.modified; }
	OFile::operator bool(void) const { return (FILEINF.RAW ? true : false); }

	OFileHash OFile::recalcHash(void){
		OFileHash tmpHash=0;
		contents.charCount=0;
		int c=0;
		for(size_t i=0;i<contents.lineCount;i++){
			for(size_t j=0;j<contents.lines[i].length;j++){
				c=(contents.lines[i].str[j]);
				tmpHash+=(c*c);
				contents.charCount++;
			}
		}
		FILEINF.HASH=tmpHash/2;
		FILEINF.SIZE=contents.charCount+(contents.lineCount-1);
		return FILEINF.HASH;
	}

	bool OFile::equalTo(OFile& other) const { return (other.getHash() == FILEINF.HASH); }
	bool OFile::operator==(OFile& other) const { return (other.getHash() == FILEINF.HASH); }

	OFileType OFile::getType(void) const { return type; }
	const char* OFile::getTypeAsString(void) const {
		switch(type){
			case OFT_UNKNOWN:{ return "OFT_UNKNOWN"; }
			case OFT_ERROR:{ return "OFT_ERROR"; }
			case OFT_TEXT:{ return "OFT_TEXT"; }
			case OFT_CONFIG:{ return "OFT_CONFIG"; }
			case OFT_IMAGE:{ return "OFT_IMAGE"; }
			case OFT_VIDEO:{ return "OFT_VIDEO"; }
			case OFT_FONT:{ return "OFT_FONT"; }
			case OFT_BINARY:{ return "OFT_BINARY"; }
		}
		return 0;
	}
	const char* OFile::getPath(void) const { return (const char*) FILEINF.PATH; }
	OFile::operator const char*(void) const { return (const char*) FILEINF.PATH; }
	const char* OFile::getName(void) const { return (const char*)misc.name; }
	const char* OFile::getExtension(void) const { return (const char*)misc.ext; }
	void* OFile::getCFile(void) const { return FILEINF.RAW; }
	OFileHash OFile::getHash(void) const { return FILEINF.HASH; }
	size_t OFile::getSize(void) const { return FILEINF.SIZE; }
	size_t OFile::getLineCount(void) const { return contents.lineCount; }
	size_t OFile::getCharCount(void) const { return contents.charCount; }
	OFileContent OFile::getContent(void) const { return contents; }
	OFileLine OFile::getLine(size_t line) const {
		if(line<contents.lineCount){
			return contents.lines[line];
		}else { return {0,0}; }
	}
	char* OFile::operator [](size_t line) const { return getLine(line); }

	void OFile::log(bool verbose){
		if(verbose){
			OLog("File : %s | Type : %s | Extension : %s | Line Count : %lu | Char Count : %lu | Size (bytes) : %lu | Hash : %lu | Modified : %s\n",
				FILEINF.PATH,getTypeAsString(),getExtension(),
				getLineCount(),getCharCount(),getSize(),FILEINF.HASH,
				( contents.modified ? "true" : "false" ));
		}else{
			for(size_t i=0;i<contents.lineCount;i++){
				OLog("%s\n",(char*)contents.lines[i]);
			}
		}
	}

/* Sub-struct definitions */
	OFileLine::operator bool(void) const { return (str ? true : false); }
	char OFileLine::operator [](size_t i) const { return str[i]; }
	OFileLine::operator char*(void) const { return (char*)str; }

	OFileContent::operator bool(void) const { return (lines ? true : false); }
	OFileLine OFileContent::operator [](size_t i) const { return lines[i]; }

/* Generic */

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
			while( (c=fgetc(_CONV(f)))!=EOF ){ r+=(c*c); }
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