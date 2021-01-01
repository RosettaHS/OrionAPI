/**********************************************************************************/
/*                                                                                */
/*                              Copyright (c) 2021                                */
/*                           Rosetta H&S International                            */
/*                                                                                */
/* Permission is hereby granted, free of charge, to any person obtaining a copy   */
/* of this software and associated documentation files (the "Software"), to deal  */
/* in the Software without restriction, including without limitation the rights   */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      */
/* copies of the Software, and to permit persons to whom the Software is          */
/* furnished to do so, subject to the following conditions:                       */
/*                                                                                */
/* The above copyright notice and this permission notice shall be included in all */
/* copies or substantial portions of the Software.                                */
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

#ifndef __ORION_OKIT_CCONTEXT_H__
#define __ORION_OKIT_CCONTEXT_H__

#include "OCol.hpp"

/* Wrapper for masks in the X Event System. */
typedef unsigned long CXMask;

namespace Orion{
	/* Internal. Higher-level abstraction of an X Window. */
	class CContext{
		public:
		/* X Information */
			/* The ID of the X Window on the screen. */
			unsigned long	XWIN;
			/* The ID of the root X Window the main XWIN is tied to. */
			unsigned long	XROOT;
			/* The mask that the X Event system uses on the XWIN. */
			CXMask			XMASK;
			/* The title of the X Window. Can be blank. */
			const char*		XTITLE;
		/* Event Handling */
			/* The top-level object that is listening for events. Something like an OButton for example. */
			void* listener;
			/* A pointer to the function that works as a router on the top level object. Takes in the listner and the XEvent. */
			void (*listenerFunc)(void* l,void* e);
		/* General Handling */
			/* Empty constructor. Sets all values to 0. */
			CContext();
			/* Destructor. Frees all memory and unlinks from X. */
			~CContext();
			/* */
			CContext(CContext* root, int x, int y, unsigned int w, unsigned int h, OCol* col, CXMask mask, bool useScale);
	};
}

#endif /* !__ORION_OKIT_CCONTEXT_H__ */