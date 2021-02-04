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
#include "include/xservice.hpp"
#include "include/application.hpp"
#include "include/errdef.h"
#include "include/OLog.hpp"
#include "include/ORect.hpp"

#define DEF_MINW 50
#define DEF_MINH 50

namespace Orion{

	ORect::~ORect(void){
		type=OT_ERROR;
		ready=false;
	}

	ORect::ORect(void){type=OT_ORECT; }

	ORect::ORect(CContainer& parent,int _x, int _y, unsigned int _w, unsigned int _h, OCol& col){
		OXONLY{
			minW=DEF_MINW,minH=DEF_MINH;
		/* Flag override checking */
			init(_x,_y,_w,_h);
			/* ORects use the secondary colour of the OApp theme by default.
			 * This checks if you're inputting another value from the OApp's theme,
			 * and if it discovers a match, it will set the pointer directly to that instead.
			 * I'm doing it the YandereDev way because a switch() statement would not work in this situation.
			 */
			// if(col.XCOL!=theme.secondary->XCOL){setSecondaryCol(col);}
			if(col.XCOL==OTHEME_PRIMARY.XCOL){theme.secondary=&OTHEME_PRIMARY;}
			else if(col.XCOL==OTHEME_SECONDARY.XCOL){theme.secondary=&OTHEME_SECONDARY;}
			else if(col.XCOL==OTHEME_TERTIARY.XCOL){theme.secondary=&OTHEME_TERTIARY;}
			else if(col.XCOL==OTHEME_ACCENT.XCOL){theme.secondary=&OTHEME_ACCENT;}
			else{setThemeSecondaryCol(col);}

			ready=true;
			parent.link(*this);
			internal.drawPtr=DRAW::ORect;
			
		}else{
			OLog("OKIT | ERROR! FAILED TO CREATE ORECT BECAUSE X HAS NOT BEEN INITIALISED!\n");
			exit(OERR_X11_NOT_INITED);
		}
	}

	void ORect::onLink(void){
		rect.init(parentContainer->internal_link.contextToUse,offsetX*scale,offsetY*scale,w*scale,h*scale,0,theme.secondary,0,CCT_TOPLEVEL,true,false);
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

	namespace DRAW{
		void ORect(CDrawable* obj){
			Orion::ORect* rect=(Orion::ORect*)obj;
			if(!rect->ready || !rect->rect.XWIN){return;}
			rect->rect.setCol(rect->theme.secondary);
			if(!rect->fullRedraw){return;}
			rect->rect.setGeometry(
				rect->offsetX*rect->scale,rect->offsetY*rect->scale,
				rect->w*rect->scale,rect->h*rect->scale,true
			);
		}
	}

}