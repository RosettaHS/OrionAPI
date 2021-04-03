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

#ifndef __ORIONAPI_OKIT_CCONTEXT_H__
#define __ORIONAPI_OKIT_CCONTEXT_H__

#include "common.hpp"
#include "core.hpp"
#include "OCol.hpp"
#include "OVec.hpp"
#include "CXEvent.hpp"

namespace Orion{
/*** Context Information ***/

/** Forward Declaration **/

	/* The base element for all OrionUI Elements. */
	class ODrawable;

	/* Internal. An enumeration of possible Context types. */
	enum CCType : char{
		CCT_ERROR,
		CCT_INPUTONLY,
		CCT_TOPLEVEL,
		CCT_DESKTOP,
		CCT_PANEL,
		CCT_TOOLTIP,
		CCT_MENU,
		CCT_ELEMENT
	};

	/* Internal. Higher-level abstraction of an X Window. */
	class CContext : public CLoggable{
		public:
			CCType   XTYPE;
			uint32_t XWIN;
			uint32_t XPARENT;
			uint32_t XCOL;
			uint32_t XMASK;
			char*    XTITLE;
			bool     XMAPPED;
			bool     XLINKED;
			struct{
				void* obj;
				void  (*func)(ODrawable* obj, CXEvent* event);
			}XLISTENER;

			~CContext(void);
			CContext(void);

			bool create(CContext* root, int16_t x, int16_t y, uint16_t w, uint16_t h, const char* t, OCol* col, uint32_t mask, CCType type);
			bool destroy(void);

			bool map(bool link);
			bool unmap(void);

			bool setTitle(const char* title);
			bool setCol(OCol* col);
			bool setPos(int16_t x, int16_t y);
			bool setSize(uint16_t w, uint16_t h);
			bool setGeometry(int16_t x, int16_t y, uint16_t w, uint16_t h);

			OVec4 getGeometry(void);

			virtual void log(bool verbose=false, bool newLine=true) override;

	};

/*** Context Handling ***/
#ifdef ORION_INTERNAL
	struct CXHANDLE{
		uint32_t  XWIN;
		CContext* context;
	};

	extern bool      CXHA_INIT(void);
	extern bool      CXHA_FREE(void);
	extern bool      CXHA_LINK(CContext* context);
	extern bool      CXHA_UNLINK(CContext* context);
	extern CContext* CXHA_FROMXID(uint32_t XWIN);

#endif /* ORION_INTERNAL */
}

#endif /* !__ORIONAPI_OKIT_CCONTEXT_H__ */