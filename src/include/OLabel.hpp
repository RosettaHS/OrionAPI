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

#ifndef __ORIONAPI_CLABEL_H__
#define __ORIONAPI_CLABEL_H__

#include "OString.hpp"
#include "CDrawable.hpp"
#include "CContainer.hpp"
#include "CLabel.hpp"

namespace Orion{
	namespace DRAW{ extern void OLabel(CDrawable*); }
	namespace HANDLE{ extern void OLabel(void*, X::CXEvent*); }

	/* A Rectangle that contains and displays your text. Similar to ORect. */
	class OLabel : public CDrawable{ friend void DRAW::OLabel(CDrawable*); friend void HANDLE::OLabel(void*, X::CXEvent*);
		protected:
			/* The text being displayed. */
			const char* string;
			/* The font being used by this Label. */
			const char* font;
			/* Internal. The actual text on the screen. */
			CLabel XLABEL;
			/* A pointer to the Colour that will be used during rendering. */
			OCol* textCol;
			/* The actual Colour that textCol points to if you set a Colour manually. */
			OCol internalTextCol;

			/* Internal. The actual Rectangle where your text will be displayed on. */
			CContext rect;

			/* Internal. Calls this method when linked. */
			virtual void onLink(void) override;
			/* Internal. Calls this method when unlinked. */
			virtual void onUnlink(void) override;
			/* Internal. Calls this method when position is modified. */
			virtual void onPosChanged(void) override;
			/* Internal. Calls this virtual method when position is modified. */
			virtual void onSizeChanged(void) override;
		public:
			/* Empty constructor. Sets all values to 0. */
			OLabel(void);
			/* Creates and initialises a Label with the given parameters.
				Pass the text you want to be displayed as the 6th parameter, or the colour you want the text to be as the 7th parameter. */
			OLabel(CContainer&, int x, int y, unsigned int w, unsigned int h, const char* label=0, OCol& labelCol=OTHEME_ACCENT);

			/* Currently Unused. Sets the font of the text to be displayed. */
			void setFont(const char*);
			/* Sets the text to be displayed. */
			void setText(const char*);
			/* Sets the colour of the text to the given parameters. */
			virtual void setCol(unsigned char r, unsigned char g, unsigned char b) override; virtual void setCol(OCol&) override;
			/* Sets the colour of the background Rectangle to the given parameters. */
			void setBackgroundCol(unsigned char r, unsigned char g, unsigned char b); void setBackgroundCol(OCol&);
	};
}

#endif /* !__ORIONAPI_OLABEL_H__ */
