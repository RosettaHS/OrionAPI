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
#include "include/application.hpp"
#include "include/OLog.hpp"
#include "include/OContainer.hpp"

namespace Orion{
	OContainer::OContainer(CContainer& parent,int _x, int _y, unsigned int _w, unsigned int _h){
		type=OT_OCONTAINER;

	/* Flag override checking */
		if(!setFlag(_x,0,0,0)){x=_x;}else{x=0;}
		if(!setFlag(0,_y,0,0)){y=_y;}else{y=0;}
		if(!setFlag(0,0,_w,0)){w=_w;}else{w=0;}
		if(!setFlag(0,0,0,_h)){h=_h;}else{h=0;}
		setMinSize(50,50);

		selfContext.init(parent.internal_link.contextToUse,x,y,w,h,0,theme.primary,ExposureMask|StructureNotifyMask,CCT_TOPLEVEL,true);
		selfContext.listener=(void*)this;
		selfContext.listenerFunc=HANDLE::OContainer;
		internal_link.contextToUse=&selfContext;
		drawableToUse=this;
		containerToUse=this;
		arr.init(10,5);
		internal.drawPtr=DRAW::OContainer;

		ready=true;
		if(!parent.link(*this)){ready=false;}
	}

	void OContainer::setPos(int _x, int _y){
		if((x==_x)&&(y==_y)){return;}
		 x=_x,y=_y;
		 if(ready){
		 	float scale=getScale(true);
		 	selfContext.setPos(_INTSCALEBYFLOAT(x,scale),_INTSCALEBYFLOAT(y,scale),true);
		 }
	}

	void OContainer::setSize(unsigned int _w, unsigned int _h, bool force){
		if( !force && (w==_w) && (h==_h) ){return;}
		if(_w<minW){w=minW;}else{w=_w;}
		if(_h<minH){h=minH;}else{h=_h;}
		if(ready){
		 	float scale=getScale(true);
		 	selfContext.setSize(_UINTSCALEBYFLOAT(w,scale),_UINTSCALEBYFLOAT(h,scale),true);
		 }
		fullRedraw=true;
		// if(internal.drawPtr){internal.drawPtr(this);}
	}

/* Handling */
	namespace DRAW{
		void OContainer(Orion::CDrawable* obj){
			if(!obj->ready){return;}
			Orion::OContainer* container=(Orion::OContainer*)obj;
			if(container->fullRedraw){
				OLog("%p IS REDRAWING!\n",obj);
				float scale=container->getScale(true);
				container->selfContext.setCol(container->theme.primary);
				container->selfContext.reparent(container->context,_INTSCALEBYFLOAT(container->x,scale),_INTSCALEBYFLOAT(container->y,scale),true);
				container->selfContext.setGeometry(_INTSCALEBYFLOAT(container->x,scale),_INTSCALEBYFLOAT(container->y,scale),_UINTSCALEBYFLOAT(container->w,scale),_UINTSCALEBYFLOAT(container->h,scale),true);
				container->sort();
				container->fullRedraw=false;
			}
		}
	}

	namespace HANDLE{
		void OContainer(void* obj, X::CXEvent* event){
			Orion::OContainer* container=(Orion::OContainer*)obj;
			if(!container->ready){return;}
			switch(event->type){
				default:{return;}
				case X::CXE_EXPOSE: case X::CXE_CONFIGURE: {
					container->fullRedraw=true;
					DRAW::OContainer(container);
					return;
				}
			}
		}
	}
}