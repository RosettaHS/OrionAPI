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
				OUnicodeType t=OCharGetUnicodeType(c);
				/* If this is confirmed to be a Unicode multi-byte, immediately stop when encountering ASCII, or another call header. */
				if(isUnicode && t!=OUNI_CONTINUE){ break; }
				get.asMultiByte[i]=c;
				bd[i]=t;
				byteCount++;
				/* Only store the first character of a multi-byte "character"  if it's not actually UTF-8. */
				if(t==OUNI_ASCII && byteCount<=1){ isUnicode=false; break; }
				else                             { isUnicode=true; }
			}else{ bd[i]=OUNI_NULL; break; }
		}
	}

	void OChar::setTo(char singlebyte){
		clear();
		get.asSingleByte=singlebyte;
		byteCount=1;
		isUnicode=false;
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
					case OUNI_NULL:     { OLog(" | OUNI_NULL "); break; }
					case OUNI_ASCII:    { OLog(" | OUNI_ASCII "); break; }
					case OUNI_HEADER_1X:{ OLog(" | OUNI_HEADER_1X "); break; }
					case OUNI_HEADER_2X:{ OLog(" | OUNI_HEADER_2X "); break; }
					case OUNI_HEADER_3X:{ OLog(" | OUNI_HEADER_3X "); break; }
					case OUNI_HEADER_4X:{ OLog(" | OUNI_HEADER_4X "); break; }
					case OUNI_HEADER_5X:{ OLog(" | OUNI_HEADER_5X "); break; }
					case OUNI_CONTINUE: { OLog(" | OUNI_CONTINUE "); break; }
					case OUNI_UNKNOWN:  { OLog(" | OUNI_UNKNOWN "); break; }
				}
				OLog("]\n");
			}
		}else{
			OLog("%s\n",get.asMultiByte);
		}
	}

/** Generic Character functions **/

	OUnicodeType OCharGetUnicodeType(char c){
		unsigned char uniflags=0;
		while(1){ if(c&bits[uniflags]){ uniflags++; }else{ break; } }
		return ( (uniflags<=6) ? (OUnicodeType)uniflags : OUNI_UNKNOWN );
	}

/*** Strings ***/

	OString::~OString(void){ clear(); }

	bool OString::clear(void){
		bool didFree=false;
		if(raw){ free(raw); didFree=true; }
		raw=0;
		length.apparent=0;
		length.real=0;
		memuse=0;
		return didFree;
	}

	bool OString::setMemory(uint32_t mem){
		if(mem==memuse){ return false; }
		if(!mem){ return clear(); }
		if(raw){
			raw=(char*)realloc(raw,sizeof(char)*mem);
			if(mem>(length.real+1)){
				for(uint32_t i=length.real+1;i<mem;i++){ raw[i]=0; }
			}else{
				raw[mem]=0;
				length.real=(mem-1);
				length.apparent=OStringLength(raw,true);
			}
			memuse=mem;
			return true;
		}else{
			raw=(char*)malloc(sizeof(char)*mem);
			length.apparent=0;
			length.real=0;
			if(!raw){ raw=0, memuse=0; return false; }
			memuse=mem;
			return true;
		}
		return false;
	}

	bool OString::setTo(const char* newText){
		uint32_t newLength=OStringLength(newText);
		if( memuse!=(newLength+1) && !setMemory(newLength+1) ){ return false; }
		for(uint32_t i=0;i<newLength;i++){ raw[i]=newText[i]; }
		length.apparent=OStringLength(newText,true);
		length.real=newLength;
		return true;
	}
	OString& OString::operator=(const char* text){ setTo(text); return *this; }

	bool OString::append(const char* appendText){
		uint32_t newLength=length.real+OStringLength(appendText);
		uint32_t curLength=length.real;
		if( (newLength+1)>memuse ){
			if( memuse!=(newLength+1) && !setMemory(newLength+1) ){ return false; }
		}
		uint32_t j=0;
		for(uint32_t i=curLength;i<newLength;i++){
			raw[i]=appendText[j];
			j++;
		}
		raw[newLength+1]=0;
		length.apparent=OStringLength(raw,true);
		length.real=newLength;
		return true;
	}
	OString& OString::operator+=(const char* text){ append(text); return *this; }

	char* OString::getText(void) const { return raw; }
	OString::operator char*(void) const{ return raw; }

	OChar OString::getChar(uint32_t index, bool indexApparentOnly){
		OChar c;
		if(index>length.real){ return c; }
		char tmp[OCHARBYTES-1]={0,0,0,0};
		if(indexApparentOnly){
			uint32_t iA=0;
			for(uint32_t i=0;i<length.real;i++){
				if(OCharGetUnicodeType(raw[i])!=OUNI_CONTINUE){ iA++; }else{ continue; }
				if((iA-1)==index){
					uint32_t j=0;
					for(uint32_t k=i;k<length.real;k++){
						if(j==OCHARBYTES-1){ break; }else{ tmp[j]=raw[k]; j++; }
					}
					c.setTo(tmp);
					break;
				}
			}
		}else{
			uint32_t j=0;
			for(uint32_t i=index;i<length.real;i++){
				if(j==OCHARBYTES-1){ break;}else{ tmp[j]=raw[i]; j++; }
			}
			c.setTo(tmp);
		}
		return c;
	}
	OChar OString::operator[](uint32_t index){ return getChar(index); }

	uint32_t OString::getLength(bool apparentLength){ return ( apparentLength ? length.apparent : length.real); }

	bool OString::equalTo(const char* text) const     { return OStringCompare(raw,text); }
	bool OString::operator==(const char* text ) const { return OStringCompare(raw,text); }
	OString OString::operator+(const char* text) const{
		OString str;
		str.setMemory( (length.real+OStringLength(text))+1 ); /* It's one less memory operation this way. */
		str.setTo(text);
		str.append(text);
		return str;
	}

	void OString::log(bool verbose){
		if(verbose){
			OLog("%s | Real Length : %d | Apparent Length : %d | Memory Size : %d\n",raw,length.real,length.apparent,memuse);
		}else{ OLog("%s\n",raw); }
	}

/** Generic String functions. **/
	void OFormat(char* output, const char* format, ...){
		if(!output){ OLog("ORIONAPI | WARNING! CANNOT PASS NULL WHEN FORMATTING A STRING TO A BUFFER!\n"); return; }
		va_list arg;
		va_start(arg,format);
		vsprintf(output,format,arg);
		va_end(arg);
	}
	
	size_t OStringLength(const char* s, bool getApparentLength){
		if(!s){ return 0; }
		size_t l=0;
		if(getApparentLength){
			size_t i=0;
			while(true){
				if(s[i]!=0){
					i++;
					if(OCharGetUnicodeType(s[i])!=OUNI_CONTINUE){ l++; }
				}else{ break; }
			}
		}else{
			while(true){
				if(s[l]!=0){ l++; }else{ break; }
			}
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
