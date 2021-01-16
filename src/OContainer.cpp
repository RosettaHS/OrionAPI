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
	OContainer::OContainer(){
		type=OT_ERROR;
		drawPtr=0;
		minW=10,minH=10;
		x=0,y=0,w=0,h=0;
		col=0;
	}
	OContainer::OContainer(CContainer& parent,int _x, int _y, unsigned int _w, unsigned int _h){
		if(linkTo(&parent)){
			type=OT_CONTAINER;
			drawPtr=X::OContainer_DRAW;
			minW=10,minH=10;
			col=&OTHEME_PRIMARY;
			x=_x,y=_y;
			if(_w<minW){w=minW;}else{w=_w;}
			if(_h<minH){h=minH;}else{h=_h;}

			selfContext.init(parent.contextToUse,x,y,w,h,0,col,ExposureMask,CCT_TOPLEVEL,true);
			selfContext.listener=(void*)this;
			selfContext.listenerFunc=X::OContainer_EVH;
			drawPtr(this);
		}else{
			type=OT_ERROR;
			printf("OKIT | WARNING! OCONTAINER %p FAILED TO LINK TO PARENT %p ON INITIALISER! OCONTAINER NON-FUNCTIONAL!\n",(void*)this,(void*)&parent);
		}
	}
	
	/* Base containers do no sorting. */
	void OContainer::sort(void){return;}

	void OContainer::setCol(unsigned char r, unsigned char g, unsigned char b){
		internalCol.setTo(r,g,b);
		col=&internalCol;
		if(type!=OT_ERROR){ selfContext.setCol(&internalCol); }else{ printf("OKIT | WARNING! OCONTAINER %p COULD NOT SET COL DUE TO IT NOT BEING INITIALISED!\n",(void*)this); }
	}
	void OContainer::setCol(OCol& newCol){
		internalCol=newCol;
		col=&internalCol;
		if(type!=OT_ERROR){ selfContext.setCol(&internalCol); }else{ printf("OKIT | WARNING! OCONTAINER %p COULD NOT SET COL DUE TO IT NOT BEING INITIALISED!\n",(void*)this); }
	}

	bool OContainer::link(CContainable* obj){
		if(type==OT_ERROR){ printf("OKIT | WARNING! CAN'T LINK %p TO OCONTAINER %p SINCE THE CONTAINER IS NOT INITIALISED!\n",(void*)obj,(void*)this); return false; }
		if((void*)this==(void*)obj){printf("OKIT | WARNING! CANNOT LINK A OCONTAINER TO ITSELF!\n"); return false;}
		if(children.link(obj)){
			childCount=children.count;
			drawPtr(this);
			return true;
		}
		return false;
	}

	bool OContainer::unlink(CContainable* obj){
		if(type==OT_ERROR){ printf("OKIT | WARNING! CAN'T UNLINK %p FROM OCONTAINER %p SINCE THE CONTAINER IS NOT INITIALISED!\n",(void*)obj,(void*)this); return false; }
		if((void*)this==(void*)obj){printf("OKIT | WARNING! CANNOT UNLINK A OCONTAINER FROM ITSELF!\n");return false;}
		if(children.link(obj)){
			childCount=children.count;
			drawPtr(this);
			return true;
		}
		return false;
	}

	int OContainer::getIndexOf(CContainable* obj){
		if(type==OT_ERROR){ printf("OKIT | WARNING! CAN'T GET INDEX OF %p ON OCONTAINER %p SINCE THE CONTAINER IS NOT INITIALISED!\n",(void*)obj,(void*)this); return -1; }
		return children.getIndexOf(obj);
	}

	void OContainer::setPos(int x, int y){
		if(type==OT_ERROR){ printf("OKIT | WARNING! CAN'T SET POS ON OCONTAINER %p SINCE THE CONTAINER IS NOT INITIALISED!\n",(void*)this); return; }
		selfContext.setPos(x,y,true);
	}

	void OContainer::setSize(unsigned int _w, unsigned int _h){
		if(type==OT_ERROR){ printf("OKIT | WARNING! CAN'T SET SIZE ON OCONTAINER %p SINCE THE CONTAINER IS NOT INITIALISED!\n",(void*)this); return; }
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
			container->sort();
			container->children.drawAll();
		}
		void OContainer_EVH(void* obj,CXEvent* event){
			OContainer* container=(OContainer*)obj;
			switch(event->type){
				default:{return;}
				case CXE_EXPOSE:{OContainer_DRAW(container);}
			}
		}
	}
}