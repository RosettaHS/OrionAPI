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
#include "../include/OKit/OVec.hpp"

namespace Orion{
	bool OVec::setTo(const char* format){
		size_t start, end, part=0, sect=0;
		start=OStringFindFirst(format,"("); end=OStringFindFirst(format,")");
		if(start==OSTRING_NOTFOUND || end==OSTRING_NOTFOUND){ return false; }
		char    tmp[10]={0,0,0,0,0,0,0,0,0,0};
		int32_t f[2]={0,0};

		for(size_t i=start+1;i<end;i++){
			OLog("part %lu | sect %lu\n",part,sect);
			if(part>10 || sect>1){ return false; }
			switch(format[i]){
				default: { tmp[part]=format[i]; part++; break; }
				case ' ':{ break; }
				case ',':{
					f[sect]=atoi(tmp);
					for(int8_t i=0;i<10;i++){ tmp[i]=0; }
					part=0; sect++; break;
				}
			}
		}
		if(sect!=1){ return false; }
		f[1]=atoi(tmp);

		setTo(f[0],f[1]);
		return true;
	}

	void OVec::log(bool verbose, bool newLine){
		if(verbose){
			OLog("OVec : %p | x : %d | y :d",(const void*)this,x,y);
		}else{
			OLog("(%d, %d)",x,y);
		}
		if(newLine){ OLog("\n"); }
	}

	bool OVec4::setTo(const char* format){
		size_t start, end, part=0, sect=0;
		start=OStringFindFirst(format,"("); end=OStringFindFirst(format,")");
		if(start==OSTRING_NOTFOUND || end==OSTRING_NOTFOUND){ return false; }
		char    tmp[10]={0,0,0,0,0,0,0,0,0,0};
		int32_t f[4]={0,0,0,0};

		for(size_t i=start+1;i<end;i++){
			OLog("part %lu | sect %lu\n",part,sect);
			if(part>10 || sect>3){ return false; }
			switch(format[i]){
				default: { tmp[part]=format[i]; part++; break; }
				case ' ':{ break; }
				case ',':{
					f[sect]=atoi(tmp);
					for(int8_t i=0;i<10;i++){ tmp[i]=0; }
					part=0; sect++; break;
				}
			}
		}
		if(sect!=3){ return false; }
		f[3]=atoi(tmp);

		setTo(f[0],f[1],f[2],f[3]);
		return true;
	}

	void OVec4::log(bool verbose, bool newLine){
		if(verbose){
			OLog("OVec4 : %p | x : %d | y : %d | w : %u | h : %lu",(const void*)this,x,y,w,h);
		}else{
			OLog("(%d, %d, %u, %u)",x,y,w,h);
		}
		if(newLine){ OLog("\n"); }
	}
}