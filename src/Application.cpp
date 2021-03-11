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
#include "include/OSL/OLog.hpp"
#include "include/OSL/OString.hpp"
#include "include/Application.hpp"

#define _MKDIRARG ( S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH )

namespace Orion{
	OAppInfo OApp;

/* Functions */

	/* Initialise the Current Working Directory. */
	static void CAppInitCWD(void){
		OAPP_CWD=(char*)malloc(sizeof(char)*OPATH_MAX);
		if(!getcwd(OAPP_CWD,OPATH_MAX)){ free(OAPP_CWD); OAPP_CWD=0; }
	}

	/* Initialise the Paths for this OApp. */
	static void CAppInitPaths(void){
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
		}
	/* Create the .orion and OIPC Folder in the user's /home/ directory */
	/* TODO: Do checks here to skip this step when on the Orion Operating System! */
		if(!chdir(OAPP_HOME)){
			mkdir(".orion",_MKDIRARG);
			mkdir(".orion/AppIDs",_MKDIRARG);
		}
	}

	/* Initialise the Storage Directories for this OApp. */
	static void CAppInitStorage(void){ /* TODO: Upgrade this using ODirectory after you've finished it. */
	/* Orion-Native */
		ONATIVEONLY{
			if(!chdir(OAPP_BINDIR)){
			/* Create the storage Folders */
				mkdir("data",_MKDIRARG);
				mkdir("libs",_MKDIRARG);
				mkdir("static",_MKDIRARG);
				mkdir(".SYS",_MKDIRARG);
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
			/* OrionAPI Internal */
				OAPP_INTPATH=(char*)malloc(sizeof(char)*OPATH_MAX);
				OFormat(OAPP_INTPATH,"%s/.SYS",OAPP_BINDIR);
			}else{ OVLog("ORIONAPI | WARNING! FAILED TO ACCESS:\t\t\t%s !\n",OAPP_BINDIR); }
	/* Non-Orion-Native */
		}else{
			if(OAPP_NAME){				
				char path[OPATH_MAX];
				OFormat(path,"%s/.local/share/%s",OAPP_HOME,OAPP_NAME);
				mkdir(path,_MKDIRARG);
				if(!chdir(path)){
				/* Create the storage Folders */
					mkdir(".libs",_MKDIRARG);
					mkdir(".static",_MKDIRARG);
					mkdir(".SYS",_MKDIRARG);
				/* User-specific Folder - Since this is in the user's /home/ directory, we don't need a substructure. */
					OAPP_DATAPATH=(char*)malloc(sizeof(char)*OPATH_MAX);
					OFormat(OAPP_DATAPATH,"%s",path);
				/* Static data Folder */
					OAPP_STATICPATH=(char*)malloc(sizeof(char)*OPATH_MAX);
					OFormat(OAPP_STATICPATH,"%s/.static",path);
					OVLog("ORIONAPI | WARNING! NON-NATIVE APPLICATIONS WILL NOT HAVE A PROPER STATIC DATA DIRECTORY!\n");
				/* Library Folder */
					OAPP_LIBPATH=(char*)malloc(sizeof(char)*OPATH_MAX);
					OFormat(OAPP_LIBPATH,"%s/.libs",path);
				/* OrionAPI Internal */
					OAPP_INTPATH=(char*)malloc(sizeof(char)*OPATH_MAX);
					OFormat(OAPP_INTPATH,"%s/.SYS",OAPP_BINDIR);
				}else{ OVLog("ORIONAPI | WARNING! FAILED TO ACCESS:\t\t\t%s !\n",path); }
			}else{ OVLog("ORIONAPI | Application name not set, and is not Orion-Native. Not creating data directories."); }
		}
	}

/* Final */
	bool CAppInit(const char* name, const char* identifier){
		if(OAPP_RUNNING){ return false; }
		char* tmp;
		OAPP_NAME=name;
		OAPP_IDENTIFIER=identifier;
		OAPP_USERNAME=getlogin();
		OAPP_PID=getpid();
		CAppInitCWD();
	/* Overrides */
		tmp=getenv("O_VERBOSE");
		if(tmp){ ( (atoi(tmp)>=1) ? OAPP_VERBOSE=true : OAPP_VERBOSE=false ); }
		else{ OAPP_VERBOSE=false; }
		if(OAPP_VERBOSE){
			OLog("ORIONAPI | Initialising Application\n");
			OLog("ORIONAPI | ------Application Information------\n");
			OLog("ORIONAPI | Application Name:\t\t%s\n",OAPP_NAME);
			OLog("ORIONAPI | Application OID:\t\t%s\n",OAPP_IDENTIFIER);
			OLog("ORIONAPI | Application PID:\t\t%lu\n",OAPP_PID);
			OLog("ORIONAPI | Username:\t\t\t%s\n",OAPP_USERNAME);
			OLog("ORIONAPI | Current Directory:\t\t%s\n",OAPP_CWD);
		}
		tmp=getenv("O_SCALE");
		if(tmp){
			float prev=OAPP_SCALE;
			if(atof(tmp)>=0.5f){ OAPP_SCALE=atof(tmp); }else{ OAPP_SCALE=0.5f; }
			OVLog("ORIONAPI | OVERRIDE | Global scale is now %.2f instead of %.2f.\n",OAPP_SCALE,prev);
		}
	/* Paths & Storage */
		CAppInitPaths();
		CAppInitStorage();
		if(OAPP_VERBOSE){
			OLog("ORIONAPI | --------Storage Information--------\n");
			OLog("ORIONAPI | Full Binary path is:\t\t%s\n",OAPP_BINPATH);
			OLog("ORIONAPI | Binary Parent Folder is:\t%s\n",OAPP_BINDIR);
			OLog("ORIONAPI | Data path is:\t\t%s\n",OAPP_DATAPATH);
			OLog("ORIONAPI | Static path is:\t\t%s\n",OAPP_STATICPATH);
			OLog("ORIONAPI | Library path is:\t\t%s\n",OAPP_LIBPATH);
			OLog("ORIONAPI | Internal path is:\t\t%s\n",OAPP_INTPATH);
			OLog("ORIONAPI | Is Native OApp?\t\t%s\n",(OAPP_NATIVE ? "true" : "false"));
		}
	/* Finalising */
		if(chdir(OAPP_CWD)!=0){ OVLog("ORIONAPI | WARNING! FAILED TO CHANGE DIRECTORY BACK TO CWD!\n"); }
		OAPP_RUNNING=true;
		return true;
	}

/* Freeing */
	bool CAppWipe(void){
		if(!OAPP_RUNNING){ return false; }
		OAPP_NAME=0;
		OAPP_IDENTIFIER=0;
		OAPP_USERNAME=0;
		OAPP_PID=0;
		OAPP_SCALE=1;
	/* Storage */
		if(OAPP_STATICPATH) { free(OAPP_STATICPATH); OAPP_STATICPATH=0; }
		if(OAPP_LIBPATH)    { free(OAPP_LIBPATH);    OAPP_LIBPATH=0; }
		if(OAPP_DATAPATH)   { free(OAPP_DATAPATH);   OAPP_DATAPATH=0; }
	/* Path */
		OAPP_HOME=0;
		if(OAPP_BINPATH)    { free(OAPP_BINPATH);    OAPP_BINPATH=0; }
		if(OAPP_BINDIR)     { free(OAPP_BINDIR);     OAPP_BINDIR=0; }
		if(OAPP_CWD)        { free(OAPP_CWD);        OAPP_CWD=0; }
		if(OAPP_INTPATH)    { free(OAPP_INTPATH);    OAPP_INTPATH=0; }
	/* Flags */
		OAPP_NATIVE=0;
		OAPP_VERBOSE=0;
		OAPP_RUNNING=false;
		return true;
	}
}