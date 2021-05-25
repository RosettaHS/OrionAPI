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
			return true;
		}
		return false;
	}

	bool CWidgetList::wipe(void){
		if(!arr){ return false; }
		free(arr);
		arr=0;
		count=0,cap=0,step=0;
		return true;
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

	int32_t CWidgetList::getIndexOf(const OWidget* widget) const{
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
#define DEF_MINW    50
#define DEF_MINH    50
#define DEF_ARRCAP  10
#define DEF_ARRSTEP 5

	/*** Constructors/Destructors ***/
	OContainer::OContainer(void){
		baseType=OUIB_CONTAINER;
		surfaceToUse=0;
		containerToUse=0;
		forceSelfOnNext=0;
		holdingInTmp=0;
	}
	OContainer::~OContainer(void){
		if(list.isReady()){
			OWidget* tmp=0;
			for(uint16_t i=0;i<list.getCount();i++){
				tmp=list[i];
				tmp->flags.linked    = false;
				tmp->onUnlink();
				tmp->parentCanvas    = 0;
				tmp->parentContainer = 0;
				tmp->parentWidget    = 0;
			}
			list.wipe();
		}
		surfaceToUse=0;
		containerToUse=0;
		canvas.destroy();
	}

	/* Three in a row! */
	OContainer::OContainer(OContainer* parent, int16_t ix, int16_t iy, uint16_t iw, uint16_t ih){
		OUIONLY{
			init(OUI_CONTAINER,ix,iy,iw,ih);
			baseType=OUIB_CONTAINER;

			surfaceToUse=&canvas;
			containerToUse=this;
			list.init(DEF_ARRCAP, DEF_ARRSTEP);
			flags.inited=true;
			flags.setColMode=OWIDGET_SETCOL_USE_SECONDARY;

			if(parent){ parent->link(this); }
		}else{
			OERROR(OERR_GRAPHICS_NOT_INITED,true,"FAILED TO CREATE OCONTAINER BECAUSE GRAPHICS ARE NOT INITIALISED!");
		}
	}
	/*** Deferrables ***/
	void OContainer::baseSort(void) { return; }
	void OContainer::sort(void)     { baseSort(); } /* Default OContainer uses the baseSort. */

	void OContainer::onLink(void){
		canvas.create(parentCanvas,x,y,w,h,theme.secondary);
		tmpRelinkAll();
		sort();
	}
	void OContainer::onUnlink(void){
		tmpUnlinkAll();
		canvas.destroy();
	}
	void OContainer::onPosChanged(void){
		canvas.setPos(x,y);
		sort();
	}
	void OContainer::onSizeChanged(void){
		canvas.setGeometry(x,y,w,h);
		sort();
	}
	void OContainer::onColChanged(void){
		canvas.setCol(theme.secondary);
	}
	

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
				if(widget->parentContainer){ widget->parentContainer->unlink(widget); }
				widget->parentContainer    = container;
				widget->parentWidget       = this;
				if(flags.linked){
					widget->parentCanvas = container->surfaceToUse;
					widget->flags.linked  = true;
					widget->onLink();
					sort();
				}
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
				widget->flags.linked    = false;
				widget->parentCanvas    = 0;
				widget->parentContainer = 0;
				widget->parentWidget    = 0;
				widget->onUnlink();
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
				tmp=container->list[i];
				tmp->flags.linked    = false;
				tmp->onUnlink();
				tmp->parentCanvas    = 0;
				tmp->parentContainer = 0;
				tmp->parentWidget    = 0;
			}
			container->list.unlinkAll();
			holdingInTmp=true;
			return true;
		}
		OWARN(false,"CAN'T UNLINK ALL CHILDREN FROM AN UNINITIALISED CONTAINER!");
		return false;
	}

	/** Temporary Transfers **/
	/*
	 * These are used when a Container is unlinked.
	 * It ensures all of the children are temporarily unlinked so
	 * they no longer do any graphical operations that might cause
	 * severe errors.
	 */
	void OContainer::tmpRelinkAll(void){
		if(holdingInTmp){
			for(uint16_t i=0;i<list.getCount();i++){
				if(list[i]->parentContainer==this){
					list[i]->flags.linked    = true;
					list[i]->parentCanvas    = surfaceToUse;
					list[i]->onLink();
				}
			}
			holdingInTmp=false;
		}
	}

	void OContainer::tmpUnlinkAll(void){
		for(uint16_t i=0;i<list.getCount();i++){
			list[i]->flags.linked  = false;
			list[i]->parentCanvas  = 0;
			list[i]->onUnlink();
		}
	}
	

	/*** Setters ***/
	/*** Getters ***/
	int32_t OContainer::getIndexOf(const OWidget* widget) const{
		if(forceSelfOnNext) { return list.getIndexOf(widget); }
		else                { return containerToUse->list.getIndexOf(widget); }
	}
	OWidget*  OContainer::getChild(uint16_t index) const{
		if(forceSelfOnNext) { return list[index]; }
		else                { return containerToUse->list[index]; }
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
#define MATCHTOSTRING(s) case s: { return #s; }
	/* holy... */
	static void recLog(uint16_t depth, uint16_t parIndex, container_t* c){
		auto indent = [](void) { OLog("    "); };
		widget_t*    w=0;
		depth++;
		for(uint16_t i=0; i<c->getChildCount(); i++){
			OLog("\t         ");
			for(uint16_t i=0;i<depth;i++){ indent(); }
			w=c->getChild(i);

			OLog("%u.%u : ",parIndex,i);
			OLog(w);
			if(w->getBaseType()==OUIB_CONTAINER){
				recLog(depth,i,(container_t*)w);
			}
		}
	}

	void OContainer::log(bool verbose, bool newLine){
		if(verbose){
			OVec  v;
			OVec4 v4;
			auto boolType       = [](bool _BOOL)       { return ( (_BOOL) ? "true" : "false" ); };
			auto setColModeType = [](uint8_t _COLMODE) {
				switch(_COLMODE){
					MATCHTOSTRING(OWIDGET_SETCOL_USE_PRIMARY)
					MATCHTOSTRING(OWIDGET_SETCOL_USE_SECONDARY)
					MATCHTOSTRING(OWIDGET_SETCOL_USE_TERTIARY)
					MATCHTOSTRING(OWIDGET_SETCOL_USE_ACCENT)
				}
				return "(INVALID)";
			};
			OLog("Container Widget : %s | %s : %p {\n",getTypeAsString(),getBTypeAsString(),this);
			OLog("\tx,y                : (%d, %d)\n",x,y);
			OLog("\tw,h                : (%u, %u)\n",w,h);
			OLog("\tminW,minH          : (%u, %u)\n",minW,minH);
			OLog("\tscale              : (%.3f)\n",scale);
			OLog("\tcustomID           : (%u)\n",customID);
			OLog("\tcanvas             : (%p)\n",&canvas);
			OLog("\tparentCanvas       : (%p)\n",parentCanvas);
			OLog("\tparentContainer    : (%p - %s)\n",parentContainer,( (parentContainer) ? parentContainer->getTypeAsString() : "OUI_ERROR") );
			OLog("\tparentWidget       : (%p - %s)\n",parentWidget,( (parentWidget) ? parentWidget->getTypeAsString() : "OUI_ERROR") );
			OLog("\tsurfaceToUse       : (%p)\n",surfaceToUse);
			OLog("\tcontainerToUse     : (%p)\n",containerToUse);
			OLog("\tforceSelfOnNext    : (%s)\n",boolType(forceSelfOnNext));
			OLog("\tholdingInTmp       : (%s)\n",boolType(holdingInTmp));
			OLog("\tflags:\n");
			OLog("\t    inited         : (%s)\n",boolType(flags.inited));
			OLog("\t    linked         : (%s)\n",boolType(flags.linked));
			OLog("\t    enabled        : (%s)\n",boolType(flags.enabled));
			OLog("\t    focused        : (%s)\n",boolType(flags.focused));
			OLog("\t    canFocus       : (%s)\n",boolType(flags.canFocus));
			OLog("\t    fullRedraw     : (%s)\n",boolType(flags.fullRedraw));
			OLog("\t    setColMode     : (%s)\n",setColModeType(flags.setColMode));
			OLog("\t    containerFlags : "); OLogBits(&flags.containerFlags,1,true);
			OLog("\ttheme:\n");
			OLog("\t     internal:\n");
			OLog("\t         primary   : "); theme.internal.primary.log();
			OLog("\t         secondary : "); theme.internal.secondary.log();
			OLog("\t         tertiary  : "); theme.internal.tertiary.log();
			OLog("\t         accent    : "); theme.internal.accent.log();
			OLog("\t     primary       : (%p)\n",theme.primary);
			OLog("\t     secondary     : (%p)\n",theme.secondary);
			OLog("\t     tertiary      : (%p)\n",theme.tertiary);
			OLog("\t     accent        : (%p)\n",theme.accent);
			OLog("\tlist:\n");
			OLog("\t     count         : (%u)\n",getChildCount());
			OLog("\t     children:\n");
			widget_t* tmp=0;
			for(uint16_t i=0;i<getChildCount();i++){
				tmp=getChild(i);
				OLog("\t         %u : ",i);
				getChild(i)->log();
				if(tmp->getBaseType()==OUIB_CONTAINER){
					recLog(0,i,(container_t*)tmp);
					/* I should never be allowed to write recursion again... */
				}
			}
			OLog("}\n");
		}else{
			OLog("(%s : %d, %d, %u, %u) ",getTypeAsString(),x,y,w,h);
			if(newLine){ OLog("\n"); }
		}
	}
}