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
#include "include/OSignal.hpp"

namespace Orion{
	void OSignal::log(void){
		printf("OSignal %p | type %d | emitter %p {\n",(void*)this,type,(void*)obj);
		switch(type){
			case OSIG_NONE:{printf("\tNo data to log...\n}\n");return;}
			case OSIG_BOOL:{printf("\tget.asBool %s\n}\n",(get.asBool ? "true" : "false"));return;}
			case OSIG_INT:{printf("\tget.asInt %d\n}\n",get.asInt);return;}
			case OSIG_UNSIGNED_INT:{printf("\tget.asUnsignedInt %u\n}\n",get.asUnsignedInt);return;}
			case OSIG_SHORT:{printf("\tget.asShort %hd\n}\n",get.asShort);return;}
			case OSIG_UNSIGNED_SHORT:{printf("\tget.asUnsignedShort %hu\n}\n",get.asUnsignedShort);return;}
			case OSIG_FLOAT:{printf("\tget.asFloat %f\n}\n",get.asFloat);return;}
			case OSIG_DOUBLE:{printf("\tget.asDouble %lf\n}\n",get.asDouble);return;}
			case OSIG_CHAR:{printf("\tget.asChar (char) %c\n\tget.asChar (number) %d\n}\n",get.asChar,get.asChar);return;}
			case OSIG_UNSIGNED_CHAR:{printf("\tget.asUnsignedChar (number) %u\n}\n",(unsigned int)get.asChar);return;}
			case OSIG_STRING:{printf("\tget.asString.text %s\n\tget.asString.length %lu\n}\n",get.asString.text,get.asString.length);return;}
			case OSIG_ARBITRARY:{printf("\tget.asArbitrary %p\n}\n",get.asArbitrary);return;}
			case OSIG_OBJECT:{printf("\tget.asObject %p\n\tget.asObject->type %d\n}\n",(void*)get.asObject,get.asObject->type);return;}
		};
	}
}