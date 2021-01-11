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

/*Orion implementation for maximum amount of characters allowed in a directory path.*/
/* OPATH_MAX is so tiny.  256?  1024 is another common minimum.  Of course, using pathconf() and _PC_PATH_MAX would be more general. */
#define OPATH_MAX 1024

/* Wrapper if() statement for exiting upon fatal errors. Include <stdlib.h>! */
#define OERR_EXIT(errcode) if(Orion::Application::errorsFatal){exit(errcode);}
/*Wrapper if() statement. Use this as an easier method of checking if application is Orion-Native. Example:
	ONATIVEONLY{doThings();} */
#define ONATIVEONLY if(Orion::Application::isNativeOApp)
/* Wrapper if() statements for printing debug information to the terminal. Include <stdio.h>! */
#define OVERB_OUT if(Orion::Application::verbose){printf(
#define OVERB_END );}

/*Quick re-route for Orion::Application::inited.*/
#define OAPP_INITED Orion::Application::inited
/*Quick re-route for Orion::Application::verbose.*/
#define OAPP_VERBOSE Orion::Application::verbose
/*Quick re-route for Orion::Application::errorsFatal.*/
#define OAPP_FATALERRORS Orion::Application::errorsFatal
/*Quick re-route for Orion::Application::isNativeOApp.*/
#define OAPP_NATIVE Orion::Application::isNativeOApp
/*Quick re-route for Orion::Application::pid.*/
#define OAPP_PID Orion::Application::pid
/*Quick re-route for Orion::Application::scale.*/
#define OAPP_SCALE Orion::Application::scale
/*Quick re-route for Orion::Application::cwd.*/
#define OAPP_CWD Orion::Application::cwd
/*Quick re-route for Orion::Application::binpath.*/
#define OAPP_BINPATH Orion::Application::binpath
/*Quick re-route for Orion::Application::bindir.*/
#define OAPP_BINDIR Orion::Application::bindir
/*Quick re-route for Orion::Application::datapath.*/
#define OAPP_DATAPATH Orion::Application::datapath

namespace Orion{
	namespace Application{
		/* Internal. Are variables initialised? See OAPP_INITED. */
		extern bool inited;
		/* Boolean to decide if OKit should print debug messages to the terminal. To utilise this as a developer, see OVERB_OUT and OVERB_END macros. */
		extern bool verbose;
		/* Boolean to decide if OKit should exit if it encounters an error. */
		extern bool errorsFatal;
		/* Global UI scale for your OApp. See OAPP_SCALE. */
		extern float scale;
		/* Global generic name for your OApp as opposed to individual OWindow titles. */
		extern const char* name;
		/* Username of the user running your OApp. */
		extern const char* username;
		/* Sets the global generic name for your OApp. */
		void setName(const char*);
		/* Gets the global generic name for your OApp. */
		const char* getName(void);

		/* Is the application an Orion-Native OApp? See ONATIVEONLY. */
		extern bool isNativeOApp;
		/* The PID of your OApp. See OAPP_PID. */
		extern pid_t pid;
		/* Current Working Directory; where you launched the OApp. */
		extern char* cwd;
		/* Global system path to your OApp binary */
		extern char* binpath;
		/* Global system directory which your OApp binary is located. */
		extern char* bindir;
		/* Global system path where OKit will store your data. Use this instead of generic write functions! */
		extern char* datapath;
		

		/* Internal. Sets up unitialised varibles for your OApp. Returns true if successful, false if not. */
		extern bool init(void);
	}
}

#endif /* !__ORION_OKIT_APPLICATION_H__ */
