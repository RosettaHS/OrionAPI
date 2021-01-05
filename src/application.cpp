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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "include/application.hpp"

/*Might wanna change this soon once you know what you're actually doing.*/
#define _MKDIRARG ( S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH )

/*I know you're going to cry looking at this, but, it works.*/

namespace Orion{
	namespace Application{
		bool inited=false;
		bool verbose=false;
		bool errorsFatal=false;
		float scale=1.0f;
		const char* name=0;
		const char* username=0;
		void setName(const char* _name){name=_name;}
		const char* getName(){return name;}

		bool isNativeOApp=false;
		pid_t pid=0;
		const char* cwd=0;
		const char* binpath=0;
		const char* bindir=0;
		const char* datapath=0;

		static void _initCWD(){
			const char* tmp=get_current_dir_name();
			if(tmp){
				cwd=tmp;
			}else{cwd=0;}
		}

		static void _initBinPathAndDir(){
			char tmp[64];
			char path[OPATH_MAX];
			
			sprintf(tmp,"/proc/%d/exe",pid);
			int length=readlink(tmp,path,OPATH_MAX-1);
			if(length!=-1){path[length]='\0';}else{
				binpath=0;
				printf("OKIT | ERROR! Failed to get binary path!\n");
				return;
			}
			binpath=(const char*)malloc(OPATH_MAX);
			memcpy((void*)binpath,path,OPATH_MAX);
			
			int place=0; /*Now I know this looks bad.... and that's because it is.*/
			for(int i=0;i<length;i++){
				if(path[i]=='/'){place=i;} /*Dumb as hell, but this finds where the directory ends...*/
			}
			path[place]='\0';
			bindir=(const char*)malloc(OPATH_MAX);
			memcpy((void*)bindir,path,OPATH_MAX);
		}

		static void _initOAppCheck(){
			char extension[6];
			if(chdir(bindir)==0){
				int length=strlen(bindir);
				for(int i=length-5;i<length;i++){
					extension[i-length+5]=bindir[i];
				}
				extension[5]='\0';

				if(strcmp(extension,".oapp")==0 /*What?*/ ){isNativeOApp=true;}else{isNativeOApp=false;}
			}else{isNativeOApp=false;printf("OKIT | ERROR! FAILED TO CHANGE TO BINARY PATH %s\n",bindir);}
		}

		static void _initDataPath(){
			char tmp[64];
			char path[OPATH_MAX];
			ONATIVEONLY{
				datapath=(const char*)malloc(OPATH_MAX);
				if(chdir(bindir)==0){
					sprintf(tmp,"data/%s",username);
					mkdir("data",_MKDIRARG);
					mkdir(tmp,_MKDIRARG);
					sprintf((char*)datapath,"%s/data/%s",binpath,username);
				}else{printf("OKIT | ERROR! FAILED TO ACCESS:\t\t\t%s !\n",bindir);}
			}else{
				if(name){
					datapath=(const char*)malloc(OPATH_MAX);
					const char*_env=getenv("HOME");
					sprintf(path,"%s/.local/share",_env);
					if(chdir(path)==0){
						sprintf(tmp,"%s",name);
						mkdir(tmp,_MKDIRARG);
						sprintf((char*)datapath,"%s/.local/share/%s",_env,name);
					}else{printf("OKIT | ERROR! FAILED TO ACCESS:\t\t\t%s/.local/share !\n",_env);}
				}else{OVERB_OUT "OKIT | Application name not set, and Application is not Orion-Native. Not creating data folder.\n" OVERB_END}
			}
		}
	
		bool init(){
			if(inited){return false;}
		/*Sets up overrides.*/
			pid=getpid();
			_initCWD();
			username=getenv("USERNAME");
			const char* _env=getenv("O_VERBOSE");
			if(_env){
				if(atoi(_env)>=1){verbose=true;}
			}
			_env=getenv("O_FATALERRORS");
			if(_env){
				if(atoi(_env)>=1){errorsFatal=true;}
			}
			
			OVERB_OUT "OKIT | Initialising Application.\n" OVERB_END
			OVERB_OUT "OKIT | Running in verbose mode!\n" OVERB_END
			OVERB_OUT "OKIT | Application Name:\t\t%s\n",name OVERB_END
			OVERB_OUT "OKIT | Username:\t\t\t%s\n",username OVERB_END
			OVERB_OUT "OKIT | Current Directory:\t\t%s\n",cwd OVERB_END	
			OVERB_OUT "OKIT | Are Errors Fatal?:\t\t%s\n",(errorsFatal ? "true" : "false") OVERB_END

			_env=getenv("O_SCALE");
			if(_env){
				if(atof(_env)>=0.5f){
					scale=atof(_env);
				}else{scale=0.5f;}
				OVERB_OUT "OKIT | OVERRIDE | Global scale is now %.2f instead of %.2f.\n",scale,1.0f OVERB_END
			}
		/*Sets up paths.*/
			_initBinPathAndDir();
			_initOAppCheck();
			_initDataPath();

			OVERB_OUT "OKIT | Full Binary path is:\t\t%s\n",binpath OVERB_END
			OVERB_OUT "OKIT | Full Binary directory is:\t%s\n",bindir OVERB_END

			OVERB_OUT "OKIT | Data path is:\t\t\t%s\n",datapath OVERB_END

			OVERB_OUT "OKIT | Is Native OApp?\t\t\t%s\n",(isNativeOApp ? "true" : "false") OVERB_END
		return true;}
	}
}