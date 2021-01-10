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

#include "CContext.hpp"
#include "CNodeArray.hpp"

namespace Orion{
	class CContainable;	/* Forward declaration, see "CContainable.hpp" for proper declaration. */

	/* Deferable abstract class that acts as a wrapper for CNodeArray. */
	class CContainer{
		protected:
			/* The amount of children the container has. */
			unsigned short childCount;
			/* The context the container draws to internally. Would exist on top of another context or container. */
			CContext selfContext;
			/* The array of children. */
			CNodeArray children;
		public:
			/* The context that children are meant to use. This is different per container. */
			CContext* contextToUse;
			/* The abstract function that all children use to connect. */
			virtual bool link(CContainable*);
			/* The abstract function that all children use to disconnect. */
			virtual bool unlink(CContainable*);	
			/* The abstract function that finds the index of a given child. */
			virtual int getIndexOf(CContainable*);	
			/* Empty constructor. Sets all values to 0. */
			CContainer(void);
			/* Internal. Top-level constructor. Does NOT initialise other values! Only creates X Window on display. */
			CContainer(int x, int y, unsigned int w, unsigned int h, const char* t, OCol* col, CXMask mask);
			/* Destructor. Sets all values to 0 and frees memory. */
			~CContainer(void);
	};
}

#endif /* !__ORION_OKIT_CCONTAINER_H__ */