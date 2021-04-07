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

#ifndef __ORIONAPI_OKIT_OVEC_H__
#define __ORIONAPI_OKIT_OVEC_H__

#include "../OSL/OLog.hpp"

namespace Orion{
	/* Used to hold position data for any OrionUI Elements. */
	struct OVec : CLoggable {
		/* The X and Y positions on the screen or of the object.*/
		int32_t x, y;

		/* Empty constructor. Sets all values to 0. */
		inline OVec(void) : x{0}, y{0} {}
		/**
		 * @brief Sets the X and Y positional values to the given parameters.
		 * @param _x The X (horizontal) positional value to set on this Vec.
		 * @param _y The Y (vertical) positional value to set on this Vec.
		 */
		inline void setTo(int32_t _x, int32_t _y) { x=_x; y=_y; }; inline OVec(int32_t _x, int32_t _y) : x{_x}, y{_y} {}
		/**
		 * @brief Attempts to set and initialise this Vector from the given formatted String. 
		 * @param format The format String to attempt to retrieve the values from.
		 * The String MUST be in a very specific format. Example: "(760,365)"
		 * The formatted part can be at any position of the String, but it MUST contain two values corresponding to each of the X and Y values.
		 * @return True on success, otherwise false if format String is not valid.
		 */
		bool setTo(const char* format);

		/**
		 * @brief Logs the information of this Vec to the terminal.
		 * @param verbose Log verbose information about this Vec instead of the traditional information. Default is false.
		 * @param newLine Should the output be placed on a newline or append to the current one if applicable? Default is true.
		 */
		virtual void log(bool verbose=false, bool newLine=true) override;
	};

	/* Used to hold position and size data for any OrionUI Elements. */
	struct OVec4 : CLoggable {
		/* The X and Y positions on the screen or of the object. */
		int16_t  x, y;
		/* The Width and Height of the object.*/
		uint16_t w, h;

		/* Empty constructor. Sets all values to 0. */
		inline OVec4(void) : x{0}, y{0}, w{0}, h{0} {}
		/**
		 * @brief Sets the positional and sizing values to the given parameters.
		 * @param _x The X (horizontal) positional value to set on this Vec4.
		 * @param _y The Y (vertical) positional value to set on this Vec4.
		 * @param _w The W (horizontal) sizing value to set on this Vec4.
		 * @param _h The H (vertical) sizing value to set on this Vec4.
		 */
		inline void setTo(int16_t _x, int16_t _y, uint16_t _w, uint16_t _h) { x=_x; y=_y; w=_w; h=_h; }; inline OVec4(int16_t _x, int16_t _y, uint16_t _w, uint16_t _h) : x{_x}, y{_y}, w{_w}, h{_h} {}
		/**
		 * @brief Attempts to set and initialise this Vector4 from the given formatted String. 
		 * @param format The format String to attempt to retrieve the values from.
		 * The String MUST be in a very specific format. Example: "(760,365,400,350)"
		 * The formatted part can be at any position of the String, but it MUST contain four values corresponding to each of the X,Y, W, and H values.
		 * @return True on success, otherwise false if format String is not valid.
		 */
		bool setTo(const char* format);

		/**
		 * @brief Logs the information of this Vec4 to the terminal.
		 * @param verbose Log verbose information about this Vec4 instead of the traditional information. Default is false.
		 * @param newLine Should the output be placed on a newline or append to the current one if applicable? Default is true.
		 */
		virtual void log(bool verbose=false, bool newLine=true) override;
	};
#ifndef ORION_NOALTNAMES

	/* Used to hold position data for any OrionUI Elements. */
	typedef OVec  vec_t;
	/* Used to hold position and size data for any OrionUI Elements. */
	typedef OVec4 vec4_t;

#endif /* !ORION_NOALTNAMES */
}
#endif /* !__ORIONAPI_OKIT_OVEC_H__ */
