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

#ifndef __ORIONAPI_OKIT_CORE_H__
#define __ORIONAPI_OKIT_CORE_H__

#include <stdint.h>

/*
 * This SHOULD be using RenderKit, but for now it's going to interface directly with XCB until I can figure out
 * what the hell I'm going to do with RenderKit.
 */

namespace Orion{

#ifdef ORION_INTERNAL
	extern void*    XCB_CON;
	extern void*    XCB_SCR;
	extern int      XCB_SID;
	extern uint32_t XCB_ROOT;
	extern bool     XCB_CONNECTED;

	/* Wrapper if() statement. Will only run the code in the brackets if XCB has been successfully initalised. */
	#define XONLY if(XCB_CONNECTED)
	/*** The following #defines require the inclusion of <xcb/xcb.h> ***/

	/* Casts the XCB connection pointer to a usable xcb_connection_t pointer. */
	#define XCON  ((xcb_connection_t*)XCB_CON)
	/* Casts the RenderKit screen pointer to a usable xcb_screen_t pointer. */
	#define XSCR  ((xcb_screen_t*)XCB_SCR)
	/* Retrieves the XCB connection screen ID. */
	#define XSID  (XCB_SID)
	/* Retrieves the XCB connection root context ID. */
	#define XROOT (XCB_ROOT)

	/**
	 * @brief Internal. Connects OrionAPI to the X Service.
	 * @return True if connection was successful, otherwise false if either connection could not be made, or service is already running.
	 */
	extern bool XCB_CONNECT(void);
	/**
	 * @brief Internal. Disconnects OrionAPI from the X Service.
	 * @return True if disconnection was successful, otherwise false if either connection could not be severed, or service isn't running.
	 */
	extern bool XCB_DISCONNECT(void);

	/* Flushes and sends any pending requests to the X Server. */
	extern void XCB_FLUSH(void);
#endif

}

#endif /* !__ORIONAPI_OKIT_CORE_H__ */