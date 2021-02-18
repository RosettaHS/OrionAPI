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
#include "../include/errdef.hpp"
#include "../include/application.hpp"
#include "../include/OSL/OLog.hpp"
#include "../include/OKit/CDrawable.hpp"
#include "../include/OKit/CContainer.hpp"

namespace Orion{

	CDrawable::~CDrawable(void){
		if(parentContainer){
			parentContainer->unlink(*this);
		}
		type=OT_ERROR;
		ready=false;
	}

	CDrawable::CDrawable(void) : 
		x{0},y{0},
		centreX{0},centreY{0},
		offsetX{0},offsetY{0},
		w{0},h{0},
		minW{0},minH{0},
		scale{1},rotation{0},index{-1},fullRedraw{false},focused{false},
		context{0},parentDrawable{0},parentContainer{0},
		internalTheme{OTHEME},theme{0,0,0,0},
		internal{0,0} {
			type=OT_CDRAWABLE;
			ready=false;
			theme.primary=&(OTHEME.primary);
			theme.secondary=&(OTHEME.secondary);
			theme.tertiary=&(OTHEME.tertiary);
			theme.accent=&(OTHEME.accent);
		}

	void CDrawable::draw(bool force){
		if(internal.drawPtr){
			fullRedraw=force;
			internal.drawPtr(this);
		}else{
			OLog("ORIONAPI | WARNING! CAN'T CALL DRAW() ON AN OBJECT THAT HASN'T BEEN INITIALISED YET!\n");
		}
	}

	/* Base drawable does nothing when linked or modified. */
	void CDrawable::onLink(void){ return; }
	void CDrawable::onPosChanged(void){ return; }
	void CDrawable::onSizeChanged(void){ return; }
	void CDrawable::onUnlink(void){ return; }
	void CDrawable::onFocusChanged(void){ return; }

	bool CDrawable::linkTo(CContainer& container){ return container.link(*this); }

	bool CDrawable::unlinkThis(void){
		if(parentContainer){
			return parentContainer->unlink(*this);		
		}else{return false;}
	}

	void CDrawable::init(int _x, int _y, unsigned int _w, unsigned int _h){
		if(!setFlag(_x,0,0,0)){x=_x;}else{x=0;}
		if(!setFlag(0,_y,0,0)){y=_y;}else{y=0;}
		if(!setFlag(0,0,_w,0)){w=_w;}else{w=minW;}
		if(!setFlag(0,0,0,_h)){h=_h;}else{h=minH;}

	/* Extra error checking */
		if(internal.modFlags & _OUI_W_FILL){
			if(internal.modFlags & _OUI_X_CENTRE){ OLog("ORIONAPI | ERROR! CAN'T COMBINE \"FILL\" SIZE FLAG WITH \"CENTRE\" POSITONAL FLAG ON THE SAME AXIS! (X | W AXIS)\n"); exit(OERR_CDRAWABLE_INVALID_FLAG); }
			if(internal.modFlags & _OUI_X_END){ OLog("ORIONAPI | ERROR! CAN'T COMBINE \"FILL\" SIZE FLAG WITH \"END\" POSITONAL FLAG ON THE SAME AXIS! (X | W AXIS)\n"); exit(OERR_CDRAWABLE_INVALID_FLAG); }
		}
		if(internal.modFlags & _OUI_H_FILL){
			if(internal.modFlags & _OUI_Y_CENTRE){ OLog("ORIONAPI | ERROR! CAN'T COMBINE \"FILL\" SIZE FLAG WITH \"CENTRE\" POSITONAL FLAG ON THE SAME AXIS! (Y | H AXIS)\n"); exit(OERR_CDRAWABLE_INVALID_FLAG); }
			if(internal.modFlags & _OUI_Y_END){ OLog("ORIONAPI | ERROR! CAN'T COMBINE \"FILL\" SIZE FLAG WITH \"END\" POSITONAL FLAG ON THE SAME AXIS! (Y | H AXIS)\n"); exit(OERR_CDRAWABLE_INVALID_FLAG); }
		}
		
		if(_w<minW){minW=_w;}
		if(_h<minH){minH=_h;}
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
			case FILL:{OLog("ORIONAPI | ERROR! CAN'T SET X POSITION FLAG WITH \"FILL\" SIZE FLAG (SIZE ONLY)!\n"); exit(OERR_CDRAWABLE_INVALID_FLAG);}
		}

		switch(yFlag){
			case START:{internal.modFlags|=_OUI_Y_START; return true;}
			case END:{internal.modFlags|=_OUI_Y_END; return true;}
			case CENTER:{internal.modFlags|=_OUI_Y_CENTRE; return true;}
			case FILL:{OLog("ORIONAPI | ERROR! CAN'T SET Y POSITION FLAG WITH \"FILL\" SIZE FLAG (SIZE ONLY)!\n"); exit(OERR_CDRAWABLE_INVALID_FLAG);}
		}

		switch(wFlag){
			case START:{OLog("ORIONAPI | ERROR! CAN'T SET WIDTH SIZE FLAG WITH \"START\" POSITIONAL FLAG (POSITIONAL ONLY)!\n"); exit(OERR_CDRAWABLE_INVALID_FLAG);}
			case END:{OLog("ORIONAPI | ERROR! CAN'T SET WIDTH SIZE FLAG WITH \"END\" POSITIONAL FLAG (POSITIONAL ONLY)!\n"); exit(OERR_CDRAWABLE_INVALID_FLAG);}
			case CENTER:{OLog("ORIONAPI | ERROR! CAN'T SET WIDTH SIZE FLAG WITH \"CENTER\" POSITIONAL FLAG (POSITIONAL ONLY)!\n"); exit(OERR_CDRAWABLE_INVALID_FLAG);}
			case FILL:{internal.modFlags|=_OUI_W_FILL; return true;}
		}

		switch(hFlag){
			case START:{OLog("ORIONAPI | ERROR! CAN'T SET HEIGHT SIZE FLAG WITH \"START\" POSITIONAL FLAG (POSITIONAL ONLY)!\n"); exit(OERR_CDRAWABLE_INVALID_FLAG);}
			case END:{OLog("ORIONAPI | ERROR! CAN'T SET HEIGHT SIZE FLAG WITH \"END\" POSITIONAL FLAG (POSITIONAL ONLY)!\n"); exit(OERR_CDRAWABLE_INVALID_FLAG);}
			case CENTER:{OLog("ORIONAPI | ERROR! CAN'T SET HEIGHT SIZE FLAG WITH \"CENTER\" POSITIONAL FLAG (POSITIONAL ONLY)!\n"); exit(OERR_CDRAWABLE_INVALID_FLAG);}
			case FILL:{internal.modFlags|=_OUI_H_FILL; return true;}
		}

		return false;
	}

	void CDrawable::setPos(int _x, int _y){
		if(x==_x && y==_y){return;}
		x=_x,y=_y;
		offsetX=(int)( (float)(x-(int)((float)centreX*(scale-1)) )/scale );
		offsetY=(int)( (float)(y-(int)((float)centreY*(scale-1)) )/scale );
		onPosChanged();
	}
	void CDrawable::setPos(OVec& v){ setPos(v.x, v.y); }
	void CDrawable::setCentre(int _x, int _y){ centreX=_x,centreY=_y; onPosChanged(); }
	void CDrawable::setCentre(OVec& v){ setCentre(v.x,v.y); }
	void CDrawable::setSize(unsigned int _w, unsigned int _h, bool force){
		if( !force && (w==_w) && (h==_h) ){return;}
		if(_w<minW){w=minW;}else{w=_w;}
		if(_h<minH){h=minH;}else{h=_h;}
		centreX=w/2;
		centreY=h/2;
		offsetX=( (x-(centreX*(scale-1)) )/scale );
		offsetY=( (y-(centreY*(scale-1)) )/scale );
		onSizeChanged();
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

	void CDrawable::setGeometry(int _x, int _y, unsigned int _w, unsigned int _h){
		bool posChanged=false;
		bool sizeChanged=false;
		if(x!=_x || y!=_y){ posChanged=true; }
		if(w!=_w || h!=_h){ sizeChanged=true; }
		x=_x,y=_y;
		if(_w<minW){w=minW;}else{w=_w;}
		if(_h<minH){h=minH;}else{h=_h;}
		centreX=w/2;
		centreY=h/2;
		offsetX=( (x-(centreX*(scale-1)) )/scale );
		offsetY=( (y-(centreY*(scale-1)) )/scale );
		if(posChanged){ onPosChanged(); }
		if(sizeChanged){ onSizeChanged(); }
	}
	void CDrawable::setGeometry(OVec4& v){ setGeometry(v.x,v.y,v.w,v.h); }

	void CDrawable::setFocus(bool newFocus){
		if(newFocus){
			if(OAPP_FOCUSEDELEMENT){
				if(OAPP_FOCUSEDELEMENT==this){ return; }
				else{ OAPP_FOCUSEDELEMENT->setFocus(false); }
			}
			OAPP_FOCUSEDELEMENT=this;
		}else{
			if(OAPP_FOCUSEDELEMENT && OAPP_FOCUSEDELEMENT!=this){
				OAPP_FOCUSEDELEMENT->setFocus(false);
			}
			OAPP_FOCUSEDELEMENT=0;
		}
		if(focused==newFocus){ return; }

		focused=newFocus;
		onFocusChanged();
	}

	void CDrawable::setCol(unsigned char r, unsigned char g, unsigned char b){
		OLog("ORIONAPI | WARNING! %s DOES NOT SUPPORT COLOUR MODIFICATION! FAILED TO SET COLOUR TO (%u, %u, %u)!\n",getTypeAsString(),r,g,b);
	}
	void CDrawable::setCol(OCol& c){ setCol(c.r,c.g,c.b); }

	void CDrawable::setTheme(OTheme& newTheme){
		if(&newTheme==&OTHEME){ resetTheme(); return; }
		internalTheme=newTheme;
		theme.primary=&(internalTheme.primary);
		theme.secondary=&(internalTheme.secondary);
		theme.tertiary=&(internalTheme.tertiary);
		theme.accent=&(internalTheme.accent);
		if(internal.drawPtr){internal.drawPtr(this);}
	}

	void CDrawable::setThemePrimaryCol(unsigned char r, unsigned char g, unsigned char b){
		internalTheme.setPrimary(r,g,b);
		theme.primary=&(internalTheme.primary);
		if(internal.drawPtr){internal.drawPtr(this);}
	}
	void CDrawable::setThemePrimaryCol(OCol& c){
		if(&c==&OTHEME_PRIMARY){ theme.primary=&OTHEME_PRIMARY; if(internal.drawPtr){internal.drawPtr(this);} return; }
		else if(&c==&OTHEME_SECONDARY){ theme.primary=&OTHEME_SECONDARY; if(internal.drawPtr){internal.drawPtr(this);} return; }
		else if(&c==&OTHEME_TERTIARY){ theme.primary=&OTHEME_TERTIARY; if(internal.drawPtr){internal.drawPtr(this);} return; }
		else if(&c==&OTHEME_ACCENT){ theme.primary=&OTHEME_ACCENT; if(internal.drawPtr){internal.drawPtr(this);} return; }
		else{ setThemeSecondaryCol(c.r,c.g,c.b); }
	}

	void CDrawable::setThemeSecondaryCol(unsigned char r, unsigned char g, unsigned char b){
		internalTheme.setSecondary(r,g,b);
		theme.secondary=&(internalTheme.secondary);
		if(internal.drawPtr){internal.drawPtr(this);}
	}
	void CDrawable::setThemeSecondaryCol(OCol& c){
		if(&c==&OTHEME_PRIMARY){ theme.secondary=&OTHEME_PRIMARY; if(internal.drawPtr){internal.drawPtr(this);} return; }
		else if(&c==&OTHEME_SECONDARY){ theme.secondary=&OTHEME_SECONDARY; if(internal.drawPtr){internal.drawPtr(this);} return; }
		else if(&c==&OTHEME_TERTIARY){ theme.secondary=&OTHEME_TERTIARY; if(internal.drawPtr){internal.drawPtr(this);} return; }
		else if(&c==&OTHEME_ACCENT){ theme.secondary=&OTHEME_ACCENT; if(internal.drawPtr){internal.drawPtr(this);}; return; }
		else{ setThemeSecondaryCol(c.r,c.g,c.b); }
	}

	void CDrawable::setThemeTertiaryCol(unsigned char r, unsigned char g, unsigned char b){
		internalTheme.setTertiary(r,g,b);
		theme.tertiary=&(internalTheme.tertiary);
		if(internal.drawPtr){internal.drawPtr(this);}
	}
	void CDrawable::setThemeTertiaryCol(OCol& c){
		if(&c==&OTHEME_PRIMARY){ theme.tertiary=&OTHEME_PRIMARY; if(internal.drawPtr){internal.drawPtr(this);} return; }
		else if(&c==&OTHEME_SECONDARY){ theme.tertiary=&OTHEME_SECONDARY; if(internal.drawPtr){internal.drawPtr(this);} return; }
		else if(&c==&OTHEME_TERTIARY){ theme.tertiary=&OTHEME_TERTIARY; if(internal.drawPtr){internal.drawPtr(this);} return; }
		else if(&c==&OTHEME_ACCENT){ theme.tertiary=&OTHEME_ACCENT; if(internal.drawPtr){internal.drawPtr(this);} return; }
		else{ setThemeTertiaryCol(c.r,c.g,c.b); }
	}

	void CDrawable::setThemeAccentCol(unsigned char r, unsigned char g, unsigned char b){
		internalTheme.setAccent(r,g,b);
		theme.accent=&(internalTheme.accent);
		if(internal.drawPtr){internal.drawPtr(this);}
	}
	void CDrawable::setThemeAccentCol(OCol& c){
		if(&c==&OTHEME_PRIMARY){ theme.accent=&OTHEME_PRIMARY; if(internal.drawPtr){internal.drawPtr(this);} return; }
		else if(&c==&OTHEME_SECONDARY){ theme.accent=&OTHEME_SECONDARY; if(internal.drawPtr){internal.drawPtr(this);} return; }
		else if(&c==&OTHEME_TERTIARY){ theme.accent=&OTHEME_TERTIARY; if(internal.drawPtr){internal.drawPtr(this);} return; }
		else if(&c==&OTHEME_ACCENT){ theme.accent=&OTHEME_ACCENT; if(internal.drawPtr){internal.drawPtr(this);}; return; }
		else{ setThemeAccentCol(c.r,c.g,c.b); }
	}

	void CDrawable::resetTheme(void){
		internalTheme=OTHEME;
		theme.primary=&(OTHEME.primary);
		theme.secondary=&(OTHEME.secondary);
		theme.tertiary=&(OTHEME.tertiary);
		theme.accent=&(OTHEME.accent);
		if(internal.drawPtr){internal.drawPtr(this);}
	}

/* Getters */

	CDrawable* CDrawable::getParent(void) const{ return parentDrawable; }

	OVec CDrawable::getPos(bool globalToWindow) const{
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

	OVec CDrawable::getCentre(void) const{ return OVec(centreX,centreY); }

	OVec4 CDrawable::getSize(bool useScale) const{
		OVec4 v;
		if(useScale){
			float s=getScale();
			v={0,0,(unsigned int)((float)w*s),(unsigned int)((float)h*s)}; /* WHY!?!?! */
		}else{
			v={0,0,w,h};
		}
		return v;
	}

	OVec4 CDrawable::getMinSize(bool useScale) const{
		OVec4 v;
		if(useScale){
			float s=getScale();
			v={0,0,(unsigned int)((float)minW*s),(unsigned int)((float)minH*s)}; /* WHY!?!?! */
		}else{
			v={0,0,minW,minH};
		}
		return v;
	}

	float CDrawable::getScale(void) const{ return scale; }
	// float CDrawable::getScale(bool includeParents){
		// float s;
		// if(includeParents&&parentDrawable){
			// s=scale*parentDrawable->getScale(true);
		// }else{
			// s=scale;
		// }
		// return s;
	// }

	float CDrawable::getRotation(bool includeParents) const{
		float r;
		if(includeParents&&parentDrawable){
			r=rotation+parentDrawable->getRotation(true);
		}else{
			r=rotation;
		}
		return r;
	}

	OVec4 CDrawable::getGeometry(bool globalToWindow) const{
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

	bool CDrawable::getFocus(void) const{ return focused; }

	OTheme CDrawable::getTheme(void) const{ return internalTheme; }

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
			OLog("\t Context->XROOT : %lu\n",(context ? context->XROOT : 0));
			OLog("\t Parent Drawable : %p\n",parentDrawable);
			OLog("\t Parent Container : %p\n",parentContainer);
			OLog("\t Draw Pointer : %p\n",(void*)internal.drawPtr);
			OLog("\t Child Index : %d\n",index);
			OLog("\t Ready/Initialised : %d\n",ready);
			OLog("\t Focused : %s\n",(focused ? "true" : "false"));
			OLog("\t----THEME----\n");
			OLog("\t Theme : primary (%d, %d, %d)\n",theme.primary->r,theme.primary->g,theme.primary->b);
			OLog("\t Theme : secondary (%d, %d, %d)\n",theme.secondary->r,theme.secondary->g,theme.secondary->b);
			OLog("\t Theme : tertiary (%d, %d, %d)\n",theme.tertiary->r,theme.tertiary->g,theme.tertiary->b);
			OLog("\t Theme : accent (%d, %d, %d)\n",theme.accent->r,theme.accent->g,theme.accent->b);
		}else{
			OLog("(x %d, y %d, w %u, h %u)\n",x,y,w,h);
		}
	}

	namespace Application{
		CDrawable* focusedElement=0;
	}
}
