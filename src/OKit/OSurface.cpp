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

#define FLUSHIF(x) if(x) OFlushSurfaces()

namespace Orion{
	/*** Constructors/Destructors ***/
	OSurface::~OSurface(void){
		raw.destroy();
		OFlushSurfaces();
	}
	OSurface::OSurface(void){ parent=0; geo={0,0,0,0}; }
	
	bool OSurface::create(OSurface* p, int16_t ix, int16_t iy, uint16_t iw, uint16_t ih, OCol* icol, OSurfaceMask imask, OWidget* listener, bool autoFlush){
		if(!raw.XWIN && !p){
		/* Error Checking */
			if( ((bool)imask) && !((bool)listener) ){
				OWARN(false,
					"CANNOT SET EVENT MASK WITH UNSET (NULL) WIDGET LISTENER WHEN CREATING OSURFACE!"
				);
				return false;
			}else if( ((bool)listener) && !((bool)imask) ){
				OWARN(false,
					"CANNOT SET WIDGET LISTENER WITH UNSET (NULL) EVENT MASK WHEN CREATING OSURFACE!"
				);
				return false;
			}
		/* Initialisation */
			parent=p;
			geo={ix,iy,iw,ih};
			if( raw.create(STC(p),ix,iy,iw,ih,0,icol,imask,CCT_ELEMENT) ){
				setListener(listener);
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
				parent=0; geo={0,0,0,0};
				FLUSHIF(autoFlush);
				return true;
			}
		}
		return false;
	}

	/*** Setters ***/
	bool OSurface::setPos(int16_t ix, int16_t iy, bool autoFlush){
		if(raw.XWIN){
			geo.x=ix,geo.y=iy;
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