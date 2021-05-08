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

#ifndef __ORIONAPI_OKIT_OWIDGET_H__
#define __ORIONAPI_OKIT_OWIDGET_H__

#include "common.hpp"
#include "CContext.hpp"
#include "OCol.hpp"

/*** Macros ***/

/* Internal. The base for flag definitions. Random magic number (that won't ever be accidentally used) that is commonly below SHRT_MAX. */
#define _OUI_FLAGDEFBASE 25678

/** Position Flag Definitions **/

/* Positional Only. Aligns to the start of the given axis. */
#define START  (_OUI_FLAGDEFBASE)
/* Positional Only. Aligns to the end of the given axis. */
#define END    (_OUI_FLAGDEFBASE-1)
/* Positional Only. Aligns to the center of the given axis. */
#define CENTER (_OUI_FLAGDEFBASE-2)
/* Positional Only. Aligns to the centre of the given axis. */
#define CENTRE (_OUI_FLAGDEFBASE-2)

/** Size Flag Definitions **/

/* Sizing Only. Fills to the end of the given axis. */
#define FILL   (_OUI_FLAGDEFBASE-3)

/** Utilities **/

/* If this is returned from getIndex(), it means the Widget has not been linked to any Containers. */
#define OWIDGET_NOTLINKED (-1)

#ifdef ORION_INTERNAL

/* Internal Flags */
	#define _OUI_X_START  0x1
	#define _OUI_X_END    0x2
	#define _OUI_X_CENTRE 0x4

	#define _OUI_Y_START  0x8
	#define _OUI_Y_END    0x10
	#define _OUI_Y_CENTRE 0x20

	#define _OUI_W_FILL   0x40
	#define _OUI_H_FILL   0x80

#endif /* ORION_INTERNAL */

/*** Declarations ***/

namespace Orion{
	/* An enumeration of OrionUI Elements. */
	enum OUIType : char{
		OUI_ERROR,
		OUI_CUSTOM,
		OUI_CONTAINER,
		/* ... */
		OUI_WINDOW
	};

	/* The base Container that contains OrionUI Elements. */
	class OContainer;

	/* The base class for all OrionUI Elements. */
	class OWidget{
		protected:
			OUIType     type;
			int16_t     x,y;
			uint16_t    w,h;
			uint16_t    minW,minH;
			float       scale;
			int16_t     index;
			CContext*   parentContext;
			OContainer* parentContainer;
			OContainer* parentWidget;
			void      (*drawPtr)(OWidget*);
			struct{
				bool    inited     : 1;
				bool    linked     : 1;
				bool    enabled    : 1;
				bool    focused    : 1;
				bool    canFocus   : 1;
				bool    fullRedraw : 1;
				char    containerFlags;
			}flags;
			struct{
				OTheme  internal;
				OCol*   primary;
				OCol*   secondary;
				OCol*   tertiary;
				OCol*   accent;
			}theme;

			virtual void       onLink(void);
			virtual void       onUnlink(void);
			virtual void       onPosChanged(void);
			virtual void       onSizeChanged(void);
			virtual void       onFocusChanged(void);
			bool               initFlag(int16_t xFlag, int16_t yFlag, uint16_t wFlag, uint16_t hFlag);
			void               init(int16_t x, int16_t y, uint16_t w, uint16_t h);

			friend class       OContainer;
		public:
			OWidget(void);

			void               redraw(bool full=false);
			inline bool        isReady(void)      const      { return ( flags.inited && flags.linked && parentContext ); }
			inline bool        isInited(void)     const      { return ( flags.inited); }
			inline bool        isFocusable(void)  const      { return flags.focused; }

			bool               linkTo(OContainer* container);
			inline bool        linkTo(OContainer& container) { return linkTo(&container); }
			bool               unlinkThis(void);

			bool               setPos(int16_t x, int16_t y);
			inline bool        setPos(OVec& v)       { return setPos(v.x,v.y); }
			inline bool        setPos(OVec* v)       { return setPos(v->x,v->y); }
			bool               setSize(uint16_t w, uint16_t h);
			inline bool        setSize(OVec& v)      { return setSize(v.x,v.y); }
			inline bool        setSize(OVec* v)      { return setSize(v->x,v->y); }
			void               setMinSize(uint16_t minW, uint16_t minH);
			inline void        setMinSize(OVec& v)   { setMinSize(v.x,v.y); }
			inline void        setMinSize(OVec* v)   { setMinSize(v->x,v->y); }
			virtual bool       setScale(float s);
			bool               setGeometry(int16_t x, int16_t y, uint16_t w, uint16_t h);
			inline bool        setGeometry(OVec4& v) { return setGeometry(v.x,v.y,v.w,v.h); }
			inline bool        setGeometry(OVec4* v) { return setGeometry(v->x,v->y,v->w,v->h); }
			bool               setFocus(bool newFocus);

			virtual void       setCol(OCol* col);
			inline void        setCol(OCol& col)               { setCol(&col); }
			void               setTheme(OTheme*);
			inline void        setTheme(OTheme& theme)         { setTheme(&theme); }
			void               setThemePrimaryCol(uint8_t r, uint8_t g, uint8_t b);
			void               setThemePrimaryCol(OCol* col);
			inline void        setThemePrimaryCol(OCol& col)   { setThemePrimaryCol(&col); }
			void               setThemeSecondaryCol(uint8_t r, uint8_t g, uint8_t b);
			void               setThemeSecondaryCol(OCol* col);
			inline void        setThemeSecondaryCol(OCol& col) { setThemeSecondaryCol(&col); }
			void               setThemeTertiaryCol(uint8_t r, uint8_t g, uint8_t b);
			void               setThemeTertiaryCol(OCol* col);
			inline void        setThemeTertiaryCol(OCol& col)  { setThemeTertiaryCol(&col); }
			void               setThemeAccentCol(uint8_t r, uint8_t g, uint8_t b);
			void               setThemeAccentCol(OCol* col);
			inline void        setThemeAccentCol(OCol& col)    { setThemeAccentCol(&col); }
			void               resetTheme(void);

			inline uint8_t     getContainerFlags(void) const { return flags.containerFlags; }
			OVec               getPos(bool globalToWindow=false) const;
			OVec               getSize(bool useScale=false) const;
			OVec               getMinSize(bool useScale=false) const;
			inline float       getScale(void)  const         { return scale; }
			OVec4              getGeometry(bool posGlobalToWindow=false, bool sizeUseScale=false) const;
			inline bool        getFocus(void)  const         { return flags.focused; }
			inline OTheme      getTheme(void)  const         { return theme.internal; }
			int16_t            getIndex(void)  const;
			inline OContainer* getParent(void) const         { return parentContainer; }

			/* Returns the type of this Element. */
			inline OUIType     getType(void) const { return type; }
			/**
			 * @brief Returns the type of this Element as a String.
			 * @return A String version of the given OUIType value corrisponding to this Element.
			 * For example: if the Element's Type is OUI_WINDOW, this will return "OUI_WINDOW" as a String. Good for easily readable debug logs.
			 */
			const char*        getTypeAsString(void) const;
	};

	#ifndef ORION_NOALTNAMES
		/* An enumeration of OrionUI Elements. */
		typedef OUIType   uitype_t;
		/* The base element for all OrionUI Elements. */
		typedef OWidget   widget_t;
	#endif /* !ORION_NOALTNAMES */
}

#endif /* !__ORIONAPI_OKIT_OWIDGET_H__ */
