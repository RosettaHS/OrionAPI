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

namespace Orion{
	/* An enumeration of OrionUI Elements. */
	enum OUIType : char{
		OUI_ERROR,
		OUI_CUSTOM,
	};

	/* The base element for all OrionUI Elements. */
	class OWidget{
		protected:
			OUIType    type;
			int16_t    x,y;
			uint16_t   w,h;
			uint16_t   minW,minH;
			float      scale;
			int16_t    index;
			CContext*  context;
			OWidget*   parent;
			void     (*drawPtr)(OWidget*);
			struct{
				bool   valid      : 1;
				bool   linked     : 1;
				bool   enabled    : 1;
				bool   focused    : 1;
				bool   fullRedraw : 1;
				char   containerFlags;
			}flags;
			struct{
				OTheme internal;
				OCol*  primary;
				OCol*  secondary;
				OCol*  tertiary;
				OCol*  accent;
			}theme;

			virtual void onLink(void);
			virtual void onUnlink(void);
			virtual void onPosChanged(void);
			virtual void onSizeChanged(void);
			virtual void onFocusChanged(void);
		public:
			OWidget(void);

			void redraw(bool full=false);

			bool        setPos(int16_t x, int16_t y);
			inline bool setPos(OVec& v)       { return setPos(v.x,v.y); }
			inline bool setPos(OVec* v)       { return setPos(v->x,v->y); }
			bool        setSize(uint16_t w, uint16_t h);
			inline bool setSize(OVec& v)      { return setSize(v.x,v.y); }
			inline bool setSize(OVec* v)      { return setSize(v->x,v->y); }
			void        setMinSize(uint16_t minW, uint16_t minH);
			inline void setMinSize(OVec& v)   { setMinSize(v.x,v.y); }
			inline void setMinSize(OVec* v)   { setMinSize(v->x,v->y); }
			bool        setScale(float s);
			bool        setGeometry(int16_t x, int16_t y, uint16_t w, uint16_t h);
			inline bool setGeometry(OVec4& v) { return setGeometry(v.x,v.y,v.w,v.h); }
			inline bool setGeometry(OVec4* v) { return setGeometry(v->x,v->y,v->w,v->h); }
			bool        setFocus(bool newFocus);

			void        setTheme(OTheme*);
			inline void setTheme(OTheme& theme)         { setTheme(&theme); }
			void        setThemePrimaryCol(uint8_t r, uint8_t g, uint8_t b);
			void        setThemePrimaryCol(OCol* col);
			inline void setThemePrimaryCol(OCol& col)   { setThemePrimaryCol(&col); }
			void        setThemeSecondaryCol(uint8_t r, uint8_t g, uint8_t b);
			void        setThemeSecondaryCol(OCol* col);
			inline void setThemeSecondaryCol(OCol& col) { setThemeSecondaryCol(&col); }
			void        setThemeTertiaryCol(uint8_t r, uint8_t g, uint8_t b);
			void        setThemeTertiaryCol(OCol* col);
			inline void setThemeTertiaryCol(OCol& col)  { setThemeTertiaryCol(&col); }
			void        setThemeAccentCol(uint8_t r, uint8_t g, uint8_t b);
			void        setThemeAccentCol(OCol* col);
			inline void setThemeAccentCol(OCol& col)    { setThemeAccentCol(&col); }
			void        resetTheme(void);

			

			/* Returns the type of this Element. */
			inline OUIType getType(void) const { return type; }
			/**
			 * @brief Returns the type of this Element as a String.
			 * @return A String version of the given OUIType value corrisponding to this Element.
			 * For example: if the Element's Type is OUI_OWINDOW, this will return "OUI_OWINDOW" as a String. Good for easily readable debug logs.
			 */
			const char* getTypeAsString(void) const;
	};

	#ifndef ORION_NOALTNAMES
		/* An enumeration of OrionUI Elements. */
		typedef OUIType   uitype_t;
		/* The base element for all OrionUI Elements. */
		typedef OWidget   widget_t;
	#endif /* !ORION_NOALTNAMES */
}

#endif /* !__ORIONAPI_OKIT_OWIDGET_H__ */