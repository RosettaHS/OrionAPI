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

	class CWidgetList{
		private:
			OWidget** arr;
			uint16_t  count;
			uint16_t  cap;
			uint8_t   step;
		public:
			inline   CWidgetList(void) : arr{0},count{0},cap{0},step{0} {}

			bool resizeTo(uint16_t newCap);
			bool init(uint16_t cap, uint8_t step);
			bool wipe(void);

			bool link(OWidget* widget);
			bool unlink(OWidget* widget);
			int32_t         getIndexOf(OWidget* widget) const;
			inline uint16_t getCount(void) const { return count; }
			inline uint16_t getCap(void)   const { return cap; }
			inline uint8_t  getStep(void)  const { return step; }
	};

	/*** Public Widget Container Class ***/
}

#endif /* !__ORIONAPI_OKIT_OCONTAINER_H__ */