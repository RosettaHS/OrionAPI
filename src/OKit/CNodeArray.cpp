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
#include "include/errdef.hpp"
#include "include/application.hpp"
#include "include/OSL/OLog.hpp"
#include "include/OKit/CDrawable.hpp"
#include "include/OKit/CNodeArray.hpp"

namespace Orion{
	CNodeArray::~CNodeArray(void){if(arr){free(arr);arr=0;}}
	CNodeArray::CNodeArray(void) : arr{0},count{0},cap{0},step{0} {}
	bool CNodeArray::init(unsigned short _cap,unsigned char _step){
		if(arr){return false;}
		count=0,cap=_cap,step=_step;
		arr=(CDrawable**)malloc(sizeof(CDrawable*)*_cap);
		if(arr){
			for(unsigned short i=0;i<_cap;i++){arr[i]=0;}
			return true;
		}else{
			OLog("ORIONAPI | ERROR! CNODEARRAY( %us,%u ) CAN'T MALLOC AN ARRAY!\n",_cap,_step);
			exit(OERR_CANTMALLOC);
			return false;
		}
	}

	bool CNodeArray::resize(unsigned short _size){
		arr=(CDrawable**)realloc(arr,sizeof(CDrawable*)*_size);
		if(arr){
			cap=_size;
			for(unsigned short i=count;i<_size;i++){arr[i]=0;}
			return true;
		}else{
			OLog("ORIONAPI | ERROR! CNODEARRAY( %us,%u ) :: resize( %us ) CAN'T MALLOC AN ARRAY!\n",cap,step,_size);
			exit(OERR_CANTMALLOC);
			return false;
		}
	}

	bool CNodeArray::link(CDrawable* node){
		if(count+1>=cap){
			if(!resize(cap+step)){return false;}
		}
		bool _completed=false;
		for(unsigned short i=0;i<cap;i++){
			if(arr[i]==0){
				arr[i]=node;
				count++;
				_completed=true;
				break;
			}
		}
		return _completed;
	}

	bool CNodeArray::unlink(CDrawable* node){
		int place=-1;
		for(unsigned short i=0;i<count;i++){
			if(arr[i]==node){
				place=i;
				break;
			}
		}
		if(place==-1){return false;}
		arr[place]=0;
		for(unsigned short i=place+1;i<cap;i++){
			arr[i-1]=arr[i];
			arr[i]=0;
		}
		count--;
		if(count==(cap-step*2)){resize(cap-step);}
		return true;
	}

	void CNodeArray::clear(void){
		for(unsigned short i=0;i<count;i++){
			arr[i]=0;
		}
		count=0;
	}

	int CNodeArray::getIndexOf(CDrawable* obj){
		for(unsigned short i=0;i<count;i++){
			if(arr[i]==obj){return i;}
		}
		return -1;
	}
	
	unsigned short CNodeArray::getCount(void){return count;}

	void CNodeArray::drawAll(void){
		for(unsigned short i=0;i<count;i++){ if(arr[i]->internal.drawPtr){arr[i]->internal.drawPtr(arr[i]);} }
	}

	void CNodeArray::log(void){
		OLog("CNodeArray %p | Size %u | Cap %u |Contents : ( ",(void*)this,count,cap);
		for(unsigned short i=0;i<count;i++){
			OLog("%p, ",(void*)arr[i]);
		}
		OLog(")\n");
	}

}
