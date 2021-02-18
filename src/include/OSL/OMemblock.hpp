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

#ifndef __ORIONAPI_OSL_OMEMBLOCK_H__
#define __ORIONAPI_OSL_OMEMBLOCK_H__

#define _OTMPBEGIN template<typename TYPE>

#include <sys/types.h>
#include <malloc.h>
#include "OLog.hpp"

namespace Orion{
/*** Declarations ***/

	/* Wrapper for malloc() and free(). Allocates a chunk of memory for the given type, and automatically frees it once this Memblock goes out of scope. */
	template<typename TYPE> class OMemblock{
		protected:
			/* A pointer to the memory allocated by this Memblock. */
			TYPE* ptr;
			/* The size of the memory allocated by this Memblock. */
			size_t size;
			/* Was the memory allocated with individual bytes? */
			bool allocRaw;
		public:
			/* Destructor. Calls clear(). */
			~OMemblock(void);
			/* Empty constructor. Sets all values to 0. */
			OMemblock(void);
			/*
			 * Allocates a block of memory with the given size.
			 * By default will allocate memory equal to the size of the type multiplied by the requested size ( sizeof(givenType)*givenSize ).
			 * Passing true as the second parameter will allocate the exact amount of bytes specified instead. 
			 */
			bool allocate(size_t, bool raw=false); OMemblock(size_t, bool raw=false);
			/*
			 * Same as allocate(), but resizes the current allocated block by the given ammount.
			 * By default will allocate memory equal to the size of the type multiplied by the requested size ( sizeof(givenType)*givenSize ).
			 * Passing true as the second parameter will allocate the exact amount of bytes specified instead. 
			 */
			bool resize(size_t, bool raw=false);
			/* Frees the allocated memory. */
			bool clear(void);

			/*
			 * Returns the size of the memory allocated.
			 * Passing true will return the exact size in bytes, otherwise returning the amount of this type allocated ( size/sizeof(givenType) ).
			 */
			size_t getSize(bool raw=false);

			/* Gets the value at the given index if this is an array. */
			TYPE& operator[](size_t);
	};

/*** Definitions ***/

	_OTMPBEGIN OMemblock<TYPE>::~OMemblock(void){ clear(); }
	_OTMPBEGIN OMemblock<TYPE>::OMemblock(void) : ptr{0},size{0},allocRaw{0} {}
	_OTMPBEGIN OMemblock<TYPE>::OMemblock(size_t s, bool raw) : ptr{0},size{s},allocRaw{0} { allocate(s,raw); }

	_OTMPBEGIN bool OMemblock<TYPE>::resize(size_t s, bool raw){
		if(ptr){
			allocRaw=raw;
			ptr=(TYPE*)malloc(ptr (raw ? s : sizeof(TYPE)*s) );
			if(ptr){ size=(raw ? s : sizeof(TYPE)*s); return true; }
			else{ return false; }
		}else{ return false; }
	}

	_OTMPBEGIN bool OMemblock<TYPE>::allocate(size_t s, bool raw){
		clear(); allocRaw=raw;
		ptr=(TYPE*)malloc( (raw ? s : sizeof(TYPE)*s) );
		if(ptr){ size=(raw ? s : sizeof(TYPE)*s); return true; }
		else{ size=0; return false; }
	}

	_OTMPBEGIN bool OMemblock<TYPE>::clear(void){
		if(ptr){ free(ptr); ptr=0; size=0; return true; }
		else{ return false; }
	}

	_OTMPBEGIN size_t OMemblock<TYPE>::getSize(bool raw){
		if(allocRaw){ return (raw ? size : size/sizeof(TYPE)); }
		else{ return (raw ? size/sizeof(TYPE) : size); }
	}

	_OTMPBEGIN TYPE& OMemblock<TYPE>::operator[](size_t s){
		size_t tmpSize=getSize(false);
		if(s>tmpSize){ OLog("ORIONAPI | WARNING! TRYING TO INDEX UNALLOCATED OR OUT-OF-BOUNDS MEMORY COULD CAUSE A SEGFAULT! ATTEMPTED INDEX : %u | MAXIMUM ALLOWED INDEX : %u\n",s,tmpSize); return 0;}
		else{ return ptr[s]; }
	}
}

#undef _OTMPBEGIN

#endif /* !__ORIONAPI_OSL_OMEMBLOCK_H__ */