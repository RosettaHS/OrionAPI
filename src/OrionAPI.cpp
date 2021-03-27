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

#include "include/OrionAPI.hpp"

namespace Orion{

	bool OAppStart(const char* AppName, const char* AppIdentifier,bool ForceONative){
		if(OAPP_RUNNING){ return false; }
	/* First make sure the names and identifiers are valid. */
		if(AppName){
			size_t i=0;
			while(AppName[i]!=0){
				if(AppName[i]==' '){ OELog(OERR_INVALIDAPPNAME,true,"APPLICATION NAMES CANNOT HAVE SPACES!\n"); }
				i++;
			}
		}
		if(AppIdentifier){
			size_t i=0;
			while(AppIdentifier[i]!=0){
				if(AppIdentifier[i]==' '){ OELog(OERR_INVALIDAPPID,true,"APPLICATION IDENTIFIERS CANNOT HAVE SPACES!\n"); }
				i++;
			}
		}
	/* If everything checks out, continue the initialisation process.. */
		CAppInit(AppName,AppIdentifier);
		if(ForceONative){
			if(!OAPP_NATIVE){
				OELog(OERR_NOTNATIVE,true,"SERVICE FORCED AS ORION-NATIVE, BUT APPLICATION EXECUTED AS STANDALONE BINARY!\n");
			}
			if(!OAPP_BINPATH || !OAPP_BINDIR|| !OAPP_DATAPATH ||
			   !OAPP_STATICPATH || !OAPP_LIBPATH ||! OAPP_INTPATH){
				OELog(OERR_NOTNATIVE,true,"SERVICE FORCED AS ORION-NAITVE, BUT ONE OR MORE OF THE REQUIRED PATHS HAS FAILED TO INITIALISE!\n");
			}
		}
		/* TODO: Add more stuff here once you've introduced most of OKit. */
		XCB_CONNECT();
		OAPP_RUNNING=true;
		return true;
	}

	bool OAppEnd(void){
		if(!OAPP_RUNNING){ return false; }
		CAppWipe();
		/* TODO: Add more stuff here once you've introduced most of OKit. */
		XCB_DISCONNECT();
		OAPP_RUNNING=false;
		return true;
	}
}