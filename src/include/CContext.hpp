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

#ifndef __ORION_OKIT_CCONTEXT_H__
#define __ORION_OKIT_CCONTEXT_H__

#include "CXEvent.hpp"
#include "OCol.hpp"

/* Wrapper for masks in the X Event System. */
typedef unsigned long CXMask;

#ifdef ORION_INTERNAL

	#define _INTSCALEBYFLOAT(x,y) ((int)((float)x*y))
	#define _UINTSCALEBYFLOAT(x,y) ((unsigned int)((float)x*y))

#endif /* !ORION_INTERNAL */

namespace Orion{
	enum CCType{
		CCT_INPUTONLY,
		CCT_TOPLEVEL,
		CCT_DESKTOP,
		CCT_PANEL,
		CCT_TOOLTIP,
		CCT_MENU,
		CCT_ELEMENT
	};
	/* Internal. Higher-level abstraction of an X Window. */
	class CContext{
		public:
		/* X Information */
			/* The ID of the X Window on the screen. */
			unsigned long	XWIN;
			/* The ID of the root X Window the main XWIN is tied to. */
			unsigned long	XROOT;
			/* The colour of the X Window. */
			unsigned long	XCOL;
			/* The mask that the X Event system uses on the XWIN. */
			CXMask			XMASK;
			/* The title of the X Window. Can be blank. */
			const char*		XTITLE;
		/* Event Handling */
			/* The top-level object that is listening for events. Something like an OButton for example. */
			void* listener;
			/* A pointer to the function that works as a router on the top level object. Takes in the listner and the XEvent. */
			void (*listenerFunc)(void* l,X::CXEvent* e);
		/* General Handling */
			/* Empty constructor. Sets all values to 0. */
			CContext(void);
			/* Destructor. Frees all memory and unlinks from X. */
			~CContext(void);
			/* Initialiser. Call this with your given parameters after creating an empty CContext. */
			bool init(CContext* root, int x, int y, unsigned int w, unsigned int h, const char* t, OCol* col, CXMask mask,CCType type, bool useScale, bool link);
			/* Disconnects from the X service. */
			bool destroy(void);
			/* Changes the colour of the CContext to the given OCol. */
			void setCol(OCol*);
			/* Changes the title of the CContext if it is a top-level Context. */
			void setTitle(const char*);
			/* Changes the position of the CContext relative to the root Context. */
			void setPos(int, int, bool useScale);
			/* Changes the size of the CContext. */
			void setSize(unsigned int, unsigned int, bool useScale);
			/* Changes both the position and size of the CContext. */
			void setGeometry(int, int, unsigned int, unsigned int, bool useScale);
			/* Reparents with another CContext. */
			void reparent(CContext* root, int x, int y, bool useScale);
	};

	namespace X{
		/* Internal. This is how the event manager gets the Context from the X service. */
		struct CXHANDLE{
			unsigned long XWIN;
			CContext* context;
		};
		/* Internal. This is a global array of all current XHandles. */
		extern CXHANDLE* CXHA;
		/* Internal. Current count of XHandles. */
		extern unsigned long CXHA_COUNT;
		/* Internal. The current cap of XHandles before the array gets resized by the step size provided in 'CContext.cpp". */
		extern unsigned long CXHA_CAP;

		/* Internal. Initialises the XHandle Array. Returns true on success, terminates program upon failure (and returns false if that fails). */
		extern bool CXHA_INIT(void);
		/* Internal. Links a Context to an XHandle and adds that to the array. Returns true on success. */
		extern bool CXHA_LINK(CContext*);
		/* Internal. Unlinks a Context from the XHandle Array. Returns true on success, false if wasn't present. */
		extern bool CXHA_UNLINK(CContext*);
		/* Internal. Returns a Context* from an XID provided by the X event system. */
		extern CContext* CXHA_GETFROMXID(unsigned long XWIN);
	}
}

#endif /* !__ORION_OKIT_CCONTEXT_H__ */