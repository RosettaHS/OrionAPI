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
#include "include/OString.hpp"

namespace Orion{
	OString::~OString(void){
		if(!isMemStatic){if(str){free(str);str=0;}}
	}
	OString::OString(void) : str{0},length{0},isMemStatic{false} {}
	OString::OString(const char* text) : str{(char*)text} {
		length=calcLength(text);
		isMemStatic=true;
	}

	size_t OString::calcLength(const char* t){
		size_t l=0;
		while(true){
			if(t[l]!='\0'){l++;}else{break;}
		}
		return l;
	}

	void OString::setTo(const char* text){
		if(!isMemStatic){free(str);}
		str=(char*)text;
		length=calcLength(text);
	}

	void OString::append(const char* text){
		size_t appendLength=calcLength(text);
		size_t newLength=0;
		char* tmp=(char*)malloc(sizeof(char)*(length+appendLength)+1);
		while(newLength<length+appendLength){
			if(newLength<length){
				tmp[newLength]=str[newLength];
			}else{
				tmp[newLength]=text[newLength-length];
			}
			newLength++;
		}
		length=newLength;
		tmp[newLength+1]='\0';
		if(!isMemStatic){free(str);}
		str=tmp;
		isMemStatic=false;
	}

	size_t OString::getLength(void){return length;}

	void OString::log(bool verbose){
		if(verbose){printf("OString %p | length %lu : %s\n",(void*)this,length,str);}
		else{printf("%s\n",str);}
	}

	OString::operator char*(void) const{return str;}
	OString::operator const char*(void) const{return str;}
	OString& OString::operator+=(const char* other){
		append(other);
		return *this;
	}
	OString OString::operator+(const char* other){
		OString newStr(str);
		newStr.append(other);
		return newStr;
	}
	OString& OString::operator=(const char* other){
		setTo(other);
		return *this;
	}
}