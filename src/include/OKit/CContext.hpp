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

	/* The base class for all OrionUI Elements. */
	class OWidget;

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
			/* The Type of this Context. */
			CCType    XTYPE;
			/* The XID of this Context. */
			uint32_t  XWIN;
			/* The XID of this Context's Parent. */
			uint32_t  XPARENT;
			/* The XID of this Context's X Graphics Context. */
			uint32_t  XGC;
			/* The raw colour of this Context. */
			uint32_t  XCOL;
			/* The XCB Event Mask used by this Context. */
			uint32_t  XMASK;
			/* The title of this Context. ONLY FOR USE ON CCT_TOPLEVEL! */
			char*     XTITLE;
			struct{
				/* Has this Context been mapped to the Window Manager? */
				bool  XMAPPED     : 1;
				/* Has this Context been mapped to the Event Dispatcher? */
				bool  XLINKED     : 1;
				/* Is this Context also an abstracted OSurface? */
				bool  XABSTRACTED : 1;
			}XFLAGS; /* Saves memory by packing these together. */

			/* Destructor. Frees all memory and unlinks from X. */
			~CContext(void);
			/* Empty constructor. Sets all values to 0. */
			CContext(void);

			/**
			 * @brief Initialises this Context based on the given parameters.
			 * Make sure to call .map() after this!
			 * @param root A pointer to the Context to use as a parent. Pass NULL to make this Context use the Window Manager as the parent.
			 * @param x The X position of this Context relative to its parent.
			 * @param y The Y position of this Context relative to its parent.
			 * @param w The width of this Context.
			 * @param h The height of this Context.
			 * @param t The title of this Context. Only used with CCT_TOPLEVEL. Can be NULL.
			 * @param col A pointer to the OCol this Context will use as its primary colour. Cannot be NULL.
			 * @param mask The XCB Event Mask used by this Context. Will dispatch these to the Listener via the Listener Function. Can be NULL.
			 * @param type The Type of this Context. See CCType.
			 * @return True if Context could be successfully created, otherwise false.
			 */
			bool create(CContext* root, int16_t x, int16_t y, uint16_t w, uint16_t h, const char* t, OCol* col, uint32_t mask, CCType type);
			/**
			 * @brief Destroys this Context, resetting it to be used again with create().
			 * @return True if Context could be successfully destroyed, otherwise false.
			 */
			bool destroy(void);

			/**
			 * @brief Maps this Context to be visible on its parent Context.
			 * Make sure to call XCB_FLUSH() after this! (XCB_FLUSH can only be accessed if ORION_INTERNAL is defined!)
			 * @param link Should this Context register itself to the Event Dispatcher (CXHA)?
			 * @return True if Context could be successfully mapped, otherwise false.
			 */
			bool map(bool link);
			/**
			 * @brief Unmaps this Context from its parent Context.
			 * @return True if Context could be successfully mapped, otherwise false.
			 */
			bool unmap(void);
			/**
			 * @brief Clears a given section and sets it to the primary colour.
			 * NOTE: Does NOT clear other Contexts!
			 * @param startX The starting X position to clear.
			 * @param startY The starting Y position to clear.
			 * @param endX The ending X position to clear.
			 * @param endY The ending Y position to clear.
			 */
			void clear(int16_t startX, int16_t startY, uint16_t endX, uint16_t endY);
			/**
			 * @brief Draws a given section and sets it to the given colour.
			 * @param x The X position of the area to draw.
			 * @param y The Y position of the area to draw.
			 * @param w The Width of the drawn area (relative to the starting point)
			 * @param h The Height of the drawn area (relative to the starting point)
			 * @param col A pointer to the colour to fill the area with
			 */
			void drawArea(int16_t x, int16_t y, uint16_t w, uint16_t h, OCol* col);

			/**
			 * @brief Sets this Context's title to the given String. Only used on CCT_TOPLEVEL Contexts.
			 * @param title The String to copy and set.
			 * @return True if title was successfully changed, otherwise false.
			 */
			bool setTitle(const char* title);
			/**
			 * @brief Sets this Context's colour to the given OCol.
			 * @param col A pointer to the Colour to set this Context's primary colour to.
			 * @return True if colour was successfully changed, otherwise false.
			 * Can return false if the "new" colour to set is identical to the current colour.
			 */
			bool setCol(OCol* col);
			/**
			 * @brief Sets this Context's position to the given values.
			 * @param x The new X position relative to the parent.
			 * @param y The new Y position relative to the parent.
			 * @return True if position was successfully changed, otherwise false. 
			 */
			bool setPos(int16_t x, int16_t y);
			/**
			 * @brief Sets this Context's size to the given values.
			 * @param w The new width of this Context.
			 * @param h The new height of this Context.
			 * @return True if size was successfully changed, otherwise false.
			 */
			bool setSize(uint16_t w, uint16_t h);
			/**
			 * @brief Sets this Context's geometry to the given values.
			 * @param x The new X position relative to the parent.
			 * @param y The new Y position relative to the parent.
			 * @param w The new width of this Context.
			 * @param h The new height of this Context
			 * @return True if geometry was successfully changed, otherwise false.
			 */
			bool setGeometry(int16_t x, int16_t y, uint16_t w, uint16_t h);

			/**
			 * @return An OVec4 containing the position and size values of this Context.
			 */
			OVec4 getGeometry(void);

			/**
			 * @brief Logs the information of this Context to the terminal.
			 * @param verbose Log verbose information about this Context instead of the traditional information. Default is false.
			 * @param newLine Should the output be placed on a newline or append to the current one if applicable? Default is true.
			 */
			virtual void log(bool verbose=false, bool newLine=true) override;

	};

/*** Context Handling ***/
#ifdef ORION_INTERNAL
	extern bool      CXHA_INIT(void);
	extern bool      CXHA_FREE(void);
	extern bool      CXHA_LINK(CContext* context);
	extern bool      CXHA_UNLINK(CContext* context);
	extern CContext* CXHA_FROMXID(uint32_t XWIN);

#endif /* ORION_INTERNAL */
}

#endif /* !__ORIONAPI_OKIT_CCONTEXT_H__ */