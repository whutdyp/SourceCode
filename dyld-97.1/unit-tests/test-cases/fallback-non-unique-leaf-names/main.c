/*
 * Copyright (c) 2005 Apple Computer, Inc. All rights reserved.
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
#include <mach-o/dyld.h>

#include "test.h"

///
///  This process has two libfoo.dylibs each of which has a foo() that returns a different value.
///  If dyld only loads one libfoo.dylib, then this test fails
///  main links directly against one libfoo.dylib and indirectly through libbar.dylib with the other.
///


int main(int argc, const char* argv[])
{
	if ( foo() == bar() ) {
		FAIL("fallback-non-unique-leaf-names");
		return EXIT_SUCCESS;
	}
	
	PASS("fallback-non-unique-leaf-names");
	return EXIT_SUCCESS;
}

