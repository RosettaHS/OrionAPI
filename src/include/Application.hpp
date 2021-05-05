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

/* Orion implementation for maximum amount of characters allowed in a directory path. */
#define OPATH_MAX 1024

#ifndef ORION_NOHELPERS

	/* Wrapper if() statement for doing operations based on if the Application is in an Orion-Native Application Architecture. */
	#define ONATIVEONLY         if(Orion::OApp.Flags.isNativeOApp)
	/* An instance of an OAppInfo struct describing this OApp. */
	#define OAPP                (Orion::OApp)
	/* The name of this OApp. */
	#define OAPP_NAME           (Orion::OApp.name)
	/* The unique identifier of this OApp. */
	#define OAPP_IDENTIFIER     (Orion::OApp.identifier)
	/* The name of the User running this OApp. */
	#define OAPP_USERNAME       (Orion::OApp.username)
	/* The process identifier of this OApp. */
	#define OAPP_PID            (Orion::OApp.pid)

	/* A Struct containing information on this OApp's Interaface. */
	#define OAPP_INTERFACE      (Orion::OApp.Interface)
	/* The global scale of the UI of this OApp. */
	#define OAPP_SCALE          (Orion::OApp.Interface.scale)
	/* A pointer to the UI Element that has the global focus of this OApp. */
	#define OAPP_FOCUSEDELEMENT (Orion::OApp.Interface.focusedElement)
	/*
	 * A dereferenced pointer (Reference) to the OTheme struct that contains the theme colours of this OApp.
	 * To access it in pointer form, use OAPP_INTERFACE.theme
	 */
	#define OAPP_THEME          (*Orion::OApp.Interface.theme)

	/* A Struct containing Storage Directories for this OApp determined by OrionAPI. */
	#define OAPP_STORAGE        (Orion::OApp.Storage)
	/* A path to a Directory where this OApp can store persistent Files irregardless of the User running it. */
	#define OAPP_STATICPATH     (Orion::OApp.Storage.staticPath)
	/* A path to a Directory where this OApp stores the library files required for use. */
	#define OAPP_LIBPATH        (Orion::OApp.Storage.libPath)
	/* A path to a Directory where this OApp can store persistent files dependent on the User running it. */
	#define OAPP_DATAPATH       (Orion::OApp.Storage.dataPath)

	#ifdef ORION_INTERNAL
		/* Internal. A path to a Directory where OrionAPI can store persistent files for this OApp. */
		#define OAPP_INTPATH    (Orion::OApp.Storage.internalPath)
	#endif /* !ORION_INTERNAL */

	/* A Struct containing generic paths determined by OrionAPI. */
	#define OAPP_PATH           (Orion::OApp.Path)
	/* The path to this OApp's binary. */
	#define OAPP_BINPATH        (Orion::OApp.Path.toBinary)
	/* The path to the folder where this OApp's binary is stored. */
	#define OAPP_BINDIR         (Orion::OApp.Path.toBinaryFolder)
	/* The path to the User's /home/ Folder. */
	#define OAPP_HOME           (Orion::OApp.Path.toHome)
	/* The path to the Working Directory of this OApp. */
	#define OAPP_CWD            (Orion::OApp.Path.toCWD)

	/* A Struct containing various flags related to this OApp. */
	#define OAPP_FLAGS          (Orion::OApp.Flags)
	/* Is this OApp in an Orion-Native File structure? */
	#define OAPP_NATIVE         (Orion::OApp.Flags.isNativeOApp)
	/* Is this OApp running in Verbose Mode? */
	#define OAPP_VERBOSE        (Orion::OApp.Flags.isVerbose)
	/* Is this OApp running and initialised? */
	#define OAPP_RUNNING        (Orion::OApp.Flags.isRunning)

#endif /* !ORION_NOHELPERS */

namespace Orion{

/** Forward Declarations **/

	/* The base class for all OrionUI Elements. */
	class  OWidget;

	/* Container struct for multiple OCols. Used for theming UI elements. */
	struct OTheme;

	/* A Struct containing information regarding a specific OApp. */
	struct OAppInfo{
		/* The name of the OApp. */
		const char* name;
		/* The unique identifier of the OApp. */
		const char* identifier;
		/* The name of the User running the OApp. */
		const char* username;
		/* The process identifier of the OApp. */
		pid_t       pid;
		struct{
			/* The global scale of the UI of the OApp. */
			float    scale;
			/* A pointer to the UI Element that has the global focus of the OApp. */
			OWidget* focusedElement;	
			/* A pointer to the OTheme struct that contains the theme colours of the OApp. */
			OTheme*  theme;
		}Interface; /* A Struct containing information on the OApp's Interaface. */
		struct{
			/* A path to a Directory where the OApp can store persistent Files irregardless of the User running it. */
			char* staticPath;
			/* A path to a Directory where the OApp stores the library files required for use. */
			char* libPath;
			/* A path to a Directory where the OApp can store persistent files dependent on the User running it. */
			char* dataPath;
			/* Internal. A path to a Directory where OrionAPI can store persistent files for the OApp. */
			char* internalPath;
		}Storage; /* A Struct containing Storage Directories for the OApp determined by OrionAPI. */
		struct{
			/* The path to the OApp's binary. */
			char* toBinary;
			/* The path to the folder where the OApp's binary is stored. */
			char* toBinaryFolder;
			/* The path to the User's /home/ Folder. */
			char* toHome;
			/* The path to the Working Directory of the OApp. */
			char* toCWD;
		}Path; /* A Struct containing generic paths determined by OrionAPI. */
		struct{
			/* Is the OApp in an Orion-Native File structure? */
			bool isNativeOApp;
			/* Is the OApp running in Verbose Mode? */
			bool isVerbose;
			/* Is the OApp running and initialised? */
			bool isRunning;
		}Flags; /* A Struct containing various flags related to the OApp. */

		/* Empty constructor. Sets all values to 0. */
		inline OAppInfo(void) :
			name{0}, identifier{0}, username{0},pid{0}, Interface{1,0,0},
			Storage{0,0,0,0}, Path{0,0,0,0}, Flags{0,0,0} {}
	};

	/* An instance of an OAppInfo struct describing this OApp. */
	extern OAppInfo OApp;

#ifdef ORION_INTERNAL
	/* Internal. Initialises the OAppInfo struct for this OApp. */
	extern bool CAppInit(const char* name, const char* identifier);
	/* Internal. Clears the OAppInfo struct of this OApp and frees all memory. */
	extern bool CAppWipe(void);
#endif /* ORION_INTERNAL */
}

#endif /* !__ORIONAPI_APPLICATION_H__ */