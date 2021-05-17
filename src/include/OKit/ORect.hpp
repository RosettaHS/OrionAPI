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

#ifndef __ORIONAPI_OKIT_ORECT_H__
#define __ORIONAPI_OKIT_ORECT_H__

#include "OWidget.hpp"
#include "OContainer.hpp"

namespace Orion{
	class ORect final : public OWidget{
		protected:
			virtual void       onLink(void)         override;
			virtual void       onUnlink(void)       override;
			virtual void       onPosChanged(void)   override;
			virtual void       onSizeChanged(void)  override;
			virtual void       onColChanged(void)   override;
			virtual void       onDraw(bool full)    override;
		public:
			ORect(OContainer* parent, int16_t x, int16_t y, uint16_t w, uint16_t h, OCol* col=0);
			ORect(OContainer* parent, int16_t x, int16_t y, uint16_t w, uint16_t h, OCol& col);
	};

	#ifndef ORION_NOALTNAMES
		typedef ORect rectangle_t;
	#endif /* !ORION_NOALTNAMES */
}

#endif /* !__ORIONAPI_OKIT_ORECT_H__ */