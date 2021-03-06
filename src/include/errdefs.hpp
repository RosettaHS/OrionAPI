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

#ifdef ORION_INTERNAL

#ifndef __ORIONAPI_ERRDEFS_H__
#define __ORIONAPI_ERRDEFS_H__

/*
 * Each OrionAPI Sub-library has a defined error definition range.
 * Note that not all numbers from these ranges will be used, this is just
 * to easily differentiate which sub-library caused an error.
 **********************************
 ** Orion Errors    :  0 - 99    **
 ** OrionAPI Errors :  100 - 199 **
 ** OSL Errors      :  200 - 299 **
 ** OKit Errors     :  300 - 399 **
 **********************************
 */
 
/*** Orion Errors ***/

/*** OrionAPI Errors ***/

#define OERR_NOTNATIVE      (100)
#define OERR_INVALIDAPPNAME (101)
#define OERR_INVALIDAPPID   (102)
#define OERR_CANTMALLOC     (103)

/*** OSL Errors ***/

/*** OKit Errors ***/
#define OERR_WIDGET_INVALID_FLAG (300)
#define OERR_GRAPHICS_NOT_INITED (301)


#endif /* !__ORIONAPI_ERRDEFS_H__ */

#endif /* ORION_INTERNAL */