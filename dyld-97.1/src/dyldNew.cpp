/* -*- mode: C++; c-basic-offset: 4; tab-width: 4 -*-
 *
 * Copyright (c) 2004-2007 Apple Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this
 * file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */


#include <stdlib.h>
#include <stdint.h>
#include <string.h>

extern "C" void* __dso_handle;

#include "dyld.h"
#include "dyldLibSystemInterface.h"

//
//	 dyld initially allocates all memory from a pool inside dyld.
//   Once libSystem.dylib is initialized, dyld uses libSystem's malloc/free.
//

#if __LP64__
	// room for about ~1000 initial dylibs
	#define DYLD_INITIAL_POOL_SIZE 400*1024
#else
	// room for about ~900 initial dylibs
	#define DYLD_INITIAL_POOL_SIZE 200*1024
#endif
static uint8_t dyldPool[DYLD_INITIAL_POOL_SIZE];
static uint8_t* curPoolPtr = dyldPool;

void* malloc(size_t size)
{
	if ( dyld::gLibSystemHelpers != NULL) {
		void* p = dyld::gLibSystemHelpers->malloc(size);
		//dyld::log("malloc(%lu) => %p from libSystem\n", size, p);
		return p;
	}
	else {
		size = (size+sizeof(void*)-1) & (-sizeof(void*)); // pointer align
		uint8_t* result = curPoolPtr;
		if ( (curPoolPtr + size) > &dyldPool[DYLD_INITIAL_POOL_SIZE] ) {
			dyld::log("initial dyld memory pool exhausted\n");
			_exit(1);
		}
		curPoolPtr += size;
		//dyld::log("%p = malloc(%lu) from pool, total = %d\n", result, size, curPoolPtr-dyldPool);
		return result;
	}
}


void free(void* ptr)
{
	// ignore any pointer within dyld (i.e. stuff from pool or static strings)
	if ( (dyld::gLibSystemHelpers != NULL) && ((ptr < &__dso_handle) || (ptr >= &dyldPool[DYLD_INITIAL_POOL_SIZE])) ) {
		//dyld::log("free(%p) from libSystem\n", ptr);
		return dyld::gLibSystemHelpers->free(ptr);
	}
	else {
		// do nothing, pool entries can't be reclaimed
		//dyld::log("free(%p) from pool\n", ptr);
	}
}


/* 在内存的动态存储区中分配 count 个长度为 size 的连续空间，
 * @return 一个指向分配起始地址的指针；如果分配不成功，返回 NULL。
 * @note 该函数在动态分配完内存后，自动初始化该内存空间为零；而malloc()不初始化，里边数据是未知的垃圾数据。
 */
void* calloc(size_t count, size_t size){
	if ( dyld::gLibSystemHelpers != NULL ) {
		void* result = dyld::gLibSystemHelpers->malloc(size);
		bzero(result, size);
		return result;
	}
	else {
		return malloc(count*size);
	}
}

/* 扩展指定指针的内存
 * @param ptr 要改变内存大小的指针名
 * @param size 新的内存大小
 * @return 如果重新分配成功则返回指向被分配内存的指针，否则返回空指针NULL。
 * @note 如果新内存空间大于原内存，则新分配部分不会被初始化；如果新内存空间小于原内存，可能会导致数据丢失；
 * @note 先判断当前的指针是否有足够的连续空间，如果有，扩大 ptr 指向的地址，并且将 ptr 返回；
 *       如果空间不够，先按照size指定的大小分配空间，将原有数据从头到尾拷贝到新分配的内存区域，
 *       而后释放原来ptr所指内存区域（注意：原来指针是自动释放，不需要使用free），同时返回新分配的内存区域的首地址。
 *       即重新分配存储器块的地址。
 */
void* realloc(void *ptr, size_t size){
	void* result = malloc(size);
	memcpy(result, ptr, size);
	return result;
}

//     void* reallocf(void *ptr, size_t size);
//     void* valloc(size_t size);

// needed __libc_init()
extern "C" int _malloc_lock;
int _malloc_lock = 0;


