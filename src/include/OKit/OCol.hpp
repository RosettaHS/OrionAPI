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

#ifndef __ORIONAPI_OKIT_OCOL_H__
#define __ORIONAPI_OKIT_OCOL_H__

#include "common.hpp"

namespace Orion{
	/* Allows you to use colours with OrionUI (OKit) Elements. Takes in RGB values.*/
	struct OCol : public CLoggable{
		/* The Red, Green, and Blue values (respectively) of the OCol.*/
		unsigned char r,g,b;
		/* Internal. Representation of the RGB values that X can use. */
		uint32_t      XCOL;

		/* Empty contructor. Sets all values to 0. */
		OCol(void);

		/**
		 * @brief Initialises and sets the Colour to the given RGB values. 
		 * @param r The red value (0 - 255) of this Colour.
		 * @param g The green value (0 - 255) of this Colour.
		 * @param b The blue value (0 - 255) of this Colour.
		 */
		void setTo(unsigned char r, unsigned char g, unsigned char b); OCol(unsigned char r, unsigned char g, unsigned char b);

		/**
		 * @brief Internal. Attempts to set and initialise this Colour from the given formatted String. 
		 * @param format The format String to attempt to retrieve the values from.
		 * The String MUST be in a very specific format. Example: "(255,255,255)"
		 * The formatted part can be at any position of the String, but it MUST contain three values corrisponding to each of the RGB values.
		 * @return True on success, otherwise false if format String is not valid.
		 */
		bool setTo(const char* format);

		/**
		 * @brief Logs the information of this Col to the terminal.
		 * @param verbose Log verbose information about this Col instead of the traditional information. Default is false.
		 * @param newLine Should the output be placed on a newline or append to the current one if applicable? Default is true.
		 */
		virtual void log(bool verbose=false, bool newLine=true) override;
	};
	
}
#endif /* !__ORIONAPI_OKIT_OCOL_H__ */