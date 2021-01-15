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

#ifndef __ORION_OKIT_CSIGNALISTENER_H__
#define __ORION_OKIT_CSIGNALISTENER_H__

#include "signals.h"
#include "OSignal.hpp"

namespace Orion{
	enum CSignalListenerType{
		CSLT_NONE,
		CSLT_VOID,
		CSLT_SIGNALER
	};

	/* Internal. Container struct for function pointers. */
	struct CSignalListener{
		/* The type of function pointer this listener contains. */
		CSignalListenerType type;
		/* The mask of this listener. This is handled within an external event loop, not by the listener. */
		OMask mask;
		/* Union of the pointer used. This is dictated by the CSignalListenerType. */
		union{
			void(*VOID)(void);
			void(*SIGNALER)(OSignal);
		}ptr;

		/* Empty constructor. Sets all values to 0. */
		CSignalListener(void);
		/* Sets all values to 0. */
		void clear(void);
		/* Composes with a VOID-accepting/returning function. */
		void compose(void(*func)(void),OMask&);
		/* Composes with an OSignal-accepting function. */
		void compose(void(*func)(OSignal),OMask&);
		/* Compares the two CSignalListeners. Returns true if they're identical, false if not. */
		bool compare(CSignalListener&);

		/* Calls all functions and sends a copy of the referenced OSignal to the functions which accept them. Otherwise calls emptily. */
		void call(OSignal&);
	};
	
}


#endif /* !__ORION_OKIT_CSIGNALISTENER_H__ */