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

#ifndef __ORION_OKIT_OSIGNAL_H__
#define __ORION_OKIT_OSIGNAL_H__

#include <sys/types.h>
#include "CLoggable.hpp"
#include "CBaseUI.hpp"
#include "OString.hpp"

namespace Orion{
	/* An enumeration of types of Signals that CSignalDispatchers can dispatch. */
	enum OSignalType{
		OSIG_NONE,
		OSIG_BOOL,
		OSIG_INT,
		OSIG_UNSIGNED_INT,
		OSIG_SHORT,
		OSIG_UNSIGNED_SHORT,
		OSIG_FLOAT,
		OSIG_DOUBLE,
		OSIG_CHAR,
		OSIG_UNSIGNED_CHAR,
		OSIG_STRING,
		OSIG_ARBITRARY,
		OSIG_OBJECT
	};
	
	/* A container struct for data types emitted on events from a given UI element. */
	struct OSignal : public CLoggable{
		/* The object that emitted this Signal. */
		CBaseUI* obj;
		/* The type of data carried by the Signal. See OSignalType. */
		OSignalType type;

		/* The data carried by the Signal. */
		union{
			bool asBool;
			int asInt;
			unsigned int asUnsignedInt;
			short asShort;
			unsigned short asUnsignedShort;
			float asFloat;
			double asDouble;
			char asChar;
			unsigned char asUnsignedChar;
			struct{
				char* text;
				size_t length;
			}asString;
			void* asArbitrary;
			CBaseUI* asObject;
		}get;

		/* Logs the information of this Signal to the terminal. Pass true for more verbose information (recommended). */
		virtual void log(bool verbose=false) override;

		/* Returns a string version of the type of this Signal. */
		const char* getTypeAsString(void);
	};
}

#endif /* !__ORION_OKIT_OSIGNAL_H__ */