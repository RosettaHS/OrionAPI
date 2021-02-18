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

#ifndef __ORIONAPI_OKIT_CNODEARRAY_H__
#define __ORIONAPI_OKIT_CNODEARRAY_H__

#include "CDrawable.hpp"

namespace Orion{
	/* Internal. Holds child elements for Containers. */
	class CNodeArray{
		public:
			/* Internal. Array of child pointers. */
			CDrawable** arr;
			/* Internal. Current amount of children. */
			unsigned short count;
			/* Internal. Maximum amount of children before resize. */
			unsigned short cap;
			/* Internal. Amount to increase array size during resize. */
			unsigned char step;
			/* Internal. Resizes array to set size. Returns true if successful. */
			bool resize(unsigned short size);

			/* Destructor. Destroys all data. */
			~CNodeArray(void);
			/* Empty constructor. Sets all values to 0. */
			CNodeArray(void);
			/* Initialises a CNodeArray with the given parameters. */
			bool init(unsigned short cap, unsigned char step);		
				
			/* Links a CDrawable to the Node Array. Returns true if successful. */
			bool link(CDrawable*);
			/* Unlinks a CDrawable to the Node Array. Returns true if successful. */
			bool unlink(CDrawable*);
			/* Unlinks all CDrawables. Use with caution! */
			void clear(void);
			/* Finds and returns the index of a given CDrawable in the Node Array. Returns -1 on error. */
			int getIndexOf(CDrawable*);
			/* Gets the child count of Node Array. */
			unsigned short getCount(void);								
			/* Draws all children of the Node Array. */
			void drawAll(void);
			/* Internal/DEBUG. Logs all child data to terminal. */
			void log(void);	
		};
}

#endif /* !__ORIONAPI_OKIT_CNODEARRAY_H__ */
