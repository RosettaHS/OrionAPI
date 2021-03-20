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