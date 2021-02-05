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

#include "CContext.hpp"
#include "CDrawable.hpp"
#include "CContainer.hpp"

namespace Orion{
	/* Handling */
	namespace DRAW{ extern void CWindow(CDrawable*); }
	namespace HANDLE{ extern void CWindow(void*,X::CXEvent*); }

	/* Debug. A top-level window. */
	class CWindow : public CDrawable, public CContainer{
		protected:
			const char* title;
		
			friend void DRAW::CWindow(CDrawable*);
			friend void HANDLE::CWindow(void*,X::CXEvent*);
		public:
			virtual void sort(void) override;
			~CWindow(void);
			CWindow(void);
			CWindow(int x, int y, unsigned int w, unsigned int h, const char* title=0);
	};
}