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

#include "include/OLog.hpp"
#include "include/application.hpp"
#include "include/OTheme.hpp"

namespace Orion{
	void OTheme::setPrimary(unsigned char r, unsigned char g, unsigned char b){primary.setTo(r,g,b);}
	void OTheme::setSecondary(unsigned char r, unsigned char g, unsigned char b){secondary.setTo(r,g,b);}
	void OTheme::setTertiary(unsigned char r, unsigned char g, unsigned char b){tertiary.setTo(r,g,b);}
	void OTheme::setAccent(unsigned char r, unsigned char g, unsigned char b){accent.setTo(r,g,b);}

	void OTheme::log(bool verbose){
		if(verbose){
			OLog("OTheme %p {\n",(void*)this);
			OLog("\t Primary\t:\tR %d | G %d | B %d | XCOL %lu\n",primary.r,primary.g,primary.b,primary.XCOL);
			OLog("\t Secondary\t:\tR %d | G %d | B %d | XCOL %lu\n",secondary.r,secondary.g,secondary.b,secondary.XCOL);
			OLog("\t Tertiary\t:\tR %d | G %d | B %d | XCOL %lu\n",tertiary.r,tertiary.g,tertiary.b,secondary.XCOL);
			OLog("\t Accent\t\t:\tR %d | G %d | B %d | XCOL %lu\n}\n",accent.r,accent.g,accent.b,accent.XCOL);
		}else{
			OLog("Primary : %d,%d,%d\n",primary.r,primary.g,primary.b);
			OLog("Secondary : %d,%d,%d\n",secondary.r,secondary.g,secondary.b);
			OLog("Tertiary : %d,%d,%d\n",tertiary.r,tertiary.g,tertiary.b);
			OLog("Accent : %d,%d,%d\n",accent.r,accent.g,accent.b);
		}
	}
	
	OTheme OTHEME_GLOBAL;
	bool OTHEME_PRIMARY_OVERRIDE=false;
	bool OTHEME_SECONDARY_OVERRIDE=false;
	bool OTHEME_TERTIARY_OVERRIDE=false;
	bool OTHEME_ACCENT_OVERRIDE=false;
}