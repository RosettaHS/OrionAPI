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

namespace Orion{
	static size_t calcLength(const char* t){
		if(!t){ return 0; }
		size_t l=0;
		while(true){
			if(t[l]!='\0'){l++;}else{break;}
		}
		return l;
	}

	CLabel::~CLabel(void){ destroy(); }

	CLabel::CLabel(void) : XFONTNAME{0},XFONT{0},XFID{0},XCOL{0},XGC{0},XT_X{0},XT_Y{0},XT_ASC{0},XT_DES{0} {}

	bool CLabel::init(CContext* drawBody, OCol* col, const char* font){
		if(!drawBody->XWIN || XFONT){ return false; }
		XFONTNAME=font;
		XFONT=XLoadQueryFont(OXDPY,XFONTNAME);
		if(!XFONT){
			OLog("OKIT | WARNING! FAILED TO LOAD FONT \"%s\"! RESORTING TO FALLBACKK FONT \"fixed\"!\n",XFONTNAME);
			XFONT=XLoadQueryFont(OXDPY,"fixed");
			if(!XFONT){ OLog("OKIT | ERROR! FAILED TO LOAD FALLBACK FONT \"fixed\"! CANNOT RENDER ANY TEXT!\n"); exit(OERR_X11_FONTFALLBACK); }
		}
		XFID=((XFontStruct*)XFONT)->fid;
		XCOL=col->XCOL;
		XGC=XCreateGC(OXDPY,drawBody->XWIN,0,0);
		XSetForeground(OXDPY,(GC)XGC,XCOL);
		XSetFont(OXDPY,(GC)XGC,XFID);
		
		return true;
	}

	bool CLabel::destroy(void){
		if(!XFONT){ return false; }
		if( XFreeFont(OXDPY,(XFontStruct*)XFONT) ){
			XFreeGC(OXDPY,(GC)XGC);
			XFONTNAME=0;
			XFONT=0;
			XFID=0;
			XCOL=0;
			XGC=0;
			return true;
		}else{ return false; }
	}

	void CLabel::draw(CContext* drawBody, OCol* col, unsigned int contextWidth, unsigned int contextHeight, const char* string){
		if(!drawBody->XWIN){ return; }
		if(col->XCOL!=XCOL){ XCOL=col->XCOL; XSetForeground(OXDPY,(GC)XGC,XCOL); }
		XCharStruct XT_OVA;
		size_t stringLength=calcLength(string);

		XTextExtents((XFontStruct*)XFONT,string,stringLength, &XT_DIR, &XT_ASC,& XT_DES, &XT_OVA);
		XT_X=(_UINTSCALEBYFLOAT(contextWidth,OAPP_SCALE)-XT_OVA.width)/2;
		XT_Y=(_UINTSCALEBYFLOAT(contextHeight,OAPP_SCALE)/2) + ((XT_ASC - XT_DES) /2);

		XDrawString(OXDPY,drawBody->XWIN,(GC)XGC,XT_X,XT_Y,string,stringLength);
	}
}