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

#ifndef __ORIONAPI_APPLICATION_H__
#define __ORIONAPI_APPLICATION_H__

#include <sys/types.h>

/*Orion implementation for maximum amount of characters allowed in a directory path.*/
#define OPATH_MAX 1024

#ifndef ORION_NOHELPERS

/* Wrapper if() statement for doing operations based on if the Application is in an Orion-Native Application Architecture. */
#define ONATIVEONLY if(OAPP_NATIVE)

/* The Name of this Application. */
#define OAPP_NAME        (Orion::Application::name)
/* The Name of the User running this Application. */
#define OAPP_USERNAME    (Orion::Application::username)
/* The PID of this Application. */
#define OAPP_PID         (Orion::Application::PID)
/* The UI Scale of this Application. */
#define OAPP_SCALE       (Orion::Application::UIScale)

/* A Path to the Folder created by OrionAPI for storing Data for this Application independent of the User running it. */
#define OAPP_STATICPATH  (Orion::Application::Storage::staticPath)
/* A Path to the Folder created by OrionAPI for storing Libraries that this Application might need to function. */
#define OAPP_LIBPATH     (Orion::Application::Storage::libPath)
/* A Path to the Folder created by OrionAPI for storing Data for this Application based on the User running it. */
#define OAPP_DATAPATH    (Orion::Application::Storage::userPath)

/* The Path to the User's /home/ Folder */
#define OAPP_HOME        (Orion::Application::Path::toHome)
/* The Path to this Application's binary.  */
#define OAPP_BINPATH     (Orion::Application::Path::toBinary)
/* The Path to the Folder where this Application's binary is stored.  */
#define OAPP_BINDIR      (Orion::Application::Path::toBinaryFolder)
/* The Path to the Working Directory of this Application.  */
#define OAPP_CWD         (Orion::Application::Path::toCWD)

/* Is this Application contained in the proper Orion-Native Application Architecture? */
#define OAPP_NATIVE      (Orion::Application::Flags::isNativeOApp)
/* Is this Application running in verbose mode? */
#define OAPP_VERBOSE     (Orion::Application::Flags::isVerbose)
/* Is the OrionAPI Service running? */
#define OAPP_RUNNING     (Orion::Application::Flags::isServiceRunning)

#endif /* !ORION_NOHELPERS */

namespace Orion{
	namespace Application{
		/* The Name of this Application. */
		extern const char* name;
		/* The Name of the User running this Application. */
		extern char* username;
		/* The PID of this Application. */
		extern pid_t PID;
		/* The UI Scale of this Application. */
		extern float UIScale;
		/* A list of Paths created by OrionAPI for Data handling. */
		namespace Storage{
			/* A Path to the Folder created by OrionAPI for storing Data for this Application independent of the User running it. */
			extern char* staticPath;
			/* A Path to the Folder created by OrionAPI for storing Libraries that this Application might need to function. */
			extern char* libPath;
			/* A Path to the Folder created by OrionAPI for storing Data for this Application based on the User running it. */
			extern char* userPath;
		}
		/* A list of generic Paths allocated by OrionAPI. */
		namespace Path{
			/* The Path to the User's /home/ Folder */
			extern char* toHome;
			/* The Path to this Application's binary.  */
			extern char* toBinary;
			/* The Path to the Folder where this Application's binary is stored.  */
			extern char* toBinaryFolder;
			/* The Path to the Working Directory of this Application.  */
			extern char* toCWD;
		}
		/* A list of Flags for this Application. */
		namespace Flags{
			/* Is this Application contained in the proper Orion-Native Application Architecture? */
			extern bool isNativeOApp;
			/* Is this Application running in verbose mode? */
			extern bool isVerbose;
			/* Is the OrionAPI Service running? */
			extern bool isServiceRunning;
		}

#ifdef ORION_INTERNAL
		/* Internal. Initialises all of the variables found in the Application Handler. */
		extern bool init(const char* appname);
		/* Internal. Wipes all of the variables from OrionAPI's Application Handler and frees unused memory. */
		extern bool wipe(void);
#endif /* ORION_INTERNAL */
	}
}

#endif /* !__ORIONAPI_APPLICATION_H__ */