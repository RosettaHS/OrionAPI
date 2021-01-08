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

#ifndef __ORION_OKIT_CXEVENT_H__
#define __ORION_OKIT_CXEVENT_H__

namespace Orion{
	namespace X{
		/* Enumeration, list of XEvents handled by CXEvent. */
		enum CXEventType{
			CXE_EXPOSE,
			CXE_MFOCUS_CHANGED,
			CXE_XWIN_MODDED,
			CXE_KEYCHAR
		};

		/* Wrapper for the Events generated by the X Service. */
		struct CXEvent{
			/* The type of the XEvent handled. */
			CXEventType type;
			/* The type of the raw XEvent. */
			int xtype;
			/* Has the composition been successful? Don't dispatch if this is false! */ 
			bool valid;
			/* Initialises data and composes with the given pointer to an XEvent. */
			void compose(void* event);
			/* Internal | DEBUG. Logs all data to termianl in a formatted manner regardless of "Orion::Application::verbose". */
			void log(void);

			/* Union of data handled. */
			union{
				struct{
					int x,y;
					unsigned int w,h;
				}expose; /* Avoid this one! Use configure instead! */
				
				struct{
					bool state;
					unsigned int mod;
				}mouseFocus;

				struct{
					int x,y;
					unsigned int w,h;
				}configure;
				
				struct{
					char letter;
					unsigned int code;
					unsigned int mod;
					bool pressed;
				}key;
			};
		};
	}
}

#endif /* !__ORION_OKIT_CXEVENT_H__ */