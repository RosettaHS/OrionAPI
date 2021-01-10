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
#include <X11/Xlib.h>
#include "include/OContainer.hpp"
#include "include/OTheme.hpp"


namespace Orion{

	OContainer::OContainer(CContainer& parent,int _x, int _y, unsigned int _w, unsigned int _h){
		drawPtr=X::OContainer_DRAW;
		minW=10,minH=10;
		col=&OTHEME_PRIMARY;
		col->log();
		x=_x,y=_y;
		if(_w<minW){w=minW;}else{w=_w;}
		if(_h<minH){h=minH;}else{h=_h;}
		
		selfContext.init(parent.contextToUse,x,y,w,h,0,col,ExposureMask|StructureNotifyMask,CCT_TOPLEVEL,true);
		linkTo(&parent);
		selfContext.listener=(void*)this;
		selfContext.listenerFunc=X::OContainer_EVH;
	}
	
	/* Base containers do no sorting. */
	void OContainer::sort(void){return;}

	void OContainer::setCol(unsigned char r, unsigned char g, unsigned char b){
		internalCol.setTo(r,g,b);
		col=&internalCol;
	}
	void OContainer::setCol(OCol& newCol){
		internalCol=newCol;
		col=&internalCol;
	}

	bool OContainer::link(CContainable* obj){
		if((void*)this==(void*)obj){printf("OKIT | WARNING! CANNOT LINK A OCONTAINER TO ITSELF!\n"); return false;}
		if(children.link(obj)){
			childCount=children.count;
			obj->drawPtr(obj);
			return true;
		}
		return false;
	}

	bool OContainer::unlink(CContainable* obj){
		if((void*)this==(void*)obj){printf("OKIT | WARNING! CANNOT UNLINK A OCONTAINER FROM ITSELF!\n");return false;}
		if(children.link(obj)){
			childCount=children.count;
			return true;
		}
		return false;
	}

	int OContainer::getIndexOf(CContainable* obj){return children.getIndexOf(obj);}

	void OContainer::setPos(int x, int y){
		selfContext.setPos(x,y,true);
	}

	void OContainer::setSize(unsigned int _w, unsigned int _h){
		unsigned int w,h;
		if(_w<minW){w=minW;}else{w=_w;}
		if(_h<minH){h=minH;}else{h=_h;}
		selfContext.setSize(w,h,true);
		X::OContainer_DRAW(this);
	}

	void OContainer::setMinSize(unsigned int w, unsigned int h){minW=w,minH=h;}

/* X Handling */
	namespace X{
		void OContainer_DRAW(CDrawable* obj){
			OContainer* container=(OContainer*)obj;
			container->children.drawAll();
		}
		void OContainer_EVH(void* obj,CXEvent* event){
			OContainer* container=(OContainer*)obj;
			switch(event->type){
				default:{return;}
				case CXE_EXPOSE: case CXE_CONFIGURE:{OContainer_DRAW(container);}
			}
		}
	}
}