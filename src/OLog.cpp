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
#include <stdarg.h>
#include "include/application.hpp"
#include "include/OLog.hpp"

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
/* Primitive Types. */

	void OLog(bool v){ OLog("%s\n",(v ? "true" : "false")); }
	void OLog(int v){ OLog("%d\n",v); }
	void OLog(unsigned int v){ OLog("u\n",v); }
	void OLog(short v){ OLog("%d\n",v); }
	void OLog(unsigned short v){ OLog("%u\n",v); }
	void OLog(float v){ OLog("%f\n",v); }
	void OLog(double v){ OLog("lf\n",v); }
	void OLog(long double v){ OLog("Lf\n",v); }
	void OLog(char v){ OLog("%c\n",v); }
	void OLog(unsigned char v){ OLog("%u\n",v); }
	void OLog(void* v){ OLog("%p\n",v); }

	void OVLog(bool v){ OVLog("%s\n",(v ? "true" : "false")); }
	void OVLog(int v){ OVLog("%d\n",v); }
	void OVLog(unsigned int v){ OVLog("u\n",v); }
	void OVLog(short v){ OVLog("%d\n",v); }
	void OVLog(unsigned short v){ OVLog("%u\n",v); }
	void OVLog(float v){ OVLog("%f\n",v); }
	void OVLog(double v){ OVLog("lf\n",v); }
	void OVLog(long double v){ OVLog("Lf\n",v); }
	void OVLog(char v){ OVLog("%c\n",v); }
	void OVLog(unsigned char v){ OVLog("%u\n",v); }
	void OVLog(void* v){ OVLog("%p\n",v); }
}
