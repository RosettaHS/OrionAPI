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
#include <dirent.h>
#include <sys/stat.h>
#include "../include/OSL/OString.hpp"
#include "../include/OSL/OFile.hpp"
#include "../include/OSL/ODirectory.hpp"

#define TODIR(x) ((DIR*)x)
#define _MKDIRARG ( S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH )

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

/*** Directory class definitions ***/
	ODirectory::ODirectory(void) :
		action{ODIR_AUTO}, path{0}, name{0},
		CDIR{0}, items{0}, itemCount{0}
	{}
	ODirectory::ODirectory(const char* directory, ODirectoryAction _action) :
		action{ODIR_AUTO}, path{0}, name{0},
		CDIR{0}, items{0}, itemCount{0}
	{ open(directory,_action); }
	ODirectory::ODirectory(const char* parentDirectory, const char* subDirectory, ODirectoryAction _action) :
		action{ODIR_AUTO}, path{0}, name{0},
		CDIR{0}, items{0}, itemCount{0}
	{ open(parentDirectory,subDirectory,_action); }

	/* Initialisation */
#define ODIR_BASECOUNT 2 /* All directories (seem) to have 2 extra entries, and this must be taken into account. */
	void ODirectory::init(void){
	/* Store some misc data regarding this Directory. */
		size_t pathL=OStringLength(path);
		size_t namePos;
		size_t nameL;
		namePos=OStringFindLast(path,"/")+1;
		nameL=(pathL-namePos);
		if((namePos-1)!=OSTRING_NOTFOUND){
			if(name){ free(name); name=0; }
			else{ name=(char*)malloc(sizeof(char)*(nameL+1)); }
			for(size_t i=namePos;i<pathL;i++){ name[i-namePos]=path[i]; }
			name[nameL]=0;
		}
	/* First calculate the number of items in the Directory. */
		itemCount=0;
		dirent* tmp=0;
		while(true){
			tmp=readdir(TODIR(CDIR.RAW));
			if(tmp){ itemCount++; }
			else{ break; }
		}
		itemCount-=ODIR_BASECOUNT; /* Subtract 2 because the current and previous directories ( "." & ".." ) are evaluated. */
		rewinddir(TODIR(CDIR.RAW));
	/* Store an array of DirectoryEntries */
		if(itemCount){ items=(ODirectoryEntry*)malloc(sizeof(ODirectoryEntry)*itemCount); }
		else         { items=0; }
		for(size_t i=0;i<itemCount;i++){
			tmp=readdir(TODIR(CDIR.RAW));
			if(tmp){
			/*/
			 * Skip this entry if it's the current or previous directory ( "." OR ".." ) 
			 * TODO: Make this a bit quicker? String comparison is fat and intensive.
			/*/
				if(OStringCompare(tmp->d_name, ".") || OStringCompare(tmp->d_name, "..")){ i--; continue; }
			/* Store the path and name. */
				char* fullPath=concat(path,tmp->d_name);
				if(fullPath){
					items[i].path=fullPath;
					items[i].name=fullPath+(OStringLength(path)+1);
					/* ^The name IS from the path. ONLY free the path, not the path and name or just name. */
				}else{
					items[i].path=0;
					items[i].name=0;
				}
			// /* Store the name. */
				// size_t l=OStringLength(tmp->d_name);
				// items[i].name=(char*)malloc(l+1);
				// for(size_t j=0;j<l;j++){ items[i].name[j]=tmp->d_name[j]; }
				// items[i].name[l]=0;
			/* Store the type. */
				switch(tmp->d_type){
					default:         { items[i].type=ODT_UNKNOWN; break; }
					case DT_DIR:     { items[i].type=ODT_DIR; break; }
					case DT_REG:     { items[i].type=ODT_FILE; break; }
					case DT_LNK:     { items[i].type=ODT_SYML; break; }
					case DT_SOCK:    { items[i].type=ODT_SOCKET; break; }
					case DT_FIFO:    { items[i].type=ODT_PIPE; break; }
				}
			}else{ OELog(OERR_CANTMALLOC,true,"FAILED TO ALLOCATE MEMORY FOR DIRECTORY ENTRY!\n"); }
			/* ^? [Citation Needed] I presume this would be the cause for failure, considering we scanned it fine initially. */
		}
		rewinddir(TODIR(CDIR.RAW));
	}

	/** Opening **/
	bool ODirectory::open(const char* directory, ODirectoryAction _action){
		if(!directory){ return false; }
		if(CDIR.RAW){ close(); }
		action=_action;

		switch(action){
			case ODIR_OPEN: { CDIR.RAW=opendir(directory); break; }
			case ODIR_NEW:  { mkdir(directory, _MKDIRARG); CDIR.RAW=opendir(directory); break; }
			case ODIR_AUTO: {
				if(ODirectoryExists(directory)){ CDIR.RAW=opendir(directory); }
				else{ mkdir(directory, _MKDIRARG); CDIR.RAW=opendir(directory); }
				break;
			}
		}

		if(CDIR.RAW){
			path=realpath(directory,0);
			init();
			return true;
		}else{
			path=0;
			name=0;
			items=0;
			itemCount=0;
			return false;
		}
	}
	bool ODirectory::open(const char* parentDirectory, const char* subDirectory, ODirectoryAction _action){
		if(!parentDirectory || !subDirectory){ return false; }
		char* tmp=concat(parentDirectory,subDirectory);
		if(tmp){
			bool result=open(tmp,_action);
			free(tmp);
			return result;
		}else{ return false; }
	}

	/** Closing **/
	bool ODirectory::close(void){
		if(CDIR.RAW){
			if(!closedir(TODIR(CDIR.RAW))){
				if(path){ free(path); }
				if(name){ free(name); }
				if(items){
					for(size_t i=0;i<itemCount;i++){
						if(items[i].path){ free(items[i].path); }
						items[i].type=ODT_ERROR;
						items[i].path=0;
						items[i].name=0;
						/* Yes this all gets freed, but it's useful to zero out the data within to show any residual pointers to NOT access it again. */
					}
					free(items);
				}
				path=0;
				name=0;
				items=0;
				itemCount=0;
				return true;
			}
		}
		return false;
	}

	/** Getters/misc ops **/

	ODirectoryEntry* ODirectory::getEntry(size_t index){
		if(items && index<itemCount){ return &items[index]; }
		else{ return 0; }
	}

	char* ODirectory::getEntryPath(size_t index){
		if(items && index<itemCount){ return items[index].path;	}
		else{ return 0; }
	}

	void ODirectory::log(bool verbose, bool newLine){
		if(verbose){
			OLog("Directory : %s  | File Count : %lu",path, itemCount);
			if(newLine){ fputc('\n',stdout); }
		}else{
			OLog("%s:\n",name);
		/* Log the sub-directories first. */
			for(size_t i=0;i<itemCount;i++){ if(items[i].type==ODT_DIR){ OLog("\t%s\n",items[i].name); } }
			for(size_t i=0;i<itemCount;i++){ if(items[i].type!=ODT_DIR){ OLog("\t%s\n",items[i].name); } }
			/* No newline is not applicable... */
		}
	}

/*** Generic Directory actions ***/
	bool ODirectoryExists(const char* directory){
		if(directory){
			DIR* tmp=opendir(directory);
			if(tmp){ closedir(tmp); return true; }
		}
		return false;
	}
	bool ODirectoryExists(const char* parentDirectory, const char* subDirectory){
		char* path=concat(parentDirectory,subDirectory);
		if(path){
			bool result=ODirectoryExists(path);
			free(path);
			return result;
		}else{ return false; }
	}
}