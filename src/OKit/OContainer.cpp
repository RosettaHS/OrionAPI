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
#include "../include/Application.hpp"
#include "../include/OSL/OMath.hpp"
#include "../include/OKit/OContainer.hpp"

namespace Orion{
	/*** Private Widget Containment ***/

	/** Memory Ops **/
	bool CWidgetList::init(uint16_t icap, uint8_t istep){
		if(arr){ return false; }
		count=0, cap=icap, step=istep;
		arr=(OWidget**)malloc( sizeof(OWidget*)*cap );
		if(arr){
			for(uint16_t i=0;i<cap;i++){ arr[i]=0; }
		}else{ return false; }
	}

	bool CWidgetList::wipe(void){
		if(!arr){ return false; }
		free(arr);
		arr=0;
		count=0,cap=0,step=0;
	}

	bool CWidgetList::resizeTo(uint16_t newCap){
		if(arr){
			arr=(OWidget**)realloc(arr, sizeof(OWidget*)*newCap );
			if(arr){
				cap=newCap;
				for(uint16_t i=count;i<cap;i++){ arr[i]=0; }
				return true;
			}
		}
		return false;
	}

	/** Content Ops **/
	bool CWidgetList::link(OWidget* widget){
		if(arr){
			if(count+1>=cap){
				if(!resizeTo(cap+step)){
					OERROR(OERR_CANTMALLOC,true,"CAN'T ALLOCATE MEMORY FOR CWIDGETARRAY!");
					return false;
				}
			}
			for(uint16_t i=0;i<cap;i++){
				if(!arr[i]){
					arr[i]=widget;
					count++;
					return true;
				}
			}
		}
		return false;
	}

	bool CWidgetList::unlink(OWidget* widget){
		if(arr){
			for(uint16_t i=0;i<count;i++){
				if(arr[i]==widget){
					arr[i]=0;
					count--;
					for(uint16_t j=i+1;j<cap;j++){
						arr[j-1]=arr[j];
						arr[j]=0;
					}
					return true;
				}
			}
		}
		return false;
	}

	int32_t CWidgetList::getIndexOf(OWidget* widget) const{
		if(arr){
			for(uint16_t i=0;i<count;i++){
				if(arr[i]==widget){ return i; }
			}
		}
		return OWIDGET_NOTLINKED;
	}

	/*** Public Widget Container Class ***/
}