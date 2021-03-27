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

#define ORION_INTERNAL

#include <xcb/xcb.h>
#include "../include/OSL/OLog.hpp"
#include "../include/OKit/core.hpp"

namespace Orion{
	bool XCB_CONNECT(void){
		if(XCB_CONNECTED){ return false; }
		XCB_CON=xcb_connect(0,&XCB_SID);
		if(xcb_connection_has_error(XCON)){ OWLog(true,"X CONNECTION COULD NOT BE ESTABLISHED! RUNNING AS HEADLESS!"); return false; }
		/* TODO: Replace this with a cleaner solution. XCB's documentation isn't very friendly. */
		xcb_screen_iterator_t scrite=xcb_setup_roots_iterator( xcb_get_setup(XCON) );
		for(; scrite.rem; ( --XCB_SID, xcb_screen_next(&scrite) ) ){
			if(!XCB_SID){ XCB_SCR=scrite.data; break; }
		}
		XCB_ROOT=XSCR->root;
		XCB_CONNECTED=true;
		return true;
	}
	
	bool XCB_DISCONNECT(void){
		if(!XCB_CONNECTED){ return false; }
		xcb_disconnect(XCON);
		XCB_CON=0;
		XCB_SCR=0;
		XCB_SID=0;
		XCB_ROOT=0;
		XCB_CONNECTED=0;
		return true;
	}
}