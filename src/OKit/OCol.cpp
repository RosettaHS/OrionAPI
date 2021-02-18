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

#include "../include/OSL/OLog.hpp"
#include "../include/OKit/OCol.hpp"

namespace Orion{
	OCol::OCol(void) : r{0},g{0},b{0},XCOL{0} {}
	OCol::OCol(unsigned char _r, unsigned char _g, unsigned char _b) :  r{_r},g{_g},b{_b},XCOL{(unsigned long)(_b + (_g<<8) + (_r<<16))} {}
	void OCol::setTo(unsigned char _r, unsigned char _g, unsigned char _b){
		r=_r,g=_g,b=_b;
		XCOL=(unsigned long)(_b + (_g<<8) + (_r<<16));
	}
	void OCol::log(bool verbose){
		if(verbose){
			OLog("OCol %p : R %d | G %d | B %d | XCOL %lu\n",(void*)this,r,g,b,XCOL);
		}else{
			OLog("(%d, %d, %d)\n",r,g,b);
		}
	}
}
