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

#ifndef __ORIONAPI_OKIT_OCONTAINER_H__
#define __ORIONAPI_OKIT_OCONTAINER_H__

#include "CDrawable.hpp"
#include "CContainer.hpp"

namespace Orion{
	namespace DRAW{ extern void OContainer(CDrawable*); }

	/* A Container for UI elements that does no automatic sorting. */
	class OContainer : public CDrawable, public CContainer{ friend void DRAW::OContainer(CDrawable*);
		protected:
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
			OContainer(void);
			/* Creates and links an OContainer to the passed parent with the given arguments */
			OContainer(CContainer& parent, int x, int y, unsigned int w, unsigned int h);

			/* This function does not work on Containers, and will result in a warning. Do not call this! */
			virtual void setScale(float) override;
			/* Sets the colour of this Container. */
			virtual void setCol(unsigned char r, unsigned char g, unsigned char b) override; virtual void setCol(OCol&) override;
	};
}

#endif /* !__ORIONAPI_OKIT_OCONTAINER_H__ */
