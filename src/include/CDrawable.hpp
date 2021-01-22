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

#ifndef __ORION_OKIT_CDRAWABLE_H__
#define __ORION_OKIT_CDRAWABLE_H__

#include <stdint.h>
#include "CLoggable.hpp"
#include "OVec.hpp"
#include "OCol.hpp"
#include "OTheme.hpp"
#include "CBaseUI.hpp"
#include "CContext.hpp"

#define _OUI_SIZE_FILL_FULL		0x1
#define _OUI_SIZE_FILL_LEFT		0x2
#define _OUI_SIZE_FILL_RIGHT	0x4
#define _OUI_SIZE_FILL_TOP		0x8
#define _OUI_SIZE_FILL_BOTTOM	0x10

/* Add positional flags here later! */

namespace Orion{
	class CContainer; /* Forward declaration. See CContainer.hpp for definition. */

	/* Base class for all drawable and containable UI elements. */
	class CDrawable : public CBaseUI , public CLoggable {
		protected:
			/* The X and Y positional coordinates of this Drawable. */
			int x,y;
			/* The positional coordinates for the centre of this Drawable. */
			int centreX, centreY;
			/* The Width and Height (size) of the Drawable. */
			unsigned int w,h;
			/* The minimum allowed width and height of this Drawable. Used in Containables when sorting. */
			unsigned int minW,minH;
			/* The UI scale of this Drawable. */
			float scale;
			/* Unused. The rotation of this Drawable. */
			float rotation;
			/* The child index of this Drawable if it is linked to Container. Is -1 if not linked. */
			int index;

			/* The Context that this Drawable can draw to. */
			CContext* context;
			/* The parent Drawable of this Drawable (if it has one). */
			CDrawable* parentDrawable;
			/* The parent Container of this Drawable (if it has one). */
			CContainer* parentContainer;

			/* Internal. The internal theme of this drawable. Not used normally, only during overrides. */
			OTheme internalTheme;
			/* Internal. Pointers to the actual colour values used by this Drawable. */
			CTheme theme;
			/* Internal. Override values for the theme of this Drawable. */
			uint8_t themeFlags;

			/* Allows Containers to access internal members of this Drawable. */
			friend class CContainer;
		public:
			/* Internal. Pointer to the draw function that the deferred Drawable will use. Takes in the Drawable as an argument. */
			void(*drawPtr)(CDrawable*);

			/* Internal. Raw flags that govern the size and positioning that non-sorting Containers will use to modify this Drawable. */
			struct{
				uint8_t size;
				uint8_t pos;
			}flags;

			/* Empty constructor. Sets all values to 0. */
			CDrawable(void);

			/* Links this Drawable to the passed Container. Returns false if could not link. */
			bool linkTo(CContainer&);
			/* Unlinks this Drawable from the passed Container. Returns false if could not unlink. */
			bool unlinkTo(CContainer&);

			/* Sets the position of this Drawable relative to its parent (if it has one). */
			void setPos(int x, int y);
			/* Sets the positional coordinates of the centre of this Drawable relative to the top left. Used during scaling. */
			void setCentre(int x, int y);
			/* Sets the size of this Drawable. */
			void setSize(unsigned int w,unsigned int h);
			/* Sets the minimum allowed size of this Drawable. */
			void setMinSize(unsigned int w, unsigned int h);
			/* Sets the UI scale of this Drawable. */
			void setScale(float);
			/* Unused. Sets the relative rotation of this Drawable. */
			void setRotation(float);
			/* Sets and overrides the theme of this Drawable. Use with caution! */
			void setTheme(OTheme&);
			/* Sets and overrides the primary colour of this Drawable. Use with caution!*/
			void setPrimaryCol(unsigned char r, unsigned char g, unsigned char b); void setPrimaryCol(OCol&);
			/* Sets and overrides the secondary colour of this Drawable. Use with caution!*/
			void setSecondaryCol(unsigned char r, unsigned char g, unsigned char b); void setSecondaryCol(OCol&);
			/* Sets and overrides the tertiary colour of this Drawable. Use with caution!*/
			void setTertiaryCol(unsigned char r, unsigned char g, unsigned char b); void setTertiaryCol(OCol&);
			/* Sets and overrides the accent colour of this Drawable. Use with caution!*/
			void setAccentCol(unsigned char r, unsigned char g, unsigned char b); void setAccentCol(OCol&);
			/* Resets the theme of this Drawable to its default values. */
			void resetTheme(void);

			/* Returns the position of this Drawable relative to its parent (if it has one). Pass true to retrieve the global position relative to the Window. */
			OVec getPos(bool globalToWindow=false);
			/* Returns the positional coordinates of the centre of this Drawable relative to the top left. */
			OVec getCentre();
			/* Returns the size of this Drawable. Only access the W and H values! Pass true to get the size modified by the UI scale of this Drawable. */
			OVec4 getSize(bool useScale=false);
			/* Returns the minimum size of this Drawable. Only access the W and H values! Pass true to get the size modified by the UI scale of this Drawable. */
			OVec4 getMinSize(bool useScale=false);
			/* Returns the local UI scale of this drawable. Pass true to retrieve the global scale of the Drawable including the scales of its parents (if it has them). */
			float getScale(bool includeParents=false);
			/* Unused. Returns the local rotation of this drawable. Pass true to retrieve the global rotation of the Drawable including the rotations of its parents (if it has them). */
			float getRotation(bool includeParents=false);
			/* Returns the local position and size of this Drawable. Pass true to retrieve the global position relative to the Window. */
			OVec4 getGeometry(bool globalToWindow=false);
			/* Returns the child index of this Drawable if it is linked to a Container. Returns -1 if not linked. */
			int getIndex(void);

			/* Returns a copy of the theme used by this Drawable. */
			OTheme getTheme(void);

			/* Log the data of this Drawable to the terminal. Pass true to get more detailed information. */
			virtual void log(bool verbose=false) override;
	};
}

#endif /* !__ORION_OKIT_CDRAWABLE_H__ */