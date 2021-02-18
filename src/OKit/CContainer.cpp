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

#include "include/OSL/OLog.hpp"
#include "include/OKit/CContainer.hpp"

#define _CHECKFORCE ( forceSelfOnNext ? this : containerToUse ); forceSelfOnNext=false

namespace Orion{
	CContainer::~CContainer(void){
		if(arr.arr){
			CDrawable* obj=0;
			for(unsigned short i=0;i<childCount;i++){
				obj=arr.arr[i];
				obj->context=0;
				obj->parentDrawable=0;
				obj->parentContainer=0;
				obj->index=-1;
				obj->onUnlink();
			}
		}
		childCount=0;
		contextToUse=0;
		drawableToUse=0;
		containerToUse=0;
		selfContext.destroy();
	}
	CContainer::CContainer(void){
		childCount=0;
		contextToUse=0;
		drawableToUse=0;
		containerToUse=this;
		forceSelfOnNext=false;
	}

	/* Base containers do no sorting. */
	void CContainer::sort(void){ return; }

	void CContainer::tempRelinkAll(void){
		forceSelfOnNext=true;
		for(unsigned short i=0;i<childCount;i++){
			if(arr.arr[i]->parentContainer==this){
				arr.arr[i]->context=contextToUse;
				arr.arr[i]->onLink();
			}
		}
		forceSelfOnNext=false;
	}

	void CContainer::tempUnlinkAll(void){
		forceSelfOnNext=true;
		for(unsigned short i=0;i<childCount;i++){
			arr.arr[i]->context=0;
			arr.arr[i]->onUnlink();
		}
		forceSelfOnNext=false;
	}


	bool CContainer::link(CDrawable& obj){
		if((void*)&obj==(void*)this){ OLog("ORIONAPI | WARNING! CAN'T LINK A CONTAINER TO ITSELF!\n"); return false; }
		if(obj.type==OT_OWINDOW){ OLog("ORIONAPI | WARNING! CAN'T LINK A WINDOW TO ANYTHING!\n"); return false; }
		if(!obj.ready){ OLog("ORIONAPI | WARNING! CAN'T LINK A NON-INITIALISED OBJECT TO ANYTHING!\n"); return false;}
		CContainer* container=_CHECKFORCE;
		if(container->arr.link(&obj)){
			if(obj.parentContainer){ obj.parentContainer->unlink(obj); }
			container->childCount=container->arr.count;
			obj.context=contextToUse;
			obj.parentContainer=container;
			obj.parentDrawable=drawableToUse;
			obj.index=containerToUse->arr.getIndexOf(&obj);
			obj.onLink();
			sort();
			return true;
		}
		return false;
	}

	bool CContainer::unlink(CDrawable& obj){
		if((void*)&obj==(void*)this){ OLog("ORIONAPI | WARNING! CAN'T UNLINK A CONTAINER FROM ITSELF!\n"); return false; }
		if(obj.type==OT_OWINDOW){ OLog("ORIONAPI | WARNING! CAN'T UNLINK A WINDOW FROM ANYTHING!\n"); return false; }
		if(!obj.ready){ OLog("ORIONAPI | WARNING! CAN'T UNLINK A NON-INITIALISED OBJECT FROM ANYTHING!\n"); return false;}
		CContainer* container=_CHECKFORCE;
		if(container->arr.unlink(&obj)){
			obj.onUnlink();
			container->childCount=container->arr.count;
			obj.context=0;
			obj.parentContainer=0;
			obj.parentDrawable=0;
			obj.index=-1;
			sort();
			return true;
		}
		return false;
	}

	void CContainer::unlinkAll(void){
		CContainer* container=_CHECKFORCE;
		if(!container->arr.arr){ OLog("ORIONAPI | WARNING! CAN'T UNLINK ALL CHILDREN FROM AN UNINITIALISED CONTAINER!\n"); return; }
		for(unsigned short i=0;i<container->childCount;i++){
			CDrawable* obj=container->arr.arr[i];
			obj->onUnlink();
			obj->context=0;
			obj->parentContainer=0;
			obj->parentDrawable=0;
			obj->index=-1;
		}
		container->arr.clear();
		container->childCount=0;
	}

	int CContainer::getIndexOf(CDrawable& obj){
		CContainer* container=_CHECKFORCE;
		if(obj.parentContainer==container){ return container->arr.getIndexOf(&obj); }
		else{ return -1; }
	}

	OChildList CContainer::getChildren(void){
		CContainer* container=_CHECKFORCE;
		return container->arr.arr;
	}

	unsigned short CContainer::getChildCount(void){
		CContainer* container=_CHECKFORCE;
		return container->arr.count;
	}

	CDrawable* CContainer::getChildAtIndex(unsigned short i){
		CContainer* container=_CHECKFORCE;
		if(i>=container->arr.count){ OLog("ORIONAPI | WARNING! TRYING TO INDEX UNALLOCATED OR OUT-OF-BOUNDS MEMORY COULD CAUSE A SEGFAULT! ATTEMPTED INDEX : %u | MAXIMUM ALLOWED INDEX : %u\n",i,container->arr.count-1); return 0; }
		return container->arr.arr[i];
	}

	CDrawable* CContainer::operator[](unsigned short i){ return getChildAtIndex(i); }
}
