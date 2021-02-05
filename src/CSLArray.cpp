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

#include <stdlib.h>
#include "include/errdef.h"
#include "include/OLog.hpp"
#include "include/CSLArray.hpp"

namespace Orion{
	CSLArray::~CSLArray(void){ if(arr){free(arr);} }
	CSLArray::CSLArray(void) : arr{0},count{0},cap{0},step{0} {}

	bool CSLArray::init(unsigned short _cap, unsigned char _step){
		count=0,cap=_cap,step=_step;
		arr=(CSignalListener*)malloc(sizeof(CSignalListener)*_cap);
		if(arr){
			for(unsigned short i=0;i<cap;i++){
				arr[i].clear();
			}
		}else{
			OLog("OKIT | ERROR! FAILED TO ALLOCATE MEMORY FOR CSLARRAY!\n");
			exit(OERR_CANTMALLOC);
		}
		return false;
	}

	bool CSLArray::resize(unsigned short newSize){
		if(arr){
			arr=(CSignalListener*)realloc(arr,sizeof(CSignalListener)*newSize);
			if(arr){
				return true;
			}else{
				OLog("OKIT | ERROR! FAILED TO RESIZE ALLOCATED MEMORY FOR CSLARRAY!\n");
				exit(OERR_CANTMALLOC);
			}
		}
		return false;
	}

	bool CSLArray::connect(CSignalListener& listener){
		if(!arr){return false;}
		if(count+1>=cap){ resize(cap+step); }
		for(unsigned short i=0;i<cap;i++){
			/* An empty SignalListener is just as useful as no SignalListener. Override. */
			if(arr[i].mask==0){
				arr[i]=listener;
				count++;
				return true;
			}else if(arr[i].compare(listener)){ /* This checks if you're linking a function that is already linked, but now with a different mask. */
				/* Just appends the new mask to the current mask. */
				arr[i].mask|=~listener.mask;
				return true;
			}
		}
		return false;
	}

	bool CSLArray::disconnect(CSignalListener& listener){
		if(!arr){return false;}
		for(unsigned short i=0;i<cap;i++){
			if(arr[i].compare(listener)){
				arr[i].mask&=listener.mask;
				if(!arr[i].mask){
					arr[i].clear();
					count--;
					return true;
				}
				return true;
			}
		}
		return false;
	}
}