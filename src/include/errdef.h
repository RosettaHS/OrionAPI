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

#ifndef __ORION_OKIT_ERRDEF_H__
#define __ORION_OKIT_ERRDEF_H__

/* This contains error macros that OKit will use when encountering an error. */

/* GENERIC */

/* Emits when OKit has encountered an error but can't resolve the type.  */
#define OERR_UNKNOWN							-1
/* Emits when OKit successfully exits. */
#define OERR_NONE								0
/* Emits when OKit can't malloc() any more memory. */
#define OERR_CANTMALLOC							1
/* Emits when OKit is forced to NativeOnly mode, and OApp boots as standalone binary. */
#define OERR_NOT_NATIVE_OAPP					2

/* X11 */

/* Emits when OKit fails to connect with X11. */
#define OERR_X11_CONNECTION_FAILURE				3
/* Emits when OKit's X11 connection gets interrupted but can't resolve the type. */
#define OERR_X11_CONNECTION_INTERRUPTED			4
/* Emits when OKit tries to create an object while X11 is not initialised. */
#define OERR_X11_NOT_INITED						5
/* Emits when X fails to return a valid Window. */
#define OERR_X11_WINDOW_CREATION_FAILURE		6

/* Signals */

/* Emits when 'call(void*)' is called on a CSignalListener with 'type' set to CSLT_ERROR. */
#define OERR_CSIGNALLISTENER_CALLONERROR		7
/* Emits when 'link(CEvent*,CSignalListener&)' is called on a CEventArray but the 'CEvent*' is NOT owned by the array. */
#define OERR_CEVENTARRAY_LINKTONONCHILD			8
/* Emits when 'unlink(CEvent*,void* func)' is called on a CEventArray but the 'CEvent*' is NOT owned by the array. */
#define OERR_CEVENTARRAY_UNLINKTONONCHILD		9

#endif /* !__ORION_OKIT_ERRDEF_H__ */