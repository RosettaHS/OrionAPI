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

#include <stdlib.h>
#include <X11/Xlib.h>
#include "include/errdef.hpp"
#include "include/OKit/xservice.hpp"
#include "include/OKit/CXEvent.hpp"
#include "include/OKit/CContext.hpp"

namespace Orion{
	namespace X{
		void* DPY=0;
		unsigned long ROOT=0;
		int SCR=0;

		bool connect(void){
			if(DPY){return false;}
			DPY=XOpenDisplay(0);
			if(!DPY){OLog("OKIT | ERROR! FAILED TO CONNECT TO X DISPLAY!\n"),exit(OERR_X11_CONNECTION_FAILURE);return false;}
			SCR=DefaultScreen(DPY);
			ROOT=RootWindow(DPY,SCR);
			return true;
		}

		bool disconnect(void){
			if(!DPY){return false;}
			XCloseDisplay((Display*)DPY);
			DPY=0,ROOT=0,SCR=0;
			return true;
		}

		void eventLoop(void){
			if(CXHA_COUNT==0){return;}
			XEvent event;
			CContext* context;
			CXEvent wrapper;
			while(CXHA_COUNT){
				XNextEvent((Display*)DPY,&event);
				context=CXHA_GETFROMXID(event.xany.window);
				if(context&&context->listenerFunc){
					wrapper.compose(&event);
					if(wrapper.valid){ context->listenerFunc(context->listener,&wrapper); }
					// wrapper.log();
				}
			}
		}
	}
}