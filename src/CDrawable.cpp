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
	CDrawable::CDrawable() : 
		x{0},y{0},
		centreX{0},centreY{0},
		w{0},h{0},
		minW{0},minH{0},
		scale{0},
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
	void CDrawable::setSecondaryCol(unsigned char r, unsigned char g, unsigned char b){
		internalTheme.setSecondary(r,g,b);
		theme.secondary=&(internalTheme.secondary);
		themeFlags|=_CTHEME_OVERRIDE_SECONDARY;
	}
	void CDrawable::setTertiaryCol(unsigned char r, unsigned char g, unsigned char b){
		internalTheme.setTertiary(r,g,b);
		theme.tertiary=&(internalTheme.tertiary);
		themeFlags|=_CTHEME_OVERRIDE_TERTIARY;
	}
	void CDrawable::setAccentCol(unsigned char r, unsigned char g, unsigned char b){
		internalTheme.setAccent(r,g,b);
		theme.accent=&(internalTheme.accent);
		themeFlags|=_CTHEME_OVERRIDE_ACCENT;
	}
	void CDrawable::resetTheme(){
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

	OVec CDrawable::getCentre(){ return OVec(centreX,centreY); }

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

	OTheme CDrawable::getTheme(){ return internalTheme; }
}