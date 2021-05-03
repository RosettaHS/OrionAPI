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

namespace Orion{

/*** Constructors/Destructors ***/
	OWidget::OWidget(void) :
		type{OUI_ERROR},
		x{0},y{0},w{0},h{0},
		minW{0},minH{0},scale{1},
		index{0},context{0},parent{0},
		drawPtr{0},flags{0,0,0,0,0,0}
		{ theme.primary=0; theme.secondary=0; theme.tertiary=0; theme.accent=0; }
/*** Deferrables ***/
	/* Base Widget does nothing when linked or modified. */
	void OWidget::onLink(void)         { return; }
	void OWidget::onUnlink(void)       { return; }
	void OWidget::onPosChanged(void)   { return; }
	void OWidget::onSizeChanged(void)  { return; }
	void OWidget::onFocusChanged(void) { return; }


/*** Setters ***/
	/** Position and Size **/
	bool OWidget::setPos(int16_t ix, int16_t iy){
		if(x==ix && y==iy){ return false; }
		x=ix, y=iy;
		/* TODO: Add offsetter */
		onPosChanged();
		return true;
	}
	bool OWidget::setSize(uint16_t iw, uint16_t ih){
		if(w==iw && h==ih){ return false; }
		w=iw,h=ih;
		/* TODO: Add offsetter */
		onSizeChanged();
		return true;
	}
	void OWidget::setMinSize(uint16_t iminW, uint16_t iminH){
		minW=iminW,minH=iminH;
		if( (w<minW) || (h<minH) ){ setSize(iminW,iminH); }
	}
	bool OWidget::setScale(float s){
		if(scale==s){ return false; }
		scale=OClampMinD(s,0.1);
		/* TODO: Add offsetter */
		onPosChanged();
		onSizeChanged();
		return true;
	}
	bool OWidget::setGeometry(int16_t ix, int16_t iy, uint16_t iw, uint16_t ih){
		bool posChanged=false;
		bool sizeChanged=false;
		if(x!=ix || y!=iy){ posChanged=true; }
		if(w!=iw || h!=ih){ sizeChanged=true; }
		x=ix,y=iy;
		/* TODO: Add offsetter */
		if(iw<minW){ w=minW; }else{ w=iw; }
		if(ih<minH){ h=minH; }else{ h=ih; }
		if(posChanged)  { onPosChanged(); }
		if(sizeChanged) { onSizeChanged(); }
		return ( posChanged || sizeChanged );
	}
	bool OWidget::setFocus(bool newFocus){
		if(newFocus){
			if(OAPP_FOCUSEDELEMENT){
				if(OAPP_FOCUSEDELEMENT==this) { return false; }
				else  { OAPP_FOCUSEDELEMENT->setFocus(false); }
			}
			OAPP_FOCUSEDELEMENT=this;
		}else{
			if(OAPP_FOCUSEDELEMENT && OAPP_FOCUSEDELEMENT!=this){
				OAPP_FOCUSEDELEMENT->setFocus(false);
			}
			OAPP_FOCUSEDELEMENT=0;
		}
		if(flags.focused==newFocus){ return false; }

		flags.focused=newFocus;
		onFocusChanged();
		return true;
	}

	/** Theme **/
	void OWidget::setTheme(OTheme* newTheme){
		if(!newTheme){ return; }
		if(newTheme==OAPP_INTERFACE.theme){ resetTheme(); return; }
		theme.internal  = *newTheme;
		theme.primary   = &(theme.internal.primary);
		theme.secondary = &(theme.internal.secondary);
		theme.tertiary  = &(theme.internal.tertiary);
		theme.accent    = &(theme.internal.accent);
		redraw();
	}

	void OWidget::setThemePrimaryCol(uint8_t r, uint8_t g, uint8_t b){
		theme.internal.setPrimary(r,g,b);
		theme.primary=&(theme.internal.primary);
		redraw();
	}
	void OWidget::setThemePrimaryCol(OCol* col){
		if      (col==&OAPP_THEME.primary)   { theme.primary = &OAPP_THEME.primary;   redraw(); return; }
		else if (col==&OAPP_THEME.secondary) { theme.primary = &OAPP_THEME.secondary; redraw(); return; }
		else if (col==&OAPP_THEME.tertiary)  { theme.primary = &OAPP_THEME.tertiary;  redraw(); return; }
		else if (col==&OAPP_THEME.accent)    { theme.primary = &OAPP_THEME.accent;    redraw(); return; }
		else{
			setThemePrimaryCol(col->raw.r,col->raw.g,col->raw.b);
			redraw();
		}
	}

	void OWidget::setThemeSecondaryCol(uint8_t r, uint8_t g, uint8_t b){
		theme.internal.setSecondary(r,g,b);
		theme.secondary=&(theme.internal.secondary);
		redraw();
	}
	void OWidget::setThemeSecondaryCol(OCol* col){
		if      (col==&OAPP_THEME.primary)   { theme.secondary = &OAPP_THEME.primary;   redraw(); return; }
		else if (col==&OAPP_THEME.secondary) { theme.secondary = &OAPP_THEME.secondary; redraw(); return; }
		else if (col==&OAPP_THEME.tertiary)  { theme.secondary = &OAPP_THEME.tertiary;  redraw(); return; }
		else if (col==&OAPP_THEME.accent)    { theme.secondary = &OAPP_THEME.accent;    redraw(); return; }
		else{
			setThemeSecondaryCol(col->raw.r,col->raw.g,col->raw.b);
			redraw();
		}
	}

	void OWidget::setThemeTertiaryCol(uint8_t r, uint8_t g, uint8_t b){
		theme.internal.setTertiary(r,g,b);
		theme.tertiary=&(theme.internal.tertiary);
		redraw();
	}
	void OWidget::setThemeTertiaryCol(OCol* col){
		if      (col==&OAPP_THEME.primary)   { theme.tertiary = &OAPP_THEME.primary;   redraw(); return; }
		else if (col==&OAPP_THEME.secondary) { theme.tertiary = &OAPP_THEME.secondary; redraw(); return; }
		else if (col==&OAPP_THEME.tertiary)  { theme.tertiary = &OAPP_THEME.tertiary;  redraw(); return; }
		else if (col==&OAPP_THEME.accent)    { theme.tertiary = &OAPP_THEME.accent;    redraw(); return; }
		else{
			setThemeTertiaryCol(col->raw.r,col->raw.g,col->raw.b);
			redraw();
		}
	}

	void OWidget::setThemeAccentCol(uint8_t r, uint8_t g, uint8_t b){
		theme.internal.setAccent(r,g,b);
		theme.accent=&(theme.internal.accent);
		redraw();
	}
	void OWidget::setThemeAccentCol(OCol* col){
		if      (col==&OAPP_THEME.primary)   { theme.accent = &OAPP_THEME.primary;   redraw(); return; }
		else if (col==&OAPP_THEME.secondary) { theme.accent = &OAPP_THEME.secondary; redraw(); return; }
		else if (col==&OAPP_THEME.tertiary)  { theme.accent = &OAPP_THEME.tertiary;  redraw(); return; }
		else if (col==&OAPP_THEME.accent)    { theme.accent = &OAPP_THEME.accent;    redraw(); return; }
		else{
			setThemeAccentCol(col->raw.r,col->raw.g,col->raw.b);
			redraw();
		}
	}
	
	void OWidget::resetTheme(void){
		theme.internal   = OAPP_THEME;
		theme.primary    = &OAPP_THEME.primary;
		theme.secondary  = &OAPP_THEME.secondary;
		theme.tertiary   = &OAPP_THEME.tertiary;
		theme.accent     = &OAPP_THEME.accent;
		redraw();
	}

/*** Getters ***/
/*** Misc ops ***/
	void OWidget::redraw(bool full)  { if(drawPtr){ flags.fullRedraw=full; drawPtr(this); } }

	const char* OWidget::getTypeAsString(void) const{
		switch(type){
			case OUI_ERROR:    { return "OUI_ERROR"; }
			case OUI_CUSTOM:   { return "OUI_CUSTOM"; }
		}
		return 0;
	}
}