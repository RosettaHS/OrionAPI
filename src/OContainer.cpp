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
#include "include/OContainer.hpp"

#define DEF_W 50
#define DEF_H 50

namespace Orion{
	OContainer::OContainer(void){
		type=OT_OCONTAINER;
	}

	OContainer::OContainer(CContainer& parent, int _x , int _y, unsigned int _w, unsigned int _h){
		OXONLY{
			type=OT_OCONTAINER;
			minW=DEF_W,minH=DEF_H;
			init(_x,_y,_w,_h);
			ready=true;

			internal_link.contextToUse=&selfContext;
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

	/* Add more to this later! */
	void OContainer::sort(void){ return; }

	void OContainer::onLink(void){
		selfContext.init(parentContainer->internal_link.contextToUse,x,y,w,h,0,theme.secondary,0,CCT_TOPLEVEL,true,false);
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
	}

	/* Containers can not be scaled. */
	void OContainer::setScale(float s){
		OLog("OKIT | WARNING! CANNOT SET THE SCALE OF CONTAINERS! FAILED SETTING OCONTAINER %p SCALE TO %2!\n",s);
	}

	void OContainer::setCol(unsigned char r, unsigned char g, unsigned char b){ setThemeSecondaryCol(r,g,b); }

/* Handling */

	namespace DRAW{
		void OContainer(CDrawable* obj){
			Orion::OContainer* container=(Orion::OContainer*)obj;
			if(!container->ready || !container->selfContext.XWIN){return;}
			OLog("DRAWING - XID %lu\n",container->selfContext.XWIN);
			// return;
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