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

#ifndef __ORIONAPI_ERRDEF_H__
#define __ORIONAPI_ERRDEF_H__

#ifdef ORION_INTERNAL

/* This contains error macros that the OrionAPI will use when encountering an error. */

/* GENERIC */

/* Emits when the OrionAPI has encountered an error but can't resolve the type.  */
#define OERR_UNKNOWN							-1
/* Emits when the OrionAPI successfully exits. */
#define OERR_NONE								0
/* Emits when the OrionAPI can't malloc() any more memory. */
#define OERR_CANTMALLOC							1
/* Emits when the OrionAPI is forced to NativeOnly mode, and OApp boots as standalone binary. */
#define OERR_NOT_NATIVE_OAPP					2

/* X11 */

/* Emits when the OrionAPI fails to connect with X11. */
#define OERR_X11_CONNECTION_FAILURE				3
/* Emits when the OrionAPI's X11 connection gets interrupted but can't resolve the type. */
#define OERR_X11_CONNECTION_INTERRUPTED			4
/* Emits when the OrionAPI tries to create an object while X11 is not initialised. */
#define OERR_X11_NOT_INITED						5
/* Emits when X fails to return a valid Window. */
#define OERR_X11_WINDOW_CREATION_FAILURE		6
/* Emits when X can't load the fallback "fixed" font. */
#define OERR_X11_FONTFALLBACK					7

/* CDRAWABLE */

/* Emits when developer tries to set an invalid axis flag */
#define OERR_CDRAWABLE_INVALID_FLAG				8


#endif /* !ORION_INTERNAL */

#endif /* !__ORIONAPI_ERRDEF_H__ */
