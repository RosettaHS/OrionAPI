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

#ifndef __ORIONAPI_OKIT_OCONTAINER_H__
#define __ORIONAPI_OKIT_OCONTAINER_H__

#include "OWidget.hpp"

namespace Orion{
	/*** Private Widget Containment ***/

	/* A wrapper array that contains pointers to Widgets. */
	class CWidgetList{
		private:
			/* The internal array of pointers to OWidgets. */
			OWidget** arr;
			/* The count of Widgets kept in the list. */
			uint16_t  count;
			/* The maximum amount of Widgets (before stepping) to be kept in the list. */
			uint16_t  cap;
			/* The amount to add to the array length when the cap is reached. */
			uint8_t   step;
		public:
			/* Empty constructor. Sets all values to 0. */
			inline   CWidgetList(void) : arr{0},count{0},cap{0},step{0} {}
			/* Destructor. Frees all memory. */
			inline  ~CWidgetList(void) { wipe(); }

			/* Returns true if the WidgetList is initialised and ready for use. */
			inline bool isReady(void) const { return ( (arr) ? true : false ); }

			/**
			 * @brief Resizes an initialised WidgetList to be able to contain the given amount of entries.
			 * @param newCap The new maximum size for the array to be resized to.
			 * Usually is the cap plus the step (cap+step) or ( getCap()+getStep() )
			 * @return True if resize was successful, otherwise false.
			 */
			bool resizeTo(uint16_t newCap);
			/**
			 * @brief Initialises this WidgetList to the given parameters.
			 * Note that an already initialised WidgetList must be wiped first with wipe() before re-initialisation.
			 * @param cap The maximum size of the array before resizing.
			 * @param step The amount to resize by when the array is encroaching its cap.
			 * @return True if initialisation is successful, otherwise false.
			 */
			bool init(uint16_t cap, uint8_t step);
			/**
			 * @brief Frees and resets this WidgetList, leaving it blank for re-initialisation.
			 * @return True if freeing was successful, otherwise false. Cannot free an un-initialised WidgetList.
			 */
			bool wipe(void);

			/**
			 * @brief Registers a pointer to a given Widget to this WidgetList.
			 * Note that the array will automatically resize (by the step) if there is no more space.
			 * @param widget The pointer to the Widget to register.
			 * @return True if linkage was successful, otherwise false.
			 */
			bool link(OWidget* widget);
			/**
			 * @brief Unregisters a pointer of a given Widget from this WidgetList.
			 * @param widget The pointer to the Widget to unregister.
			 * @return True if unlinkage was successful, otherwise false.
			 */
			bool unlink(OWidget* widget);
			/**
			 * @brief Unregisters all Widgets from this WidgetList.
			 * @return True if unlinkages were successful, otherwise false.
			 */
			bool unlinkAll(void);
			/**
			 * @brief Returns the index if a given Widget if it is registered in this WidgetList.
			 * Note that indices can and will change overtime, make sure to check this frequently
			 * @param widget A pointer to the Widget to attempt to search for.
			 * @return The index of the Widget in the WidgetList if it could be found, otherwise returns OWIDGET_NOTLINKED
			 */
			int32_t          getIndexOf(const OWidget* widget) const;
			/* Return the count of registered Widgets in this WidgetList. */
			inline uint16_t  getCount(void) const { return count; }
			/* Return the maximum allowed amount of registered Widgets in this WidgetList before resizing. */
			inline uint16_t  getCap(void)   const { return cap; }
			/* Return the amount to resize by when the count of registered Widgets encroaches on the maximum amount. */
			inline uint8_t   getStep(void)  const { return step; }
			/* Returns the internal array used by this WidgetList. */
			inline OWidget** getArray(void) const { return arr; }
			/**
			 * @brief Returns the Widget found at the given index.
			 * @param index	The index to attempt to retrieve
			 * @returns A pointer to the Widget if successful, otherwise returns NULL
			 */
			OWidget*         getFromIndex(uint16_t index) const; OWidget* operator [](uint16_t index) const { return getFromIndex(index); }
			/**
			 * @brief Returns the Widget found at the given index, without checking if the index is valid or if the array is initialised.
			 * @param index	The index to attempt to retrieve
			 * @returns A pointer to the Widget if successful, otherwise returns NULL
			 */
			OWidget*         getFromIndexFast(uint16_t index) const { return arr[index]; }
	};

	/*** Public Widget Container Class ***/

	// namespace DRAW{ void OContainer(Orion::OWidget* widget); }

	/* The base Container for containing OrionUI Elements. */
	class OContainer : public OWidget{
		protected:
			/* The internal list of child Widgets. */
			CWidgetList list;
			/* The Context used to display this Container (and its children) to the screen. */
			CContext    selfContext;
			/* The Context for linked Widgets to use. */
			CContext*   contextToUse;
			/*
			 * The Container for linked Widgets to use.
			 * Some derived Containers may consist of several nested Containers.
			 */
			OContainer* containerToUse;
			/* Should this Container force itself to be used next time link() is called, instead of containerToUse? */
			bool        forceSelfOnNext;
			/* Has this Container been unlinked from its parent, and is awaiting relinkage? */
			bool        holdingInTmp;

			/* Empty constructor. Sets all values to 0. */
			OContainer(void);

			/* The base sort used by OContainer. Does nothing, and lets the Widgets sort themselves with flags. */
			void             baseSort(void);
			/* Calls when this Container gets linked to a new parent. */
			virtual void     onLink(void)         override;
			/* Calls when this Container gets unlinked from its parent. */
			virtual void     onUnlink(void)       override;
			/* Calls when this Container gets its position modified. */
			virtual void     onPosChanged(void)   override;
			/* Calls when this Container gets its size modified. */
			virtual void     onSizeChanged(void)  override;
			/* Calls when this Container gets its theme modified. */
			virtual void     onColChanged(void)   override;
			/* Temporarily unlinks all Widgets when this Container gets unlinked and stores them for later when it gets relinked. */
			void             tmpUnlinkAll(void);
			/* Relinks all temporarily held Widgets once this Container gets linked to a new parent. */
			void             tmpRelinkAll(void);
		public:
			/* Destructor. Unlinks all Widgets and frees all memory. */
			virtual ~OContainer(void);
			/**
			 * @brief Creates and links an OContainer to the parent Container with the given parameters.
			 * @param parent The Container to link this Container to.
			 * @param x The X position relative to the parent Container.
			 * @param y The Y position relative to the parent Container.
			 * @param w The width of this Container
			 * @param h The height of this Container
			 */
			OContainer(OContainer* parent, int16_t x, int16_t y, uint16_t w, uint16_t h);

			/**
			 * @brief Links a Widget to this Container.
			 * @param widget The Widget (pointer/reference) to link to this Container.
			 * @return True if linkage was successful, otherwise false.
			 */
			bool link(OWidget* widget); inline bool link(OWidget& widget)     { return link(&widget); }
			/**
			 * @brief Unlinks a Widget from this Container.
			 * @param widget The Widget (pointer/reference) to unlink from this Container.
			 * @return True if unlinkage was successful, otherwise false if Widget could not be unlinked, or Widget is not linked to this Container.
			 */
			bool unlink(OWidget* widget); inline bool unlink(OWidget& widget) { return unlink(&widget); }
			/**
			 * @brief Unlinks all Widgets from this Container.
			 * @return True if unlinkages were successful, otherwise false.
			 */
			bool unlinkAll(void);

			/*
			 * Sorts the child Widgets of this Container.
			 * Automatically called whenever Container is resized, or new Widgets are added.
			 */
			virtual void sort(void);

			/**
			 * @brief Sets the Colour of this Container to the given RGB values.
			 * @param r The red value of the Colour to set.
			 * @param g The greenvalue of the Colour to set.
			 * @param b The blue value of the Colour to set.
			 */
			virtual void setCol(uint8_t r, uint8_t g, uint8_t b) override;
			/**
			 * @brief Sets the Colour of this Container to the given OCol.
			 * @param col The OCol (pointer/reference) to set as this Container's background.
			 */			
			virtual void setCol(OCol* col) override; virtual void setCol(OCol& col) override;

			/**
			 * @brief Returns the index of the given Widget if it is linked to this Container.
			 * Note: The index can change at any moment if new Widgets are added or removed, therefore it is unsafe to store this value and never update it.
			 * @param widget The Widget (pointer/reference) to attempt to scan and retrieve the index of.
			 * @return If the Widget was found, its index is returned. Otherwise, returns OWIDGET_NOTLINKED.
			 */
			int32_t  getIndexOf(const OWidget* widget) const; inline int32_t getIndexOf(const OWidget& widget) const { return getIndexOf(&widget); }
			/**
			 * @brief Returns the child Widget at the given index.
			 * @param index The index to attempt to retrieve the Widget from
			 * @return A pointer to the Widget if it could be found, otherwise returns NULL.
			 */
			OWidget*   getChild(uint16_t index)    const; inline OWidget* operator[](uint16_t index)  const { return getChild(index); }
			/* Returns the array of Widget pointers used by this Container. */
			OWidget**  getChildren(void)           const;
			/* Returns the count of Widgets linked to this Container. */
			uint16_t   getChildCount(void)         const;
	};

	#ifndef ORION_NOALTNAMES
		/* The base Container for containing OrionUI Elements. */
		typedef OContainer container_t;
	#endif /* !ORION_NOALTNAMES */
}

#endif /* !__ORIONAPI_OKIT_OCONTAINER_H__ */