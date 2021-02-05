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
#include "include/errdef.h"
#include "include/application.hpp"
#include "include/CEventArray.hpp"

namespace Orion{
	CEvent::CEvent(void) : arr{0},count{0},cap{0} {}
	void CEvent::clear(void){
		if(arr){free(arr);}
		arr=0,count=0,cap=0;
	}

	CEventArray::~CEventArray(void){
		if(arr){
			for(unsigned short i=0;i<count;i++){
				arr[i].clear();
			}
			free(arr);
			arr=0;
		}
	}

	CEventArray::CEventArray(void) : arr{0},count{0},cap{0},step{0} {}

	bool CEventArray::resize(unsigned short newSize){
		if(!arr){return false;}
		arr=(CEvent*)realloc(arr,sizeof(CEvent)*newSize);
		if(!arr){printf("OKIT | ERROR! CEVENTARRAY CAN'T REALLOCATE MEMORY FOR AN ARRAY OF CEVENTS!\n");exit(OERR_CANTMALLOC);}
		cap=newSize;
		return true;
	}

	bool CEventArray::init(unsigned short _cap, unsigned char _step){
		if(arr){return false;}
		arr=(CEvent*)malloc(sizeof(CEvent)*_cap);
		if(arr){
			count=0,cap=_cap,step=_step;
		}else{
			printf("OKIT | ERROR! CEVENTARRAY CAN'T ALLOCATE MEMORY FOR AN ARRAY OF CEVENTS!\n");
			exit(OERR_CANTMALLOC);
		}
		return false;
	}


	/* CEvent handling */

	bool CEventArray::resizeEvent(CEvent* event, unsigned short newSize){
		if(!event->arr){return false;}
		event->arr=(CSignalListener*)realloc(arr,sizeof(CSignalListener)*newSize);
		if(!event->arr){printf("OKIT | ERROR! CEVENTARRAY CAN'T REALLOCATE MEMORY FOR AN ARRAY OF CSIGNALLISTENERS!\n");exit(OERR_CANTMALLOC);}
		event->cap=newSize;
		return true;
	}

	CEvent* CEventArray::createEvent(unsigned short _cap){
		if(arr){
			if(count+1>=cap){ if(!resize(cap+step)){return 0;} }
			for(unsigned short i=0;i<cap;i++){
				if(arr[i].arr==0){
					arr[i].arr=(CSignalListener*)malloc(sizeof(CSignalListener)*_cap);
					if(!arr[i].arr){printf("OKIT | ERROR! CEVENT CAN'T REALLOCATE MEMORY FOR AN ARRAY OF CSIGNALLISTENERS!\n");exit(OERR_CANTMALLOC);}
					arr[i].count=0,arr[i].cap=_cap;
					count++;
					return &(arr[i]);
				}
			}
		}
		return 0;
	}

	bool CEventArray::link(CEvent* event,CSignalListener& listener){
		if(!arr){return false;}
		for(unsigned short i=0;i<count;i++){
			if(event==&arr[i]){
				if(arr[i].arr){
					if(arr[i].count+1>=arr[i].cap){ if(!resize(arr[i].cap+step)){return false;} }
					for(unsigned short j=0;j<arr[i].cap;j++){
						if(arr[i].arr[j].type==CSLT_NONE){arr[i].arr[j]=listener;arr[i].count++;return true;}
					}
				}
				return false;
			}
		}
		return false;
	}

	bool CEventArray::unlink(CEvent* event,CSignalListener& listener){
		if(!arr){return false;}
		for(unsigned short i=0;i<count;i++){
			if(event==&arr[i]){
				if(arr[i].arr){
					bool found=false;
					for(unsigned short j=0;j<arr[i].cap;j++){
						if(found){
							arr[i].arr[j-1]=arr[i].arr[j];
							arr[i].arr[j].clear();
						}else{
							if(arr[i].arr[j].compare(listener)){
								arr[i].arr[j].clear();
								found=true;
								arr[i].count--;
								continue;
							}
						}
					}
					return found;
				}else{return false;}
			}
		}
		return false;
	}
}