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

#ifndef __ORION_OKIT_CDRAWABLE_H__
#define __ORION_OKIT_CDRAWABLE_H__

#include "CBaseUI.hpp"
#include "CContext.hpp"

namespace Orion{
	/* Abstract class for all drawable UI elements. */
	class CDrawable : public CBaseUI{
		protected:
			/* The X and Y positional coordinates of the Drawable. */
			int x,y;
			/* The Width and Height (size) of the Drawable. */
			unsigned int w,h;
			/* The minimum allowed width and height of the Drawable. Used in Containables. */
			unsigned int minW,minH;
			/* The Context that the Drawable can draw to. */
			CContext* context;
		public:
			/* Pointer to the draw function that the deferred Drawable will use. Takes in the Drawable as an argument. */
			void(*drawPtr)(CDrawable* obj);
			/* Virtual method, sets the position of the Drawable and does additional computation that the deferred class may require. */
			virtual void setPos(int x, int y) = 0;
			/* Virtual method, sets the size of the Drawable and does additional computation that the deferred class may require. */
			virtual void setSize(unsigned int w, unsigned int h) = 0;
			/* Virtual method, sets the minimum size allowed on the Drawable and does additional computation that the deferred class may require. */
			virtual void setMinSize(unsigned int w, unsigned int h) = 0;
	};
}

#endif /* ! __ORION_OKIT_CDRAWABLE_H__ */