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

	/**
	 * @brief Logs a formatted String out to the terminal.
	 * @param string The String to be formatted and logged. The rest of the arguments can be of any type as long as they're capable of being formatted into the String.
	 * @param ... Variadic arguments. Can be of any type as long as long as they're capable of being formatted into the String.
	 */
	extern void OLog(const char* string, ...);
	/**
	 * @brief Logs a formatted String out to the terminal ONLY if OAPP_VERBOSE is true.
	 * @param string The String to be formatted and logged. The rest of the arguments can be of any type as long as they're capable of being formatted into the String.
	 * @param ... Variadic arguments. Can be of any type as long as long as they're capable of being formatted into the String.
	 */
	extern void OVLog(const char* string, ...);

	/**
	 * @brief Logs the individual bits of the given amount of bytes at the given memory address. 
	 * @param data A pointer to the memory address to index and log.
	 * @param bytes The number of bytes to index for logging.
	 * @param newLine Should the output be placed on a newline or append to the current one? Default is true.
	 */
	extern void OLogBits(const void* data, size_t bytes, bool newLine=true);
	/**
	 * @brief Internal. Logs the formatted string out to the terminal alongside the error code,
	 * @param errcode The error code to display prior to the message.
	 * @param autoQuit Should the program immediately quit after delivering the error message?
	 * @param newLine Should the output be placed on a newline or append to the current one? Default is true.
	 */
	extern void OELog(unsigned long errcode, bool autoQuit, const char* string, ...);
	/**
	 * @brief Internal. Logs the formatted string out to the terminal.
	 * @param verboseOnly If this is true, the message will ONLY be logged out to the terminal if OAPP_VERBOSE is also true.
	 * @param string The String to be formatted and logged. The rest of the arguments can be of any type as long as they're capable of being formatted into the String. 
	 * @param ... Variadic arguments. Can be of any type as long as long as they're capable of being formatted into the String.
	 */
	extern void OWLog(bool verboseOnly, const char* string, ...);

/*** Loggable Types **/

	/* Base class for all Loggable Types. */
	struct CLoggable{
		/**
		 * @brief Logs the information of this Type to the terminal.
		 * @param verbose Log verbose information about this Type instead of the traditional information. Default is false.
		 * @param newLine Should the output be placed on a newline or append to the current one if applicable? Default is true.
		 */
		virtual void log(bool verbose=false, bool newLine=true);
	};
	/**
	 * @brief Logs the information of this Type to the terminal.
	 * @param verbose Log verbose information about this Type instead of the traditional information. Default is false.
	 * @param newLine Should the output be placed on a newline or append to the current one if applicable? Default is true.
	 */
	extern void OLog(CLoggable&, bool verbose=false, bool newLine=true); extern void OLog(CLoggable*, bool verbose=false, bool newLine=true);
	/**
	 * @brief Logs the information of this Type to the terminal ONLY if OAPP_VERBOSE is true.
	 * @param verbose Log verbose information about this Type instead of the traditional information. Default is false.
	 * @param newLine Should the output be placed on a newline or append to the current one if applicable? Default is true.
	 */
	extern void OVLog(CLoggable&, bool verbose=false, bool newLine=true); extern void OVLog(CLoggable*, bool verbose=false, bool newLine=true);
}

#endif /* !__ORIONAPI_OSL_OLOG_H__ */
