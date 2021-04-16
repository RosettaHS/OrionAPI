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
			OFileHash tmpHash=0;
			int c;
			if(flags.storeLinearly){
				contents.asLinear.byteCount=0;
				contents.asLinear.bytes=0;
				contents.modified=0;
				while( (c=fgetc(TOFILE(CFILE.RAW)))!=EOF ){
					tmpHash+=((char)c*(char)c);
					contents.asLinear.byteCount++;
				}
				hash=tmpHash/2;
				size=contents.asLinear.byteCount;
			}else{
				contents.asLines.lineCount=1; /* All files have at least one line, although this makes iteration confusing. */
				contents.asLines.charCount=0;
				contents.asLines.list=0;
				contents.modified=0;
				while( (c=fgetc(TOFILE(CFILE.RAW)))!=EOF ){
					tmpHash+=((char)c*(char)c);
					if((char)c=='\n'){ contents.asLines.lineCount++; }else{ contents.asLines.charCount++; }
				}
				hash=tmpHash/2;
				size=contents.asLines.charCount+(contents.asLines.lineCount-1);
			}
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
		if(flags.storeMem && flags.evalContents){
		/*
		 * Avoid storing the File's contents to memory by default if it's a heavy File such as an image, binary, or video.
		 * These three specifically due to what I plan to do with RenderKit.
		 */
			if(type!=OFT_BINARY && type!=OFT_IMAGE && type!=OFT_VIDEO){ storeToMem(); }
		}
	}

	/* Store File to memory. */
#define TMPLINELENGTH 1024
#define TMPLINESTEP   256
	bool OFile::storeToMem(void){
	/* Completely skip all of this if we don't have a File opened. */
		if(CFILE.RAW){
			contents.isLinear=flags.storeLinearly;
			if(contents.isLinear){ /* If data is to be stored linearly. */
			/* Skip storing to memory if we've already stored this File to memory. */
				if(contents.asLinear.bytes) { return false; }
			/* Store some variables we'll use later. */
				int    c=0;
				size_t i=0;
			/* First check if the File's byte count has been determined, if not, determine it here. */
				if(!contents.asLinear.byteCount){
					contents.asLinear.byteCount=0;
					contents.asLinear.bytes=0;
					contents.modified=0;
					OFileHash tmpHash=0;
					while( (c=fgetc(TOFILE(CFILE.RAW)))!=EOF ){
						tmpHash+=((char)c*(char)c);
						contents.asLinear.byteCount++;
					}
					hash=tmpHash/2;
					size=contents.asLinear.byteCount;
				}
				rewind(TOFILE(CFILE.RAW));
				if(!contents.asLinear.byteCount){ return false; }
				contents.asLinear.bytes=(uint8_t*)malloc(contents.asLinear.byteCount);
			/* Search and save each byte. */
				while(true){
					c=fgetc(TOFILE(CFILE.RAW));
					if(c==EOF){
						break;
					}else{ contents.asLinear.bytes[i]=c; i++; }
				}
				/* Clean-up and return. */
				rewind(TOFILE(CFILE.RAW));
				return true;
			}else{ /* If data is to be stored on separate Lines. */
			/* Skip storing to memory if we've already stored this File to memory. */
				if(contents.asLines.list)   { return false; }
			/* Store some variables we'll use later. */
				int    c=0;
				size_t l=0, i=0;
			/* First check if the File's Line count has been determined, if not, determine it here. */
				rewind(TOFILE(CFILE.RAW));
				if(!contents.asLines.lineCount){
					contents.asLines.lineCount=1;
					contents.asLines.charCount=0;
					OFileHash tmpHash=0;
					while( (c=fgetc(TOFILE(CFILE.RAW)))!=EOF ){
						tmpHash+=((char)c*(char)c);
						if((char)c=='\n'){ contents.asLines.lineCount++; }else{ contents.asLines.charCount++; }
					}
					hash=tmpHash/2;
					size=contents.asLines.charCount+(contents.asLines.lineCount-1);
				}
				rewind(TOFILE(CFILE.RAW));
				contents.asLines.list=(OFileLine*)malloc(sizeof(OFileLine)*(contents.asLines.lineCount));
				for(size_t i=0;i<contents.asLines.lineCount;i++){ contents.asLines.list[i]={0,0}; }
			/* Search and save each Line. */
				unsigned char* tmp=(unsigned char*)malloc(TMPLINELENGTH);
				size_t         tmpL=TMPLINELENGTH;
				if(!tmp){ return false; }
				while(true){
					c=fgetc(TOFILE(CFILE.RAW));
					if(c=='\n' || c==EOF){
						contents.asLines.list[l]={ i, (unsigned char*)malloc( (i+1) ) };
						if(contents.asLines.list[l]){
							for(size_t j=0;j<i;j++){ contents.asLines.list[l].str[j]=tmp[j]; }
							contents.asLines.list[l].str[i]=0;
						}
						i=0; l++;
						if(c==EOF){ break; }
					}else{
						if(i<tmpL){ tmp[i]=c; }
						else{
							tmpL+=TMPLINESTEP;
							tmp=(unsigned char*)realloc(tmp,tmpL);
						}
						i++;
					}
				}
			/* Clean-up and return. */
				rewind(TOFILE(CFILE.RAW));
				free(tmp);
				return true;
			}
		}
		return false;
	}

	/** Constructors **/
	OFile::OFile(void) :
		type{OFT_ERROR},action{OFILE_AUTO},
		contents{0,0,0,0,0},hash{0},size{0},
		name{0},path{0},ext{0},CFILE{0,0},flags{1,0,1,1}
		{}
	OFile::OFile(const char* filename, OFileAction _action) :
		type{OFT_ERROR},action{OFILE_AUTO},
		contents{0,0,0,0,0},hash{0},size{0},
		name{0},path{0},ext{0},CFILE{0,0},flags{1,0,1,1}
		{ open(filename,_action); }
	OFile::OFile(const char* directory, const char* filename, OFileAction _action) :
		type{OFT_ERROR},action{OFILE_AUTO},
		contents{0,0,0,0,0},hash{0},size{0},
		name{0},path{0},ext{0},CFILE{0,0},flags{1,0,1,1}
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
			if(contents.isLinear){
				contents.asLinear.byteCount=0;
				contents.asLinear.bytes=0;
			}else{
				contents.asLines.lineCount=0;
				contents.asLines.charCount=0;
				contents.asLines.list=0;
			}
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
		if(applyChanges){ save(); }
		if( !fclose( TOFILE(CFILE.RAW) ) ){
			if(name) { free(name); }
			if(path) { free(path); }
			if(ext)  { free(ext); }
			if(contents.isLinear){
				if(contents.asLinear.bytes){ free(contents.asLinear.bytes); }
				contents.asLinear.byteCount=0;
				contents.asLinear.bytes=0;
			}else{
				if(contents.asLines.list){
					for(size_t i=0;i<contents.asLines.lineCount;i++){ if(contents.asLines.list[i].str){ free(contents.asLines.list[i].str); } }
					free(contents.asLines.list);
				}
				contents.asLines.lineCount=0;
				contents.asLines.charCount=0;
				contents.asLines.list=0;
			}
			type=OFT_ERROR;
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

	/** File modifcation **/
	/* Saving */
	bool OFile::save(void){
		if(action!=OFILE_OPEN && action!=OFILE_NEW){ return false; }
		if(contents.modified){
			FILE* F=fopen(path,"w");
			if(F){
				if(contents.isLinear){
					for(size_t i=0;i<contents.asLinear.byteCount;i++){ fputc(contents.asLinear.bytes[i],F); }
				}else{
					for(size_t i=0;i<contents.asLines.lineCount;i++){
						for(size_t j=0;j<contents.asLines.list[i].length;j++){
							fputc(contents.asLines.list[i].str[j],F);
						}
						if(i!=contents.asLines.lineCount-1){ fputc('\n',F); }
					}
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
		if(!file){ return false; }
		if(CFILE.RAW){
			FILE* F=fopen(file,"w");
			if(F){
				if(contents.isLinear){
					for(size_t i=0;i<contents.asLinear.byteCount;i++){ fputc(contents.asLinear.bytes[i],F); }
				}else{
					for(size_t i=0;i<contents.asLines.lineCount;i++){
						for(size_t j=0;j<contents.asLines.list[i].length;j++){
							fputc(contents.asLines.list[i].str[j],F);
						}
						if(i!=contents.asLines.lineCount-1){ fputc('\n',F); }
					}
				}
				fclose(F);
				return true;
			}
		}
		return false;
	}
	bool OFile::saveAs(const char* directory, const char* file){
		if(!directory || !file){ return false; }
		char* path=concat(directory,file);
		bool result=saveAs(path);
		if(path){ free(path); }
		return result;
	}

	/* Renaming the current File. */
	bool OFile::rename(const char* newName){
		if(CFILE.RAW){
			fclose(TOFILE(CFILE.RAW));
			CFILE.RAW=0;
			bool result=false;
			if(OFileRename(path,newName)){
				if(path){ free(path); }
				path=realpath(newName,0);
				init(true);
				result=true;
			}
			switch(action){
				case OFILE_OPEN:              { CFILE.RAW=fopen(path,"r+"); break; }
				case OFILE_OPEN_READONLY:     { CFILE.RAW=fopen(path,"r");  break; }
				case OFILE_NEW:               { CFILE.RAW=fopen(path,"w+"); break; }
				/* THIS IS NOT A VALID TYPE! If this is detected, there is something very wrong. */
				case OFILE_AUTO:{
					OLog("ORIONAPI | WARNING! OFILE DETECTED WITH TYPE OFILE_AUTO! THIS IS A HELPER TYPE, IF IT'S ACTUALLY SET TO THIS THEN SOMETHING HAS GONE WRONG!\n");
					break;
				}
			}
			return result;
		}
		return false;
	}

	/* Resetting any modifications. */
	bool OFile::reset(void){
		if(TOFILE(CFILE.RAW) && contents.modified){
			if(contents.isLinear){
				if(contents.asLinear.bytes){ free(contents.asLinear.bytes); }
				contents.asLinear.bytes=0;
			}else{
				if(contents.asLines.list){
					for(size_t i=0;i<contents.asLines.lineCount;i++){ if(contents.asLines.list[i].str){ free(contents.asLines.list[i].str); } }
					free(contents.asLines.list);
				}
				contents.asLines.list=0;
			}
			init(false);
			contents.modified=false;
			return true;
		}
		return false;
	}

	/* Deleting the current File. */
	bool OFile::deleteCurrent(void){
		if(CFILE.RAW){
			if( !remove(path) ){
				if(name) { free(name); }
				if(path) { free(path); }
				if(ext)  { free(ext); }
				if(contents.isLinear){
					if(contents.asLinear.bytes){ free(contents.asLinear.bytes); }
					contents.asLinear.byteCount=0;
					contents.asLinear.bytes=0;
				}else{
					if(contents.asLines.list){
						for(size_t i=0;i<contents.asLines.lineCount;i++){ if(contents.asLines.list[i].str){ free(contents.asLines.list[i].str); } }
						free(contents.asLines.list);
					}
					contents.asLines.lineCount=0;
					contents.asLines.charCount=0;
					contents.asLines.list=0;
				}
				type=OFT_ERROR;
				contents.modified=false;
				hash=0;
				name=0;
				path=0;
				ext=0;
				CFILE.RAW=0;
				CFILE.DESC=0;
				return true;
			}
		}
		return false;
	}

	/* Setting a specific line. TODO: Reintroduce error messages with new error system. */
	bool OFile::setLine(size_t line, const char* newText){
		if(contents.isLinear){ return false; } /* Can't set lines if they're not stored as lines. */
		if(CFILE.RAW && contents.asLines.list){
			if(action==OFILE_OPEN_READONLY){ return false; }
		/* Checks if the given Line is actually available, and if not, resize the File for it to be available. */
			if(line>=contents.asLines.lineCount){
				contents.asLines.list=(OFileLine*)realloc(contents.asLines.list,sizeof(OFileLine)*(line+1));
				if(!contents.asLines.list){
					// OLog("ORIONAPI | ERROR! COULD NOT ALLOCATE EXTRA LINES FOR FILE! CURRENT COUNT : %lu | ATTEMPTED ALLOCATION : %lu\n",contents.lineCount,line);
					// exit(OERR_CANTMALLOC);
					return false;
				}
				for(size_t i=contents.asLines.lineCount;i<line+1;i++){
					contents.asLines.list[i]={ 0,(unsigned char*)malloc(1) };
					contents.asLines.list[i].str[0]=0;
				}
				contents.asLines.lineCount=(line+1);
			}
		/* Actually sets the Line data */
			OFileLine* lineToSet=&contents.asLines.list[line];
			if(!lineToSet->str){ return false; }
			size_t l1=lineToSet->length;
			size_t l2=OStringLength(newText);
			if(l2>l1){ lineToSet->str=(unsigned char*)realloc(lineToSet->str,(l2+1)); }
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

/** Getters/misc ops **/
	// void OFile::shouldStoreMisc(bool v) { flags.storeMisc=v; }
	// void OFile::shouldStoreToMem(bool v) { flags.storeMem=v; }
	bool OFile::valid(void) const { return ( CFILE.RAW ? true : false ); }
	bool OFile::hasBeenModified(void) const { return contents.modified; }
	OFile::operator bool(void) const { return (CFILE.RAW ? true : false); }

	OFileHash OFile::recalcHash(void){
		OFileHash tmpHash=0;
		int c=0;
		if(contents.isLinear){ /* This is redundant, since there's never a situation where the hash would need to be recalculated if being stored linearly. */
			for(size_t i=0;i<contents.asLinear.byteCount;i++){
				c=(contents.asLinear.bytes[i]);
				tmpHash+=(c*c);
			}
		}else{
			contents.asLines.charCount=0;
			for(size_t i=0;i<contents.asLines.lineCount;i++){
				for(size_t j=0;j<contents.asLines.list[i].length;j++){
					c=(contents.asLines.list[i].str[j]);
					tmpHash+=(c*c);
					contents.asLines.charCount++;
				}
			}
			size=contents.asLines.charCount+(contents.asLines.lineCount-1);
		}
		hash=tmpHash/2;
		return hash;
	}

	bool OFile::equalTo(OFile& other) const { return (other.getHash() == hash); }
	bool OFile::operator==(OFile& other) const { return (other.getHash() == hash); }

	OFileType OFile::getType(void) const { return type; }
	const char* OFile::getTypeAsString(void) const {
		switch(type){
			case OFT_UNKNOWN: { return "OFT_UNKNOWN"; }
			case OFT_ERROR:   { return "OFT_ERROR"; }
			case OFT_TEXT:    { return "OFT_TEXT"; }
			case OFT_CONFIG:  { return "OFT_CONFIG"; }
			case OFT_IMAGE:   { return "OFT_IMAGE"; }
			case OFT_VIDEO:   { return "OFT_VIDEO"; }
			case OFT_FONT:    { return "OFT_FONT"; }
			case OFT_BINARY:  { return "OFT_BINARY"; }
		}
		return 0;
	}
	const char* OFile::getPath(void) const { return (const char*) path; }
	OFile::operator const char*(void) const { return (const char*) path; }
	const char* OFile::getName(void) const { return (const char*)name; }
	const char* OFile::getExtension(void) const { return (const char*)ext; }
	void* OFile::getCFile(void) const { return CFILE.RAW; }
	OFileHash OFile::getHash(void) const { return hash; }
	size_t OFile::getSize(void) const { return size; }
	size_t OFile::getLineCount(void) const { return ( (contents.isLinear) ? contents.asLines.lineCount : 0 ); }
	size_t OFile::getCharCount(void) const { return ( (contents.isLinear) ? contents.asLines.charCount : 0 ); }
	// OFileContent* OFile::getContents(void) { return &contents; }
	OFileLine OFile::getLine(size_t line) const {
		if(line<contents.asLines.lineCount){
			return contents.asLines.list[line];
		}else { return {0,0}; }
	}
	char* OFile::operator [](size_t line) const { return getLine(line); }

	void OFile::log(bool verbose, bool newLine){
		if(verbose){
			if(contents.isLinear){
				OLog("File : %s | Type : %s | Extension : %s | Size (bytes) : %lu | Hash : %lu",
					path,getTypeAsString(),getExtension(),getSize(),hash
				);
			}else{
				OLog("File : %s | Type : %s | Extension : %s | Line Count : %lu | Char Count : %lu | Size (bytes) : %lu | Hash : %lu | Modified : %s",
					path,getTypeAsString(),getExtension(),
					getLineCount(),getCharCount(),getSize(),hash,
					( contents.modified ? "true" : "false" ));
			}
			if(newLine){ fputc('\n',stdout); }
		}else{
			if(contents.isLinear){
				for(size_t i=0;i<contents.asLinear.byteCount;i++){ fputc(contents.asLinear.bytes[i],stdout); }
				if(newLine){ fputc('\n',stdout); } /* May not be applicable... */
			}else{
				for(size_t i=0;i<contents.asLines.lineCount;i++){
					for(size_t j=0;j<contents.asLines.list[i].length;j++){ fputc(contents.asLines.list[i].str[j],stdout); }
					if(newLine){ fputc('\n',stdout); }
				}
			}
		}
	}


/*** Sub-struct definitions ***/
	OFileLine::operator bool(void) const { return (str ? true : false); }
	char OFileLine::operator [](size_t i) const { return str[i]; }
	OFileLine::operator char*(void) const { return (char*)str; }

	// OFileContent::operator bool(void) const { return (lines ? true : false); }
	// OFileLine OFileContent::operator [](size_t i) const { return lines[i]; }

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