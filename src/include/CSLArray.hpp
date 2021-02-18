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

#ifndef __ORIONAPI_CSLARRAY_H__
#define __ORIONAPI_CSLARRAY_H__

#include "CSignalListener.hpp"

namespace Orion{
	/* A wrapper for an array of SignalListeners. */
	struct CSLArray{
		/* The internal array of Listeners. */
		CSignalListener* arr;
		/* The current count of Listeners. */
		unsigned short count;
		/* The maximum amount of Listeners before the internal array needs to resize. */
		unsigned short cap;
		/* The amount at which the internal array resizes by. */
		unsigned char step;

		/* Destructor. Frees all memory. */
		~CSLArray(void);
		/* Empty constructor. Sets all values to 0. */
		CSLArray(void);

		/* Initialises an SLArray with the given parameters. */
		bool init(unsigned short cap, unsigned char step);
		/* Internal, resizes the internal array to the given size. */
		bool resize(unsigned short);

		/* Connects a Listener to this array. */
		bool connect(CSignalListener&);
		/* Disconnects Listener from this array. */
		bool disconnect(CSignalListener&);
	};
}

#endif /* !__ORIONAPI_CSLARRAY_H__ */
