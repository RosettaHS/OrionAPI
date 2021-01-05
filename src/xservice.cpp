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

#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include "include/errdef.h"
#include "include/xservice.hpp"
#include "include/CContext.hpp"

namespace Orion{
	namespace X{
		void* 			DPY=0;
		unsigned long 	ROOT=0;
		int	 			SCR=0;

		bool connect(void){
			if(DPY){return false;}
			DPY=XOpenDisplay(0);
			if(!DPY){printf("OKIT | FATAL ERROR! FAILED TO CONNECT TO X DISPLAY!\n"),exit(OERR_X11_CONNECTION_FAILURE);return false;}
			SCR=DefaultScreen(DPY);
			ROOT=RootWindow(DPY,SCR);
			return true;
		}

		void eventLoop(void){
			XEvent event;
			CContext* context;
			while(CXHA_COUNT){
				XNextEvent((Display*)DPY,&event);
				context=CXHA_GETFROMXID(event.xany.window);
				if(context){context->listenerFunc(context->listener,(void*)&event);}
			}
		}
	}
}