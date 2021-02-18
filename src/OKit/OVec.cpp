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

#define ORION_INTERNAL

#include "include/OSL/OLog.hpp"
#include "include/OKit/OVec.hpp"

namespace Orion{
	OVec::OVec(void) : x{0},y{0} {}
	OVec::OVec(int _x, int _y) : x{_x},y{_y} {}
	void OVec::log(bool verbose){
		if(verbose){
			OLog("OVec %p : (%d, %d)\n",(void*)this,x,y);
		}else{
			OLog("(%d, %d)\n",x,y);
		}
	}

	OVec4::OVec4(void) : x{0},y{0},w{0},h{0} {}
	OVec4::OVec4(int _x, int _y, unsigned int _w, unsigned int _h) : x{_x},y{_y},w{_w},h{_h} {}
	void OVec4::log(bool verbose){
		if(verbose){
			OLog("OVec4 %p {\n",(void*)this);
			OLog("\t x %d, y %d\n",x,y);
			OLog("\t w %u, h %u\n}\n",w,h);
		}else{
			OLog("(%d, %d, %u, %u)\n",x,y,w,h);
		}
	}
}
