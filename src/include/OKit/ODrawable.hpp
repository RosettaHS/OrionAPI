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

#ifndef __ORIONAPI_OKIT_ODRAWABLE_H__
#define __ORIONAPI_OKIT_ODRAWABLE_H__

#include "common.hpp"
#include "CContext.hpp"
#include "OCol.hpp"

namespace Orion{
	/* An enumeration of OrionUI Elements. */
	enum OUIType : char{
		OUI_ERROR
	};

	/* The base element for all OrionUI Elements. */
	class ODrawable{
		protected:
			OUIType    type;
			int16_t    x,y;
			uint16_t   w,h;
			uint16_t   minW,minH;
			float      scale;
			int16_t    index;
			CContext*  context;
			ODrawable* parent;
			void     (*drawPtr)(ODrawable*);
			struct{
				bool    ready      : 1;
				bool    valid      : 1;
				bool    enabled    : 1;
				bool    focused    : 1;
				bool    fullRedraw : 1;
				uint8_t containerFlags;
			}flags;
			struct{
				OTheme overridden;
				OCol*  primary;
				OCol*  secondary;
				OCol*  tertiary;
				OCol*  accent;
			}theme;
		public:
			ODrawable(void);

			inline void draw(void) { if(drawPtr){ drawPtr(this); } }

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
		typedef ODrawable drawable_t;
	#endif /* !ORION_NOALTNAMES */
}

#endif /* !__ORIONAPI_OKIT_ODRAWABLE_H__ */