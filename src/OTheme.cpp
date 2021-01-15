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

#include <stdio.h>
#include <stdlib.h>
#include "include/application.hpp"
#include "include/OTheme.hpp"

namespace Orion{
	void OTheme::setPrimary(unsigned char r, unsigned char g, unsigned char b){primary.setTo(r,g,b);}
	void OTheme::setSecondary(unsigned char r, unsigned char g, unsigned char b){secondary.setTo(r,g,b);}
	void OTheme::setTertiary(unsigned char r, unsigned char g, unsigned char b){tertiary.setTo(r,g,b);}
	void OTheme::setAccent(unsigned char r, unsigned char g, unsigned char b){accent.setTo(r,g,b);}
	void OTheme::log(void){
		printf("OTheme %p {\n",(void*)this);
		printf("\t Primary : %d,%d,%d\n",primary.r,primary.g,primary.b);
		printf("\t Secondary : %d,%d,%d\n",secondary.r,secondary.g,secondary.b);
		printf("\t Tertiary : %d,%d,%d\n",tertiary.r,tertiary.g,tertiary.b);
		printf("\t Accent : %d,%d,%d\n",accent.r,accent.g,accent.b);
		printf("};\n");
	}

	void OLog(OTheme& t){t.log();}
	void OVLog(OTheme& t){if(OAPP_VERBOSE){t.log();}}
	void OLog(OTheme*t){t->log();}
	void OVLog(OTheme*t){if(OAPP_VERBOSE){t->log();}}
	
	OTheme OTHEME_GLOBAL;
	bool OTHEME_PRIMARY_OVERRIDE=false;
	bool OTHEME_SECONDARY_OVERRIDE=false;
	bool OTHEME_TERTIARY_OVERRIDE=false;
	bool OTHEME_ACCENT_OVERRIDE=false;
}