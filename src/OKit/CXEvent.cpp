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

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "../include/OSL/OLog.hpp"
#include "../include/OKit/CXEvent.hpp"

namespace Orion{
	namespace X{
		void CXEvent::compose(void* _e){
			XEvent* e=(XEvent*)_e;
			xtype=e->type;
			valid=true;
			switch(e->type){
				case Expose:{type=CXE_EXPOSE;expose.x=e->xexpose.x,expose.y=e->xexpose.y,expose.w=e->xexpose.width,expose.h=e->xexpose.height;return;}
				case EnterNotify: case LeaveNotify:{type=CXE_MFOCUS_CHANGED;mouseFocus.state=(e->type==EnterNotify ? true : false);mouseFocus.mod=e->xcrossing.state;return;}
				case ConfigureNotify:{type=CXE_CONFIGURE;configure.x=e->xconfigure.x,configure.y=e->xconfigure.y,configure.w=e->xconfigure.width,configure.h=e->xconfigure.height;return;}
				case KeyPress: case KeyRelease:{type=CXE_KEYCHAR;XLookupString(&e->xkey,&key.letter,1,0,0);(e->type==KeyPress ? key.pressed=true : key.pressed=false);key.code=e->xkey.keycode;key.mod=e->xkey.state;return;} /* I know the syntax is pretty bad but it's more efficient. */
				case ButtonPress: case ButtonRelease:{type=CXE_MOUSECLICK;mouse.x=e->xbutton.x,mouse.y=e->xbutton.y,mouse.x_root=e->xbutton.x_root,mouse.y_root=e->xbutton.y_root;mouse.state=e->xbutton.state,mouse.button=e->xbutton.button;(e->type==ButtonPress ? mouse.pressed=true : mouse.pressed=false);return;}
				case MotionNotify:{type=CXE_MOUSEMOVE;mouse.x=e->xmotion.x,mouse.y=e->xmotion.y,mouse.x_root=e->xmotion.x_root,mouse.y_root=e->xmotion.y_root;mouse.state=e->xmotion.state,mouse.button=0,mouse.pressed=false;return;}
			}
			valid=false;
		}


		void CXEvent::log(void){
			OLog("CXEvent %p | valid %s | type %d | xtype %d | {\n",(void*)this,(valid ? "true" : "false"),type,xtype);
			switch(type){
				case CXE_NONE: case CXE_FORCERENDER:{OLog("\t No event data to log...\n}\n");return;}
				case CXE_EXPOSE:{OLog("\texpose.x %d\n\texpose.y %d\n\texpose.w %u\n\texpose.h %u\n}\n",expose.x,expose.y,expose.w,expose.h);return;}
				case CXE_MFOCUS_CHANGED:{OLog("\tmouseFocus.state %d\n\tmouseFocus.mod %u\n}\n",mouseFocus.state,mouseFocus.mod);return;}
				case CXE_CONFIGURE:{OLog("\tconfigure.x %d\n\tconfigure.y %d\n\tconfigure.w %u\n\tconfigure.h %u\n}\n",configure.x,configure.y,configure.w,configure.h);return;}
				case CXE_KEYCHAR:{OLog("\tkey.letter \"%c\"\n\tkey.pressed %d\n\tkey.code %u\n\tkey.mod %u\n}\n",key.letter,key.pressed,key.code,key.mod);return;}
				case CXE_MOUSEMOVE:{OLog("\tmouse.x %d\n\tmouse.y %d\n\tmouse.x_root %d\n\tmouse.y_root %d\n\tmouse.state %u\n}\n",mouse.x,mouse.y,mouse.x_root,mouse.y_root,mouse.state);return;}
				case CXE_MOUSECLICK:{OLog("\tmouse.x %d\n\tmouse.y %d\n\tmouse.x_root %d\n\tmouse.y_root %d\n\tmouse.state %u\n\tmouse.button %u\n\tmouse.pressed %s\n}\n",mouse.x,mouse.y,mouse.x_root,mouse.y_root,mouse.state,mouse.button,(mouse.pressed ? "true" : "false"));return;}
			}
		}
	}
}
