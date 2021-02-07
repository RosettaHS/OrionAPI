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
#include <X11/Xatom.h>
#include "include/errdef.h"
#include "include/application.hpp"
#include "include/xservice.hpp"
#include "include/CContext.hpp"

namespace Orion{
	CContext::~CContext(void){
		if(destroy()){ OVLog("OKIT | CContext %p destroyed.\n",(void*)this); }
	}
	CContext::CContext(void) : XWIN{0},XROOT{0},XCOL{0},XMASK{0},XTITLE{0},listener{0},listenerFunc{0} {}

	bool CContext::destroy(void){
		OXONLY{
			if(XWIN){
				XUnmapWindow(OXDPY,XWIN);
				XDestroyWindow(OXDPY,XWIN);
				X::CXHA_UNLINK(this);
				XWIN=0,XROOT=0,XMASK=0,XTITLE=0,listener=0,listenerFunc=0;
				return true;
			}else{return false;}
		}else{return false;}
	}

	bool CContext::init(CContext* root, int _x, int _y, unsigned int _w, unsigned int _h, const char* t, OCol* col, CXMask mask, CCType type, bool useScale, bool link){
		XWIN=0,XROOT=0,XMASK=0,XTITLE=0,listener=0,listenerFunc=0;
		OXONLY{
			int x,y;
			unsigned int w,h;
			XMASK=mask;
			XTITLE=t;
			if(root){XROOT=root->XWIN;}else{XROOT=OXROOT;}
			if(useScale){
				if(root){x=_INTSCALEBYFLOAT(_x,OAPP_SCALE),y=_INTSCALEBYFLOAT(_y,OAPP_SCALE);}else{x=_x,y=_y;}
				w=_UINTSCALEBYFLOAT(_w,OAPP_SCALE),h=_UINTSCALEBYFLOAT(_h,OAPP_SCALE);
			}else{
				x=_x,y=_y;
				w=_w,h=_h;
			}
			if(w<=1){w=1;}
			if(h<=1){h=1;}

			XSetWindowAttributes attr;
			Atom _XATOM=XInternAtom(OXDPY,"_NET_WM_WINDOW_TYPE",False);
			long atomval=0;
			unsigned int _XCLASS=InputOutput;
			CXMask attrmask=CWBackPixel;
			attr.background_pixel=col->XCOL;
			XCOL=col->XCOL;

			switch(type){
				case CCT_INPUTONLY:{_XCLASS=InputOnly;attrmask=0;break;}
				case CCT_TOPLEVEL:{atomval=XInternAtom(OXDPY,"_NET_WM_WINDOW_TYPE_NORMAL",False);break;}
				case CCT_DESKTOP:{atomval=XInternAtom(OXDPY,"_NET_WM_WINDOW_TYPE_DESKTOP",False);break;}
				case CCT_PANEL:{atomval=XInternAtom(OXDPY,"_NET_WM_WINDOW_TYPE_DOCK",False);break;}
				case CCT_TOOLTIP:{atomval=XInternAtom(OXDPY,"_NET_WM_WINDOW_TYPE_TOOLTIP",False);attrmask|=CWOverrideRedirect;attr.override_redirect=1;break;}
				case CCT_MENU:{atomval=XInternAtom(OXDPY,"_NET_WM_WINDOW_TYPE_MENU",False);attrmask|=CWOverrideRedirect;attr.override_redirect=1;break;}
				case CCT_ELEMENT:{atomval=0;break;}
			}

			// XWIN=XCreateSimpleWindow(OXDPY,XROOT,x,y,w,h,0,col->XCOL,col->XCOL);
			XWIN=XCreateWindow(OXDPY,XROOT,x,y,w,h,0,CopyFromParent,_XCLASS,CopyFromParent,attrmask,&attr);
			if(XWIN){
				XSelectInput(OXDPY,XWIN,SubstructureNotifyMask | ExposureMask);
				if(t){XStoreName(OXDPY,XWIN,t);}
				if(atomval){XChangeProperty(OXDPY,XWIN,_XATOM,XA_ATOM,32,PropModeReplace,(unsigned char *)&atomval,1);}
				if(type==CCT_TOPLEVEL){
					// _XATOM=XInternAtom(OXDPY,"_NET_WM_STATE",False);
					// long _atomval=XInternAtom(OXDPY,"_NET_WM_STATE_FULLSCREEN",False); /* TODO: REPLACE WITH BETTER SOLUTION!! HACKY!! */
					// XChangeProperty(OXDPY,XWIN,_XATOM,XA_ATOM,32,PropModeReplace,(unsigned char *)&_atomval,1);
					struct {
						unsigned long flags;
						unsigned long functions;
						unsigned long decorations;
						long inputMode;
						unsigned long status;
					}hints;
					hints.flags=2,hints.decorations=0;
					_XATOM=XInternAtom(OXDPY,"_MOTIF_WM_HINTS",true);
					XChangeProperty(OXDPY,XWIN,_XATOM,XA_ATOM,32,PropModeReplace,(unsigned char *)&hints,1); /* TODO: REPLACE WITH MORE PORTABLE SOLUTION!! */
				}else if(type==CCT_PANEL){
					_XATOM=XInternAtom(OXDPY,"_NET_WM_STATE",False);
					long _atomval=XInternAtom(OXDPY,"_NET_WM_STATE_STICKY",False);
					XChangeProperty(OXDPY,XWIN,_XATOM,XA_ATOM,32,PropModeReplace,(unsigned char *)&_atomval,1);
				}
				XMapWindow(OXDPY,XWIN);
				XMoveResizeWindow(OXDPY,XWIN,x,y,w,h);
				XEvent e;
				XNextEvent(OXDPY,&e);
				XSelectInput(OXDPY,XWIN,XMASK);

				XChangeWindowAttributes(OXDPY,XWIN,attrmask,&attr);
				if(link){X::CXHA_LINK(this);}
				OVLog("OKIT | Successfully created CContext %p with parameters ( %p , %d , %d , %u , %u, %s )\n",(void*)this,(void*)root,_x,_y,_w,_h, (useScale ? "true" : "false"));
				return true;
			}else{
				OLog("OKIT | ERROR! FAILED TO CREATE CContext %p WITH PARAMETERS( %p , %d , %d , %u , %u, %s ) BECAUSE XCreateSimpleWindow DID NOT RETURN A VALID WINDOW!\n",(void*)this,(void*)root,_x,_y,_w,_h, (useScale ? "true" : "false") );
				exit(OERR_X11_WINDOW_CREATION_FAILURE);
				return false;
			}
		}else{
			XWIN=0;
			OLog("OKIT | ERROR! FAILED TO CREATE CContext( %p , %d , %d , %u , %u, %s ) BECAUSE X IS NOT INITIALISED!\n",(void*)root,_x,_y,_w,_h, (useScale ? "true" : "false") );
			exit(OERR_X11_NOT_INITED);
			return false;
		}
	}

	void CContext::setTitle(const char* title){
		int length=0;
		while(true){
			if(title[length]==0){break;}
			length++;
		}
		XChangeProperty(OXDPY,XWIN,XInternAtom(OXDPY,"_NET_WM_NAME",False),XInternAtom(OXDPY,"UTF8_STRING",False),8,PropModeReplace,(unsigned char *)title,length);
	}

	void CContext::setCol(OCol* col){
		if(!XWIN){OLog("OKIT | WARNING! CAN'T RUN SETCOL ON OBJECT THAT HAS NOT BEEN LINKED YET!\n"); return;}
		if(col->XCOL==XCOL){return;}
		XSetWindowBackground(OXDPY,XWIN,col->XCOL);
		XClearWindow(OXDPY,XWIN);
		if(listenerFunc){
			X::CXEvent force;
			force.valid=true;
			force.type=X::CXE_FORCERENDER;
			listenerFunc(listener,&force);
		}
		return;
	}

	void CContext::setPos(int _x, int _y, bool useScale){
		if(!XWIN){OLog("OKIT | WARNING! CAN'T RUN SETPOS ON OBJECT THAT HAS NOT BEEN LINKED YET!\n"); return;}
		int x,y;
		if(useScale){	if(XROOT!=OXROOT){x=(int)_x*OAPP_SCALE,y=(int)_y*OAPP_SCALE;}else{x=_x,y=_y;}	}else{x=_x,y=_y;}
		XMoveWindow(OXDPY,XWIN,x,y);
		if(listenerFunc){
			X::CXEvent force;
			force.valid=true;
			force.type=X::CXE_FORCERENDER;
			listenerFunc(listener,&force);
		}
		return;
	}

	void CContext::setSize(unsigned int _w, unsigned int _h, bool useScale){
		if(!XWIN){OLog("OKIT | WARNING! CAN'T RUN SETSIZE ON OBJECT THAT HAS NOT BEEN LINKED YET!\n"); return;}
		int w,h;
		if(useScale){	w=(unsigned int)_w*OAPP_SCALE,h=(unsigned int)_h*OAPP_SCALE;	}else{w=_w,h=_h;}
		if(w<=1){w=1;}
		if(h<=1){h=1;}
		XResizeWindow(OXDPY,XWIN,w,h);
		if(listenerFunc){
			X::CXEvent force;
			force.valid=true;
			force.type=X::CXE_FORCERENDER;
			listenerFunc(listener,&force);
		}
		return;
	}

	void CContext::setGeometry(int _x, int _y, unsigned int _w, unsigned int _h, bool useScale){
		if(!XWIN){OLog("OKIT | WARNING! CAN'T RUN SETGEOMETRY ON OBJECT THAT HAS NOT BEEN LINKED YET!\n"); return;}
		int x,y;
		unsigned int w,h;
		if(useScale){
			if(XROOT!=OXROOT){x=(int)_x*OAPP_SCALE,y=(int)_y*OAPP_SCALE;}else{x=_x,y=_y;}
			w=(unsigned int)_w*OAPP_SCALE,h=(unsigned int)_h*OAPP_SCALE;
		}else{
			x=_x,y=_y;
			w=_w,h=_h;
		}
		XMoveResizeWindow(OXDPY,XWIN,x,y,w,h);
		if(listenerFunc){
			X::CXEvent force;
			force.valid=true;
			force.type=X::CXE_FORCERENDER;
			listenerFunc(listener,&force);
		}
		return;
	}

	void CContext::reparent(CContext* root, int _x, int _y, bool useScale){
		if(!XWIN){OLog("OKIT | WARNING! CAN'T REPARENT CONTEXT THAT HAS NOT BEEN INITIALISED YET!\n"); return;}
		if(root->XWIN==XROOT){return;}
		int x,y;
		if(useScale){
			x=_INTSCALEBYFLOAT(_x,OAPP_SCALE);
			y=_INTSCALEBYFLOAT(_y,OAPP_SCALE);
		}else{x=_x, y=_y;}
		OLog("REPARENT - x %d | y %d\n",x,y);
		if(XReparentWindow(OXDPY,XWIN,root->XWIN,x,y)){
			OLog("Success. Old %lu | New %lu | New Root %p\n",XROOT,root->XWIN,root);
			XROOT=root->XWIN;	
		}
	}

/* X HANDLER */

#undef __CXHA_DEFAULT_CAP
#define __CXHA_DEFAULT_CAP 15
#undef __CXHA_DEFAULT_STEP
#define __CXHA_DEFAULT_STEP 5

	namespace X{
		CXHANDLE* CXHA=0;
		volatile unsigned long CXHA_COUNT=0;
		unsigned long CXHA_CAP=__CXHA_DEFAULT_CAP;

		static bool _CXHA_RESIZE(unsigned long size){
			if(!CXHA){return false;}
			CXHA=(CXHANDLE*)realloc(CXHA,sizeof(CXHANDLE)*size);
			if(!CXHA){OLog("OKIT | ERROR! CXHA FAILED TO RESIZE!! CAN'T REALLOC!\n");exit(OERR_CANTMALLOC);return false;}
			for(unsigned long i=CXHA_COUNT;i<CXHA_CAP;i++){CXHA[i]={0,0};}
			CXHA_CAP=size;
			return true;
		}

		bool CXHA_INIT(void){
			if(CXHA){return false;}
			CXHA=(CXHANDLE*)malloc(sizeof(CXHANDLE)*__CXHA_DEFAULT_CAP);
			if(!CXHA){OLog("OKIT | ERROR! CXHA FAILED TO INITALISE! CAN'T MALLOC!\n");exit(OERR_CANTMALLOC);return false;}
			for(unsigned long i=0;i<__CXHA_DEFAULT_CAP;i++){CXHA[i]={0,0};}
			return true;
		}

		bool CXHA_DESTROY(void){
			if(!CXHA){return false;}
			free(CXHA);
			CXHA=0;
			CXHA_COUNT=0;
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
			CXHA_COUNT--;
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