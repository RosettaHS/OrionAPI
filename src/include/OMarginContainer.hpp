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

#ifndef __ORION_OKIT_OMARGINCONTAINER_H__
#define __ORION_OKIT_OMARGINCONTAINER_H__

#include "OContainer.hpp"

namespace Orion{
	namespace DRAW{ void OMarginContainer(CDrawable*); }

	class OMarginContainer : public OContainer{ friend void DRAW::OMarginContainer(CDrawable*);
		protected:
			unsigned short margin_left,margin_right,margin_top,margin_bottom;

			virtual void sort(void) override;
			virtual void onLink(void) override;
			virtual void onUnlink(void) override;
			virtual void onPosChanged(void) override;
			virtual void onSizeChanged(void) override;
		public:
			OMarginContainer(void);
			OMarginContainer(CContainer& parent, int x, int y, unsigned int w, unsigned int h, unsigned short margin=8);

			void setMargin(unsigned short);
			void setLeftMargin(unsigned short);
			void setRightMargin(unsigned short);
			void setTopMargin(unsigned short);
			void setBottomMargin(unsigned short);

			unsigned short getLeftMargin(void);
			unsigned short getRightMargin(void);
			unsigned short getTopMargin(void);
			unsigned short getBottomMargin(void);
	};
}

#endif /* !__ORION_OKIT_OMARGINCONTAINER_H__ */