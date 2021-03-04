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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include "include/OrionAPI.hpp"

namespace Orion{

	static OTheme OTHEME_FALLBACK;

	static bool _setThemeFromSystem(void){
	/* Setting the fallback. */
		OTHEME_FALLBACK.primary.setTo(30,25,25);
		OTHEME_FALLBACK.secondary.setTo(40,35,35);
		OTHEME_FALLBACK.tertiary.setTo(25,15,15);
		OTHEME_FALLBACK.accent.setTo(255,86,15);
	/* Generate the potential path for the ThemeFile. */
		char themepath[OPATH_MAX];
		OFile themefile;
		OFormat(themepath,"%s/.orion/theme",getenv("HOME"));
	/* Check if ThemeFile exists. */
		if( themefile.open(themepath,OFILE_OPEN_READONLY) ){
	/* Run through the Lines of the ThemeFile and attempt to set the OApp's Theme. */
			for(size_t i=0;i<themefile.getLineCount();i++){
				char* line=themefile[i];
				if     ( OStringFindFirst(line,"[PRIMARY]")!=OSTRING_NOTFOUND )   { if(!OTHEME.primary.setTo(line)){ OTHEME.primary=OTHEME_FALLBACK.primary; } continue; }
				else if( OStringFindFirst(line,"[SECONDARY]")!=OSTRING_NOTFOUND ) { if(!OTHEME.secondary.setTo(line)){ OTHEME.secondary=OTHEME_FALLBACK.secondary; } continue; }
				else if( OStringFindFirst(line,"[TERTIARY]")!=OSTRING_NOTFOUND )  { if(!OTHEME.tertiary.setTo(line)){ OTHEME.tertiary=OTHEME_FALLBACK.tertiary; } continue; }
				else if( OStringFindFirst(line,"[ACCENT]")!=OSTRING_NOTFOUND )    { if(!OTHEME.accent.setTo(line)){ OTHEME.accent=OTHEME_FALLBACK.accent; } continue; }
			}
			return true;
		}
	/* On total failure, set it to fallback and return false. */
		OTHEME=OTHEME_FALLBACK;
		return false;
	}

	bool OAppStart(const char* name,bool ForceONative){
		if(OAPP_INITED){return false;}
		if(name){OAPP_NAME=name;}
		Application::init();
		if(ForceONative){
			if(!OAPP_NATIVE){OLog("ORIONAPI | ERROR! SERVICE FORCED AS ORION-NATIVE, BUT APPLICATION EXECUTED AS STANDALONE BINARY!\n");exit(OERR_NOT_NATIVE_OAPP);}
			if(OAPP_BINPATH==0||OAPP_BINDIR==0||OAPP_DATAPATH==0){OLog("ORIONAPI | ERROR! SERVICE FORCED AS ORION-NAITVE, BUT ONE OR MORE OF THE REQUIRED PATHS HAS FAILED TO INITIALISE!\n");exit(OERR_NOT_NATIVE_OAPP);}
		}
		X::connect();
		X::CXHA_INIT();
		_setThemeFromSystem();
		OVLog("ORIONAPI | Service sucessfully initialised!\n\n");
		OAPP_INITED=true;
		return true;
	}

	bool OAppEnd(void){
		if(!OAPP_INITED){return false;}
		X::eventLoop(); /* TODO: Actually add something to this!! */
		X::disconnect();
		X::CXHA_DESTROY();
		OAPP_INITED=false;
		OVLog("\nORIONAPI | Service sucessfully finished!\n\n");
		return true;
	}

	OVec OScreenGetSize(void){
		OVec v;
		XWindowAttributes attr;
		XGetWindowAttributes(OXDPY,OXROOT,&attr);
		v.x=attr.width,v.y=attr.height;
		return v;
	}
}
