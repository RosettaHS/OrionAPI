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
#include "include/errdef.hpp"
#include "include/application.hpp"
#include "include/OSL/OLog.hpp"
#include "include/OKit/xservice.hpp"
#include "include/OKit/ORect.hpp"

#define DEF_MINW 50
#define DEF_MINH 50

namespace Orion{
	ORect::ORect(void){ type=OT_ORECT; }

	ORect::ORect(CContainer& parent,int _x, int _y, unsigned int _w, unsigned int _h, OCol& col){
		OXONLY{
			type=OT_ORECT;
			minW=DEF_MINW,minH=DEF_MINH;
			init(_x,_y,_w,_h);
			/* ORects use the secondary colour of the OApp theme by default.
			 * This checks if you're inputting another value from the OApp's theme,
			 * and if it discovers a match, it will set the pointer directly to that instead.
			 */
			setThemeSecondaryCol(col);

			ready=true;
			parent.link(*this);
			internal.drawPtr=DRAW::ORect;
			
		}else{
			OLog("ORIONAPI | ERROR! FAILED TO CREATE ORECT BECAUSE X HAS NOT BEEN INITIALISED!\n");
			exit(OERR_X11_NOT_INITED);
		}
	}

	void ORect::onLink(void){
		rect.init(context,offsetX*scale,offsetY*scale,w*scale,h*scale,0,theme.secondary,0,CCT_ELEMENT,true,false);
	}

	void ORect::onUnlink(void){ rect.destroy(); }

	void ORect::onPosChanged(void){
		rect.setPos(offsetX*scale, offsetY*scale, true);
	}
	void ORect::onSizeChanged(void){
		rect.setGeometry(offsetX*scale, offsetY*scale, w*scale, h*scale, true);
	}

	void ORect::setCol(unsigned char r, unsigned char g, unsigned char b){
		setThemeSecondaryCol(r,g,b);
	}
	void ORect::setCol(OCol& c){ setThemeSecondaryCol(c); }

	namespace DRAW{
		void ORect(CDrawable* obj){
			Orion::ORect* rect=(Orion::ORect*)obj;
			if(!rect->ready || !rect->rect.XWIN || !rect->context){return;}
			rect->rect.setCol(rect->theme.secondary);
			if(!rect->fullRedraw){return;}
			rect->rect.setGeometry(
				rect->offsetX*rect->scale,rect->offsetY*rect->scale,
				rect->w*rect->scale,rect->h*rect->scale,true
			);
			rect->fullRedraw=false;
		}
	}

}
