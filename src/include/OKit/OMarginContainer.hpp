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

#ifndef __ORIONAPI_OKIT_OMARGINCONTAINER_H__
#define __ORIONAPI_OKIT_OMARGINCONTAINER_H__

#include "OContainer.hpp"

namespace Orion{
	namespace DRAW{ extern void OMarginContainer(CDrawable*); }

	/* A Container for UI elements that has a margin around the edges. */
	class OMarginContainer : public OContainer{ friend void DRAW::OMarginContainer(CDrawable*);
		protected:
			/* The margin along the left side. */
			unsigned short margin_left;
			/* The margin along the right side. */
			unsigned short margin_right;
			/* The margin along the top. */
			unsigned short margin_top;
			/* The margin along the bottom. */
			unsigned short margin_bottom;

			/* Internal. This method is called when this Container is linked from its parent. */
			virtual void onLink(void) override;
			/* Internal. This method is called when this Container is unlinked from its parent. */
			virtual void onUnlink(void) override;
			/* Internal. This method is called when position is modified. */
			virtual void onPosChanged(void) override;
			/* Internal. This method is called when size is modified. */
			virtual void onSizeChanged(void) override;
			/* Sorts the children of this Container. Automatically calls whenever the Container redraws. */
			virtual void sort(void) override;
		public:
			/* Empty constructor. Sets all values to 0. */
			OMarginContainer(void);
			/* Creates and links a MarginContainer to the passed parent with the given parameters. A 6th argument can be passed to set the default margin. */
			OMarginContainer(CContainer& parent, int x, int y, unsigned int w, unsigned int h, unsigned short margin=8);

			/* Sets the margin across all sides to the passed value. */
			void setMargin(unsigned short);
			/* Sets the margin along the left side to the passed value. */
			void setLeftMargin(unsigned short);
			/* Sets the margin along the right side to the passed value. */
			void setRightMargin(unsigned short);
			/* Sets the margin along the top to the passed value. */
			void setTopMargin(unsigned short);
			/* Sets the margin along the bottom to the passed value. */
			void setBottomMargin(unsigned short);

			/* Gets the margin along the left side. */
			unsigned short getLeftMargin(void);
			/* Gets the margin along the right side. */
			unsigned short getRightMargin(void);
			/* Gets the margin along the top. */
			unsigned short getTopMargin(void);
			/* Gets the margin along the bottom. */
			unsigned short getBottomMargin(void);
	};
}

#endif /* !__ORIONAPI_OKIT_OMARGINCONTAINER_H__ */
