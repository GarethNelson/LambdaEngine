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
//      Part of the lambda_render module, implements basic 2D geometry
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

SDL_Window *screen;
SDL_GLContext glcontext;
// TODO - make these dynamic
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

// TODO - add texture loading
// TODO - switch to VBOs
void draw_quad(float x, float y, float w, float h) {
     glBegin( GL_QUADS );
            glColor3f( 0.f, 1.f, 1.f );
            glVertex2f(x, y );
            glVertex2f(x+w, y );
            glVertex2f(x+w,  y+h );
            glVertex2f(x,  y+h );
     glEnd();
}

