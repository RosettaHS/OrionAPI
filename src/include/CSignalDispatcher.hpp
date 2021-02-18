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

#ifndef __ORIONAPI_CSIGNALDISPATCHER_H__
#define __ORIONAPI_CSIGNALDISPATCHER_H__

#include "CSLArray.hpp"

namespace Orion{
	/* Deferrable class for anything that emits events. */
	class CSignalDispatcher{
		protected:
			/* The event manager itself. */
			CSLArray SIGMGR;

			/* Runs through the event manager and calls the linked functions that match a bit on the passed mask. */
			void emit(OMask, OSignal&);
		public:
			/* Links a function to be called by a specific event mask on the given UI element. */
			virtual bool connect(OMask&, void(*func)(void));
			/* Links a function to be called by a specific event mask on the given UI element. */
			virtual bool connect(OMask&, void(*func)(OSignal));

			/* Unlinks a function with the given event mask from the UI element. */
			virtual bool disconnect(OMask&, void(*func)(void));
			/* Unlinks a function with the given event mask from the UI element. */
			virtual bool disconnect(OMask&, void(*func)(OSignal));
	};
}

#endif /* !__ORIONAPI_CSIGNALDISPATCHER_H__ */
