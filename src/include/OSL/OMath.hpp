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

#ifndef __ORIONAPI_OSL_OMATH_H__
#define __ORIONAPI_OSL_OMATH_H__

#include "common.hpp"

namespace Orion{
	/**
	 * @brief Returns a version of the given value clamped between the two other values.
	 * @param x The value to clamp.
	 * @param min The minimum for the value.
	 * @param max The maximum for the value.
	 * @return If the given value "x" is within the range between "min" and "max", it will be returned as normal.
	 * Otherwise, the minimum or maximum value will be returned depending on which is the closest value to "x".
	 */
	inline long OClamp(long& x, long& min, long& max){ return ( (x<min) ? (min) : ( (x>max) ? max : x) ); }
	/**
	 * @brief Returns a version of the given value clamped below the second value.
	 * @param x The value to clamp.
	 * @param max The maximum for the value.
	 * @return If the given value "x" is less than the maximum value, the value of "x" will be returned.
	 * Otherwise, the maximum value will be returned.
	 */
	inline long OClampMax(long& x, long& max){ return ( (x>max) ? max : x ); }
	/**
	 * @brief Returns a version of the given value clamped above the second value.
	 * @param x The value to clamp.
	 * @param min The minimum for the value
	 * @return If the given value "x" is greater than the minimum value, the value of "x" will be returned.
	 * Otherwise, the minimum value will be returned.
	 */
	inline long OClampMin(long& x, long& min){ return ( (x<min) ? min : x ); }
}

#endif /* !__ORIONAPI_OSL_OMATH_H__ */