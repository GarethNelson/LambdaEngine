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

#define __IN_RENDER_
#include <lambda_api.h>


void __attribute__((constructor)) init_module() {
     printf("lambda_render/r_init.c:init_module() - module loaded\n");
     INIT_LAMBDA_API()
}

void render_init() {
     printf("lambda_render/r_init.c:render_init() - Nothing to do!\n");
}
