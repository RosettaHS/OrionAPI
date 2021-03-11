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

	void OELog(unsigned long errcode, bool autoQuit, const char* string, ...){
		OLog("ORIONAPI | ERROR! [%lu] | ",errcode);
		va_list arg;
		va_start(arg,string);
		vfprintf(stdout,string,arg);
		va_end(arg);
		if(autoQuit){ exit(errcode); }
	}

	extern void OWLog(unsigned long warncode, bool verboseOnly, const char* string, ...){
		if(verboseOnly && !OAPP_VERBOSE){ return; }
		OLog("ORIONAPI | WARNING! [%lu] | ",warncode);
		va_list arg;
		va_start(arg,string);
		vfprintf(stdout,string,arg);
		va_end(arg);
	}
}
