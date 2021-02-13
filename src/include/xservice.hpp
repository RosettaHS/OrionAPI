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

#ifndef __ORION_OAPI_XSERVICE_H__
#define __ORION_OAPI_XSERVICE_H__

#ifdef ORION_INTERNAL

	/* Quick re-route to if(Orion::X::DPY). Add a scope and do any calls in this that you only want to occur if X initialises. */
	#define OXONLY if(Orion::X::DPY)

	/* Quick re-route to Orion::X::DPY. Use this to get the X Display connection instead of writing out the namespaces. */
	#define OXDPY (Display*)Orion::X::DPY
	/* Quick re-route to Orion::X::ROOT. Use this to get the X root window (window manager) instead of writing out the namespaces. */
	#define OXROOT Orion::X::ROOT
	/* Quick re-route to Orion::X::SCR. Use this to get the current X Screen instead of writing out the namespaces. */
	#define OXSCR Orion::X::SCR

#endif /* !ORION_INTERNAL */

namespace Orion{
	namespace X{
		/* The connection to the X server, used in every graphics call. */
		extern void* DPY;
		/* The root window (window manager), used when creating an OWindow, or other top level Contexts. */
		extern unsigned long ROOT;
		/* The X Screen to use in certain graphics calls. */
		extern int SCR;
		/* Initialises a connection to the X display. Returns true on success, false if the X connection is already initalised, or crashes the program if X is not found. */
		extern bool connect(void);
		/* Disconnects from the X service. Returns true on succcess, false if couldn't disconnect or X connection hasn't been initialised. */
		extern bool disconnect(void);
		/* Minimal event system. Blocks the program and dispatches events from the X service. */
		extern void eventLoop(void);
	}
}

#endif /* !__ORION_OAPI_XSERVICE_H__ */
