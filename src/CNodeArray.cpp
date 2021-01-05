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
#include "include/CDrawable.hpp"
#include "include/CNodeArray.hpp"

namespace Orion{
	CNodeArray::~CNodeArray(void){free(arr);}
	CNodeArray::CNodeArray(void) : arr{0},count{0},cap{0},step{0} {}
	CNodeArray::CNodeArray(unsigned short _cap,unsigned char _step) : arr{0},count{0},cap{_cap},step{_step} {
		arr=(CDrawable**)malloc(sizeof(CDrawable*)*_cap);
		if(arr){
			for(unsigned short i=0;i<_cap;i++){arr[i]=0;}
		}else{
			printf("OKIT | ERROR! CNodeArray( %us,%u ) CAN'T MALLOC AN ARRAY!\n",_cap,_step);
			OERR_EXIT(OERR_CANTMALLOC)
		}
	}

	bool CNodeArray::resize(unsigned short _size){
		arr=(CDrawable**)realloc(arr,_size);
		if(arr){
			cap=_size;
			for(unsigned short i=count;i<_size;i++){arr[i]=0;}
			return true;
		}else{
			printf("OKIT | ERROR! CNodeArray( %us,%u ) :: resize( %us ) CAN'T MALLOC AN ARRAY!\n",cap,step,_size);
			OERR_EXIT(OERR_CANTMALLOC)
			return false;
		}
	}

	bool CNodeArray::link(CDrawable* node){
		/* printf("Starting. Cap %d | Step %d | Count %d | Node %p\n",cap,step,count,node); */
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
		/* printf("Finished. Cap %d | Step %d | Count %d | Success %s\n",cap,step,count,_completed ? "true" : "false"); */
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
		if(count<=(cap-step*2)){resize(cap-step);}
		return true;
	}

	int CNodeArray::getIndexOf(CDrawable* obj){
		for(unsigned short i=0;i<count;i++){
			if(arr[i]==obj){return i;}
		}
		return -1;
	}
	
	unsigned short CNodeArray::getCount(void){return count;}

	void CNodeArray::drawAll(void){
		for(unsigned short i=0;i<count;i++){arr[i]->drawPtr(arr[i]);}
	}

	void CNodeArray::log(void){
		printf("CNodeArray %p | Size %u | Cap %u |Contents : ( ",this,count,cap);
		for(unsigned short i=0;i<count;i++){
			printf("%p, ",arr[i]);
		}
		printf(")\n");
	}

}