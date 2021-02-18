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
#include "../include/OKit/OSignal.hpp"

namespace Orion{

	const char* OSignal::getTypeAsString(void){
		switch(type){
			case OSIG_NONE:{return "OSIG_NONE";}
			case OSIG_BOOL:{return "OSIG_BOOL";}
			case OSIG_INT:{return "OSIG_INT";}
			case OSIG_UNSIGNED_INT:{return "OSIG_UNSIGNED_INT";}
			case OSIG_SHORT:{return "OSIG_SHORT";}
			case OSIG_UNSIGNED_SHORT:{return "OSIG_UNSIGNED_SHORT";}
			case OSIG_FLOAT:{return "OSIG_FLOAT";}
			case OSIG_DOUBLE:{return "OSIG_DOUBLE";}
			case OSIG_LONG:{return "OSIG_LONG";}
			case OSIG_UNSIGNED_LONG:{return "OSIG_UNSIGNED_LONG";}
			case OSIG_CHAR:{return "OSIG_CHAR";}
			case OSIG_UNSIGNED_CHAR:{return "OSIG_UNSIGNED_CHAR";}
			case OSIG_STRING:{return "OSIG_STRING";}
			case OSIG_ARBITRARY:{return "OSIG_ARBITRARY";}
			case OSIG_OBJECT:{return "OSIG_OBJECT";}
		};
		return 0;
	}

	void OSignal::log(bool verbose){
		if(verbose){
			OLog("OSignal %p | type %s | emitter %p | emitter->type %s {\n",(void*)this,getTypeAsString(),(void*)emitter,emitter->getTypeAsString());
			switch(type){
				case OSIG_NONE:{OLog("\tNo data to log...\n}\n");return;}
				case OSIG_BOOL:{OLog("\tget.asBool : %s\n}\n",(get.asBool ? "true" : "false"));return;}
				case OSIG_INT:{OLog("\tget.asInt : %d\n}\n",get.asInt);return;}
				case OSIG_UNSIGNED_INT:{OLog("\tget.asUnsignedInt : %u\n}\n",get.asUnsignedInt);return;}
				case OSIG_SHORT:{OLog("\tget.asShort : %hd\n}\n",get.asShort);return;}
				case OSIG_UNSIGNED_SHORT:{OLog("\tget.asUnsignedShort : %hu\n}\n",get.asUnsignedShort);return;}
				case OSIG_FLOAT:{OLog("\tget.asFloat : %f\n}\n",get.asFloat);return;}
				case OSIG_DOUBLE:{OLog("\tget.asDouble : %lf\n}\n",get.asDouble);return;}
				case OSIG_LONG:{OLog("\tget.asLong : %l\n}\n",get.asLong);return;}
				case OSIG_UNSIGNED_LONG:{OLog("\tget.asUnsignedLong : %lu\n}\n",get.asUnsignedLong);return;}
				case OSIG_CHAR:{OLog("\tget.asChar : (char) %c\n\tget.asChar : (number) %d\n}\n",get.asChar,get.asChar);return;}
				case OSIG_UNSIGNED_CHAR:{OLog("\tget.asUnsignedChar : (number) %u\n}\n",(unsigned int)get.asChar);return;}
				case OSIG_STRING:{OLog("\tget.string.asText : %s\n\tget.string.asLength : %lu\n}\n",get.string.asText,get.string.asLength);return;}
				case OSIG_ARBITRARY:{OLog("\tget.asArbitrary : %p\n}\n",get.asArbitrary);return;}
				case OSIG_OBJECT:{OLog("\tget.asObject : %p\n\tget.asObject->type : %s\n}\n",(void*)get.asObject,get.asObject->getTypeAsString());return;}
			};
		}else{
			switch(type){
				case OSIG_NONE:{OLog("No data to log...\n");return;}
				case OSIG_BOOL:{OLog("get.asBool : %s\n",(get.asBool ? "true" : "false"));return;}
				case OSIG_INT:{OLog("get.asInt : %d\n",get.asInt);return;}
				case OSIG_UNSIGNED_INT:{OLog("get.asUnsignedInt : %u\n\n",get.asUnsignedInt);return;}
				case OSIG_SHORT:{OLog("get.asShort : %hd\n",get.asShort);return;}
				case OSIG_UNSIGNED_SHORT:{OLog("get.asUnsignedShort : %hu\n\n",get.asUnsignedShort);return;}
				case OSIG_FLOAT:{OLog("get.asFloat : %f\n",get.asFloat);return;}
				case OSIG_DOUBLE:{OLog("get.asDouble : %lf\n",get.asDouble);return;}
				case OSIG_LONG:{OLog("get.asLong : %l\n",get.asLong);return;}
				case OSIG_UNSIGNED_LONG:{OLog("get.asUnsignedLong : %lu\n",get.asUnsignedLong);return;}
				case OSIG_CHAR:{OLog("get.asChar : (char) %c\tget.asChar : (number) %d\n",get.asChar,get.asChar);return;}
				case OSIG_UNSIGNED_CHAR:{OLog("get.asChar : (number) %u\n",(unsigned int)get.asChar);return;}
				case OSIG_STRING:{OLog("get.string.asText : %s\n",get.string.asText);return;}
				case OSIG_ARBITRARY:{OLog("get.asArbitrary : %p\n",get.asArbitrary);return;}
				case OSIG_OBJECT:{OLog("get.asObject : %p\tget.asObject->type : %s\n",(void*)get.asObject,get.asObject->getTypeAsString());return;}
			};
		}
	}
}
