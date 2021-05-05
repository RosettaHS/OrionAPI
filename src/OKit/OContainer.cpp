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

	bool CWidgetList::unlinkAll(void){
		if(arr){
			for(uint16_t i=0;i<count;i++){ arr[i]=0; }
			count=0;
			return true;
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

	OWidget* CWidgetList::getFromIndex(uint16_t index) const{
		if(arr){
			if(index<count){
				return arr[index];
			}
		}
		return 0;
	}

	/*** Public Widget Container Class ***/

	/*** Constructors/Destructors ***/
	OContainer::OContainer(void){
		contextToUse=0;
		containerToUse=0;
		forceSelfOnNext=0;
	}
	OContainer::~OContainer(void){
		
	}
	/*** Deferrables ***/
	void OContainer::baseSort(void) { return; }
	void OContainer::sort(void)     { return; }
	

	/*** Registration ***/
#define CHECKFORCE ( forceSelfOnNext ? this : containerToUse ); forceSelfOnNext=false

	bool OContainer::link(OWidget* widget){
		if(widget && list.isReady()){
		/** Error Checking **/
			if(widget==this)                  { OWARN(false,"CAN'T LINK A CONTAINER TO ITSELF!");                return false; }
			if(widget->getType()==OUI_WINDOW) { OWARN(false,"CAN'T LINK A WINDOW TO ANYTHING!");                 return false; }
			if(!widget->isInited())           { OWARN(false,"CAN'T LINK A NON-INITIALISED WIDGET TO ANYTHING!"); return false; }
		/** Linkage **/
			OContainer* container=CHECKFORCE;
			if(container->list.link(widget)){
				if(widget->parent){ widget->parent->unlink(widget); }
				widget->context = container->contextToUse;
				widget->parent  = container;
				widget->index   = container->getIndexOf(widget);
				widget->flags.linked=true;
				widget->onLink();
				sort();
				return true;
			}
		}
		return false;
	}

	bool OContainer::unlink(OWidget* widget){
		if(widget && list.isReady()){
		/** Error Checking **/
			if(widget==this)                  { OWARN(false,"CAN'T UNLINK A CONTAINER FROM ITSELF!");                return false; }
			if(widget->getType()==OUI_WINDOW) { OWARN(false,"CAN'T UNLINK A WINDOW FROM ANYTHING!");                 return false; }
			if(!widget->isInited())           { OWARN(false,"CAN'T UNLINK A NON-INITIALISED WIDGET FROM ANYTHING!"); return false; }
		/** Unlinkage **/
			OContainer* container=CHECKFORCE;
			if(container->list.unlink(widget)){
				widget->flags.linked=false;
				widget->onUnlink();
				widget->context = 0;
				widget->parent  = 0;
				widget->index   = OWIDGET_NOTLINKED;
				return true;
			}
		}
		return false;
	}

	bool OContainer::unlinkAll(void){
		OContainer* container=CHECKFORCE;
		if(container->list.isReady()){
			OWidget* tmp=0;
			for(uint16_t i=0;i<container->list.getCount();i++){
				tmp=container->list.getFromIndexFast(i);
				tmp->flags.linked=false;
				tmp->onUnlink();
				tmp->context = 0;
				tmp->parent  = 0;
				tmp->index   = OWIDGET_NOTLINKED;
			}
			container->list.unlinkAll();
			return true;
		}
		OWARN(false,"CAN'T UNLINK ALL CHILDREN FROM AN UNINITIALISED CONTAINER!");
		return false;
	}

	/*** Setters ***/
	/*** Getters ***/
	int32_t OContainer::getIndexOf(OWidget* widget) const{
		if(forceSelfOnNext) { return list.getIndexOf(widget); }
		else                { return containerToUse->list.getIndexOf(widget); }
	}
	OWidget*  OContainer::getChild(uint16_t index) const{
		if(forceSelfOnNext) { return list.getFromIndex(index); }
		else                { return containerToUse->list.getFromIndex(index); }
	}
	OWidget** OContainer::getChildren(void) const{
		if(forceSelfOnNext) { return list.getArray(); }
		else                { return containerToUse->list.getArray(); }
	}
	uint16_t OContainer::getChildCount(void) const{
		if(forceSelfOnNext) { return list.getCount(); }
		else                { return containerToUse->list.getCount(); }	
	}
	/*** Misc ops ***/
}