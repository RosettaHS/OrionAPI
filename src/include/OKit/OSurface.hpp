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

#ifndef __ORIONAPI_OKIT_OSURFACE_H__
#define __ORIONAPI_OKIT_OSURFACE_H__

#include "CContext.hpp"

namespace Orion{
	typedef uint32_t OSurfaceMask;

	class OSurfaceRect{
		protected:
			OVec4 geo;
			OCol  col;
		public:
			inline      OSurfaceRect(int16_t x, int16_t y, uint16_t w, uint16_t h, OCol* icol) : geo{x,y,w,h} { if(icol){ col=*icol; } }
			inline      OSurfaceRect(int16_t x, int16_t y, uint16_t w, uint16_t h, OCol icol)  : geo{x,y,w,h} { col=icol; }
			inline      OSurfaceRect(int16_t x, int16_t y, uint16_t w, uint16_t h, uint8_t r, uint8_t g, uint8_t b)  : geo{x,y,w,h}, col(r,g,b) {}

			inline      OSurfaceRect(OVec4 igeo, OCol* icol) : geo{igeo} { if(icol){ col=*icol; } }
			inline      OSurfaceRect(OVec4 igeo, OCol icol)  : geo{igeo} { col=icol; }
			inline      OSurfaceRect(OVec4 igeo, uint8_t r, uint8_t g, uint8_t b)  : geo{igeo}, col(r,g,b) {}

			inline void setTo(int16_t x, int16_t y, uint16_t w, uint16_t h, OCol* icol)                       { geo={x,y,w,h}; if(icol){ col=*icol; } }
			inline void setTo(int16_t x, int16_t y, uint16_t w, uint16_t h, OCol icol)                        { geo={x,y,w,h}; col=icol; }
			inline void setTo(int16_t x, int16_t y, uint16_t w, uint16_t h, uint8_t r, uint8_t g, uint8_t b)  { geo={x,y,w,h}; col.setTo(r,g,b); }

			inline void setTo(OVec4 igeo, OCol* icol)                       { geo=igeo; if(icol){ col=*icol; } }
			inline void setTo(OVec4 igeo, OCol icol)                        { geo=igeo; col=icol; }
			inline void setTo(OVec4 igeo, uint8_t r, uint8_t g, uint8_t b)  { geo=igeo; col.setTo(r,g,b); }

			inline OVec  getPos(void)      const { return { geo.x, geo.y }; }
			inline OVec  getSize(void)     const { return { geo.w, geo.h }; }
			inline OVec4 getGeometry(void) const { return geo; }
			inline OCol  getCol(void)      const { return col; }
	};

	class OSurface{
		protected:
			CContext  raw;
			OSurface* parent;
			OVec4     geo;
			OWidget*  widget;

			void scaleXYByWidget(int16_t& x, int16_t& y);
			void scaleWHByWidget(uint16_t& w, uint16_t& h);
		public:
			virtual ~OSurface(void);
			         OSurface(void);
			inline   OSurface(OSurface* parent, int16_t x, int16_t y, uint16_t w, uint16_t h, OCol* col, OSurfaceMask mask=0, OWidget* listener=0, bool autoFlush=true)
			                  { create(parent,x,y,w,h,col,mask,listener,autoFlush); }
			inline   OSurface(OSurface* parent, OVec4 v, OCol* col, OSurfaceMask mask=0, OWidget* listener=0, bool autoFlush=true)
			                  { create(parent,v.x,v.y,v.w,v.h,col,mask,listener,autoFlush); }

			void        registerTo(OWidget* widget);
			void        unregister(void);
			bool        create(OSurface* parent, int16_t x, int16_t y, uint16_t w, uint16_t h, OCol* col, OSurfaceMask mask=0, OWidget* listener=0, bool autoFlush=true);
			inline bool create(OSurface* parent, OVec4 v, OCol* col, OSurfaceMask mask=0, OWidget* listener=0, bool autoFlush=true)
			                   {  return create(parent,v.x,v.y,v.w,v.h,col,mask,listener,autoFlush); }
			bool        destroy(bool autoFlush=true);

			bool        clearRects(bool autoFlush=true);
			bool        drawRect(OSurfaceRect area, bool autoFlush=true);
			bool        drawRect(int16_t x, int16_t y, uint16_t w, uint16_t h, OCol* col, bool autoFlush=true);
			inline bool drawRect(int16_t x, int16_t y, uint16_t w, uint16_t h, OCol col, bool autoFlush=true) { return drawRect(x,y,w,h,&col,autoFlush); }
			inline bool drawRect(OVec4 v, OCol* col, bool autoFlush=true) { return drawRect(v.x,v.y,v.w,v.h,col,autoFlush); }
			inline bool drawRect(OVec4 v, OCol col, bool autoFlush=true)  { return drawRect(v.x,v.y,v.w,v.h,&col,autoFlush); }
			bool        drawRects(OSurfaceRect* rectList, uint16_t rectCount, bool autoFlush=true);

			inline void setListener(OWidget* listener)             { raw.XLISTENER=listener; }
			bool        setPos(int16_t x, int16_t y, bool autoFlush=true);
			inline bool setPos(OVec v, bool autoFlush=true)        { return setPos(v.x,v.y,autoFlush); }
			bool        setSize(uint16_t w, uint16_t h, bool autoFlush=true);
			inline bool setSize(OVec4 v, bool autoFlush=true)      { return setSize(v.w,v.h,autoFlush); }
			bool        setGeometry(int16_t x, int16_t y, uint16_t w, uint16_t h, bool autoFlush=true);
			inline bool setGeometry(OVec4 v, bool autoFlush=true)  { return setGeometry(v.x,v.y,v.w,v.h,autoFlush); }
			bool        setCol(OCol* col, bool autoFlush=true);
			inline bool setCol(OCol& col, bool autoFlush=true)     { return setCol(&col,autoFlush); }
			bool        setCol(uint8_t r, uint8_t g, uint8_t b, bool autoFlush=true);

			inline OWidget*  getListener(void) const { return raw.XLISTENER; }
			inline OVec      getPos(void)      const { return { geo.x, geo.y }; }
			inline OVec      getSize(void)     const { return { geo.w, geo.h }; }
			inline OVec4     getGeometry(void) const { return geo;}
			OCol             getCol(void)      const;
			inline OSurface* getParent(void)   const { return parent; }
			inline CContext* getAsContext(void)      { return &raw; }
	};

	enum OSEType : char{
		OSE_ERROR
	};

	struct OSurfaceEvent{
		OSurface* surface;
		OSEType   type;
	};

	extern void OFlushSurfaces(void);

	#ifndef ORION_NOALTNAMES
		typedef uint32_t          surfacemask_t;
		typedef OSurface          surface_t;
		typedef OSurfaceRect      surfrect_t;
		typedef OSurfaceEvent     surfevent_t;
		typedef OSEType           surfeventtype_t;
	#endif /* !ORION_NOALTNAMES */
}

#endif /* !__ORIONAPI_OKIT_OSURFACE_H__ */