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
			int32_t          getIndexOf(OWidget* widget) const;
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

	/* The base Container for containing OrionUI Elements. */
	class OContainer : public OWidget{
		protected:
			CWidgetList list;
			CContext    selfContext;
			CContext*   contextToUse;
			OContainer* containerToUse;
			bool        forceSelfOnNext;

			OContainer(void);

			virtual void     baseSort(void);
		public:
			~OContainer(void);

			bool             link(OWidget* widget);
			inline bool      link(OWidget& widget)   { return link(&widget); }
			bool             unlink(OWidget* widget);
			inline bool      unlink(OWidget& widget) { return unlink(&widget); }
			bool             unlinkAll(void);

			virtual void     sort(void);

			int32_t          getIndexOf(OWidget* widget) const;
			inline int32_t   getIndexOf(OWidget& widget) const { return getIndexOf(&widget); }
			OWidget*         getChild(uint16_t index)    const;
			inline OWidget*  operator[](uint16_t index)  const { return getChild(index); }
			OWidget**        getChildren(void)           const;
			uint16_t         getChildCount(void)         const;
	};

	#ifndef ORION_NOALTNAMES
		/* The base Container for containing OrionUI Elements. */
		typedef OContainer container_t;
	#endif /* !ORION_NOALTNAMES */
}

#endif /* !__ORIONAPI_OKIT_OCONTAINER_H__ */