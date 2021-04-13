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

	void OChar::clear(void){
		byteCount=1; isUnicode=0;
		for(unsigned char i=0;i<OCHARBYTES;i++){ get.asMultiByte[i]=0; bd[i]=OUNI_NULL; }
	}

	void OChar::setTo(const char* multibyte){
		clear();
		byteCount=0;
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

	void OChar::log(bool verbose, bool newLine){
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
			( (newLine) ? OLog("%s\n",get.asMultiByte) : OLog("%s",get.asMultiByte) );
		}
	}

/** Generic Character functions **/

	OUnicodeType OCharGetUnicodeType(char c){
		if(!c){ return OUNI_NULL; }
		unsigned char uniflags=0;
		while(1){ if(c&bits[uniflags]){ uniflags++; }else{ break; } }
		return ( (uniflags<=6) ? (OUnicodeType)uniflags : OUNI_UNKNOWN );
	}

/*** Strings ***/

	OString::~OString(void){ clear(); }

	uint32_t OString::apparentToReal(uint32_t index){
		uint32_t iA=0;
		for(uint32_t i=0;i<length.real;i++){
			if(OCharGetUnicodeType(raw[i])!=OUNI_CONTINUE){ iA++; }else{ continue; }
			if((iA-1)==index){ return i; }
		}
		return 0;
	}

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

	bool OString::setChar(uint32_t index, OChar c){
		if(raw && index<length.apparent){
			OChar charToReplace=getChar(index);
			uint32_t iR=apparentToReal(index);
			uint32_t j=0;
			int32_t excess=charToReplace.byteCount-c.byteCount;
			if( (length.real-excess) > memuse){ setMemory(memuse+c.byteCount-1); }
		/* If the sizes of the new character doesn't match the size of the character to replace, do some other operations first. */
			if(excess<0){
				for(uint32_t i=memuse;i>=iR-excess; i--){
					raw[i]=raw[i+excess];
				}
			}else if(excess>0){
				for(uint32_t i=iR+excess;i<length.real;i++){
					raw[i-excess]=raw[i];
				}
				raw[length.real-excess]=0;
			}
		/* Actually setting the new character. */
			for(uint32_t i=iR;i<memuse;i++){
				if(j==c.byteCount){ break; }else{ raw[i]=c.get.asMultiByte[j]; j++; }
			}
			if(excess!=0){
				length.real=OStringLength(raw);
				length.apparent=OStringLength(raw,true);
			}
			return true;
		}
		return false;
	}

	char* OString::getText(void) const { return raw; }
	OString::operator char*(void) const{ return raw; }

	OChar OString::getChar(uint32_t index){
		OChar c;
		if(raw && index<length.apparent){
			char tmp[OCHARBYTES-1]={0,0,0,0};
			uint32_t j=0;
			for(uint32_t i=apparentToReal(index);i<length.real;i++){
				if(j==OCHARBYTES-1){ break; }else{ tmp[j]=raw[i]; j++; }
			}
			c.setTo(tmp);
		}
		return c;
	}
	OChar OString::operator[](uint32_t index){ return getChar(index); }

	uint32_t OString::getLength(bool realLength){ return ( realLength ? length.real : length.apparent); }

	bool OString::equalTo(const char* text) const     { return OStringCompare(raw,text); }
	bool OString::operator==(const char* text ) const { return OStringCompare(raw,text); }

	bool OString::ready(void) const    { return ( raw ? true : false ); }
	OString::operator bool(void) const { return ( raw ? true : false ); }

	bool OString::contains(const char* substring){
		if(raw){
			return ( (OStringFindFirst(raw,substring)!=OSTRING_NOTFOUND) ? true : false );
		}
		return false;
	}

	/* I'll figure out how to do this later, more pressing issues at the moment. */
	// bool OString::replace(const char* substring, const char* replacement){
		// if(raw){
			// (void)replacement;
			// uint32_t occurances=OStringFindOccurances(raw,substring);
			// if(occurances){
				// uint32_t substringLength=OStringLength(substring);
				// uint32_t replacementLength=OStringLength(replacement);
				// int32_t excess=substringLength-replacementLength;
			// OLogBits(raw,memuse,true);
				// if( (length.real-excess)+1 > memuse){ setMemory(memuse+(replacementLength*occurances)-1); }
				// /*
				 // * Is C++ idio(t)matic new/delete faster than malloc()? It's prettier at least..
				 // * Still, I refuse to use this syntax for anything serious.
				 // */
				// uint32_t* occuranceList = new uint32_t[occurances];
			// /* Loop through the String again to store the positions of all occurances. */
				// uint32_t j=0,k=0;
				// for(uint32_t i=0;i<length.real+1;i++){
					// if(j==substringLength){ j=0,occuranceList[k]=i,k++; }
					// if(raw[i]==substring[j]){ j++; }
					// else{ j=0; }
				// } j=0;k=0; /* Reset these in case we need them later. */
			// /* If the size of the replacement String doesn't match the given substring, do some other operations first. */
				// if(excess<0){
					// ODLog("excess negative\n");
					// for(uint32_t i=0;i<occurances;i++){
						// for(j=memuse;j>=occuranceList[i]-excess;j--){
							// raw[j]=raw[j+excess];
							// ODLog("setting raw[%d] to raw[%d]\n",j,j+excess);
						// }
						// ODLog("looped!\n");
						// if(i){ occuranceList[i]+=excess; } /* Skip the first one. */
					// }
				// }else if(excess>0){
					// ODLog("excess positive\n");
					// for(uint32_t i=0;i<occurances;i++){
						// if(i){ occuranceList[i]+=excess; } /* Skip the first one. */
						// for(j=occuranceList[i]+excess;j<length.real;j++){
							// raw[j-excess]=raw[j];
						// }
						// ODLog("looped!\n");
					// }
					// // for(uint32_t i=iR+excess;i<length.real;i++){
						// // raw[i-excess]=raw[i];
					// // }
					// raw[length.real]=0;
				// }
			// /* Set the new replacement String. */
				// for(uint32_t i=0;i<length.real;i++){
					// if(i==occuranceList[k]-1){
						// for(j=0;j<replacementLength;j++){
							// raw[i]=replacement[j];
							// i++;
						// }
						// k++;
					// }
				// }
			// /* Clean up and return. */
				// delete[] occuranceList;
				// OLogBits(raw,memuse,true);
				// return true;
			// }
		// }
		// return false;
	// }
	
	OString OString::operator+(const char* text) const{
		OString str;
		str.setMemory( (length.real+OStringLength(text))+1 ); /* It's one less memory operation this way. */
		str.setTo(text);
		str.append(text);
		return str;
	}

	void OString::log(bool verbose, bool newLine){
		if(verbose){
			( (newLine) ? 
				OLog("%s | Real Length : %d | Apparent Length : %d | Memory Size : %d\n",raw,length.real,length.apparent,memuse)
				:
				OLog("%s | Real Length : %d | Apparent Length : %d | Memory Size : %d",raw,length.real,length.apparent,memuse)
			);
		}else{ ( (newLine) ?  OLog("%s\n",raw) :  OLog("%s",raw) ); }
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

	size_t OStringFindOccurances(const char* string, const char* substring){
		size_t l1=OStringLength(string);
		size_t l2=OStringLength(substring);
		size_t i=0,j=0,occurances=0;

		for(i=0;i<l1+1;i++){
			if(j==l2){ j=0,occurances++; }
			if(string[i]==substring[j]){ j++; }
			else{ j=0; }
		}

		return occurances;
	}

}
