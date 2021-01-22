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

#ifndef __ORION_OKIT_OLOG_H__
#define __ORION_OKIT_OLOG_H__

namespace Orion{
	/* Logs the formatted string out to the terminal. */
	extern void OLog(const char* string, ...);
	/* Logs the formatted string out to the terminal ONLY if OAPP_VERBOSE is true. */
	extern void OVLog(const char* string, ...);

/* Primitive Types. */
/* Yes... this is bad... But it works for now! */

	/* Logs this primitive type to the terminal. */
	extern void OLog(bool);
	/* Logs this primitive type to the terminal. */
	extern void OLog(int);
	/* Logs this primitive type to the terminal. */
	extern void OLog(unsigned int);
	/* Logs this primitive type to the terminal. */
	extern void OLog(short);
	/* Logs this primitive type to the terminal. */
	extern void OLog(unsigned short);
	/* Logs this primitive type to the terminal. */
	extern void OLog(float);
	/* Logs this primitive type to the terminal. */
	extern void OLog(double);
	/* Logs this primitive type to the terminal. */
	extern void OLog(long double v);
	/* Logs this primitive type to the terminal. */
	extern void OLog(char);
	/* Logs this primitive type to the terminal. */
	extern void OLog(unsigned char);
	/* Logs this primitive type to the terminal. */
	extern void OLog(void*);

	/* Logs this primitive type to the terminal ONLY if OAPP_VERBOSE is true. */
	extern void OVLog(bool);
	/* Logs this primitive type to the terminal ONLY if OAPP_VERBOSE is true. */
	extern void OVLog(int);
	/* Logs this primitive type to the terminal ONLY if OAPP_VERBOSE is true. */
	extern void OVLog(unsigned int);
	/* Logs this primitive type to the terminal ONLY if OAPP_VERBOSE is true. */
	extern void OVLog(short);
	/* Logs this primitive type to the terminal ONLY if OAPP_VERBOSE is true. */
	extern void OVLog(unsigned short);
	/* Logs this primitive type to the terminal ONLY if OAPP_VERBOSE is true. */
	extern void OVLog(float);
	/* Logs this primitive type to the terminal ONLY if OAPP_VERBOSE is true. */
	extern void OVLog(double);
	/* Logs this primitive type to the terminal ONLY if OAPP_VERBOSE is true. */
	extern void OVLog(long double);
	/* Logs this primitive type to the terminal ONLY if OAPP_VERBOSE is true. */
	extern void OVLog(char);
	/* Logs this primitive type to the terminal ONLY if OAPP_VERBOSE is true. */
	extern void OVLog(unsigned char);
	/* Logs this primitive type to the terminal ONLY if OAPP_VERBOSE is true. */
	extern void OVLog(void*);
}

#endif /* !__ORION_OKIT_OLOG_H__ */