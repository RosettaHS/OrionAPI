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

#include "include/OLog.hpp"
#include "include/CDrawable.hpp"

namespace Orion{
	CDrawable::CDrawable(void) : 
		x{0},y{0},
		centreX{0},centreY{0},
		w{0},h{0},
		minW{0},minH{0},
		scale{1},rotation{0},
		context{0},parentDrawable{0},
		internalTheme{OTHEME},theme{0,0,0,0},themeFlags{0},
		drawPtr{0},flags{0,0} {
			type=OT_CDRAWABLE;
			ready=false;
			theme.primary=&(OTHEME.primary);
			theme.secondary=&(OTHEME.secondary);
			theme.tertiary=&(OTHEME.tertiary);
			theme.accent=&(OTHEME.accent);
		}

/* Setters */

	void CDrawable::setPos(int _x, int _y){ x=_x,y=_y; }
	void CDrawable::setCentre(int _x, int _y){ centreX=_x,centreY=_y; }
	void CDrawable::setSize(unsigned int _w, unsigned int _h){
		if(_w<minW){w=minW;}else{w=_w;}
		if(_h<minH){h=minH;}else{h=_h;}
		if(drawPtr){drawPtr(this);}
	}
	void CDrawable::setMinSize(unsigned int _w, unsigned int _h){
		minW=_w,minH=_h;
		if( (w<minW) || (h<minH) ){setSize(_w,_h);}
	}
	void CDrawable::setScale(float _s){ scale=_s; if(drawPtr){drawPtr(this);} }
	void CDrawable::setRotation(float _r){ rotation=_r; if(drawPtr){drawPtr(this);} }

	void CDrawable::setTheme(OTheme& newTheme){
		internalTheme=newTheme;
		theme.primary=&(internalTheme.primary);
		theme.secondary=&(internalTheme.secondary);
		theme.tertiary=&(internalTheme.tertiary);
		theme.accent=&(internalTheme.accent);
		themeFlags=_CTHEME_OVERRIDE_PRIMARY|_CTHEME_OVERRIDE_SECONDARY|_CTHEME_OVERRIDE_TERTIARY|_CTHEME_OVERRIDE_ACCENT;
	}

	void CDrawable::setPrimaryCol(unsigned char r, unsigned char g, unsigned char b){
		internalTheme.setPrimary(r,g,b);
		theme.primary=&(internalTheme.primary);
		themeFlags|=_CTHEME_OVERRIDE_PRIMARY;
	}
	void CDrawable::setPrimaryCol(OCol& c){setPrimaryCol(c.r,c.g,c.b);}

	void CDrawable::setSecondaryCol(unsigned char r, unsigned char g, unsigned char b){
		internalTheme.setSecondary(r,g,b);
		theme.secondary=&(internalTheme.secondary);
		themeFlags|=_CTHEME_OVERRIDE_SECONDARY;
	}
	void CDrawable::setSecondaryCol(OCol& c){setSecondaryCol(c.r,c.g,c.b);}

	void CDrawable::setTertiaryCol(unsigned char r, unsigned char g, unsigned char b){
		internalTheme.setTertiary(r,g,b);
		theme.tertiary=&(internalTheme.tertiary);
		themeFlags|=_CTHEME_OVERRIDE_TERTIARY;
	}
	void CDrawable::setTertiaryCol(OCol& c){setTertiaryCol(c.r,c.g,c.b);}

	void CDrawable::setAccentCol(unsigned char r, unsigned char g, unsigned char b){
		internalTheme.setAccent(r,g,b);
		theme.accent=&(internalTheme.accent);
		themeFlags|=_CTHEME_OVERRIDE_ACCENT;
	}
	void CDrawable::setAccentCol(OCol& c){setAccentCol(c.r,c.g,c.b);}

	void CDrawable::resetTheme(void){
		internalTheme=OTHEME;
		theme.primary=&(OTHEME.primary);
		theme.secondary=&(OTHEME.secondary);
		theme.tertiary=&(OTHEME.tertiary);
		theme.accent=&(OTHEME.accent);
		themeFlags=0;
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
			float s=getScale(true);
			v={0,0,(unsigned int)((float)w*s),(unsigned int)((float)h*s)}; /* WHY!?!?! */
		}else{
			v={0,0,w,h};
		}
		return v;
	}

	OVec4 CDrawable::getMinSize(bool useScale){
		OVec4 v;
		if(useScale){
			float s=getScale(true);
			v={0,0,(unsigned int)((float)minW*s),(unsigned int)((float)minH*s)}; /* WHY!?!?! */
		}else{
			v={0,0,minW,minH};
		}
		return v;
	}

	float CDrawable::getScale(bool includeParents){
		float s;
		if(includeParents&&parentDrawable){
			s=scale*parentDrawable->getScale(true);
		}else{
			s=scale;
		}
		return s;
	}

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
			OLog("\t Scale - Global : %f\n",getScale(true));
			OLog("\t----INTERNAL----\n");
			OLog("\t Context : %p\n",(void*)context);
			OLog("\t Parent Drawable : %p\n",parentDrawable);
			OLog("\t Draw Pointer : %p\n",(void*)drawPtr);
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