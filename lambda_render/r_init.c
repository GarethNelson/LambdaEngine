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
//      Part of the lambda_render module, setup and init code
//
//-----------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#define __IN_RENDER_
#include <lambda_api.h>

#include <ft2build.h>
#include FT_FREETYPE_H

SDL_Window *screen;
SDL_GLContext glcontext;
FT_Library font_library;

void __attribute__((constructor)) init_module() {
     printf("lambda_render/r_init.c:init_module() - module loaded\n");
     INIT_LAMBDA_API()
     IMPORT(vfs_cache_read)
     IMPORT(vfs_cache_filelen)
}

void render_font_init() {
     FT_Init_FreeType(&font_library);
}

void render_init() {
     printf("lambda_render/r_init.c:render_init() - Setup font subsystem\n");
     render_font_init();
}
