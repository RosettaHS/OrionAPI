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

#ifndef __ORION_OKIT_OCONTAINER_H__
#define __ORION_OKIT_OCONTAINER_H__

#include "CContainer.hpp"
#include "CContainable.hpp"

namespace Orion{
	namespace X{
		void OContainer_EVH(void* container, CXEvent* event);
		void OContainer_DRAW(CDrawable* container);
	}

	/* Base container. UI Elements can be placed on this but are not modified by the container. */
	class OContainer : public CContainer, public CContainable{
		protected:
			OCol internalCol;
			OCol* col;
			virtual void sort(void) override;
		public:
			/* Empty constructor. Sets all values to 0.*/
			OContainer();
			/* Constructs an OContainer as a child of the first argument. */
			OContainer(CContainer& parent, int x, int y, unsigned int w, unsigned int h);

			friend void X::OContainer_EVH(void*,CXEvent*);
			friend void X::OContainer_DRAW(CDrawable* container);
			
			/* Sets the background colour of the OContainer. */
			void setCol(unsigned char r, unsigned char g, unsigned char b);
			/* Sets the background colour of the OContainer. */
			void setCol(OCol& col);
			
			/* Adds the argument as a child of the OContainer. */
			virtual bool link(CContainable*) override;
			/* Removes the argument from child list of the OContainer. */
			virtual bool unlink(CContainable*) override;	
			/* Finds the index of a given child. Returns -1 if argument is not a child of the OContainer. */
			virtual int getIndexOf(CContainable*) override;	

			/* Sets the position of the OContainer relative to its parent. */
			virtual void setPos(int x, int y) override;
			/* Sets the size of the OContainer. Parents may override this! */
			virtual void setSize(unsigned int w, unsigned int h) override;
			/* Sets the minimum size of the OContainer. Parents can't shrink the OContainer smaller than this. */
			virtual void setMinSize(unsigned int w, unsigned int h) override;
	};

	
}

#endif /* !__ORION_OKIT_OCONTAINER_H__ */