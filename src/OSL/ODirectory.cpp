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
#include "../include/OSL/OString.hpp"
#include "../include/OSL/OFile.hpp"
#include "../include/OSL/ODirectory.hpp"

#define TODIR(x) ((DIR*)x)

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
	ODirectoryEntry::ODirectoryEntry(void) : type{ODT_ERROR}, name{0} {}
	ODirectory::ODirectory(void) : CDIR{0}, items{0}, itemCount{0} {}

	/* Initialisation */
#define ODIR_BASECOUNT 2 /* All directories (seem) to have 2 extra entries, and this must be taken into account. */
	void ODirectory::init(void){
	/* First calculate the number of items in the Directory. */
		itemCount=0;
		dirent* tmp=0;
		while(true){
			tmp=readdir(TODIR(CDIR.RAW));
			if(tmp){ OLog("read entry : count=%d | entry name=%s\n",itemCount,tmp->d_name); itemCount++; }
			else{ break; }
		}
		itemCount-=ODIR_BASECOUNT; /* Subtract 2 because the current and previous directories ( "." & ".." ) are evaluated. */
OLog("finished eval. count=%d\n",itemCount);
		rewinddir(TODIR(CDIR.RAW));
	/* Store an array of DirectoryEntries */
		if(itemCount){ items=(ODirectoryEntry*)malloc(sizeof(ODirectoryEntry)*itemCount); }
		else         { items=0; }
		for(size_t i=0;i<itemCount;i++){
ODLog("loop\n");
			tmp=readdir(TODIR(CDIR.RAW));
			if(tmp){
			/*/
			 * Skip this entry if it's the current or previous directory ( "." OR ".." ) 
			 * TODO: Make this a bit quicker? String comparison is fat and intensive.
			/*/
				if(OStringCompare(tmp->d_name, ".") || OStringCompare(tmp->d_name, "..")){ i--; continue; }
ODLog("scan2 | index %lu reading %s\n",i,tmp->d_name);
			/* Store the name. */
				size_t l=OStringLength(tmp->d_name);
ODLog("got name length\n");
				items[i].name=(char*)malloc(l+1);
ODLog("allocated name\n");
				for(size_t j=0;j<l;j++){ items[i].name[j]=tmp->d_name[j]; }
ODLog("set name\n");
				items[i].name[l]=0;
ODLog("name stored\n");
			/* Store the type. */
				switch(tmp->d_type){
					default:         { items[i].type=ODT_UNKNOWN; break; }
					case DT_UNKNOWN: { items[i].type=ODT_UNKNOWN; break; }
					case DT_DIR:     { items[i].type=ODT_DIR; break; }
					case DT_REG:     { items[i].type=ODT_FILE; break; }
					case DT_LNK:     { items[i].type=ODT_SYML; break; }
					case DT_SOCK:    { items[i].type=ODT_SOCKET; break; }
					case DT_FIFO:    { items[i].type=ODT_PIPE; break; }
				}
ODLog("type stored\n");
			}else{ OELog(OERR_CANTMALLOC,true,"FAILED TO ALLOCATE MEMORY FOR DIRECTORY ENTRY!\n"); }
			/* ^? [Citation Needed] I presume this would be the cause for failure, considering we scanned it fine initially. */
		}
		rewinddir(TODIR(CDIR.RAW));
OLog("-----------finished-----------\n");
	/* DEBUG: Log the contents. */
		for(size_t i=0;i<itemCount;i++){
			ODLog("Entry %lu - %s\n",i,items[i].name);
		}
	}

	bool ODirectory::open(const char* directory){
		if(!directory){ return false; }
		if(CDIR.RAW){ /* Do freeing first */ }

		CDIR.RAW=opendir(directory);
		if(CDIR.RAW){
			init();
			return true;
		}else{
			
			return false;
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