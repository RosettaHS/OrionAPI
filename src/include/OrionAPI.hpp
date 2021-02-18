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


/**********************************************************************************/
/* #                                                                            # */
/*                   Welcome to the Orion Application Framework                   */
/*                       Development for Orion, made easy.                        */
/*                                                                                */
/**********************************************************************************/
/*                                                                                */
/*    This is Orion's Application Framework, it's used to create all sorts of     */
/*        graphical applications native for the Orion Operating System,           */
/*       and provides loads of useful utilities for developers to utilise         */
/*           with minimal, if any effort at all on the developer's part.          */
/*                                                                                */
/*    Documentation on how to use OrionAPI is available at the following link,    */
/*      alongside the latest release. Get started developing  for Orion now!      */
/*                                                                                */
/**********************************************************************************/
/*                                                                                */
/*                     https://github.com/RosettaHS/OrionAPI                      */
/* #                                                                            # */
/**********************************************************************************/


#ifndef __ORIONAPI_H__
#define __ORIONAPI_H__

/*
* The current version of the OrionAPI installed on this system.
* For a more up-to-date version of the OrionAPI, please visit https://github.com/RosettaHS/OrionAPI to download the latest release.
*/
#define ORIONAPI_VERSION  0
/*
* The current revision of the OrionAPI installed on this system.
* For a more up-to-date version of the OrionAPI, please visit https://github.com/RosettaHS/OrionAPI to download the latest release.
*/
#define ORIONAPI_REVISION 0

/* The following are forward declarations for Visual Studio (Code) Tooltips */

/* The OrionAPI Namespace. */
namespace Orion{
	/* OrionAPI - The namespace that contains all functions and global variables for the OApp. */
	namespace Application{}
	/* OrionAPI - The namespace that contains all internal X connection information. */
	namespace X{}
	/* OrionAPI - The namespace that contains all internal render functions for each class. */
	namespace DRAW{}
	/* OrionAPI - The namespace that contains all internal event handling functions for each class. */
	namespace HANDLE{}
}

/* Includes */
	/* System */
	#include <stdint.h>
	#include <limits.h>
	#include <sys/types.h>

	/* OrionAPI */
	#include "errdef.hpp"
	#include "application.hpp"

	/* Sub-libraries */
	#include "OSL/OSL.hpp"   /* The Orion Standard Library. */
	#include "OKit/OKit.hpp" /* The Window Widget Toolkit for the Orion Operating System. */

/* Service Functions */
namespace Orion{
	/*
	* Starts the OrionAPI service. This is required to use any Orion UI elements. 
	* Optionally you can pass a name for your application, and pass a boolean to force Orion-exclusivity.
	*/
	extern bool OAppStart(const char* AppName=0,bool ForceONative=false);
	/*
	* Runs the OrionAPI graphical service (if UI elements are present) and once done, frees all memory allocated by the OrionAPI service.
	* Blocks function execution until the service ends. Place this at the bottom of main().
	*/
	extern bool OAppEnd(void);

	/* Returns the current screen size. */
	extern OVec OScreenGetSize(void);
}


/*
*	Since "using namespace Orion;" is so common in OApps, and since everything is prefixed anyway,
*	it might make more sense to force "using namespace Orion;" on and allow it to be disabled,
*	rather than force everyone to write "using namespace Orion;" in each of their files,
*	as is dictated in each tutorial and example for the OrionAPI. 
*/

#ifndef ORION_UNUSE_NAMESPACE
using namespace Orion;
#endif /* !ORION_UNUSE_NAMESPACE */

#endif /* !__ORIONAPI_H__ */
