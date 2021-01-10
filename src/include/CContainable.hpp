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

#ifndef __ORION_OKIT_CCONTAINABLE_H__
#define __ORION_OKIT_CCONTAINABLE_H__

#include "CDrawable.hpp"
#include "CContainer.hpp"

namespace Orion{
	/* The base class that all Containable objects inherit from. */
	class CContainable : public CDrawable {
		protected:
			/* The index of the object. Is -1 if not linked. */
			int index;
		public:
			/* Links an object to a given container. */
			virtual bool linkTo(CContainer*);
			/* Unlinks an object from a given container. */
			virtual bool unlinkTo(CContainer*);
			/* Returns the index of the object. Is kept protected to prevent tampering. */
			int getIndex(void);
			/* Empty constructor. Sets all values to 0. */
			CContainable(void);
	};
}

#endif /* !__ORION_OKIT_CCONTAINABLE_H__ */