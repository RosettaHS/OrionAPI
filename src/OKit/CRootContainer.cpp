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
#include "../include/OKit/OWidget.hpp"
#include "../include/OKit/OContainer.hpp"
#include "../include/OKit/CRootContainer.hpp"

#define DEF_MINW    50
#define DEF_MINH    50
#define DEF_ARRCAP  10
#define DEF_ARRSTEP 5

namespace Orion{
	/*** Constructors/Destructors ***/	
	CRootContainer::CRootContainer(int16_t ix, int16_t iy, uint16_t iw, uint16_t ih, const char* it, uint32_t imask, CCType itype){
		OUIONLY{
			init(OUI_ROOTCONTAINER,ix,iy,iw,ih);

			surfaceToUse=&canvas;
			containerToUse=this;
			list.init(DEF_ARRCAP, DEF_ARRSTEP);
			flags.inited=true;
			flags.setColMode=OWIDGET_SETCOL_USE_PRIMARY;

			CContext* context=canvas.getAsContext(); /* Weird pointer trickery. Don't do this in production code! */
			context->create(0,x,y,w,h,it,theme.primary,imask,itype);
			context->map( ( (imask) ? true : false ) );
			flags.linked=true;
			XCB_FLUSH();
			/*
			 * The only reason the weird shit above works is because
			 * Surface checks the Context in all of its methods.
			 */
		}else{
			OERROR(OERR_GRAPHICS_NOT_INITED,true,"FAILED TO CREATE CROOTCONTAINER BECAUSE GRAPHICS ARE NOT INITIALISED!");
		}
	}
	/*** Deferrables ***/
	void CRootContainer::onPosChanged(void){
		canvas.setPos(x,y);
		sort();
	}
	void CRootContainer::onSizeChanged(void){
		canvas.setGeometry(x,y,w,h);
		sort();
	}
	void CRootContainer::onColChanged(void){
		canvas.setCol(theme.primary);
	}
	/*** Setters ***/
	/*** Getters ***/
	/*** Misc ops ***/
}