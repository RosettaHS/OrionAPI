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
#include "../include/Application.hpp"
#include "../include/OSL/OMath.hpp"
#include "../include/OKit/OSurface.hpp"
#include "../include/OKit/OWidget.hpp"

#define FLUSHIF(x) if(x) OFlushSurfaces()

namespace Orion{
	/*** Constructors/Destructors ***/
	OSurface::~OSurface(void){
		destroy(true);
		unregister();
	}
	OSurface::OSurface(void) : raw(), geo{0,0,0,0}, widget{0} {}

	void OSurface::registerTo(OWidget* iwidget){
		widget=iwidget;
		/* TODO: Add more functionality to this. */
	}
	void OSurface::unregister(void){
		widget=0;
		/* TODO: Add more functionality to this. */
	}

	/** INTERNAL **/
	void OSurface::scaleXYByWidget(int16_t& x, int16_t& y, bool addOffset){
		if(widget){
			float s=widget->scale;
			if(addOffset && &widget->canvas==this){
				x = ( x-( (widget->w/2)*(s-1) ) ) /s;
				y = ( y-( (widget->h/2)*(s-1) ) ) /s;
			}
			x*=s;
			y*=s;
		}
	}
	void OSurface::scaleWHByWidget(uint16_t& w, uint16_t& h){
		if(widget){
			w*=widget->scale;
			h*=widget->scale;
		}
	}
	
	bool OSurface::create(OSurface* p, int16_t ix, int16_t iy, uint16_t iw, uint16_t ih, OCol* icol, OSurfaceMask imask, bool autoFlush){
		if(!raw.XWIN && p){
		/* Error Checking */
			if( ((bool)imask) && !((bool)widget) ){
				OWARN(false,
					"CANNOT SET EVENT MASK WITH UNREGISTERED (NULL) PARENT WIDGET WHEN CREATING OSURFACE!"
				);
				return false;
			}
		/* Initialisation */
			geo={ix,iy,iw,ih};
			if( raw.create(p->getAsContext(),ix,iy,iw,ih,0,icol,imask,CCT_ELEMENT) ){
				raw.XFLAGS.XABSTRACTED=true;
				raw.map(imask);
				FLUSHIF(autoFlush);
				return true;
			}
		}
		return false;
	}

	bool OSurface::destroy(bool autoFlush){
		if(raw.XWIN){
			if( raw.destroy() ){
				geo={0,0,0,0};
				FLUSHIF(autoFlush);
				return true;
			}
		}
		return false;
	}

	/*** Area Rendering ***/
	bool OSurface::clearRects(bool autoFlush){
		if(raw.XWIN){
			raw.clear(0,0,geo.w,geo.h);
			FLUSHIF(autoFlush);
			return true;
		}
		return false;
	}
	bool OSurface::drawRect(OSurfaceRect area, bool autoFlush){
		if(raw.XWIN){
			vec4_t v=area.getGeometry();
			col_t  c=area.getCol();
			float s=( (widget) ? widget->scale : 1);
			raw.drawArea(
				v.x*s,v.y*s,
				v.w*s,v.h*s,
				&c
			);
			FLUSHIF(autoFlush);
			return true;
		}
		return false;
	}

	bool OSurface::drawRect(int16_t x, int16_t y, uint16_t w, uint16_t h, OCol* col, bool autoFlush){
		if(raw.XWIN){
			col_t tmp;
			float s=( (widget) ? widget->scale : 1);
			raw.drawArea(
				x*s,y*s,
				w*s,h*s,
				( (col) ? col : &tmp )
			);
			FLUSHIF(autoFlush);
			return true;
		}
		return false;
	}

	bool OSurface::drawRect(int16_t x, int16_t y, uint16_t w, uint16_t h, uint8_t r, uint8_t g, uint8_t b, bool autoFlush){
		if(raw.XWIN){
			col_t tmp(r,g,b);
			float s=( (widget) ? widget->scale : 1);
			raw.drawArea(
				x*s,y*s,
				w*s,h*s,
				&tmp
			);
			FLUSHIF(autoFlush);
			return true;
		}
		return false;
	}

	bool OSurface::drawRects(OSurfaceRect* areaList, uint16_t areaCount, bool autoFlush){
		if(raw.XWIN){
			for(uint16_t i=0;i<areaCount;i++){
				drawRect(areaList[i],false);
			}
			FLUSHIF(autoFlush);
			return true;
		}
		return false;
	}

	/*** Setters ***/
	bool OSurface::setPos(int16_t ix, int16_t iy, bool autoFlush){
		if(raw.XWIN){
			geo.x=ix,geo.y=iy;
			scaleXYByWidget(ix,iy);
			if( raw.setPos(ix,iy) ){
				FLUSHIF(autoFlush);
				return true;
			}
		}
		return false;
	}

	bool OSurface::setSize(uint16_t iw, uint16_t ih, bool autoFlush){
		if(raw.XWIN){
			geo.w=iw,geo.h=ih;
			scaleWHByWidget(iw,ih);
			if( raw.setSize(iw,ih) ){
				FLUSHIF(autoFlush);
				return true;
			}
		}
		return false;
	}
	bool OSurface::setGeometry(int16_t ix, int16_t iy, uint16_t iw, uint16_t ih, bool autoFlush){
		if(raw.XWIN){
			geo={ix,iy,iw,ih};
			scaleXYByWidget(ix,iy);
			scaleWHByWidget(iw,ih);
			if( raw.setGeometry(ix,iy,iw,ih) ){
				FLUSHIF(autoFlush);
				return true;
			}
		}
		return false;
	}
	bool OSurface::setCol(OCol* icol, bool autoFlush){
		if(raw.XWIN){
			if( raw.setCol(icol) ){
				raw.clear(0,0,geo.w,geo.h);
				FLUSHIF(autoFlush);
				return true;
			}
		}
		return false;
	}
	bool OSurface::setCol(uint8_t r, uint8_t g, uint8_t b, bool autoFlush){
		if(raw.XWIN){
			OCol tmp(r,g,b);
			if( raw.setCol(&tmp) ){
				FLUSHIF(autoFlush);
				return true;
			}
		}
		return false;
	}

	/*** Getters ***/
	OCol OSurface::getCol(void) const{
		OCol tmp;
		tmp.XCOL=raw.XCOL;
		return tmp;
	}

	/*** Misc ops ***/
	void OFlushSurfaces(void){ XCB_FLUSH(); }
}