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

#ifndef __ORION_OKIT_APPLICATION_H__
#define __ORION_OKIT_APPLICATION_H__

#include <sys/types.h>

#ifndef ORION_NOHELPERS

/*Orion implementation for maximum amount of characters allowed in a directory path.*/
#define OPATH_MAX 1024
/* Consider changing this ^ at some point, something like pathconf() and _PC_PATH_MAX would be more general. */

/*Wrapper if() statement. Use this as an easier method of checking if application is Orion-Native. Example:
	ONATIVEONLY{doThings();} */
#define ONATIVEONLY if(Orion::Application::isNativeOApp)

/* Global generic name for your OApp as opposed to individual OWindow titles. */
#define OAPP_NAME Orion::Application::name
/* Username of the user running your OApp. */
#define OAPP_USERNAME Orion::Application::username
/* Internal. Are OApp variables initialised? */
#define OAPP_INITED Orion::Application::inited
/* Boolean to decide if OKit should print debug messages to the terminal. To utilise this as a developer, see OVLog(). */
#define OAPP_VERBOSE Orion::Application::verbose
/* Is the application an Orion-Native OApp? See ONATIVEONLY. */
#define OAPP_NATIVE Orion::Application::isNativeOApp
/* The PID of your OApp. */
#define OAPP_PID Orion::Application::pid
/* Global UI scale for your OApp. See OAPP_SCALE. */
#define OAPP_SCALE Orion::Application::scale
/* The Current Working Directory; where the OApp was launched from. */
#define OAPP_CWD Orion::Application::cwd
/* Global system path to your OApp binary's exact location. */
#define OAPP_BINPATH Orion::Application::binpath
/* Global system directory where your OApp binary is located. */
#define OAPP_BINDIR Orion::Application::bindir
/* Global system path where OKit will store your data for this OApp. Use this instead of generic write functions! */
#define OAPP_DATAPATH Orion::Application::datapath

#endif /* !ORION_NOHELPERS */

namespace Orion{
	namespace Application{
		/* Internal. Are OApp variables initialised? */
		extern bool inited;
		/* Boolean to decide if OKit should print debug messages to the terminal. To utilise this as a developer, see OVLog(). */
		extern bool verbose;
		/* Global UI scale for your OApp. See OAPP_SCALE. */
		extern float scale;
		/* Global generic name for your OApp as opposed to individual OWindow titles. */
		extern const char* name;
		/* Username of the user running your OApp. */
		extern const char* username;

		/* Is the application an Orion-Native OApp? See ONATIVEONLY. */
		extern bool isNativeOApp;
		/* The PID of your OApp. See OAPP_PID. */
		extern pid_t pid;
		/* The Current Working Directory; where the OApp was launched from. */
		extern char* cwd;
		/* Global system path to your OApp binary's exact location. */
		extern char* binpath;
		/* Global system directory where your OApp binary is located. */
		extern char* bindir;
		/* Global system path where OKit will store your data for this OApp. Use this instead of generic write functions! */
		extern char* datapath;
		

		/* Internal. Sets up unitialised varibles for your OApp. Returns true if successful, false if not. */
		extern bool init(void);
	}
}

#endif /* !__ORION_OKIT_APPLICATION_H__ */
