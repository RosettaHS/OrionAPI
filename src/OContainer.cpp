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
#include "include/errdef.h"
#include "include/xservice.hpp"
#include "include/OContainer.hpp"

#define DEF_MINW 50
#define DEF_MINH 50

namespace Orion{
	OContainer::OContainer(void){
		type=OT_OCONTAINER;
	}

	OContainer::OContainer(CContainer& parent, int _x , int _y, unsigned int _w, unsigned int _h){
		OXONLY{
			type=OT_OCONTAINER;
			minW=DEF_MINW,minH=DEF_MINH;
			init(_x,_y,_w,_h);
			ready=true;

			contextToUse=&selfContext;
			containerToUse=this;
			drawableToUse=this;
			arr.init(10,5);
			internal.drawPtr=DRAW::OContainer;
			
			parent.link(*this);
		}else{
			OLog("OKIT | ERROR! FAILED TO CREATE OCONTAINER BECAUSE X IS NOT INITIALISED!\n");
			exit(OERR_X11_NOT_INITED);
		}
	}

	void OContainer::sort(void){
		CDrawable* obj;
		uint8_t flag;
		OVec4 oldGeometry;
		OVec4 newGeometry;
		for(unsigned short i=0;i<childCount;i++){
			obj=arr.arr[i];
			flag=obj->internal.modFlags;

			/*
			*  New to bit fields/bit masking... forgive me for doing this the YandereDev way... again... 
			*  Feel free to replace this with a less eye-r*pey version if you can get it working just as good.
			*  ... You know, for the core of an operating system, this is quite stupid.
			*/

			if(flag){
				oldGeometry=obj->getGeometry();
				newGeometry={0,0,0,0};
			/* X Axis */
				if     (flag & _OUI_X_START) { newGeometry.x=0; }
				else if(flag & _OUI_X_CENTRE){ newGeometry.x=( (w/2)-(oldGeometry.w/2) ); }
				else if(flag & _OUI_X_END)   { newGeometry.x=( w-(oldGeometry.w) ); }
				else{ newGeometry.x=oldGeometry.x; }
			/* Y Axis */
				if     (flag & _OUI_Y_START) { newGeometry.y=0; }
				else if(flag & _OUI_Y_CENTRE){ newGeometry.y=( (h/2)-(oldGeometry.h/2) ); }
				else if(flag & _OUI_Y_END)   { newGeometry.y=( h-(oldGeometry.h) ); }
				else{ newGeometry.y=oldGeometry.y; }
			/* Width */
				if     (flag & _OUI_W_FILL){ newGeometry.w=w-newGeometry.x; }
				else{ newGeometry.w=oldGeometry.w; }
			/* Height */
				if     (flag & _OUI_H_FILL){ newGeometry.h=h-newGeometry.h; }
				else{ newGeometry.h=oldGeometry.h; }

				obj->setGeometry(newGeometry);
			}else{ continue; }

		}
	}

	void OContainer::onLink(void){
		selfContext.init(context,x,y,w,h,0,theme.secondary,0,CCT_ELEMENT,true,false);
		tempRelinkAll();
		sort();
	}

	void OContainer::onUnlink(void){
		tempUnlinkAll();
		selfContext.destroy();
	}

	void OContainer::onPosChanged(void){
		selfContext.setPos(x, y, true);
	}
	void OContainer::onSizeChanged(void){
		selfContext.setGeometry(x, y, w, h, true);
		sort();
	}

	/* Containers can not be scaled. */
	void OContainer::setScale(float s){
		OLog("OKIT | WARNING! CANNOT SET THE SCALE OF CONTAINERS! FAILED SETTING CONTAINER %p SCALE TO %f!\n",s);
	}

	void OContainer::setCol(unsigned char r, unsigned char g, unsigned char b){ setThemeSecondaryCol(r,g,b); }
	void OContainer::setCol(OCol& c){ setThemeSecondaryCol(c.r,c.g,c.b); }

/* Handling */

	namespace DRAW{
		void OContainer(CDrawable* obj){
			Orion::OContainer* container=(Orion::OContainer*)obj;
			if(!container->ready || !container->selfContext.XWIN){return;}
			container->selfContext.setCol(container->theme.secondary);
			if(!container->fullRedraw){return;}
			container->selfContext.setGeometry(
				container->x,container->y,
				container->w,container->h,true
			);
			container->sort();
		}
	}

}