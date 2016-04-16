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
#include <dlfcn.h>
#include <stdlib.h>
#include "vfs_init.h"
#include "lib_tools.h"

int main(int argc, char* argv[]) {
    int  (*video_init)();
    void (*video_pre_render)();
    void (*video_post_render)();
    void (*render_init)();

    printf("\n*** LAMBDA ENGINE STARTUP ***\n\n");
    printf("l_main.c:main() - Lambda engine starting up\n");
    vfs_init(argv[0]);
    init_libs();

    video_init        = dlsym(RTLD_DEFAULT,"video_init");
    video_pre_render  = dlsym(RTLD_DEFAULT,"video_pre_render");
    video_post_render = dlsym(RTLD_DEFAULT,"video_post_render");
    if(video_init() != 0) {
       printf("l_main.c:main() - Failed to setup video!\n");
       exit(1);
    }
    
//    render_init       = dlsym(RTLD_DEFAULT,"render_init");
    
    int running=1; 
    while(running) {
        video_pre_render();
        // process_events();
        // render_frame();
        video_post_render();       
    }
}
