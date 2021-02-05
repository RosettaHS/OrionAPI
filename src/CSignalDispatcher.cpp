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
#include "include/CSignalDispatcher.hpp"

namespace Orion{
	bool CSignalDispatcher::connect(OMask& mask, void(*func)(void)){
		if(SIGMGR.arr){
			CSignalListener listener;
			listener.compose(func,mask);
			return SIGMGR.connect(listener);
		}else{ OLog("OKIT | WARNING! CAN'T CONNECT FUNCTION TO SIGNAL ON OBJECT THAT HAS NOT BEEN INITIALISED YET!\n"); }
		return false;
	}

	bool CSignalDispatcher::connect(OMask& mask, void(*func)(OSignal)){
		if(SIGMGR.arr){
			CSignalListener listener;
			listener.compose(func,mask);
			return SIGMGR.connect(listener);
		}else{ OLog("OKIT | WARNING! CAN'T CONNECT FUNCTION TO SIGNAL ON OBJECT THAT HAS NOT BEEN INITIALISED YET!\n"); }
		return false;
	}

	bool CSignalDispatcher::disconnect(OMask& mask, void(*func)(void)){
		if(SIGMGR.arr){
			CSignalListener listener;
			listener.compose(func,mask);
			return SIGMGR.disconnect(listener);
		}else{ OLog("OKIT | WARNING! CAN'T DISCONNECT FUNCTION FROM SIGNAL ON AN OBJECT THAT HAS NOT BEEN INITIALISED YET!\n"); }
		return false;
	}

	bool CSignalDispatcher::disconnect(OMask& mask, void(*func)(OSignal)){
		if(SIGMGR.arr){
			CSignalListener listener;
			listener.compose(func,mask);
			return SIGMGR.disconnect(listener);
		}else{ OLog("OKIT | WARNING! CAN'T DISCONNECT FUNCTION FROM SIGNAL ON AN OBJECT THAT HAS NOT BEEN INITIALISED YET!\n"); }
		return false;
	}
}