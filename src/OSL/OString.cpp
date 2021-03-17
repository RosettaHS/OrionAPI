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

#include <stdarg.h>
#include <malloc.h>
#include "../include/OSL/OString.hpp"
#include "../include/OSL/OLog.hpp"

#define BITCOUNT 8

MAXCONST unsigned char bits[BITCOUNT]={0x80,0x40,0x20,0x10,0x8,0x4,0x2,0x1};

namespace Orion{
/*** Characters ***/

	OChar::OChar(const char* multibyte)            { setTo(multibyte);  }
	OChar::OChar(char singlebyte)                  { setTo(singlebyte); }
	OChar& OChar::operator=(const char* multibyte) { setTo(multibyte);  return *this; }
	OChar& OChar::operator=(char singlebyte)       { setTo(singlebyte); return *this; }

	void OChar::setTo(const char* multibyte){
		clear();
		for(unsigned char i=0;i<OCHARBYTES-1;i++){
			unsigned char c=multibyte[i];
			if(c!=0){
				get.asMultiByte[i]=c;
				unsigned char uniflags=0;
				while(1){ if(c&bits[uniflags]){ uniflags++; }else{ break; } }
				bd[i]=( (uniflags<=6) ? (OUnicodeType)uniflags : OUNI_UNKNOWN );
				byteCount++;
				/* Only store the first character of a multi-byte "character"  if it's not actually UTF-8. */
				if(uniflags==OUNI_ASCII && byteCount<=1){ isUnicode=false; break; }
				else                                    { isUnicode=true; }
			}else{ bd[i]=OUNI_NULL; break; }
		}
	}

	void OChar::log(bool verbose){
		if(verbose){
			for(unsigned char i=0;i<OCHARBYTES;i++){
				OLog("[ ");
				for(unsigned char j=0;j<BITCOUNT;j++){
					( ((get.asMultiByte[i])&bits[j]) ? OLog("1") : OLog("0") );
				}
				switch(bd[i]){
					default:            { OLog(" | ERROR "); break; }
					case OUNI_NULL:     { OLog(" | UCBD_NULL "); break; }
					case OUNI_ASCII:    { OLog(" | UCBD_ASCII "); break; }
					case OUNI_HEADER_1X:{ OLog(" | UCBD_HEADER_1X "); break; }
					case OUNI_HEADER_2X:{ OLog(" | UCBD_HEADER_2X "); break; }
					case OUNI_HEADER_3X:{ OLog(" | UCBD_HEADER_3X "); break; }
					case OUNI_HEADER_4X:{ OLog(" | UCBD_HEADER_4X "); break; }
					case OUNI_HEADER_5X:{ OLog(" | UCBD_HEADER_5X "); break; }
					case OUNI_CONTINUE: { OLog(" | UCBD_CONT "); break; }
					case OUNI_UNKNOWN:  { OLog(" | UCBD_CONT "); break; }
				}
				OLog("]\n");
			}
		}else{
			OLog("%s\n",get.asMultiByte);
		}
	}

/** Generic Character functions **/

/* TODO: Add them. */

/*** Strings ***/

/** Generic String functions. **/
	void OFormat(char* output, const char* format, ...){
		if(!output){ OLog("ORIONAPI | WARNING! CANNOT PASS NULL WHEN FORMATTING A STRING TO A BUFFER!\n"); return; }
		va_list arg;
		va_start(arg,format);
		vsprintf(output,format,arg);
		va_end(arg);
	}
	
	size_t OStringLength(const char* s){
		if(!s){ return 0; }
		size_t l=0;
		while(true){
			if(s[l]!='\0'){l++;}else{break;}
		}
		return l;
	}

	bool OStringCompare(const char* s1, const char* s2){
		size_t l1=OStringLength(s1);
		size_t l2=OStringLength(s2);
		bool comp=false;
		if(l1==l2){
			comp=true;
			for(size_t i=0;i<l1;i++){
				if(s1[i]!=s2[i]){ comp=false; }
			}
		}
		return comp;
	}

	size_t OStringFindFirst(const char* string, const char* substring){
		size_t l1=OStringLength(string);
		size_t l2=OStringLength(substring);
		size_t i=0,j=0,place=OSTRING_NOTFOUND;

		for(i=0;i<l1+1;i++){
			if(j==l2){ return place; }
			if(string[i]==substring[j]){ if(!j){ place=i; } j++; }
			else{ j=0,place=OSTRING_NOTFOUND; }
		}

		return OSTRING_NOTFOUND;
	}

	size_t OStringFindLast(const char* string, const char* substring){
		size_t l1=OStringLength(string);
		size_t l2=OStringLength(substring);
		size_t i=0,j=0,occurances=0,place=OSTRING_NOTFOUND;

		for(i=0;i<l1+1;i++){
			if(j==l2){ j=0,occurances++; }
			if(string[i]==substring[j]){ if(!j){ place=i; } j++; }
			else{ j=0; }
		}

		return ( occurances ? place : OSTRING_NOTFOUND );
	}

}
