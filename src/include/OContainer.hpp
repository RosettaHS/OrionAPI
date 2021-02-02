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

#ifndef __ORION_OKIT_OCONTAINER_H__
#define __ORION_OKIT_OCONTAINER_H__

#include "CDrawable.hpp"
#include "CContainer.hpp"

namespace Orion{

	namespace DRAW{ void OContainer(CDrawable*); }
	namespace HANDLE{ void OContainer(void*, X::CXEvent*); }

	class OContainer : public CDrawable, public CContainer{
		friend void DRAW::OContainer(CDrawable*); friend void HANDLE::OContainer(void*, X::CXEvent*);
		protected:

			virtual void onLink(void) override;
			virtual void onUnlink(void) override;
			virtual void onPosChanged(void) override;
			virtual void onSizeChanged(void) override;
			virtual void sort(void) override;
		public:
			// ~OContainer(void);
			OContainer(void);
			OContainer(CContainer& parent, int x, int y, unsigned int w, unsigned int h);

			virtual void setScale(float) override;
			virtual void setCol(unsigned char r, unsigned char g, unsigned char b) override;
	};
}

#endif /* !__ORION_OKIT_OCONTAINER_H__ */