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

#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include "include/Application.hpp"
#include "include/OSL/OSL.hpp"

#define _MKDIRARG ( S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH )

namespace Orion{
/* Definitions */
	namespace Application{
		const char* name=0;
		char* username=0;
		pid_t PID=0;
		float UIScale=1;
		namespace Storage{
			char* staticPath=0;
			char* libPath=0;
			char* userPath=0;
		}
		namespace Path{
			char* toHome=0;
			char* toBinary=0;
			char* toBinaryFolder=0;
			char* toCWD=0;
		}
		namespace Flags{
			bool isNativeOApp=0;
			bool isVerbose=0;
			bool isServiceRunning=0;
		}

/* Functions */
	void _initCWD(void){
		OAPP_CWD=(char*)malloc(sizeof(char)*OPATH_MAX);
		if(!getcwd(OAPP_CWD,OPATH_MAX)){ free(OAPP_CWD); OAPP_CWD=0; }
	}

	void _initPaths(void){
	/* toHome */
		char* env=getenv("HOME");
		if(env){ OAPP_HOME=env; }else{ OAPP_HOME=0; OVLog("ORIONAPI | WARNING! FAILED TO GET HOME FOLDER!\n"); }
	/* toBinary & toBinaryFolder */
		char proc[64];
		char path[OPATH_MAX];
		OFormat(proc,"/proc/%d/exe",OAPP_PID);
		ssize_t length=readlink(proc,path,OPATH_MAX-1);
		if(length!=-1){
			OAPP_BINPATH=(char*)malloc(sizeof(char)*OPATH_MAX);
			OAPP_BINDIR=(char*)malloc(sizeof(char)*OPATH_MAX);
			size_t dirEnd=OStringFindLast(path,"/");
			for(ssize_t i=0;i<length;i++){
				OAPP_BINPATH[i]=path[i];
				OAPP_BINDIR[i]=path[i];
			}
			OAPP_BINPATH[length]=0;
			if(dirEnd!=OSTRING_NOTFOUND){ OAPP_BINDIR[dirEnd]=0; }
		/* Orion-Native checking */
			if(OStringFindFirst(path,".oapp")!=OSTRING_NOTFOUND){ OAPP_NATIVE=true; }else{ OAPP_NATIVE=false; }
		}else{ OAPP_BINPATH=0; OAPP_BINDIR=0; OVLog("ORIONAPI | WARNING! FAILED TO GET BINARY PATH!\n");	}
	}

	void _initStorage(void){ /* TODO: Upgrade this using ODirectory after you've finished it. */
	/* Orion-Native */
		ONATIVEONLY{
			if(chdir(OAPP_BINDIR)==0){
			/* Create the storage Folders */
				mkdir("data",_MKDIRARG);
				mkdir("libs",_MKDIRARG);
				mkdir("static",_MKDIRARG);
			/* User-specific Folder */
				char tmp[OPATH_MAX];
				OFormat(tmp,"data/%s",OAPP_USERNAME);
				mkdir(tmp,_MKDIRARG);
				OAPP_DATAPATH=(char*)malloc(sizeof(char)*OPATH_MAX);
				OFormat(OAPP_DATAPATH,"%s/data/%s",OAPP_BINDIR,OAPP_USERNAME);
			/* Static data Folder */
				OAPP_STATICPATH=(char*)malloc(sizeof(char)*OPATH_MAX);
				OFormat(OAPP_STATICPATH,"%s/static",OAPP_BINDIR);
			/* Library Folder */
				OAPP_LIBPATH=(char*)malloc(sizeof(char)*OPATH_MAX);
				OFormat(OAPP_LIBPATH,"%s/libs",OAPP_BINDIR);
			}else{ OVLog("ORIONAPI | WARNING! FAILED TO ACCESS:\t\t\t%s !\n",OAPP_BINDIR); }
	/* Non-Orion-Native */
		}else{
			if(OAPP_NAME){				
				char path[OPATH_MAX];
				OFormat(path,"%s/.local/%s",OAPP_HOME,OAPP_NAME);
				mkdir(path,_MKDIRARG);
				if(chdir(path)==0){
				/* Create the storage Folders */
					mkdir("data",_MKDIRARG);
					mkdir("libs",_MKDIRARG);
					mkdir("static",_MKDIRARG);
				/* User-specific Folder - Since this is in the user's /home/ directory, we don't need a substructure. */
					OAPP_DATAPATH=(char*)malloc(sizeof(char)*OPATH_MAX);
					OFormat(OAPP_DATAPATH,"%s/data",path);
				/* Static data Folder */
					OAPP_STATICPATH=(char*)malloc(sizeof(char)*OPATH_MAX);
					OFormat(OAPP_STATICPATH,"%s/static",path);
				/* Library Folder */
					OAPP_LIBPATH=(char*)malloc(sizeof(char)*OPATH_MAX);
					OFormat(OAPP_LIBPATH,"%s/libs",path);
					OVLog("ORIONAPI | WARNING! NON-NATIVE APPLICATIONS WILL NOT HAVE A STATIC DATA DIRECTORY!\n");
				}else{ OVLog("ORIONAPI | WARNING! FAILED TO ACCESS:\t\t\t%s !\n",path); }
			}else{ OVLog("ORIONAPI | Application name not set, and is not Orion-Native. Not creating data directories."); }
		}
	}

	/* Initing */
		bool init(const char* appname){
			if(OAPP_RUNNING){ return false; }
			char* tmp;
			name=appname;
			username=getlogin();
			PID=getpid();
			_initCWD();
		/* Overrides */
			tmp=getenv("O_VERBOSE");
			if(tmp){ ( (atoi(tmp)>=1) ? OAPP_VERBOSE=true : OAPP_VERBOSE=false ); }
			else{ OAPP_VERBOSE=false; }
			if(OAPP_VERBOSE){
				OLog("ORIONAPI | Initialising Application.\n");
				OLog("ORIONAPI | Running in verbose mode!\n");
				OLog("ORIONAPI | Application Name:\t\t%s\n",OAPP_NAME);
				OLog("ORIONAPI | Username:\t\t\t%s\n",OAPP_USERNAME);
				OLog("ORIONAPI | Current Directory:\t\t%s\n",OAPP_CWD);
			}
			tmp=getenv("O_SCALE");
			if(tmp){
				if(atof(tmp)>=0.5f){ OAPP_SCALE=atof(tmp); }else{ OAPP_SCALE=0.5f; }
				OVLog("ORIONAPI | OVERRIDE | Global scale is now %.2f instead of %.2f.\n",OAPP_SCALE,1.0f);
			}
		/* Paths & Storage */
			_initPaths();
			_initStorage();
			if(OAPP_VERBOSE){
				OLog("ORIONAPI | Full Binary path is:\t\t%s\n",OAPP_BINPATH);
				OLog("ORIONAPI | Full Binary directory is:\t%s\n",OAPP_BINDIR);
				OLog("ORIONAPI | Data path is:\t\t%s\n",OAPP_DATAPATH);
				OLog("ORIONAPI | Static path is:\t\t%s\n",OAPP_STATICPATH);
				OLog("ORIONAPI | Is Native OApp?\t\t%s\n",(OAPP_NATIVE ? "true" : "false"));
			}
		/* Finalising */
			if(chdir(OAPP_CWD)!=0){ OVLog("ORIONAPI | WARNING! FAILED TO CHANGE DIRECTORY BACK TO CWD!\n"); }
			return true;
		}

	/* Freeing */
		bool wipe(void){
			if(!OAPP_RUNNING){ return false; }
			name=0,username=0; /* These two are not free-able. */
			PID=0,UIScale=1;
		/* Storage */
			if(OAPP_STATICPATH) { free(OAPP_STATICPATH); OAPP_STATICPATH=0; }
			if(OAPP_LIBPATH)    { free(OAPP_LIBPATH); OAPP_LIBPATH=0; }
			if(OAPP_DATAPATH)   { free(OAPP_DATAPATH); OAPP_DATAPATH=0; }
		/* Path */
			OAPP_HOME=0;
			if(OAPP_BINPATH)    { free(OAPP_BINPATH); OAPP_BINPATH=0; }
			if(OAPP_BINDIR)     { free(OAPP_BINDIR); OAPP_BINDIR=0; }
			if(OAPP_CWD)        { free(OAPP_CWD); OAPP_CWD=0; }
		/* Flags */
			OAPP_NATIVE=0;
			OAPP_VERBOSE=0;
			return true;
		}
	}
}
