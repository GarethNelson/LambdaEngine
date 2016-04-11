//-----------------------------------------------------------------------------
//
// $Id:$
//
// Copyright (C) 2016 by Gareth Nelson (gareth@garethnelson.com)
//
// This file is part of the Lambda engine.
//
// The Lambda engine is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// The Lambda engine is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
//
// $Log:$
//
// DESCRIPTION:
//      Program entry point
//
//-----------------------------------------------------------------------------

#include <stdio.h>
#include "vfs_init.h"
#include "lib_tools.h"

void init_libs() {
    printf("Libs: init_libs() - Locate libraries...");
    // load a list of libraries and checksums here
    printf("DONE!\n");
    
    printf("Libs: init_libs() - Checking cache...");
    // Check cache to see what we need to update
    printf("DONE!\n");

    printf("Libs: init_libs() - Need to update %d files in cache\n",1);
    
    // put the below in a loop
    printf("Libs: init_libs() - Updating 1 of 1 files in cache: whatever.so...");
    // load whatever.so from PhysFS, write it to cache
    printf("DONE!\n");

    printf("Libs: init_libs() - Need to load %d libraries\n",1);
    
    // put the below in a loop, use the libraries list returned from PhysFS
    printf("Libs: init_libs() - Loading 1 of 1 libraries: whatever.so:\n");
    // load whatever.so from cache using dlopen()
    // whatever.so should print out it's own debugging details
}

int main(int argc, char* argv[]) {
    printf("\n*** LAMBDA ENGINE STARTUP ***\n\n");
    printf("i_main.c:main() - Lambda engine starting up\n");
    vfs_init(argv[0]);
    init_libs();
}
