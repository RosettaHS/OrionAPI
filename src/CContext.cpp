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
#include "include/application.hpp"
#include "include/xservice.hpp"
#include "include/CContext.hpp"

namespace Orion{
	CContext::~CContext(void){
		OXONLY{
			if(XWIN){
				XUnmapWindow(OXDPY,XWIN);
				XDestroyWindow(OXDPY,XWIN);
				X::CXHA_UNLINK(this);
				OVERB_OUT "OKIT | CContext %p destroyed.\n",this OVERB_END
			}
		}
	}
	CContext::CContext(void) : XWIN{0},XROOT{0},XMASK{0},XTITLE{0},listener{0},listenerFunc{0} {}

	bool CContext::init(CContext* root, int _x, int _y, unsigned int _w, unsigned int _h, const char* t, OCol* col, CXMask mask, bool useScale){
		XWIN=0,XROOT=0,XMASK=0,XTITLE=0,listener=0,listenerFunc=0;
		OXONLY{
			int x,y;
			unsigned int w,h;
			XMASK=mask;
			XTITLE=t;
			if(root){XROOT=root->XWIN;}else{XROOT=OXROOT;}
			if(useScale){
				if(root){x=(int)_x*OAPP_SCALE,y=(int)_y*OAPP_SCALE;}else{x=_x,y=_y;}
				w=(unsigned int)_w*OAPP_SCALE,h=(unsigned int)_h*OAPP_SCALE;
			}else{
				x=_x,y=_y;
				w=_w,h=_h;
			}

			XWIN=XCreateSimpleWindow(OXDPY,XROOT,x,y,w,h,0,col->XCOL,col->XCOL);
			if(XWIN){
				XSelectInput(OXDPY,XWIN,SubstructureNotifyMask | ExposureMask);
				if(t){XStoreName(OXDPY,XWIN,t);}
				XMapWindow(OXDPY,XWIN);
				XMoveResizeWindow(OXDPY,XWIN,x,y,w,h);
				XEvent e;
				XNextEvent(OXDPY,&e);
				XSelectInput(OXDPY,XWIN,XMASK);
				X::CXHA_LINK(this);
				OVERB_OUT "OKIT | Successfully created CContext( %p , %d , %d , %u , %u, %s )\n",root,_x,_y,_w,_h, (useScale ? "true" : "false") OVERB_END
				return true;
			}else{
				printf("OKIT | ERROR! FAILED TO CREATE CContext( %p , %d , %d , %u , %u, %s ) BECAUSE XCreateSimpleWindow DID NOT RETURN A VALID WINDOW!\n",root,_x,_y,_w,_h, (useScale ? "true" : "false") );
				exit(OERR_X11_WINDOW_CREATION_FAILURE);
				return false;
			}
		}else{
			XWIN=0;
			printf("OKIT | ERROR! FAILED TO CREATE CContext( %p , %d , %d , %u , %u, %s ) BECAUSE X IS NOT INITIALISED!\n",root,_x,_y,_w,_h, (useScale ? "true" : "false") );
			exit(OERR_X11_NOT_INITED);
			return false;
		}
	}

	void CContext::setCol(OCol* col){
		XSetWindowBackground(OXDPY,XWIN,col->XCOL);
		XClearWindow(OXDPY,XWIN);
		X::CXEvent force;
		force.valid=true;
		force.type=X::CXE_FORCERENDER;
		listenerFunc(listener,&force);
		return;
	}

/* X HANDLER */

#undef __CXHA_DEFAULT_CAP
#define __CXHA_DEFAULT_CAP 15
#undef __CXHA_DEFAULT_STEP
#define __CXHA_DEFAULT_STEP 5

	namespace X{
		CXHANDLE* CXHA=0;
		unsigned long CXHA_COUNT=0;
		unsigned long CXHA_CAP=__CXHA_DEFAULT_CAP;

		static bool _CXHA_RESIZE(unsigned long size){
			if(!CXHA){return false;}
			CXHA=(CXHANDLE*)realloc(CXHA,sizeof(CXHANDLE)*size);
			if(!CXHA){printf("OKIT | ERROR! CXHA FAILED TO RESIZE!! CAN'T REALLOC!\n");exit(OERR_CANTMALLOC);return false;}
			for(unsigned long i=CXHA_COUNT;i<CXHA_CAP;i++){CXHA[i]={0,0};}
			CXHA_CAP=size;
			return true;
		}

		bool CXHA_INIT(void){
			if(CXHA){return false;}
			CXHA=(CXHANDLE*)malloc(sizeof(CXHANDLE)*__CXHA_DEFAULT_CAP);
			if(!CXHA){printf("OKIT | ERROR! CXHA FAILED TO INITALISE! CAN'T MALLOC!\n");exit(OERR_CANTMALLOC);return false;}
			for(unsigned long i=0;i<__CXHA_DEFAULT_CAP;i++){CXHA[i]={0,0};}
			return true;
		}
		
		bool CXHA_LINK(CContext* c){
			if(!CXHA){return false;}
			if(CXHA_COUNT+1>=CXHA_CAP){if(_CXHA_RESIZE(CXHA_COUNT+__CXHA_DEFAULT_STEP)){return false;}}
			for(unsigned long i=0;i<CXHA_CAP;i++){
				if(!CXHA[i].XWIN){
					CXHA[i]={c->XWIN,c};
					CXHA_COUNT++;
					return true;
				}
			}
			return false;
		}
		
		bool CXHA_UNLINK(CContext* c){
			if(!CXHA){return false;}
			unsigned long place=0;
			bool found=false;
			for(unsigned long i=0;i<CXHA_CAP;i++){
				if(CXHA[i].context==c){place=i;found=true;break;}
			}
			if(!found){return false;}
			for(unsigned long i=place+1;i<CXHA_CAP;i++){
				CXHA[i-1]=CXHA[i];
				CXHA[i]={0,0};
			}
			return true;
		}
		
		CContext* CXHA_GETFROMXID(unsigned long XWIN){
			if(!CXHA){return 0;}
			for(unsigned long i=0;i<CXHA_COUNT;i++){
				if(CXHA[i].XWIN==XWIN){return CXHA[i].context;}
			}
			return 0;
		}
	}
}