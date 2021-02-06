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

#ifndef __ORION_OKIT_CCONTAINER_H__
#define __ORION_OKIT_CCONTAINER_H__

#include "CDrawable.hpp"
#include "CNodeArray.hpp"

typedef Orion::CDrawable** OChildList;

namespace Orion{
	class CContainer{
		protected:
			/* Internal. The Context that this Container creates to draw its content on. */
			CContext selfContext;
			/* Internal. The array of Drawables owned by this Container. */
			CNodeArray arr;
			/* Internal. The amount of Drawables owned by this Container. */
			unsigned short childCount;
			/* Internal. The Context that child Drawables will link to when using this Container. */
			CContext* contextToUse;
			/* Internal. The Drawable that child Drawables will link to when using this Container. */
			CDrawable* drawableToUse;
			/* Internal. The Container that Drawables will link to when using this Container. Some Containers are made of nested Containers so this is necessary. */
			CContainer* containerToUse;
			/* Internal. If this is true, then this Container will ignore containerToUse and just use itself next time link() or unlink() is called. */
			bool forceSelfOnNext;
			/* Allows Drawables to access internal members of this Container. */
			friend class CDrawable;
			/* Calls when this Container gets unlinked. Temporarily unlinks all children but stores them in memory to relink later. */
			void tempUnlinkAll(void);
			/* Calls when this Container gets relinked. Links all children stored in memory back to this Container. */
			void tempRelinkAll(void);
			/* Sorts the children of this Container. Automatically calls whenever the Container redraws. */
			virtual void sort(void);
		public:
			/* Destructor. Unlinks all children. */
			~CContainer(void);
			/* Empty constructor. Sets all values to 0. */
			CContainer(void);

			/* Links a Drawable to this Container. Returns true on successful linkage. */
			bool link(CDrawable&);
			/* Unlinks a Drawable from this Container. Returns false if could not unlink Drawable. */
			bool unlink(CDrawable&);
			/* Unlinks all the children owned by this Container. Use with caution! */
			void unlinkAll(void);
			
			/* Returns the child index of the passed Drawable. Returns -1 if Drawable is not linked. */
			int getIndexOf(CDrawable&);
			/* Returns an array of children (Pointers to Drawables) owned by this Container. */
			OChildList getChildren(void);
			/* Returns the amount of children owned by this Drawable. */
			unsigned short getChildCount(void);
		
	};
}

#endif /* !__ORION_OKIT_CCONTAINER_H__ */