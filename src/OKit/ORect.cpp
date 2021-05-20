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

#include "../include/Application.hpp"
#include "../include/OSL/OMath.hpp"
#include "../include/OKit/ORect.hpp"

/*
 * This is the most barebones Widget.
 * Good for learning how Widgets work internally.
 */

namespace Orion{
	/*** Constructors/Destructors ***/
	ORect::ORect(OContainer* parent, int16_t ix, int16_t iy, uint16_t iw, uint16_t ih, OCol* icol){
		OUIONLY{
			init(OUI_RECT,ix,iy,iw,ih);
			flags.setColMode=OWIDGET_SETCOL_USE_SECONDARY;
			setCol(icol);

			flags.inited=true;
			linkTo(parent);
		}else{
			OERROR(OERR_GRAPHICS_NOT_INITED,true,"FAILED TO CREATE ORECT BECAUSE GRAPHICS ARE NOT INITIALISED!");
		}
	}

	/*** Deferrables ***/
	void ORect::onLink(void){
 		canvas.create(parentSurface,getTrueGeo(),theme.secondary);
	}
	void ORect::onUnlink(void){
		canvas.destroy();
	}
	void ORect::onPosChanged(void){
		canvas.setPos( getTruePos() );
	}
	void ORect::onSizeChanged(void){
		canvas.setGeometry( getTrueGeo() );
	}
	void ORect::onColChanged(void){
		canvas.setCol(theme.secondary);
	}
	void ORect::onDraw(bool full){
		canvas.setCol(theme.secondary);
		if(full){ canvas.setGeometry( getTrueGeo() ); }
	}
}