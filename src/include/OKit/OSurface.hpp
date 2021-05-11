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

#ifdef ORION_INTERNAL
	/* Converts an OSurface* to a CContext* */
	#define STC(surf) ((CContext*)surf)
	#define CTS(cont) ((OSurface*)cont)
#endif /* ORION_INTERNAL */

namespace Orion{
	typedef uint32_t OSurfaceMask;

	extern void OFlushSurfaces(void);

	class OSurface{
		protected:
			CContext  raw;
			OSurface* parent;
			OVec4     geo;
		public:
			virtual ~OSurface(void);
			OSurface(void);

			bool  create(OSurface* parent, int16_t x, int16_t y, uint16_t w, uint16_t h, OCol* col, OSurfaceMask mask=0, bool autoFlush=true);
			bool  destroy(bool autoFlush=true);

			bool         setPos(int16_t x, int16_t y, bool autoFlush=true);
			inline bool  setPos(OVec v, bool autoFlush=true)        { return setPos(v.x,v.y,autoFlush); }
			bool         setSize(uint16_t w, uint16_t h, bool autoFlush=true);
			inline bool  setSize(OVec4 v, bool autoFlush=true)      { return setSize(v.w,v.h,autoFlush); }
			bool         setGeometry(int16_t x, int16_t y, uint16_t w, uint16_t h, bool autoFlush=true);
			inline bool  setGeometry(OVec4 v, bool autoFlush=true)  { return setGeometry(v.x,v.y,v.w,v.h,autoFlush); }
			bool         setCol(OCol* col, bool autoFlush=true);
			inline bool  setCol(OCol& col, bool autoFlush=true)     { return setCol(&col,autoFlush); }
			bool         setCol(uint8_t r, uint8_t g, uint8_t b, bool autoFlush=true);

			inline OVec  getPos(void)      const { return { geo.x, geo.y }; }
			inline OVec  getSize(void)     const { return { geo.w, geo.h }; }
			inline OVec4 getGeometry(void) const { return geo;}
			OCol         getCol(void)      const;
	};

	#ifndef ORION_NOALTNAMES
		typedef uint32_t   surfacemask_t;
		typedef OSurface   surface_t;
	#endif /* !ORION_NOALTNAMES */
}

#endif /* !__ORIONAPI_OKIT_OSURFACE_H__ */
