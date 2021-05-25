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

/*** Template section comments for deferred classes ***/
#define DEF_MINW    50
#define DEF_MINH    50
	/*** Constructors/Destructors ***/
	/*** Deferrables ***/
	/*** Setters ***/
	/*** Getters ***/
	/*** Misc ops ***/

namespace Orion{

/*** Initialisation/Constructors/Destructors ***/
	OWidget::OWidget(void) : /* FIXME: In case Widgets just stop working at some point, this is why. */
		type{OUI_ERROR},baseType{OUIB_WIDGET},
		x{0},y{0},w{0},h{0},
		minW{0},minH{0},scale{1},customID{0},canvas(),
		parentCanvas{0},parentContainer{0},parentWidget{0},
		flags{0,0,0,0,0,0,OWIDGET_SETCOL_USE_PRIMARY,0}
		{ theme.primary=0; theme.secondary=0; theme.tertiary=0; theme.accent=0; }

	OWidget::~OWidget(void){
		if(OAPP_FOCUSEDELEMENT==this){ setFocus(false); }
		type=OUI_ERROR;
		unlinkThis();
	}

	/* Technically part of the initialiser. */
	bool OWidget::initFlag(int16_t xFlag, int16_t yFlag, uint16_t wFlag, uint16_t hFlag){
		if( ((xFlag!=0)+(yFlag!=0)+(wFlag!=0)+(hFlag!=0))>1  ){ /* Could this be any uglier? */
			OERROR(OERR_WIDGET_INVALID_FLAG,true,"CAN ONLY SET ONE FLAG AT A TIME!");
			return false;
		}
		switch(xFlag){
			case START:  { flags.containerFlags|=_OUI_X_START;  return true; }
			case END:    { flags.containerFlags|=_OUI_X_END;    return true; }
			case CENTRE: { flags.containerFlags|=_OUI_X_CENTRE; return true; }
			case FILL:   { OERROR(OERR_WIDGET_INVALID_FLAG,true,"CAN'T SET X POSITION FLAG WITH \"FILL\" SIZE FLAG (SIZE [W/H] ONLY)!"); return false; }
		}
		switch(yFlag){
			case START:  { flags.containerFlags|=_OUI_Y_START;  return true; }
			case END:    { flags.containerFlags|=_OUI_Y_END;    return true; }
			case CENTRE: { flags.containerFlags|=_OUI_Y_CENTRE; return true; }
			case FILL:   { OERROR(OERR_WIDGET_INVALID_FLAG,true,"CAN'T SET Y POSITION FLAG WITH \"FILL\" SIZE FLAG (SIZE [W/H] ONLY)!"); return false; }
		}
		switch(wFlag){
			case START:  { OERROR(OERR_WIDGET_INVALID_FLAG,true,"CAN'T SET W SIZE FLAG WITH \"START\" POSITIONAL FLAG (POSITIONAL [X/Y] ONLY)!");  return false; }
			case END:    { OERROR(OERR_WIDGET_INVALID_FLAG,true,"CAN'T SET W SIZE FLAG WITH \"END\" POSITIONAL FLAG (POSITIONAL [X/Y] ONLY)!");    return false; }
			case CENTRE: { OERROR(OERR_WIDGET_INVALID_FLAG,true,"CAN'T SET W SIZE FLAG WITH \"CENTRE\" POSITIONAL FLAG (POSITIONAL [X/Y] ONLY)!"); return false; }
			case FILL:   { flags.containerFlags|=_OUI_W_FILL; return true; }
		}
		switch(hFlag){
			case START:  { OERROR(OERR_WIDGET_INVALID_FLAG,true,"CAN'T SET H SIZE FLAG WITH \"START\" POSITIONAL FLAG (POSITIONAL [X/Y] ONLY)!");  return false; }
			case END:    { OERROR(OERR_WIDGET_INVALID_FLAG,true,"CAN'T SET H SIZE FLAG WITH \"END\" POSITIONAL FLAG (POSITIONAL [X/Y] ONLY)!");    return false; }
			case CENTRE: { OERROR(OERR_WIDGET_INVALID_FLAG,true,"CAN'T SET H SIZE FLAG WITH \"CENTRE\" POSITIONAL FLAG (POSITIONAL [X/Y] ONLY)!"); return false; }
			case FILL:   { flags.containerFlags|=_OUI_H_FILL; return true; }
		}
		return false;
	}

	void OWidget::init(OUIType itype, int16_t ix, int16_t iy, uint16_t iw, uint16_t ih, uint16_t iminW, uint16_t iminH){
		type=itype;
		if( initFlag(ix,0,0,0) ){ x=0; }   else{ x=ix; }
		if( initFlag(0,iy,0,0) ){ y=0; }   else{ y=iy; }
		if( initFlag(0,0,iw,0) ){ w=minW; }else{ w=iw; }
		if( initFlag(0,0,0,ih) ){ h=minH; }else{ h=ih; }
	/* Extra error checking */
		if(flags.containerFlags&_OUI_W_FILL){
			if( flags.containerFlags & _OUI_X_CENTRE) { OERROR(OERR_WIDGET_INVALID_FLAG,true,"CAN'T COMBINE \"FILL\" SIZE FLAG WITH \"CENTRE\" POSITIONAL FLAG ON THE SAME AXIS! [X | W AXIS]"); }
			if( flags.containerFlags & _OUI_X_END)    { OERROR(OERR_WIDGET_INVALID_FLAG,true,"CAN'T COMBINE \"FILL\" SIZE FLAG WITH \"END\" POSITIONAL FLAG ON THE SAME AXIS! [X | W AXIS]"); }
		}
		if(flags.containerFlags&_OUI_H_FILL){
			if( flags.containerFlags & _OUI_Y_CENTRE) { OERROR(OERR_WIDGET_INVALID_FLAG,true,"CAN'T COMBINE \"FILL\" SIZE FLAG WITH \"CENTRE\" POSITIONAL FLAG ON THE SAME AXIS! [Y | H AXIS]"); }
			if( flags.containerFlags & _OUI_Y_END)    { OERROR(OERR_WIDGET_INVALID_FLAG,true,"CAN'T COMBINE \"FILL\" SIZE FLAG WITH \"END\" POSITIONAL FLAG ON THE SAME AXIS! [Y | H AXIS]"); }
		}
		if(iw<minW){ minW=iw; }else{ minW=iminW; }
		if(ih<minH){ minH=ih; }else{ minH=iminH; }
		resetTheme();
		/* TODO: Add offsetter */
	}

	OVec OWidget::getTruePos(void){
		int16_t xx = ( x-( (w/2)*(scale-1) ) ) /scale;
		int16_t yy = ( y-( (h/2)*(scale-1) ) ) /scale;
		return { (int16_t)(xx*scale), (int16_t)(yy*scale) };
	}

	OVec OWidget::getTrueSize(void){ return { uint16_t(w*scale),uint16_t(h*scale) }; }

	OVec4 OWidget::getTrueGeo(void){
		OVec  tmpXY=getTruePos();
		OVec  tmpWH=getTrueSize();
		return {
			(int16_t) tmpXY.x, (int16_t) tmpXY.y,
			(uint16_t)tmpWH.x, (uint16_t)tmpWH.y
		};
	}

/*** Deferrables ***/
	/* Base Widget does nothing when linked or modified. */
	void OWidget::onLink(void)            { return; }
	void OWidget::onUnlink(void)          { return; }
	void OWidget::onPosChanged(void)      { return; }
	void OWidget::onSizeChanged(void)     { return; }
	void OWidget::onFocusChanged(void)    { return; }
	void OWidget::onColChanged(void)      { return; }
	void OWidget::onDraw(bool)            { return; }
	void OWidget::onEvent(OSurfaceEvent*) { return; }


/*** Setters ***/
	bool OWidget::linkTo(OContainer* container){
		if(container){ return container->link(this); }
		return false;
	}

	bool OWidget::unlinkThis(void){
		if(parentContainer){ return parentContainer->unlink(this); }
		return false;
	}

	/** Position and Size **/
	bool OWidget::setPos(int16_t ix, int16_t iy){
		if(x==ix && y==iy){ return false; }
		x=ix, y=iy;
		onPosChanged();
		return true;
	}
	bool OWidget::setSize(uint16_t iw, uint16_t ih){
		if(w==iw && h==ih){ return false; }
		w=iw,h=ih;
		onSizeChanged();
		return true;
	}
	void OWidget::setMinSize(uint16_t iminW, uint16_t iminH){
		minW=iminW,minH=iminH;
		if( (w<minW) || (h<minH) ){ setSize(iminW,iminH); }
	}
	bool OWidget::setScale(float s){
		if(baseType==OUIB_CONTAINER){
			OWARN(true,"CONTAINER WIDGETS CANNOT BE SCALED!");
			return false;
		}
		if(scale==s){ return false; }
		scale=OClampMinD(s,0.1);
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
		if(iw<minW){ w=minW; }else{ w=iw; }
		if(ih<minH){ h=minH; }else{ h=ih; }
		if(posChanged)  { onPosChanged(); }
		if(sizeChanged) { onSizeChanged(); }
		return ( posChanged || sizeChanged );
	}

	bool OWidget::setContainerFlags(uint16_t xFlag, uint16_t yFlag, uint16_t wFlag, uint16_t hFlag){
		bool flagSet=false;

		if(xFlag){ if( initFlag(xFlag,0,0,0) ){ flagSet=true; } x=0; }
		if(yFlag){ if( initFlag(0,yFlag,0,0) ){ flagSet=true; } y=0; }
		if(wFlag){ if( initFlag(0,0,wFlag,0) ){ flagSet=true; } w=minW; }
		if(hFlag){ if( initFlag(0,0,0,hFlag) ){ flagSet=true; } h=minH; }

		if(flagSet && parentContainer) { parentContainer->sort(); }
		return flagSet;
	}

	
	bool OWidget::setFocus(bool newFocus){
		if(!flags.canFocus || flags.focused == newFocus){ return false; }
		if(newFocus){
			if(OAPP_FOCUSEDELEMENT){
				if(OAPP_FOCUSEDELEMENT==this) { return false; }
				else  { OAPP_FOCUSEDELEMENT->setFocus(false); }
			}
			OAPP_FOCUSEDELEMENT=this;
		}else{
			if(OAPP_FOCUSEDELEMENT==this){ OAPP_FOCUSEDELEMENT=0; }
			else{ return false; }
		}

		flags.focused=newFocus;
		onFocusChanged();
		return true;
	}

	/** Theme **/
	void OWidget::setCol(uint8_t r, uint8_t g, uint8_t b){
		OCol tmp(r,g,b);
		setCol(&tmp);
	}
	void OWidget::setCol(OCol* col){
		switch(flags.setColMode){
			case OWIDGET_SETCOL_USE_PRIMARY:   { setThemePrimaryCol(col);   return; }
			case OWIDGET_SETCOL_USE_SECONDARY: { setThemeSecondaryCol(col); return; }
			case OWIDGET_SETCOL_USE_TERTIARY:  { setThemeTertiaryCol(col);  return; }
			case OWIDGET_SETCOL_USE_ACCENT:    { setThemeAccentCol(col);    return; }
		}
		OWARN(false,"INVALID SETCOL MODE! NOT SETTING COLOUR ON WIDGET!");
		return;
	}

	void OWidget::setTheme(OTheme* newTheme){
		if(!newTheme){ return; }
		if(newTheme==OAPP_INTERFACE.theme){ resetTheme(); return; }
		theme.internal  = *newTheme;
		theme.primary   = &(theme.internal.primary);
		theme.secondary = &(theme.internal.secondary);
		theme.tertiary  = &(theme.internal.tertiary);
		theme.accent    = &(theme.internal.accent);
		onColChanged();
	}

	void OWidget::setThemePrimaryCol(uint8_t r, uint8_t g, uint8_t b){
		theme.internal.setPrimary(r,g,b);
		theme.primary=&(theme.internal.primary);
		onColChanged();
	}
	void OWidget::setThemePrimaryCol(OCol* col){
		if      (col==&OAPP_THEME.primary)   { theme.primary = &OAPP_THEME.primary;   onColChanged(); return; }
		else if (col==&OAPP_THEME.secondary) { theme.primary = &OAPP_THEME.secondary; onColChanged(); return; }
		else if (col==&OAPP_THEME.tertiary)  { theme.primary = &OAPP_THEME.tertiary;  onColChanged(); return; }
		else if (col==&OAPP_THEME.accent)    { theme.primary = &OAPP_THEME.accent;    onColChanged(); return; }
		else if (col){
			setThemePrimaryCol(col->raw.r,col->raw.g,col->raw.b);
		}
	}

	void OWidget::setThemeSecondaryCol(uint8_t r, uint8_t g, uint8_t b){
		theme.internal.setSecondary(r,g,b);
		theme.secondary=&(theme.internal.secondary);
		onColChanged();
	}
	void OWidget::setThemeSecondaryCol(OCol* col){
		if      (col==&OAPP_THEME.primary)   { theme.secondary = &OAPP_THEME.primary;   onColChanged(); return; }
		else if (col==&OAPP_THEME.secondary) { theme.secondary = &OAPP_THEME.secondary; onColChanged(); return; }
		else if (col==&OAPP_THEME.tertiary)  { theme.secondary = &OAPP_THEME.tertiary;  onColChanged(); return; }
		else if (col==&OAPP_THEME.accent)    { theme.secondary = &OAPP_THEME.accent;    onColChanged(); return; }
		else if (col){
			setThemeSecondaryCol(col->raw.r,col->raw.g,col->raw.b);
		}
	}

	void OWidget::setThemeTertiaryCol(uint8_t r, uint8_t g, uint8_t b){
		theme.internal.setTertiary(r,g,b);
		theme.tertiary=&(theme.internal.tertiary);
		onColChanged();
	}
	void OWidget::setThemeTertiaryCol(OCol* col){
		if      (col==&OAPP_THEME.primary)   { theme.tertiary = &OAPP_THEME.primary;   onColChanged(); return; }
		else if (col==&OAPP_THEME.secondary) { theme.tertiary = &OAPP_THEME.secondary; onColChanged(); return; }
		else if (col==&OAPP_THEME.tertiary)  { theme.tertiary = &OAPP_THEME.tertiary;  onColChanged(); return; }
		else if (col==&OAPP_THEME.accent)    { theme.tertiary = &OAPP_THEME.accent;    onColChanged(); return; }
		else if (col){
			setThemeTertiaryCol(col->raw.r,col->raw.g,col->raw.b);
		}
	}

	void OWidget::setThemeAccentCol(uint8_t r, uint8_t g, uint8_t b){
		theme.internal.setAccent(r,g,b);
		theme.accent=&(theme.internal.accent);
		onColChanged();
	}
	void OWidget::setThemeAccentCol(OCol* col){
		if      (col==&OAPP_THEME.primary)   { theme.accent = &OAPP_THEME.primary;   onColChanged(); return; }
		else if (col==&OAPP_THEME.secondary) { theme.accent = &OAPP_THEME.secondary; onColChanged(); return; }
		else if (col==&OAPP_THEME.tertiary)  { theme.accent = &OAPP_THEME.tertiary;  onColChanged(); return; }
		else if (col==&OAPP_THEME.accent)    { theme.accent = &OAPP_THEME.accent;    onColChanged(); return; }
		else if (col){
			setThemeAccentCol(col->raw.r,col->raw.g,col->raw.b);
		}
	}
	
	void OWidget::resetTheme(void){
		theme.internal   = OAPP_THEME;
		theme.primary    = &OAPP_THEME.primary;
		theme.secondary  = &OAPP_THEME.secondary;
		theme.tertiary   = &OAPP_THEME.tertiary;
		theme.accent     = &OAPP_THEME.accent;
		onColChanged();
	}

/*** Getters ***/

	OVec OWidget::getPos(bool globalToWindow) const{
		OVec v={0,0};
		if( globalToWindow && parentWidget ){         /* This is intentional */
			if(parentWidget->getType()!=OUI_WINDOW){ v=parentContainer->getPos(true); }
			v.x+=x,v.y+=y;
		}else{ v={x,y}; }
		return v;
	}

	/* FIXME: The following two methods WILL come back to bite me in the ass later.. I just know it.. */
	OVec OWidget::getSize(bool useScale) const{
		OVec v;
		if(useScale){
			float s=getScale();
			v.x=(w*s);
			v.y=(h*s);
		}else{ v={w,h}; }
		return v;
	}

	OVec OWidget::getMinSize(bool useScale) const{
		OVec v;
		if(useScale){
			float s=getScale();
			v.x=(minW*s);
			v.y=(minH*s);
		}else{ v={minW,minH}; }
		return v;
	}

	OVec4 OWidget::getGeometry(bool posGlobalToWindow, bool sizeUseScale) const{
		OVec4 v;
		OVec  tmp;
		tmp=getPos(posGlobalToWindow);
		v.x=tmp.x, v.y=tmp.y;
		tmp=getSize(sizeUseScale);
		v.w=tmp.x, v.h=tmp.y;
		return v;
	}

	OCol OWidget::getCol(void) const{
		OCol tmp;
		switch(flags.setColMode){
			case OWIDGET_SETCOL_USE_PRIMARY:   { tmp=*theme.primary;   break; }
			case OWIDGET_SETCOL_USE_SECONDARY: { tmp=*theme.secondary; break; }
			case OWIDGET_SETCOL_USE_TERTIARY:  { tmp=*theme.tertiary;  break; }
			case OWIDGET_SETCOL_USE_ACCENT:    { tmp=*theme.accent;    break; }
		}
		return tmp;
	}

	OTheme OWidget::getTheme(void) const{
		OTheme tmp;

		tmp.setPrimary  (*theme.primary);
		tmp.setSecondary(*theme.secondary);
		tmp.setTertiary (*theme.tertiary);
		tmp.setAccent   (*theme.accent);

		return tmp;
	}

	int16_t OWidget::getIndex(void) const{
		if(parentContainer){ return parentContainer->getIndexOf((const OWidget*)this); }
		else               { return OWIDGET_NOTLINKED; }
	}

/*** Misc ops ***/
	// void OWidget::redraw(bool full)  { if(drawPtr){ flags.fullRedraw=full; drawPtr(this); } }
	bool OWidget::redraw(bool full){
		if(isReady()){
			flags.fullRedraw=full;
			onDraw(full);
			flags.fullRedraw=false;
			return true;
		}
		return false;
	}

#define MATCHTOSTRING(s) case s: { return #s; }
	const char* OWidget::getTypeAsString(void) const{
		switch(type){
			MATCHTOSTRING(OUI_ERROR)
			MATCHTOSTRING(OUI_CUSTOM)
			MATCHTOSTRING(OUI_CONTAINER)
			MATCHTOSTRING(OUI_ROOTCONTAINER)
			MATCHTOSTRING(OUI_RECT)
			MATCHTOSTRING(OUI_WINDOW)
		}
		return 0;
	}
	const char* OWidget::getBTypeAsString(void) const{
		switch(baseType){
			MATCHTOSTRING(OUIB_ERROR)
			MATCHTOSTRING(OUIB_WIDGET)
			MATCHTOSTRING(OUIB_CONTAINER)
			MATCHTOSTRING(OUIB_BUTTON)
		}
		return 0;
	}

	void OWidget::log(bool verbose, bool newLine){
		if(verbose){
			OVec  v;
			OVec4 v4;
			auto boolType       = [](bool _BOOL)       { return ( (_BOOL) ? "true" : "false" ); };
			auto setColModeType = [](uint8_t _COLMODE) {
				switch(_COLMODE){
					MATCHTOSTRING(OWIDGET_SETCOL_USE_PRIMARY)
					MATCHTOSTRING(OWIDGET_SETCOL_USE_SECONDARY)
					MATCHTOSTRING(OWIDGET_SETCOL_USE_TERTIARY)
					MATCHTOSTRING(OWIDGET_SETCOL_USE_ACCENT)
				}
				return "(INVALID)";
			};
			OLog("Widget : %s | %s : %p {\n",getTypeAsString(),getBTypeAsString(),this);
			OLog("\tx,y                : (%d, %d)\n",x,y);
			OLog("\tw,h                : (%u, %u)\n",w,h);
			OLog("\tminW,minH          : (%u, %u)\n",minW,minH);
			OLog("\tscale              : (%.3f)\n",scale);
			OLog("\tcustomID           : (%u)\n",customID);
			OLog("\tcanvas             : (%p)\n",&canvas);
			OLog("\tparentCanvas       : (%p)\n",parentCanvas);
			OLog("\tparentContainer    : (%p - %s)\n",parentContainer,( (parentContainer) ? parentContainer->getTypeAsString() : "OUI_ERROR") );
			OLog("\tparentWidget       : (%p - %s)\n",parentWidget,( (parentWidget) ? parentWidget->getTypeAsString() : "OUI_ERROR") );
			OLog("\tflags:\n");
			OLog("\t    inited         : (%s)\n",boolType(flags.inited));
			OLog("\t    linked         : (%s)\n",boolType(flags.linked));
			OLog("\t    enabled        : (%s)\n",boolType(flags.enabled));
			OLog("\t    focused        : (%s)\n",boolType(flags.focused));
			OLog("\t    canFocus       : (%s)\n",boolType(flags.canFocus));
			OLog("\t    fullRedraw     : (%s)\n",boolType(flags.fullRedraw));
			OLog("\t    setColMode     : (%s)\n",setColModeType(flags.setColMode));
			OLog("\t    containerFlags : "); OLogBits(&flags.containerFlags,1,true);
			OLog("\ttheme:\n");
			OLog("\t     internal:\n");
			OLog("\t         primary   : "); theme.internal.primary.log();
			OLog("\t         secondary : "); theme.internal.secondary.log();
			OLog("\t         tertiary  : "); theme.internal.tertiary.log();
			OLog("\t         accent    : "); theme.internal.accent.log();
			OLog("\t     primary       : (%p)\n",theme.primary);
			OLog("\t     secondary     : (%p)\n",theme.secondary);
			OLog("\t     tertiary      : (%p)\n",theme.tertiary);
			OLog("\t     accent        : (%p)\n}\n",theme.accent);
		}else{
			OLog("(%s : %d, %d, %u, %u) ",getTypeAsString(),x,y,w,h);
			if(newLine){ OLog("\n"); }
		}
	}


/*** External ***/

	void CWidgetFireSignal(OWidget* widget, CWidgetSignal signal, void* data){
		switch(signal){
			case CWS_ONLINK:           { widget->onLink();                      return; }
			case CWS_ONUNLINK:         { widget->onUnlink();                    return; }
			case CWS_ONPOSCHANGED:     { widget->onPosChanged();                return; }
			case CWS_ONSIZECHANGED:    { widget->onSizeChanged();               return; }
			case CWS_ONFOCUSEDCHANGED: { widget->onFocusChanged();              return; }
			case CWS_ONCOLCHANGED:     { widget->onColChanged();                return; }
			case CWS_ONDRAW:           { widget->onDraw( *((bool*)data) );      return; }
			case CWS_ONEVENT:          { widget->onEvent((OSurfaceEvent*)data); return; }
		}
	}
}
