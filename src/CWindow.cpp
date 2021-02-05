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

#include <X11/Xlib.h>
#include <stdlib.h>
#include "include/errdef.h"
#include "include/xservice.hpp"
#include "include/application.hpp"
#include "include/OLog.hpp"
#include "include/CWindow.hpp"
#include "include/OService.hpp"

#define _OWINDOW_DEFMASK StructureNotifyMask|ButtonPressMask

namespace Orion{

	CWindow::~CWindow(void){
		type=OT_ERROR;
		minW=400,minH=350;
		title=0;
		ready=false;
	}

	CWindow::CWindow(void){
		type=OT_OWINDOW;
		minW=400,minH=350;
		title=0;
		ready=false;
	}

	CWindow::CWindow(int _x, int _y, unsigned int _w, unsigned int _h, const char* _t){
		OXONLY{
			type=OT_OWINDOW;
			OVec _screenSize=OScreenGetSize();
		/* Variable Initialisation */
			title=_t;
			w=_w,h=_h;
			if(_w<minW){minW=_w;}
			if(_h<minH){minH=_h;}
			
			switch(_x){
				default:{ x=_x; break; }
				case START:{ x=0; break; }
				case END:{ x=((_screenSize.x)-(_w)*OAPP_SCALE); break; }
				case CENTER:{ x=((_screenSize.x/2)-(_w/2)*OAPP_SCALE); break; }
				case FILL:{ x=0; OLog("OKIT | WARNING! CAN'T SET X POSITION WITH FILL FLAG!\n"); break;}
			}

			switch(_y){
				default:{ y=_y; break; }
				case START:{ y=0; break; }
				case END:{ y=((_screenSize.y)-(_h)*OAPP_SCALE); break; }
				case CENTER:{ y=((_screenSize.y/2)-(_h/2)*OAPP_SCALE); break; }
				case FILL:{ y=0; OLog("OKIT | WARNING! CAN'T SET Y POSITION WITH FILL FLAG!\n"); break;}
			}
		/* Context Initialisation */
			if(selfContext.init(0,x,y,w,h,title,theme.accent,_OWINDOW_DEFMASK,CCT_TOPLEVEL,true,true)){
				selfContext.listener=(void*)this;
				selfContext.listenerFunc=HANDLE::CWindow;
				// drawPtr=DRAW::OWindow;

				context=&selfContext;
				contextToUse=&selfContext;
				drawableToUse=(CDrawable*)this;
				containerToUse=(CContainer*)this;
				arr.init(5,1);
				ready=true;
			}else{type=OT_ERROR;}
			
			
		}else{
			OLog("OKIT | ERROR! FAILED TO CREATE OWINDOW(%d %d %u %u %s) BECAUSE X IS NOT INITIALISED!\n",_x,_y,_w,_h,_t);
			type=OT_ERROR;
			exit(OERR_X11_NOT_INITED);
		}
	}

	void CWindow::sort(void){
		arr.drawAll();
	}

	// namespace DRAW{
		// void OWindow(CDrawable* obj){
			// Orion::OWindow* win=(Orion::OWindow*)obj;
		// }
	// }

	namespace HANDLE{
		void CWindow(void* obj,X::CXEvent* event){
			Orion::CWindow* win=(Orion::CWindow*)obj;
			if(!win->ready){OLog("NOT READY!\n");return;}
			switch(event->type){
				default:{OLog("DEFAULT!\n");return;}
				// case X::CXE_EXPOSE:{ win->sort(); OLog("EXPOSE!\n"); return; }
				case X::CXE_CONFIGURE:{
					win->x=event->configure.x/OAPP_SCALE,win->y=event->configure.y/OAPP_SCALE;
					win->w=event->configure.w/OAPP_SCALE,win->h=event->configure.h/OAPP_SCALE;
					win->sort();
					return;
				}
				case X::CXE_MOUSECLICK:{
					OLog("Exiting\n");
					exit(0);
				}
			}
		}
	}
}