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

#ifndef __ORIONAPI_OSL_OLOG_H__
#define __ORIONAPI_OSL_OLOG_H__

#include "common.hpp"

#ifndef ORION_NODEBUG
	#define _DEBUGLOG(...) Orion::OLog("%s - LINE : %d\t",__FILE__,__LINE__), Orion::OLog(__VA_ARGS__)
#else
	#define _DEBUGLOG(...)
#endif /* !ORION_NODEBUG */

#ifndef ORION_NOHELPERS
	/* Logs the formatted string out to the terminal, alongside the name of the File and the Line at which this is called for debugging. */
	#define ODLog(...) _DEBUGLOG(__VA_ARGS__)
#endif /* !ORION_NOHELPERS */

namespace Orion{
/*** Base logging ***/
	/* Logs the formatted string out to the terminal. */
	extern void OLog(const char* string, ...);
	/* Logs the formatted string out to the terminal ONLY if OAPP_VERBOSE is true. */
	extern void OVLog(const char* string, ...);
	/*
	 * Logs the individual bits of the given amount of bytes at the given memory address. 
	 * Pass a boolean as a third argument to determine whether a new line should be created or not.
	 */
	extern void OLogBits(const void* data, size_t bytes, bool newLine=true);
	/* Internal. Logs the formatted string out to the terminal alongside the error code, and if "autoQuit" is true, will quit with the error code. */
	extern void OELog(unsigned long errcode, bool autoQuit, const char* string, ...);
	/* Internal. Logs the formatted string out to the terminal. If verboseOnly is true then this will only log if OAPP_VERBOSE is true. */
	extern void OWLog(bool verboseOnly, const char* string, ...);

/*** Loggable Types **/
	/* Base class for all Loggable Types. */
	struct CLoggable{
		/* Logs the information of this Type to the terminal. Pass true for more verbose information. */
		virtual void log(bool verbose=false);
	};
	/* Logs this Loggable Type out to the terminal. Pass true on the second parameter to log verbose information. */
	extern void OLog(CLoggable&, bool verbose=false); extern void OLog(CLoggable*, bool verbose=false);
	/* Logs this Loggable Type out to the terminal ONLY if OAPP_VERBOSE is true. Pass true on the second parameter to log verbose information. */
	extern void OVLog(CLoggable&, bool verbose=false); extern void OVLog(CLoggable*, bool verbose=false);
}

#endif /* !__ORIONAPI_OSL_OLOG_H__ */
