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

#ifndef __ORION_OKIT_OVEC_H__
#define __ORION_OKIT_OVEC_H__

namespace Orion{
	/* Used to hold position data for any OKit object. */
	struct OVec{
		/* The X and Y positions on the screen or of the object.*/
		int x,y;

		/* Empty constructor. Sets all values to 0. */
		OVec(void);
		/* Creates an OVec with the given parameters. */		
		OVec(int x, int y);
		/* Prints all the data to the terminal. */
		void log(void);
	};

	/* Used to hold position and size data for any OKit object. */
	struct OVec4{
		/* The X and Y positions on the screen or of the object. */
		int x,y;
		/* The Width and Height of the object.*/
		unsigned int w,h;

		/* Empty constructor. Sets all values to 0. */
		OVec4(void);
		/*Creates an OVec4 with the given parameters. Takes in X, Y, Width, Height respectively. */
		OVec4(int x, int y, unsigned int w, unsigned int h);
		/* Prints all the data to the terminal. */
		void log(void);
	};

	void OLog(OVec&);
	void OVLog(OVec&);
	void OLog(OVec*);
	void OVLog(OVec*);

	void OLog(OVec4&);
	void OVLog(OVec4&);
	void OLog(OVec4*);
	void OVLog(OVec4*);
}
#endif /* !__ORION_OKIT_OVEC_H__ */