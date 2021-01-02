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

#ifndef __ORION_OKIT_OCOL_H__
#define __ORION_OKIT_OCOL_H__

namespace Orion{
	/* Allows you to use colours with OKit. Takes in RGB values.*/
	struct OCol{
		/* The Red, Green, and Blue values (respectively) of the OCol.*/
		unsigned char r,g,b;
		/* Internal. Representation of the RGB values that X can use. */
		unsigned long XCOL;
	/* General Handling */
		/* Empty contructor. Sets all values to 0. */
		OCol();
		/* Creates an OCol with the given parameters. */
		OCol(unsigned char r, unsigned char g, unsigned char b);
		/* Sets the OCol to use the given parameters. */
		void setTo(unsigned char r, unsigned char g, unsigned char b);
		/* Prints RGB data to terminal. */
		void log();
	};
	
}
#endif /* !__ORION_OKIT_OCOL_H__ */