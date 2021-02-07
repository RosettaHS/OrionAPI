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
#include <limits.h>
#include "CLoggable.hpp"
#include "OVec.hpp"
#include "OCol.hpp"
#include "OTheme.hpp"
#include "CBaseUI.hpp"
#include "CContext.hpp"

/* Position Flag Definitions */
	/* Positional Only. Aligns to the start of the given axis. */
	#define START (SHRT_MAX)
	/* Positional Only. Aligns to the end of the given axis. */
	#define END (SHRT_MAX-1)
	/* Positional Only. Aligns to the center of the given axis. */
	#define CENTER (SHRT_MAX-2)
	/* Positional Only. Aligns to the centre of the given axis. */
	#define CENTRE (SHRT_MAX-2)

/* Size Flag Definitions */
	/* Size Only. Fills to the end of the given axis. */
	#define FILL (SHRT_MAX-3)


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

#endif /* !ORION_INTERNAL */

namespace Orion{
	class CContainer; /* Forward declaration. See CContainer.hpp for definition. */

	/* Base class for all drawable and containable UI elements. */
	class CDrawable : public CBaseUI , public CLoggable {
		protected:
			/* The X and Y positional coordinates of this Drawable. */
			int x,y;
			/* The relative positional coordinates for the centre of this Drawable. */
			int centreX, centreY;
			/* The internal offset (real) positional coordinates of this Drawable. */
			int offsetX, offsetY;
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
			/* Internal. Flag that dictates whether the draw function in drawPtr should do a complete wipe and redraw of this Drawable. */
			bool fullRedraw;

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

			/* Internal. Sets the raw flags of internal.modFlags. Sets one at a time! Returns true if flag was valid. */
			bool setFlag(short xFlag, short yFlag, short wFlag, short hFlag);
			/* Internal. Sets flags and other data. */
			void init(int, int, unsigned int, unsigned int);
			/* Internal. Calls this virtual method when linked. */
			virtual void onLink(void);
			/* Internal. Calls this virtual method when unlinked. */
			virtual void onUnlink(void);
			/* Internal. Calls this virtual method when position is modified. */
			virtual void onPosChanged(void);
			/* Internal. Calls this virtual method when size is modified. */
			virtual void onSizeChanged(void);

			/* Allows Containers to access internal members of this Drawable. */
			friend class CContainer;
		public:
			struct{
				/* Internal. Pointer to the draw function that the deferred Drawable will use. Takes in the Drawable as an argument. */
				void(*drawPtr)(CDrawable*);	
				/* Internal. Raw flags that govern the size and positioning that non-sorting Containers will use to modify this Drawable. */
				uint8_t modFlags;
			}internal;

			/* Destructor. Unlinks from its parent (if it has one) and frees all memory. */
			~CDrawable(void);
			/* Empty constructor. Sets all values to 0. */
			CDrawable(void);

			/* Links this Drawable to the passed Container. Returns false if could not link. */
			bool linkTo(CContainer&);
			/* Unlinks this Drawable its parent Container. Returns false if could not unlink. */
			bool unlinkThis(void);

			/* Sets the position of this Drawable relative to its parent (if it has one). */
			void setPos(int x, int y); void setPos(OVec&);
			/* Sets the positional coordinates of the centre of this Drawable relative to the top left. Used during scaling. */
			void setCentre(int x, int y); void setCentre(OVec&);
			/* Sets the size of this Drawable. Pass true at the end to force a redraw. */
			void setSize(unsigned int w,unsigned int h, bool force=false); void setSize(OVec&, bool force=false);
			/* Sets the minimum allowed size of this Drawable. */
			void setMinSize(unsigned int w, unsigned int h); void setMinSize(OVec&);
			/* Sets the UI scale of this Drawable. Not applicable on Containers. */
			virtual void setScale(float);
			/* Unused. Sets the relative rotation of this Drawable. */
			void setRotation(float);
			/* Sets the position and size of this Drawable with the passed parameters. */
			void setGeometry(int x, int y, unsigned int w, unsigned int h); void setGeometry(OVec4&);

			/* Sets the colour of this Drawable. Some derived classes do not allow this! */
			virtual void setCol(unsigned char r, unsigned char g, unsigned char b); virtual void setCol(OCol&);
	
			/* Internal. Sets and overrides the theme of this Drawable. Use with caution! */
			void setTheme(OTheme&);
			/* Internal. Sets and overrides the primary colour of this Drawable. Use with caution!*/
			void setThemePrimaryCol(unsigned char r, unsigned char g, unsigned char b); void setThemePrimaryCol(OCol&);
			/* Internal. Sets and overrides the secondary colour of this Drawable. Use with caution!*/
			void setThemeSecondaryCol(unsigned char r, unsigned char g, unsigned char b); void setThemeSecondaryCol(OCol&);
			/* Internal. Sets and overrides the tertiary colour of this Drawable. Use with caution!*/
			void setThemeTertiaryCol(unsigned char r, unsigned char g, unsigned char b); void setThemeTertiaryCol(OCol&);
			/* Internal. Sets and overrides the accent colour of this Drawable. Use with caution!*/
			void setThemeAccentCol(unsigned char r, unsigned char g, unsigned char b); void setThemeAccentCol(OCol&);
			/* Internal. Resets the theme of this Drawable to its default values. */
			void resetTheme(void);

			/* Returns a pointer to the parent of this Drawable (if it has one). */
			CContainer* getParent(void);
			/* Returns the position of this Drawable relative to its parent (if it has one). Pass true to retrieve the global position relative to the Window. */
			OVec getPos(bool globalToWindow=false);
			/* Returns the positional coordinates of the centre of this Drawable relative to the top left. */
			OVec getCentre(void);
			/* Returns the size of this Drawable. Only access the W and H values! Pass true to get the size modified by the UI scale of this Drawable. */
			OVec4 getSize(bool useScale=false);
			/* Returns the minimum size of this Drawable. Only access the W and H values! Pass true to get the size modified by the UI scale of this Drawable. */
			OVec4 getMinSize(bool useScale=false);
			/* Returns the local UI scale of this Drawable. Containers cannot scale. */
			float getScale(void);
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