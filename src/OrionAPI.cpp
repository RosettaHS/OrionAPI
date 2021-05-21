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
#include "include/OrionAPI.hpp"

namespace Orion{

	static OTheme OTHEME_FALLBACK;

	static bool CAppSetThemeFromSystem(void){
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
				if     ( OStringFindFirst(line,"[PRIMARY]")!=OSTRING_NOTFOUND )   { if(!OAPP_THEME.primary.setTo(line))   { OAPP_THEME.primary=OTHEME_FALLBACK.primary; }     continue; }
				else if( OStringFindFirst(line,"[SECONDARY]")!=OSTRING_NOTFOUND ) { if(!OAPP_THEME.secondary.setTo(line)) { OAPP_THEME.secondary=OTHEME_FALLBACK.secondary; } continue; }
				else if( OStringFindFirst(line,"[TERTIARY]")!=OSTRING_NOTFOUND )  { if(!OAPP_THEME.tertiary.setTo(line))  { OAPP_THEME.tertiary=OTHEME_FALLBACK.tertiary; }   continue; }
				else if( OStringFindFirst(line,"[ACCENT]")!=OSTRING_NOTFOUND )    { if(!OAPP_THEME.accent.setTo(line))    { OAPP_THEME.accent=OTHEME_FALLBACK.accent; }       continue; }
			}
			return true;
		}
	/* On total failure, set it to fallback and return false. */
		OAPP_THEME=OTHEME_FALLBACK;
		return false;
	}


	bool OAppStart(const char* AppName, const char* AppIdentifier, bool ForceONative, bool headless){
		if(OAPP_RUNNING){ return false; }
	/* First make sure the names and identifiers are valid. */
		if(AppName){
			size_t i=0;
			while(AppName[i]!=0){
				if(AppName[i]==' '){ OERROR(OERR_INVALIDAPPNAME,true,"APPLICATION NAMES CANNOT HAVE SPACES!"); }
				i++;
			}
		}
		if(AppIdentifier){
			size_t i=0;
			while(AppIdentifier[i]!=0){
				if(AppIdentifier[i]==' '){ OERROR(OERR_INVALIDAPPID,true,"APPLICATION IDENTIFIERS CANNOT HAVE SPACES!"); }
				i++;
			}
		}
	/* If everything checks out, continue the initialisation process.. */
		CAppInit(AppName,AppIdentifier);
		OAPP_HEADLESS=headless;
		if(ForceONative){
			if(!OAPP_NATIVE){
				OERROR(OERR_NOTNATIVE,true,"SERVICE FORCED AS ORION-NATIVE, BUT APPLICATION EXECUTED AS STANDALONE BINARY!");
			}
			if(!OAPP_BINPATH || !OAPP_BINDIR|| !OAPP_DATAPATH ||
			   !OAPP_STATICPATH || !OAPP_LIBPATH ||! OAPP_INTPATH){
				OERROR(OERR_NOTNATIVE,true,"SERVICE FORCED AS ORION-NAITVE, BUT ONE OR MORE OF THE REQUIRED PATHS HAS FAILED TO INITIALISE!");
			}
		}
		CAppSetThemeFromSystem();
		/* TODO: Add more stuff here once you've introduced most of OKit. */
		if(!OAPP_HEADLESS){
			if(!XCB_CONNECT() && ForceONative){
				OERROR(OERR_NOTNATIVE,true,"SERVICE FORCED AS ORION-NAITVE WITH HEADLESS MODE OFF, BUT ORIONAPI COULD NOT ESTABLISH A CONNECTION TO THE X SERVICE!");
			}
			CXHA_INIT();
		}
		OAPP_RUNNING=true;
		return true;
	}

	bool OAppEnd(void){
		if(!OAPP_RUNNING){ return false; }
		CAppWipe();
		/* TODO: Add more stuff here once you've introduced most of OKit. */
		if(!OAPP_HEADLESS){
			XCB_DISCONNECT();
			CXHA_FREE();
		}
		OAPP_RUNNING=false;
		return true;
	}
}