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
#include <unistd.h>
#include <X11/Xlib.h>
#include "include/OKit.hpp"
using namespace Orion;

int border=8;

CContext context;
CContext context2;

void myFunc(void* listener, X::CXEvent* event){
	printf("Printing from event listener! Values | Listener %p | Event %p\n",listener,event);
	if(event->type==X::CXE_MOUSECLICK&&event->mouse.button==1){
		OCol c;
		if(event->mouse.pressed){c.setTo(30,27,27);}else{c.setTo(255,86,15);}
		((CContext*)listener)->setCol(&c);
	}else if(event->type==X::CXE_CONFIGURE){
		context2.setSize(event->configure.w-(border*2*OAPP_SCALE),event->configure.h-(border*2*OAPP_SCALE),false);
	}
}

// void myFunc2(void* listener, void* event){
	// printf("Printing from event listener 2 2 2! Values | Listener %p | Event %p\n",listener,event);
// }

int main(){
	/* This ends up leading to a "~/.local/share/My OApp" directory being made, and having spaces in directories or filenames is not a good idea. */
	OKitStart("My OApp");
	
	OCol col(255,86,15);
	OCol col2(30,27,27);

	// context.init(0,100,100,400,350,"My OApp",&col,ButtonPressMask|ButtonReleaseMask|StructureNotifyMask,CCT_TOPLEVEL,true);
	// context.listener=&context;
	// context.listenerFunc=myFunc;
	// context2.init(&context,border,border,400-border*2,350-border*2,0,&col2,0,CCT_TOPLEVEL,true);

	CContainer r(0,0,400,350,"My OApp",&col,0);

	OContainer c(r,0,0,100,100);
	
	// context2.listenerFunc=myFunc2;

	OKitEventLoop();
}
