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
#include "include/errdef.hpp"
#include "include/xservice.hpp"
#include "include/application.hpp"
#include "include/OLog.hpp"
#include "include/CLabel.hpp"

#define DEF_MINW 50
#define DEF_MINH 50

namespace Orion{
	CLabel::CLabel(void){ type=OT_OLABEL; }

	CLabel::CLabel(CContainer& parent,int _x, int _y, unsigned int _w, unsigned int _h, const char* label){
		OXONLY{
			type=OT_OLABEL;
			minW=DEF_MINW,minH=DEF_MINH;
			init(_x,_y,_w,_h);
			XTEXT.string.setTo(label);
			XTEXT.fontName="fixed";
			XTEXT.XFONT=0;
			XTEXT.XGC=0;

			ready=true;
			parent.link(*this);
			internal.drawPtr=DRAW::CLabel;
		}else{
			OLog("OKIT | ERROR! FAILED TO CREATE OLABEL BECAUSE X HAS NOT BEEN INITIALISED!\n");
			exit(OERR_X11_NOT_INITED);
		}
	}

	void CLabel::onLink(void){
		rect.init(context,offsetX*scale,offsetY*scale,w*scale,h*scale,0,theme.secondary,ExposureMask,CCT_ELEMENT,true,true);
		rect.listener=(void*)this;
		rect.listenerFunc=HANDLE::CLabel;

		XTEXT.XFONT=XLoadQueryFont(OXDPY,XTEXT.fontName);
		XTEXT.XGC=XCreateGC(OXDPY,rect.XWIN,0,0);
		XSetForeground(OXDPY,(GC)XTEXT.XGC,theme.accent->XCOL);
		XID FID=((XFontStruct*)XTEXT.XFONT)->fid;
		XSetFont(OXDPY,(GC)XTEXT.XGC,FID);

		int XT_X, XT_Y;
		int XT_DIR, XT_ASC, XT_DES;
		XCharStruct XT_OVA;

		XTextExtents((XFontStruct*)XTEXT.XFONT,XTEXT.string,XTEXT.string.getLength(),
			&XT_DIR,&XT_ASC,&XT_DES,&XT_OVA
		);
		XT_X=(w-XT_OVA.width)/2;
		XT_Y=(h/2) + ((XT_ASC - XT_DES) /2);

		XDrawString(OXDPY,rect.XWIN,(GC)XTEXT.XGC,XT_X,XT_Y,XTEXT.string,XTEXT.string.getLength());
	}

	void CLabel::onUnlink(void){ rect.destroy(); }

	void CLabel::onPosChanged(void){
		rect.setPos(offsetX*scale, offsetY*scale, true);
	}
	void CLabel::onSizeChanged(void){
		rect.setGeometry(offsetX*scale, offsetY*scale, w*scale, h*scale, true);
	}


/* Placeholder */
	void CLabel::setCol(unsigned char r, unsigned char g, unsigned char b){
		OLog("OKIT | WARNING! OLABEL'S TEXT CANNOT BE COLOURED! IF YOU WANT TO CHANGE THE COLOUR OF THE BACKGROUND, USE \"setBackgroundCol()\"!");
		OLog("ERROR SETTING COLOUR TO (%u %u %u)\n",r,g,b);
	}
	void CLabel::setCol(OCol& c){
		OLog("OKIT | WARNING! OLABEL'S TEXT CANNOT BE COLOURED! IF YOU WANT TO CHANGE THE COLOUR OF THE BACKGROUND, USE \"setBackgroundCol()\"!");
		OLog("ERROR SETTING COLOUR TO (%u %u %u)\n",c.r,c.g,c.b);
	}
/* !Placeholder */


	void CLabel::setBackgroundCol(unsigned char r, unsigned char g, unsigned char b){
		setThemeSecondaryCol(r,g,b);
	}
	void CLabel::setBackgroundCol(OCol& c){ setThemeSecondaryCol(c); }

	namespace DRAW{
		void CLabel(CDrawable* obj){
			Orion::CLabel* label=(Orion::CLabel*)obj;
			if(!label->ready || !label->rect.XWIN || !label->context){return;}
			label->rect.setCol(label->theme.secondary);
			if(!label->fullRedraw){return;}

			label->rect.setGeometry(
				label->offsetX*label->scale,label->offsetY*label->scale,
				label->w*label->scale,label->h*label->scale,true
			);
			label->fullRedraw=false;
		}
	}

	namespace HANDLE{
		void CLabel(void* obj, X::CXEvent* event){
			Orion::CLabel* label=(Orion::CLabel*)obj;
			// if(!label->ready || !label->rect.XWIN || !label->context){return;}
			switch(event->type){
				default:{ return; }
				case X::CXE_EXPOSE:{ DRAW::CLabel(label); }
			}
		}
	}

}