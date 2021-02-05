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

#ifndef __ORION_OKIT_CEVENTARRAY_H__
#define __ORION_OKIT_CEVENTARRAY_H__

#include "signals.h"
#include "OSignal.hpp"
#include "CSignalListener.hpp"

namespace Orion{
	/* Internal. Essentially a contained array of CSignalListeners. */
	struct CEvent{
		/* The array of CSignalListeners. */
		CSignalListener* arr;
		/* The current count of valid CSignalListeners. */
		unsigned short count;
		/* The maximum amount of valid CSignalListeners before resize. */
		unsigned short cap;

		/* Empty constructor. Sets all values to 0. */
		CEvent(void);
		/* Sets all values to 0. */
		void clear(void);
	};

	struct CEventArray{
		/* The array of CEvents. */
		CEvent* arr;
		/* The count of valid CEvents. */
		unsigned short count;
		/* The maximum amount of valid CEvents before resize. */
		unsigned short cap;
		/* The amount to increase the array size during resize. */
		unsigned char step;

		/* Empty constructor. Sets all values to 0. */
		CEventArray(void);
		/* Destructor. Frees all memory. */
		~CEventArray(void);

		/* Initialiser. Sets up the array with the given parameters. */
		bool init(unsigned short cap, unsigned char step);
		/* Internal. Resizes the current array to the given size. */
		bool resize(unsigned short size);

	/* CEvent handling */

		/* Creates an event that this array will handle and returns a pointer to it. */
		CEvent* createEvent(unsigned short cap);
		/* Resizes an event owned by this array to the given size. */
		bool resizeEvent(CEvent*,unsigned short size);
		/* Links a CSignalListener to a given event handled by this array. */
		bool link(CEvent*,CSignalListener&);
		/* Unlinks a CSignalListener to a given event handled by this array. */
		bool unlink(CEvent*,CSignalListener&);
	};
}


#endif /* !__ORION_OKIT_CEVENTARRAY_H__ */