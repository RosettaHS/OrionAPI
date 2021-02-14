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


#ifndef __ORION_OAPI_H__
#define __ORION_OAPI_H__

/*
* The current version of the OrionAPI installed on this system.
* For a more up-to-date version of the OrionAPI, please visit https://github.com/RosettaHS/OrionAPI to download the latest release.
*/
#define OAPI_VERSION  0
/*
* The current revision of the OrionAPI installed on this system.
* For a more up-to-date version of the OrionAPI, please visit https://github.com/RosettaHS/OrionAPI to download the latest release.
*/
#define OAPI_REVISION 0

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

/*
*	These following three includes are all the system headerfiles that the OrionAPI automatically includes.
*	No need to worry about too much clutter or contamination!
*/

#include <stdint.h>
#include <limits.h>
#include <sys/types.h>

/* Core Modules */
#include "errdef.hpp"				/* OrionAPI */
#include "application.hpp"			/* OrionAPI */
#include "xservice.hpp"				/* OKit */
#include "OLog.hpp"					/* OSL */
#include "CLoggable.hpp"			/* OSL */
#include "OString.hpp"				/* OSL */
#include "OVec.hpp"					/* OKit */
#include "OCol.hpp"					/* OKit */
#include "OTheme.hpp"				/* OKit */
#include "CBaseUI.hpp"				/* OKit */
#include "OSignal.hpp"				/* OKit */
#include "CSignalListener.hpp"		/* OKit */
#include "CSLArray.hpp"				/* OKit */
#include "CSignalDispatcher.hpp"	/* OKit */
#include "CXEvent.hpp"				/* OKit */
#include "CContext.hpp"				/* OKit */
#include "CLabel.hpp"				/* OKit */
#include "CDrawable.hpp"			/* OKit */
#include "CNodeArray.hpp"			/* OKit */
#include "CContainer.hpp"			/* OKit */
/* Element Modules */
#include "ORect.hpp"				/* OKit */
#include "OLabel.hpp"				/* OKit */
#include "OContainer.hpp"			/* OKit */
#include "OMarginContainer.hpp"		/* OKit */

#include "CWindow.hpp" 				/* OKit - Debug */

#include "OService.hpp"				/* OrionAPI */

/*
*	Since "using namespace Orion;" is so common in OApps, and since everything is prefixed anyway,
*	it might make more sense to force "using namespace Orion;" on and allow it to be disabled,
*	rather than force everyone to write "using namespace Orion;" in each of their files,
*	as is dictated in each tutorial and example for the OrionAPI. 
*/

#ifndef ORION_UNUSE_NAMESPACE
using namespace Orion;
#endif /* !ORION_UNUSE_NAMESPACE */

#endif /* !__ORION_OAPI_H__ */
