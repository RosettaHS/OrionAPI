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
#include <stdarg.h>
#include "../include/Application.hpp"
#include "../include/OSL/OLog.hpp"

/* Modification and simplification of GNU's implementation for printf(). */

namespace Orion{
	void OLog(const char* string, ...){
		va_list arg;
		va_start(arg,string);
		vfprintf(stdout,string,arg);
		va_end(arg);
	}

	void OVLog(const char* string, ...){
		if(OAPP_VERBOSE){
			va_list arg;
			va_start(arg,string);
			vfprintf(stdout,string,arg);
			va_end(arg);
		}
	}

	#define BITCOUNT 8
	void OLogBits(const void* data, size_t bytes, bool newLine){
		MAXCONST unsigned char bits[BITCOUNT]={0x80,0x40,0x20,0x10,0x8,0x4,0x2,0x1};
		char* c=(char*)data;
		OLog("[ ");
		for(size_t i=0;i<bytes;i++){
			for(size_t j=0;j<BITCOUNT;j++){
				( ((c[i])&bits[j]) ? OLog("1") : OLog("0") );
			}
			OLog(" ");
		}
		OLog("]");
		if(newLine){ OLog("\n"); }else{ OLog(" "); }
	}

	void OELog(unsigned long errcode, bool autoQuit, const char* string, ...){
		OLog("ORIONAPI | ERROR! [%lu] | ",errcode);
		va_list arg;
		va_start(arg,string);
		vfprintf(stderr,string,arg);
		va_end(arg);
		if(autoQuit){ exit(errcode); }
	}

	void OWLog(bool verboseOnly, const char* string, ...){
		if(verboseOnly && !OAPP_VERBOSE){ return; }
		OLog("ORIONAPI | WARNING! ");
		va_list arg;
		va_start(arg,string);
		vfprintf(stderr,string,arg);
		va_end(arg);
	}


	/* Base class for all Loggable Types. */
	void CLoggable::log(bool verbose, bool newLine){
		if(newLine){
	 		( (verbose) ? OLog("Nothing to log...\n") : OLog("%p - Nothing to log...\n",(void*)this) );
		}else{
			( (verbose) ? OLog("Nothing to log...") : OLog("%p - Nothing to log...",(void*)this) );
		}
	}
	void OLog(CLoggable& obj, bool verbose, bool newLine)  { obj.log(verbose, newLine); }
	void OLog(CLoggable* obj, bool verbose, bool newLine)  { obj->log(verbose,newLine); }
	void OVLog(CLoggable& obj, bool verbose, bool newLine) { if(OAPP_VERBOSE){ obj.log(verbose,newLine); } }
	void OVLog(CLoggable* obj, bool verbose, bool newLine) { if(OAPP_VERBOSE){ obj->log(verbose,newLine); } }
}
