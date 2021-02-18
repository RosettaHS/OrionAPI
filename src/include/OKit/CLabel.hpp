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

#ifndef __ORIONAPI_OKIT_CXLABEL_H__
#define __ORIONAPI_OKIT_CXLABEL_H__

#include "CContext.hpp"

#ifdef ORION_INTERNAL

	#define __DEFAULTFONT "-misc-fixed-medium-r-normal--%u-*-*-*-*-*-iso8859-15"
	#define __DEFAULTFONTSIZE 12
	#define __SCALEFONTSIZE(x,y) ((unsigned short)((float)x*y))

#endif /* ORION_INTERNAL */

namespace Orion{
	/* Internal. Higher-level abstraction for basic text rendering with X. */
	class CLabel{
		public:
			/* The name of the font being used. Fallback is "fixed". */
			const char* XFONTNAMERAW;
			/* The name of the font (size modified) being used. Fallback is "fixed". */
			char* XFONTNAME;
			/* A pointer to the XFontStruct owned by this Label. */
			void* XFONT;
			/* The ID of the font stored in XFONT. */
			unsigned long XFID;
			/* The colour of the text rendered by X. */
			unsigned long XCOL;
			/* The size of the font being used (without OAPP_SCALE) */
			unsigned short XFONTSIZE;
			/* A pointer to an X Graphics Context owned by this Label. */
			void* XGC;
			/* The X and Y positions of the raw text relative to its parent Context. */
			int XT_X, XT_Y;
			/* The direction (right/left), ascent above baseline, and descent below baseline respectively. */
			int XT_DIR, XT_ASC, XT_DES;

			/* Destructor. Frees all memory. */
			~CLabel(void);
			/* Empty constructor. Sets all values to 0. */
			CLabel(void);

			/* Initialises (but does not draw) this Label with the following parameters. */
			bool init(CContext* drawBody, OCol* col, const char* font, unsigned short fontSize);
			/* Destroys this Label and frees all memory. Called by the destructor. */
			bool destroy(void);
			/* Changes the current font to the given font. Returns false if unsuccessful. */
			bool setFont(const char* font, unsigned short fontSize);
			/* Renders the text on the given Context with the given parameters. */
			void draw(CContext* drawBody, OCol* col, unsigned int contextWidth, unsigned int contextHeight, const char* string, unsigned short fontSize);
	};
}

#endif /* !__ORIONAPI_OKIT_CXLABEL_H__ */
