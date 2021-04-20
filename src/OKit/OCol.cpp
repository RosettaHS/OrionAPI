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

#include <stdlib.h>
#include "../include/OSL/OString.hpp"
#include "../include/OKit/OCol.hpp"

namespace Orion{
	bool OCol::setTo(const char* format){
		size_t start, end, part=0, sect=0;
		start=OStringFindFirst(format,"("); end=OStringFindFirst(format,")");
		if(start==OSTRING_NOTFOUND || end==OSTRING_NOTFOUND){ return false; }
		char    tmp[3]={0,0,0};
		uint8_t f[3]={0,0,0};

		for(size_t i=start+1;i<end;i++){
			if(part>3 || sect>3){ return false; }
			switch(format[i]){
				default: { tmp[part]=format[i]; part++; break; }
				case ' ':{ break; }
				case ',':{ f[sect]=atoi(tmp); tmp[0]=0,tmp[1]=0,tmp[2]=0; part=0; sect++; break; }
			}
		}
		if(sect!=2){ return false; }
		f[2]=atoi(tmp);

		setTo(f[0],f[1],f[2]);
		return true;
	}

	void OCol::log(bool verbose, bool newLine){
		if(verbose){
			OLog("OCol %p : R %d | G %d | B %d | XCOL %lu",(void*)this,raw.r,raw.g,raw.b,XCOL);
			if(newLine){ OLog("\n"); }
		}else{
			OLog("(%d, %d, %d)",raw.r,raw.g,raw.b);
			if(newLine){ OLog("\n"); }
		}
	}

	void OTheme::log(bool verbose, bool	newLine){
		if(verbose){
			OLog("(%lu), (%lu), (%lu), (%lu)",primary.XCOL,secondary.XCOL,tertiary.XCOL,accent.XCOL);
			if(newLine){ OLog("\n"); }
		}else{
			if(newLine){
				OLog("\tPrimary   : (%d,%d,%d)\n\tSecondary : (%d,%d,%d)\n\tTertiary  : (%d,%d,%d)\n\tAccent    : (%d,%d,%d)\n",
					primary.raw.r,primary.raw.g,primary.raw.b,
					secondary.raw.r,secondary.raw.g,secondary.raw.b,
					tertiary.raw.r,tertiary.raw.g,tertiary.raw.b,
					accent.raw.r,accent.raw.g,accent.raw.b);
			}else{
				OLog("(%d,%d,%d), (%d,%d,%d), (%d,%d,%d), (%d,%d,%d)",
					primary.raw.r,primary.raw.g,primary.raw.b,
					secondary.raw.r,secondary.raw.g,secondary.raw.b,
					tertiary.raw.r,tertiary.raw.g,tertiary.raw.b,
					accent.raw.r,accent.raw.g,accent.raw.b);
			}
		}
	}
}
