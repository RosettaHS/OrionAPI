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

#include <stdlib.h>
#include "include/errdef.h"
#include "include/xservice.hpp"
#include "include/OMarginContainer.hpp"

#define DEF_MINW 50
#define DEF_MINH 50

namespace Orion{
	OMarginContainer::OMarginContainer(void){ type=OT_OMARGINCONTAINER; }

	OMarginContainer::OMarginContainer(CContainer& parent, int _x, int _y, unsigned int _w, unsigned int _h, unsigned short margin){
		OXONLY{
			type=OT_OMARGINCONTAINER;
			minW=DEF_MINW,minH=DEF_MINH;
			init(_x,_y,_w,_h);
			margin_left=margin;
			margin_right=margin;
			margin_top=margin;
			margin_bottom=margin;
			ready=true;

			contextToUse=&selfContext;
			containerToUse=this;
			drawableToUse=this;
			arr.init(10,5);
			internal.drawPtr=DRAW::OMarginContainer;
			
			parent.link(*this);
		}else{
			OLog("OKIT | ERROR! FAILED TO CREATE OMARGINCONTAINER BECAUSE X IS NOT INITIALISED!\n");
			exit(OERR_X11_NOT_INITED);
		}
	}

	/* Yes this is a cut-and-paste from OContainer, and that's because this is an OContainer, just margined. */
	void OMarginContainer::sort(void){
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
				else if(flag & _OUI_X_CENTRE){ newGeometry.x=( (( w-margin_right*2 )/2)-(oldGeometry.w/2) ); }
				else if(flag & _OUI_X_END)   { newGeometry.x=( (w-margin_right*2)-(oldGeometry.w) ); }
				else{ newGeometry.x=oldGeometry.x; }
			/* Y Axis */
				if     (flag & _OUI_Y_START) { newGeometry.y=0; }
				else if(flag & _OUI_Y_CENTRE){ newGeometry.y=( (( h-margin_bottom*2 )/2)-(oldGeometry.h/2) ); }
				else if(flag & _OUI_Y_END)   { newGeometry.y=( (h-margin_bottom*2)-(oldGeometry.h) ); }
				else{ newGeometry.y=oldGeometry.y; }
			/* Width */
				if     (flag & _OUI_W_FILL){ newGeometry.w=(w-margin_right*2)-newGeometry.x; }
				else{ newGeometry.w=oldGeometry.w; }
			/* Height */
				if     (flag & _OUI_H_FILL){ newGeometry.h=(h-margin_bottom*2)-newGeometry.h; }
				else{ newGeometry.h=oldGeometry.h; }

				obj->setGeometry(newGeometry);
			}else{ continue; }

		}
	}


	void OMarginContainer::onLink(void){
		selfContext.init(context,x+margin_left,y+margin_top,w-(margin_bottom*2),h-(margin_right*2),0,theme.secondary,0,CCT_ELEMENT,true,false);
		tempRelinkAll();
		sort();
	}

	void OMarginContainer::onUnlink(void){
		tempUnlinkAll();
		selfContext.destroy();
	}

	void OMarginContainer::onPosChanged(void){
		selfContext.setPos(x+margin_left, y+margin_top, true);
	}
	void OMarginContainer::onSizeChanged(void){
		selfContext.setGeometry(x+margin_left, y+margin_top, w-(margin_bottom*2), h-(margin_right*2), true);
		sort();
	}

/* Setters */
	void OMarginContainer::setMargin(unsigned short m){ margin_left=m,margin_right=m,margin_top=m,margin_bottom=m; fullRedraw=true;if(internal.drawPtr){ internal.drawPtr(this); } }
	void OMarginContainer::setLeftMargin(unsigned short m){ margin_left=m; fullRedraw=true;if(internal.drawPtr){ internal.drawPtr(this); } }
	void OMarginContainer::setRightMargin(unsigned short m){ margin_right=m; fullRedraw=true;if(internal.drawPtr){ internal.drawPtr(this); } }
	void OMarginContainer::setTopMargin(unsigned short m){ margin_top=m; fullRedraw=true;if(internal.drawPtr){ internal.drawPtr(this); } }
	void OMarginContainer::setBottomMargin(unsigned short m){ margin_bottom=m; fullRedraw=true;if(internal.drawPtr){ internal.drawPtr(this); } }

/* Getters */
	unsigned short OMarginContainer::getLeftMargin(void){ return margin_left; }
	unsigned short OMarginContainer::getRightMargin(void){ return margin_right; }
	unsigned short OMarginContainer::getTopMargin(void){ return margin_top; }
	unsigned short OMarginContainer::getBottomMargin(void){ return margin_bottom; }

/* Handling */

	namespace DRAW{
		void OMarginContainer(CDrawable* obj){
			Orion::OMarginContainer* container=(Orion::OMarginContainer*)obj;
			if(!container->ready || !container->selfContext.XWIN){return;}
			container->selfContext.setCol(container->theme.secondary);
			if(!container->fullRedraw){return;}
			container->selfContext.setGeometry(
				container->x+container->margin_left,container->y+container->margin_top,
				container->w-(container->margin_right*2),container->h-(container->margin_bottom*2),true
			);
			container->sort();
		}
	}

}