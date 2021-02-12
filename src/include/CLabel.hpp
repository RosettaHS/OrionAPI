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

#ifndef __ORION_OKIT_CLABEL_H__
#define __ORION_OKIT_CLABEL_H__

#include "OString.hpp"
#include "CDrawable.hpp"
#include "CContainer.hpp"

namespace Orion{
	namespace DRAW{ extern void CLabel(CDrawable*); }
	namespace HANDLE{ extern void CLabel(void*, X::CXEvent*); }

	class CLabel : public CDrawable{ friend void DRAW::CLabel(CDrawable*); friend void HANDLE::CLabel(void*, X::CXEvent*);
		protected:
			struct{
				OString string;
				const char* fontName;
				void* XFONT;
				void* XGC;
			}XTEXT;
			CContext rect;

			virtual void onLink(void) override;
			virtual void onUnlink(void) override;
			virtual void onPosChanged(void) override;
			virtual void onSizeChanged(void) override;
		public:
			CLabel(void);
			CLabel(CContainer&, int x, int y, unsigned int w, unsigned int h, const char* label=0);

			virtual void setCol(unsigned char r, unsigned char g, unsigned char b) override;
			virtual void setCol(OCol&) override;

			void setBackgroundCol(unsigned char r, unsigned char g, unsigned char b);
			void setBackgroundCol(OCol&);
	};
}

#endif /* !__ORION_OKIT_OLABEL_H__ */