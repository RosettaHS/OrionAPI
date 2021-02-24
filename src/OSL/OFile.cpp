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

/* Initialisation */

	OFile::~OFile(void){ close(); }
	OFile::OFile(void) : action{OFILE_OPEN},path{0},name{0},ext{0},FILERAW{0},FILEDESC{0},type{OFT_ERROR} {}

	OFile::OFile(const char* file, OFileAction _action) : action{OFILE_OPEN},path{0},name{0},ext{0},FILERAW{0},FILEDESC{0},type{OFT_ERROR} { open(file,_action); }
	OFile::OFile(const char* directory, const char* file, OFileAction _action) : action{OFILE_OPEN},path{0},name{0},ext{0},FILERAW{0},FILEDESC{0},type{OFT_ERROR} { open(directory,file,_action); }

	bool OFile::open(const char* file, OFileAction _action){
		if(!file){ OLog("ORIONAPI | WARNING! CANNOT PASS NULL WHEN OPENING A FILE!\n"); return false; }
		if(FILERAW){ close(); }
		action=_action;
		switch(action){
			case OFILE_OPEN:              { FILERAW=fopen(file,"r+"); break; }
			case OFILE_OPEN_READONLY:     { FILERAW=fopen(file,"r");  break; }
			case OFILE_NEW:               { FILERAW=fopen(file,"w+"); break; }
			case OFILE_NEW_WRITEONLY:     { FILERAW=fopen(file,"w");  break; }
		}

		if(FILERAW){
		/* Store some data regarding the path, and keep some variables for use later in this block. */
			FILEDESC=fileno( _CONV(FILERAW) );
			path=realpath(file,0);
			size_t pathl=OStringLength(path);
			size_t optPos;
			size_t optl;
		/* Store the file's raw extension. */
			optPos=OStringFindLast(path,".")+1;
			optl=(pathl-optPos);
			if((optPos-1)!=OSTRING_NOTFOUND){
				if(optPos==OStringFindFirst(path,".")){ ext=0;} /* Prevents weird issues with hidden files */
				else{
					ext=(char*)malloc(sizeof(char)*(optl+1));
					for(size_t i=optPos;i<pathl;i++){ ext[i-optPos]=path[i]; }
					ext[optl+1]=0;
				}
			}else{ ext=0; }
		/* Store the filename (with extension) */
			optPos=OStringFindLast(path,"/");
			optl=(pathl-optPos);
			if((optPos-1)!=OSTRING_NOTFOUND){
				name=(char*)malloc(sizeof(char)*(optl+1));
					for(size_t i=optPos;i<pathl;i++){ name[i-optPos]=path[i]; }
					name[optl+1]=0;
			}else{
				/*
				 * We still have to allocate a new block of memory for "name" even if "name" is the same as "path"
				 * because close() will free them both separately, and if "name" points to "path" this will cause a segfault.
				 */
				 name=(char*)malloc(sizeof(char)*(pathl+1));
				 for(size_t i=0;i<pathl;i++){ name[i]=path[i]; }
				 name[pathl+1]=0;
			}
			return true;
		}else{ return false; }
	}

	bool OFile::open(const char* directory, const char* file, OFileAction _action){
		if(!directory || !file){ OLog("ORIONAPI | WARNING! CANNOT PASS NULL WHEN OPENING A FILE!\n"); return false; }
		char* dir=concat(directory,file);
		bool result=open(dir,_action);
		if(dir){ free(dir); }
		return result;
	}

	bool OFile::close(void){
		if(!FILERAW){ return false; }
		if( fclose( _CONV(FILERAW) ) ){ /* Oy Vey! */
			if(path){ free(path); }
			if(name){ free(name); }
			if(ext) { free(ext); }
			FILERAW=0;
			FILEDESC=0;
			return true;
		}else{ return false; }
	}

/* File modifcation */

	/* TODO: Add abstracted functionality*/

/* Getters/misc ops */

	bool OFile::valid(void) const{ return ( FILERAW ? true : false ); }
	OFile::operator bool(void) const{ return (FILERAW ? true : false); }

	const char* OFile::getExtension(void) const { return (const char*)ext; }
	const char* OFile::getName(void) const { return (const char*)name; }
	const char* OFile::getFullPath(void) const { return (const char*) path; }
	void* OFile::getCFile(void) const { return FILERAW; }

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