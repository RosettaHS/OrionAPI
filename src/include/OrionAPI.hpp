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
	#include <sys/types.h>
	#include <limits.h>

	/* Core */
	#include "errdefs.hpp"
	#include "Application.hpp"

	/* Sub-libraries */
	#include "OSL/OSL.hpp"
	#include "OKit/OKit.hpp"

#ifdef ORION_NOGRAPHICS
	#define __ORION_ISHEADLESS true
#else
	#define __ORION_ISHEADLESS false
#endif /* ORION_NOGRAPHICS */

/* Service Functions */
namespace Orion{
	/**
	 * @brief Starts the OrionAPI Service. This is required to use any OrionUI Elements.
	 * @param AppName The name of this Application. If this is non-zero, and the binary is not in an Orion-Native Application Structure,
	 * OrionAPI will create a data storage folder using this name in the user's .local/share folder, allowing use of OrionAPI data directories for non-standard Applications.
	 * @param AppIdentifier An identifier for this Application, used for Orion-Native IPC.
	 * The format for this would be "mycompany.myapplication" or "mycompany.myproduct.myservice".
	 * Example: "rosetta.orion.files"
	 * @param ForceONative If this is true, the Application will immediately quit if the graphical service hasn't been initialised (if headless is false) or binary is not in an Orion-Native Application Structure.
	 * @param headless If this is true, OrionAPI will not establish a graphical connection, and won't allow OrionUI Elements to be created.
	 * By default, this is false. If, however, ORION_NOGRAPHICS is defined, this will be true by default.
	 * @return True on successful initialisation, otherwise false if service is already initialised.
	 */
	extern bool OAppStart(const char* AppName=0, const char* AppIdentifier=0, bool ForceONative=false, bool headless=__ORION_ISHEADLESS);
	/*
	* Runs the OrionAPI graphical service (if UI elements are present) and once done, frees all memory allocated by the OrionAPI service.
	* Blocks function execution until the service ends. Place this at the bottom of main().
	*/

	/**
	 * @brief Blocks execution until the OrionAPI Graphical Service (OKit) ends, and once done, frees all memory allocated by OrionAPI, such as data directories.
	 * Place this near the bottom of main().
	 * @return True if service was running and was successfully closed, otherwise false if service was never running in the first place.
	 */
	extern bool OAppEnd(void);
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

#undef __ORION_ISHEADLESS

#endif /* !__ORIONAPI_H__ */
