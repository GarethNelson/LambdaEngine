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
//     Global variables
//
//-----------------------------------------------------------------------------

#include <lambda_api.h>
#include <lambda_state.h>

global_state_t     *global_state;
hook_callbacks_t   *hook_callbacks    = NULL;
single_callbacks_t *_single_callbacks = NULL;

void init_globals() {
     global_state     = (global_state_t*)malloc(sizeof(global_state_t));
     single_callbacks = &_single_callbacks;
}

