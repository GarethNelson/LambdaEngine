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
//     This magical header file includes the API used by all modules
//
//-----------------------------------------------------------------------------

#ifndef __LAMBDA_API_H_
#define __LAMBDA_API_H_

#include <dlfcn.h>
#include "lambda_state.h"

#if defined(__APPLE__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#endif

static int    (*video_init)();
static void   (*video_pre_render)();
static void   (*video_post_render)();

static void   (*render_init)();
static GLuint (*load_texture)(char* vfs_filename);
static void   (*draw_triangle_rot)(float x, float y, float rot);
static void   (*draw_quad)(float x,float y, float w, float h,GLuint tex_id);

// Only extern stuff from l_main.c if we're not actually in l_main.c
#ifndef __IN_MAIN_
extern global_state_t global_state;
#endif

#endif

