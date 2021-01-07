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
#include "include/application.hpp"
#include "include/CDrawable.hpp"
#include "include/CContainer.hpp"
#include "include/CContainable.hpp"

namespace Orion{
	CContainer::~CContainer(void){
		childCount=0;
		contextToUse=0;
		children.~CNodeArray();
	}
	CContainer::CContainer(void){
		childCount=0;
		contextToUse=0;
	}
	bool CContainer::link(CContainable* obj){
		if((void*)this==(void*)obj){printf("OKIT | WARNING! CANNOT LINK A CCONTAINER TO ITSELF!\n"); return false;}
		if(children.link(obj)){
			childCount=children.count;
			return true;
		}
		return false;
	}

	bool CContainer::unlink(CContainable* obj){
		if((void*)this==(void*)obj){printf("OKIT | WARNING! CANNOT UNLINK A CCONTAINER FROM ITSELF!\n");return false;}
		if(children.link(obj)){
			childCount=children.count;
			return true;
		}
		return false;
	}

	int CContainer::getIndexOf(CContainable* obj){return children.getIndexOf(obj);}
}