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
#include "include/CContainer.hpp"

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
			}
		}
		childCount=0;
		contextToUse=0;
		drawableToUse=0;
		containerToUse=0;
	}
	CContainer::CContainer(void){
		childCount=0;
		contextToUse=0;
		drawableToUse=0;
		containerToUse=this;
	}

	/* Base containers do no sorting. */
	void CContainer::sort(void){ return; }

	bool CContainer::link(CDrawable& obj){
		if((void*)&obj==(void*)this){ OLog("OKIT | WARNING! CAN'T LINK A CONTAINER TO ITSELF!\n"); return false; }
		if(containerToUse->arr.link(&obj)){
			if(obj.parentContainer){ obj.parentContainer->unlink(obj); }
			childCount=containerToUse->arr.count;
			obj.context=contextToUse;
			obj.parentContainer=containerToUse;
			obj.parentDrawable=drawableToUse;
			obj.index=containerToUse->arr.getIndexOf(&obj);
			sort();
			if(obj.drawPtr){ obj.drawPtr(&obj); }
			return true;
		}
		return false;
	}

	bool CContainer::unlink(CDrawable& obj){
		if((void*)&obj==(void*)this){ OLog("OKIT | WARNING! CAN'T UNLINK A CONTAINER FROM ITSELF!\n"); return false; }
		if(containerToUse->arr.unlink(&obj)){
			childCount=containerToUse->arr.count;
			obj.context=0;
			obj.parentContainer=0;
			obj.parentDrawable=0;
			obj.index=-1;
			sort();
			return true;
		}
		return false;
	}

	int CContainer::getIndexOf(CDrawable& obj){
		if(obj.parentContainer==containerToUse){ return containerToUse->arr.getIndexOf(&obj); }
		else{ return -1; }
	}
}