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

#include "include/OLog.hpp"
#include <stdlib.h>
#include "include/errdef.h"
#include "include/application.hpp"
#include "include/xservice.hpp"
#include "include/OTheme.hpp"
#include "include/CContext.hpp"
#include "include/OService.hpp"


namespace Orion{
	static bool _setThemeFromSystem(){
		return false; /* Add proper support here! */
	}

	static void _setThemeToFallback(){
		OTHEME_PRIMARY.setTo(30,25,25);
		OTHEME_SECONDARY.setTo(40,35,35);
		OTHEME_TERTIARY.setTo(25,15,15);
		OTHEME_ACCENT.setTo(255,86,15);
	}

	bool OKitStart(const char* name,bool ForceONative){
		if(OAPP_INITED){return false;}
		if(name){Application::setName(name);}
		Application::init();
		if(ForceONative){
			if(!OAPP_NATIVE){OLog("OKIT | ERROR! SERVICE FORCED AS ORION-NATIVE, BUT APPLICATION EXECUTED AS STANDALONE BINARY!\n");exit(OERR_NOT_NATIVE_OAPP);}
			if(OAPP_BINPATH==0||OAPP_BINDIR==0||OAPP_DATAPATH==0){OLog("OKIT | ERROR! SERVICE FORCED AS ORION-NAITVE, BUT ONE OR MORE OF THE REQUIRED PATHS HAS FAILED TO INITIALISE!\n");exit(OERR_NOT_NATIVE_OAPP);}
		}
		X::connect();
		X::CXHA_INIT();
		if(!_setThemeFromSystem()){_setThemeToFallback();}
		OVLog("OKIT | Service sucessfully initialised!\n\n");
		OAPP_INITED=true;
		return true;
	}

	bool OKitEventLoop(void){
		if(!OAPP_INITED){return false;}
		X::eventLoop(); /* TODO: Actually add something to this!! */
		X::disconnect();
		OVLog("\nOKIT | Service sucessfully finished!\n\n");
		return true;
	}
}