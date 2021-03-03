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
#define BUFFER 5

#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include "../include/errdef.hpp"
#include "../include/application.hpp"
#include "../include/OSL/OLog.hpp"
#include "../include/OSL/OString.hpp"
#include "../include/OKit/CLabel.hpp"
#include "../include/OKit/xservice.hpp"

namespace Orion{
	namespace X{
		const char* DEFAULTFONT="-misc-fixed-medium-r-normal--%u-*-*-*-*-*-iso8859-15";
	}

	CLabel::~CLabel(void){ destroy(); }

	CLabel::CLabel(void) :
		XFONTNAMERAW{0},XFONTNAME{0},
		XFONT{0},XFID{0},XCOL{0},XFONTSIZE{0},XFONTSIZERAW{0},
		XGC{0},XT_X{0},XT_Y{0},XT_DIR{0},XT_ASC{0},XT_DES{0}
		{}

	bool CLabel::init(CContext* drawBody, OCol* col, const char* font, unsigned short fontSize){
		if(!drawBody->XWIN || XFONT){ return false; }
		XFONTNAMERAW=font;
		XFONTSIZE=fontSize;
		XFONTSIZERAW=_SCALEFONTSIZE(fontSize,OAPP_SCALE);
		XFONTNAME=(char*)malloc(sizeof(char)*OStringLength(font)+BUFFER);
		sprintf(XFONTNAME,XFONTNAMERAW,XFONTSIZERAW);
		XFONT=XLoadQueryFont(OXDPY,XFONTNAME);
		if(!XFONT){
			OLog("ORIONAPI | WARNING! FAILED TO LOAD FONT \"%s\"! RESORTING TO FALLBACK FONT \"fixed\"!\n",XFONTNAME);
			if(XFONTNAME){ free(XFONTNAME); }
			XFONTNAME=(char*)malloc(sizeof(char)*OStringLength(XFONTNAMERAW)+BUFFER);
			sprintf(XFONTNAME,_DEFAULTFONT,_SCALEFONTSIZE(_DEFAULTFONTSIZE,OAPP_SCALE));
			XFONT=XLoadQueryFont(OXDPY,XFONTNAME);
			if(!XFONT){ OLog("ORIONAPI | ERROR! FAILED TO LOAD FALLBACK FONT \"fixed\"! CANNOT RENDER ANY TEXT!\n"); exit(OERR_X11_FONTFALLBACK); }
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
			if(XFONTNAME){ free(XFONTNAME); }
			XFONTNAMERAW=0;
			XFONTNAME=0;
			XFONT=0;
			XFID=0;
			XCOL=0;
			XFONTSIZE=0;
			XFONTSIZERAW=0;
			XGC=0;
			return true;
		}else{ return false; }
	}

	bool CLabel::setFont(const char* font, unsigned short fontSize){
		if(!XFONT){ return false; }
		if(XFONTNAME){ free(XFONTNAME); }
		XFONTNAME=(char*)malloc(sizeof(char)*OStringLength(font)+BUFFER);
		XFONTSIZERAW=_SCALEFONTSIZE(fontSize,OAPP_SCALE);
		sprintf(XFONTNAME,font,XFONTSIZERAW);
		XFONT=XLoadQueryFont(OXDPY,XFONTNAME);
		if(XFONT){
			XFONTNAMERAW=font;
			XFONTSIZE=fontSize;
			XFID=((XFontStruct*)XFONT)->fid;
			XSetFont(OXDPY,(GC)XGC,XFID);
			return true;
		}else{
			if(XFONTNAME){ free(XFONTNAME); }
			XFONTNAME=(char*)malloc(sizeof(char)*OStringLength(XFONTNAMERAW)+BUFFER);
			sprintf(XFONTNAME,font,_SCALEFONTSIZE(fontSize,OAPP_SCALE));
			XFONT=XLoadQueryFont(OXDPY,XFONTNAME);
			XFID=((XFontStruct*)XFONT)->fid;
			XSetFont(OXDPY,(GC)XGC,XFID);
			return false;
		}
	}

	void CLabel::draw(CContext* drawBody, OCol* col, unsigned int contextWidth, unsigned int contextHeight, const char* string, unsigned short fontSize){
		if(!drawBody->XWIN){ return; }
		if(col->XCOL!=XCOL){ XCOL=col->XCOL; XSetForeground(OXDPY,(GC)XGC,XCOL); }
		if( _SCALEFONTSIZE(fontSize,OAPP_SCALE) != XFONTSIZERAW ){ setFont(XFONTNAMERAW,fontSize); }
		XCharStruct XT_OVA;
		size_t stringLength=OStringLength(string);

		XTextExtents((XFontStruct*)XFONT,string,stringLength, &XT_DIR, &XT_ASC,& XT_DES, &XT_OVA);
		XT_X=(_UINTSCALEBYFLOAT(contextWidth,OAPP_SCALE)-XT_OVA.width)/2;
		XT_Y=(_UINTSCALEBYFLOAT(contextHeight,OAPP_SCALE)/2) + ((XT_ASC - XT_DES) /2);

		XDrawString(OXDPY,drawBody->XWIN,(GC)XGC,XT_X,XT_Y,string,stringLength);
	}
}
