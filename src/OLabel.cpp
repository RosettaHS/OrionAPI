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
#include "include/OLabel.hpp"

#define DEF_MINW 50
#define DEF_MINH 50

namespace Orion{
	OLabel::OLabel(void){ type=OT_OLABEL; }

	OLabel::OLabel(CContainer& parent,int _x, int _y, unsigned int _w, unsigned int _h, const char* label, OCol& labelCol){
		OXONLY{
			type=OT_OLABEL;
			minW=DEF_MINW,minH=DEF_MINH;
			init(_x,_y,_w,_h);
			string=label;
			setCol(labelCol);
			font="fixed";

			ready=true;
			parent.link(*this);
			internal.drawPtr=DRAW::OLabel;
		}else{
			OLog("OKIT | ERROR! FAILED TO CREATE OLABEL BECAUSE X HAS NOT BEEN INITIALISED!\n");
			exit(OERR_X11_NOT_INITED);
		}
	}

	void OLabel::onLink(void){
		rect.init(context,offsetX*scale,offsetY*scale,w*scale,h*scale,0,theme.secondary,ExposureMask,CCT_ELEMENT,true,true);
		rect.listener=(void*)this;
		rect.listenerFunc=HANDLE::OLabel;
		XLABEL.init(&rect,textCol,font);
		XLABEL.draw(&rect,textCol,w*scale,h*scale,string);
	}

	void OLabel::onUnlink(void){ rect.destroy(); XLABEL.destroy(); }

	void OLabel::onPosChanged(void){
		rect.setPos(offsetX*scale, offsetY*scale, true);
	}
	void OLabel::onSizeChanged(void){
		rect.setGeometry(offsetX*scale, offsetY*scale, w*scale, h*scale, true);
	}

	void OLabel::setFont(const char* newFont){
		font=newFont;
		if(rect.XWIN){
			XLABEL.destroy();
			XLABEL.init(&rect,textCol,font);
			if(internal.drawPtr){ internal.drawPtr(this); }
		}
	}

	void OLabel::setText(const char* newText){
		string=newText;
		if(internal.drawPtr){ internal.drawPtr(this); }
	}

	void OLabel::setCol(unsigned char r, unsigned char g, unsigned char b){
		internalTextCol.setTo(r,g,b);
		textCol=&internalTextCol;
		if(internal.drawPtr){ internal.drawPtr(this); }
	}
	void OLabel::setCol(OCol& c){
		if	    (&c==&OTHEME_PRIMARY)  { textCol=&OTHEME_PRIMARY; }
		else if (&c==&OTHEME_SECONDARY){ textCol=&OTHEME_SECONDARY; }
		else if (&c==&OTHEME_TERTIARY) { textCol=&OTHEME_TERTIARY; }
		else if (&c==&OTHEME_ACCENT)   { textCol=&OTHEME_ACCENT; }
		else{ internalTextCol=c; textCol=&internalTextCol; }

		if(internal.drawPtr){ internal.drawPtr(this); }
	}


	void OLabel::setBackgroundCol(unsigned char r, unsigned char g, unsigned char b){
		setThemeSecondaryCol(r,g,b);
	}
	void OLabel::setBackgroundCol(OCol& c){ setThemeSecondaryCol(c); }

	namespace DRAW{
		void OLabel(CDrawable* obj){
			Orion::OLabel* label=(Orion::OLabel*)obj;
			if(!label->ready || !label->rect.XWIN || !label->context){return;}
			label->rect.setCol(label->theme.secondary);
			label->rect.clear();
			label->XLABEL.draw(&(label->rect),label->textCol,label->w*label->scale,label->h*label->scale,label->string);
			if(!label->fullRedraw){return;}

			label->rect.setGeometry(
				label->offsetX*label->scale,label->offsetY*label->scale,
				label->w*label->scale,label->h*label->scale,true
			);
			label->fullRedraw=false;
		}
	}

	namespace HANDLE{
		void OLabel(void* obj, X::CXEvent* event){
			Orion::OLabel* label=(Orion::OLabel*)obj;
			if(!label->ready || !label->rect.XWIN || !label->context){return;}
			switch(event->type){
				default:{ return; }
				case X::CXE_EXPOSE:{ DRAW::OLabel(label); return; }
			}
		}
	}

}