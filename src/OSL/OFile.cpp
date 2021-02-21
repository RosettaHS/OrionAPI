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
	OFile::~OFile(void){ close(); }
	OFile::OFile(void) : action{OFILE_READ},path{0},FILERAW{0},FILEDESC{0},type{OFT_ERROR} {}

	OFile::OFile(const char* file, OFileAction _action) : action{OFILE_READ},path{0},FILERAW{0},FILEDESC{0},type{OFT_ERROR} { open(file,_action); }
	OFile::OFile(const char* directory, const char* file, OFileAction _action) : action{OFILE_READ},path{0},FILERAW{0},FILEDESC{0},type{OFT_ERROR} { open(directory,file,_action); }

	bool OFile::open(const char* file, OFileAction _action){
		if(!file){ OLog("ORIONAPI | WARNING! CANNOT PASS NULL WHEN OPENING A FILE!\n"); return false; }
		if(FILERAW){ close(); }
		action=_action;
		switch(action){
			case OFILE_READ:     { FILERAW=fopen(file,"r"); break; }
			case OFILE_WRITE:    { FILERAW=fopen(file,"w"); break; }
			case OFILE_READWRITE:{ FILERAW=fopen(file,"r+"); break; }
		}

		if(FILERAW){
			FILEDESC=fileno( _CONV(FILERAW) );
			path=realpath(file,0);
			OLog("%s\n",path);
			return true;
		}else{ return false; }
	}

	bool OFile::open(const char* directory, const char* file, OFileAction _action){
		if(!directory || !file){ OLog("ORIONAPI | WARNING! CANNOT PASS NULL WHEN OPENING A FILE!\n"); return false; }
		char* dir=realpath(directory,0);
		if(dir){
			size_t l1=OStringLength(directory);
			size_t l2=OStringLength(file);

			char* tmp=(char*)malloc(sizeof(char)*(l1+l2)+1);
			sprintf(tmp,"%s/%s",dir,file);
			bool result=open(tmp,_action);

			free(dir);
			free(tmp);
			return result;
		}else{ return false; }
	}

	bool OFile::close(void){
		if(!FILERAW){ return false; }
		if( fclose( _CONV(FILERAW) ) ){ /* Oy Vey! */
			if(path){ free(path); }
			FILERAW=0;
			FILEDESC=0;
			return true;
		}else{ return false; }
	}

}