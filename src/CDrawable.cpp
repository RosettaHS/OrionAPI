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
#include "include/application.hpp"
#include "include/OLog.hpp"
#include "include/CDrawable.hpp"
#include "include/CContainer.hpp"

namespace Orion{
	CDrawable::CDrawable(void) : 
		x{0},y{0},
		centreX{0},centreY{0},
		offsetX{0},offsetY{0},
		w{0},h{0},
		minW{0},minH{0},
		scale{1},rotation{0},index{-1},fullRedraw{false},
		context{0},parentDrawable{0},parentContainer{0},
		internalTheme{OTHEME},theme{0,0,0,0},themeFlags{0},
		internal{0,0} {
			type=OT_CDRAWABLE;
			ready=false;
			theme.primary=&(OTHEME.primary);
			theme.secondary=&(OTHEME.secondary);
			theme.tertiary=&(OTHEME.tertiary);
			theme.accent=&(OTHEME.accent);
		}

	bool CDrawable::linkTo(CContainer& container){ return container.link(*this); }

	bool CDrawable::unlinkTo(CContainer& container){ return container.unlink(*this); }

	void CDrawable::init(int _x, int _y, unsigned int _w, unsigned int _h){
		if(!setFlag(_x,0,0,0)){x=_x;}else{x=0;}
		if(!setFlag(0,_y,0,0)){y=_y;}else{y=0;}
		if(!setFlag(0,0,_w,0)){w=_w;}else{w=0;}
		if(!setFlag(0,0,0,_h)){h=_h;}else{h=0;}
		if(_w<minW){minW=_w;}
		if(_w<minH){minH=_h;}
		w=_w,h=_h;
		centreX=w/2,centreY=h/2;
		offsetX=( (x-(centreX*(scale-1)) )/scale );
		offsetY=( (y-(centreY*(scale-1)) )/scale );
	}

/* Setters */

	bool CDrawable::setFlag(short xFlag, short yFlag, short wFlag, short hFlag){
		switch(xFlag){
			case START:{internal.modFlags|=_OUI_X_START; return true;}
			case END:{internal.modFlags|=_OUI_X_END; return true;}
			case CENTER:{internal.modFlags|=_OUI_X_CENTRE; return true;}
			case FILL:{OLog("OKIT | ERROR! CAN'T SET X POSITION WITH FILL (SIZE ONLY) FLAG!\n"); exit(OERR_CDRAWABLE_INVALID_FLAG);}
		}

		switch(yFlag){
			case START:{internal.modFlags|=_OUI_Y_START; return true;}
			case END:{internal.modFlags|=_OUI_Y_END; return true;}
			case CENTER:{internal.modFlags|=_OUI_Y_CENTRE; return true;}
			case FILL:{OLog("OKIT | ERROR! CAN'T SET Y POSITION WITH FILL (SIZE ONLY) FLAG!\n"); exit(OERR_CDRAWABLE_INVALID_FLAG);}
		}

		switch(wFlag){
			case START:{OLog("OKIT | ERROR! CAN'T SET W SIZE WITH START (POSITIONAL ONLY) FLAG!\n"); exit(OERR_CDRAWABLE_INVALID_FLAG);}
			case END:{OLog("OKIT | ERROR! CAN'T SET W SIZE WITH END (POSITIONAL ONLY) FLAG!\n"); exit(OERR_CDRAWABLE_INVALID_FLAG);}
			case CENTER:{OLog("OKIT | ERROR! CAN'T SET W SIZE WITH CENTER (POSITIONAL ONLY) FLAG!\n"); exit(OERR_CDRAWABLE_INVALID_FLAG);}
			case FILL:{internal.modFlags|=_OUI_W_FILL; return true;}
		}

		switch(hFlag){
			case START:{OLog("OKIT | ERROR! CAN'T SET H SIZE WITH START (POSITIONAL ONLY) FLAG!\n"); exit(OERR_CDRAWABLE_INVALID_FLAG);}
			case END:{OLog("OKIT | ERROR! CAN'T SET H SIZE WITH END (POSITIONAL ONLY) FLAG!\n"); exit(OERR_CDRAWABLE_INVALID_FLAG);}
			case CENTER:{OLog("OKIT | ERROR! CAN'T SET H SIZE WITH (POSITIONAL ONLY) CENTER FLAG!\n"); exit(OERR_CDRAWABLE_INVALID_FLAG);}
			case FILL:{internal.modFlags|=_OUI_H_FILL; return true;}
		}

		return false;
	}

	void CDrawable::setPos(int _x, int _y){
		x=_x,y=_y;
		offsetX=(int)( (float)(x-(int)((float)centreX*(scale-1)) )/scale );
		offsetY=(int)( (float)(y-(int)((float)centreY*(scale-1)) )/scale );
	}
	void CDrawable::setPos(OVec& v){ setPos(v.x, v.y); }
	void CDrawable::setCentre(int _x, int _y){ centreX=_x,centreY=_y; }
	void CDrawable::setSize(unsigned int _w, unsigned int _h, bool force){
		if( !force && (w==_w) && (h==_h) ){return;}
		if(_w<minW){w=minW;}else{w=_w;}
		if(_h<minH){h=minH;}else{h=_h;}
		centreX=w/2;
		centreY=h/2;
		offsetX=( (x-(centreX*(scale-1)) )/scale );
		offsetY=( (y-(centreY*(scale-1)) )/scale );
		fullRedraw=true;
		if(internal.drawPtr){internal.drawPtr(this);}
	}
	void CDrawable::setSize(OVec& v, bool force){ setSize(v.x,v.y,force); }
	void CDrawable::setMinSize(unsigned int _w, unsigned int _h){
		minW=_w,minH=_h;
		if( (w<minW) || (h<minH) ){setSize(_w,_h);}
	}
	void CDrawable::setMinSize(OVec& v){ setMinSize(v.x,v.y); }
	void CDrawable::setScale(float _s){
		if(_s>=0.01){scale=_s;}else{scale=0.01;}
		setPos(x,y);
		setSize(w,h,true);
	}
	void CDrawable::setRotation(float _r){
		rotation=_r;
		fullRedraw=true;
		if(internal.drawPtr){internal.drawPtr(this);}
	}

	void CDrawable::setCol(unsigned char r, unsigned char g, unsigned char b){
		OLog("OKIT | WARNING! %s DOES NOT SUPPORT COLOUR MODIFICATION! FAILED TO SET COLOUR TO (%u, %u, %u)!\n",getTypeAsString(),r,g,b);
	}
	void CDrawable::setCol(OCol& c){ setCol(c.r,c.g,c.b); }

	void CDrawable::setTheme(OTheme& newTheme){
		internalTheme=newTheme;
		theme.primary=&(internalTheme.primary);
		theme.secondary=&(internalTheme.secondary);
		theme.tertiary=&(internalTheme.tertiary);
		theme.accent=&(internalTheme.accent);
		themeFlags=_CTHEME_OVERRIDE_PRIMARY|_CTHEME_OVERRIDE_SECONDARY|_CTHEME_OVERRIDE_TERTIARY|_CTHEME_OVERRIDE_ACCENT;
		fullRedraw=true;
		if(internal.drawPtr){internal.drawPtr(this);}
	}

	void CDrawable::setPrimaryCol(unsigned char r, unsigned char g, unsigned char b){
		internalTheme.setPrimary(r,g,b);
		theme.primary=&(internalTheme.primary);
		themeFlags|=_CTHEME_OVERRIDE_PRIMARY;
		fullRedraw=true;
		if(internal.drawPtr){internal.drawPtr(this);}
	}
	void CDrawable::setPrimaryCol(OCol& c){setPrimaryCol(c.r,c.g,c.b);}

	void CDrawable::setSecondaryCol(unsigned char r, unsigned char g, unsigned char b){
		internalTheme.setSecondary(r,g,b);
		theme.secondary=&(internalTheme.secondary);
		themeFlags|=_CTHEME_OVERRIDE_SECONDARY;
		fullRedraw=true;
		if(internal.drawPtr){internal.drawPtr(this);}
	}
	void CDrawable::setSecondaryCol(OCol& c){setSecondaryCol(c.r,c.g,c.b);}

	void CDrawable::setTertiaryCol(unsigned char r, unsigned char g, unsigned char b){
		internalTheme.setTertiary(r,g,b);
		theme.tertiary=&(internalTheme.tertiary);
		themeFlags|=_CTHEME_OVERRIDE_TERTIARY;
		fullRedraw=true;
		if(internal.drawPtr){internal.drawPtr(this);}
	}
	void CDrawable::setTertiaryCol(OCol& c){setTertiaryCol(c.r,c.g,c.b);}

	void CDrawable::setAccentCol(unsigned char r, unsigned char g, unsigned char b){
		internalTheme.setAccent(r,g,b);
		theme.accent=&(internalTheme.accent);
		themeFlags|=_CTHEME_OVERRIDE_ACCENT;
		fullRedraw=true;
		if(internal.drawPtr){internal.drawPtr(this);}
	}
	void CDrawable::setAccentCol(OCol& c){setAccentCol(c.r,c.g,c.b);}

	void CDrawable::resetTheme(void){
		internalTheme=OTHEME;
		theme.primary=&(OTHEME.primary);
		theme.secondary=&(OTHEME.secondary);
		theme.tertiary=&(OTHEME.tertiary);
		theme.accent=&(OTHEME.accent);
		themeFlags=0;
		fullRedraw=true;
		if(internal.drawPtr){internal.drawPtr(this);}
	}

/* Getters */

	OVec CDrawable::getPos(bool globalToWindow){
		OVec v;
		if(globalToWindow&&parentDrawable){
			if(parentDrawable->type!=OT_OWINDOW){
				v=parentDrawable->getPos(true);
				v.x+=x,v.y+=y;
			}else{
				v={0,0};
			}
		}else{
			v={x,y};
		}
		return v;
	}

	OVec CDrawable::getCentre(void){ return OVec(centreX,centreY); }

	OVec4 CDrawable::getSize(bool useScale){
		OVec4 v;
		if(useScale){
			float s=getScale();
			v={0,0,(unsigned int)((float)w*s),(unsigned int)((float)h*s)}; /* WHY!?!?! */
		}else{
			v={0,0,w,h};
		}
		return v;
	}

	OVec4 CDrawable::getMinSize(bool useScale){
		OVec4 v;
		if(useScale){
			float s=getScale();
			v={0,0,(unsigned int)((float)minW*s),(unsigned int)((float)minH*s)}; /* WHY!?!?! */
		}else{
			v={0,0,minW,minH};
		}
		return v;
	}

	float CDrawable::getScale(void){ return scale; }

	float CDrawable::getRotation(bool includeParents){
		float r;
		if(includeParents&&parentDrawable){
			r=rotation+parentDrawable->getRotation(true);
		}else{
			r=rotation;
		}
		return r;
	}

	OVec4 CDrawable::getGeometry(bool globalToWindow){
		OVec4 v;
		if(globalToWindow&&parentDrawable){
			if(parentDrawable->type!=OT_OWINDOW){
				v=parentDrawable->getGeometry(true);
				v.x+=x,v.y+=y;
				v.w+=w,v.h+=h;
			}else{v={0,0,0,0};}
		}else{
			v={x,y,w,h};
		}
		return v;
	}

	int CDrawable::getIndex(void){
		if(parentContainer){ index=parentContainer->getIndexOf(*this); return index; }
		else{ return -1; }
	}

	OTheme CDrawable::getTheme(void){ return internalTheme; }

	void CDrawable::log(bool verbose){
		if(verbose){
			OVec v;
			OVec4 v4;

			OLog("CDrawable %p | type %s {\n",(void*)this,getTypeAsString());
			v=getPos();
			OLog("\t----POSITION----\n");
			OLog("\t Position : (%d, %d)\n",v.x,v.y);
			v=getPos(true);
			OLog("\t Position - Global : (%d, %d)\n",v.x,v.y);
			v=getCentre();
			OLog("\t Relative Centre : (%d, %d)\n",v.x,v.y);
			v4=getSize();
			OLog("\t----SIZE----\n");
			OLog("\t Size : (%u, %u)\n",v4.w,v4.h);
			v4=getSize(true);
			OLog("\t Size - Scaled Global : (%u, %u)\n",v4.w,v4.h);
			v4=getMinSize();
			OLog("\t Minimum Size : (%u, %u)\n",v4.w,v4.h);
			v4=getMinSize(true);
			OLog("\t Minimum Size - Scaled Global : (%u, %u)\n",v4.w,v4.h);
			OLog("\t Scale : %f\n",getScale());
			OLog("\t----INTERNAL----\n");
			OLog("\t Context : %p\n",(void*)context);
			OLog("\t Context->XROOT : %lu\n",context->XROOT);
			OLog("\t Parent Drawable : %p\n",parentDrawable);
			OLog("\t Parent Container : %p\n",parentContainer);
			OLog("\t Draw Pointer : %p\n",(void*)internal.drawPtr);
			OLog("\t Child Index : %d\n",index);
			OLog("\t Ready/Initialised : %d\n",ready);
			OLog("\t----THEME----\n");
			OLog("\t Theme : primary (%d, %d, %d)\n",theme.primary->r,theme.primary->g,theme.primary->b);
			OLog("\t Theme : secondary (%d, %d, %d)\n",theme.secondary->r,theme.secondary->g,theme.secondary->b);
			OLog("\t Theme : tertiary (%d, %d, %d)\n",theme.tertiary->r,theme.tertiary->g,theme.tertiary->b);
			OLog("\t Theme : accent (%d, %d, %d)\n",theme.accent->r,theme.accent->g,theme.accent->b);
			OLog("\t Theme - Flags (P,S,T,A Overrides) : (%d, %d, %d, %d)\n}\n",(themeFlags>>_CTHEME_OVERRIDE_PRIMARY),(themeFlags>>_CTHEME_OVERRIDE_SECONDARY),(themeFlags>>_CTHEME_OVERRIDE_TERTIARY),(themeFlags>>_CTHEME_OVERRIDE_ACCENT));
		}else{
			OLog("(x %d, y %d, w %u, h %u)\n",x,y,w,h);
		}
	}
}