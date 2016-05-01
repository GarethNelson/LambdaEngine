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
//     Lambda map compiler
//
//-----------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>

#include <SDL.h>
#include <SDL_image.h>

#include "ini.h"

typedef struct {
    char* map_name;
    unsigned int tile_res_w;
    unsigned int tile_res_h;
} mapinfo_t;

static mapinfo_t mapinfo;

#define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0

static int def_handler(void* user, const char* section, const char* name, const char* value) {
       if(MATCH("MAPINFO","MAPNAME")) {
          mapinfo.map_name = strdup(value);
       } else if (MATCH("MAPINFO","TILERESW")) {
          mapinfo.tile_res_w = atoi(value);
       } else if (MATCH("MAPINFO","TILERESH")) {
          mapinfo.tile_res_h = atoi(value);
       } else {
         return 0;
       }
       return 1;
}

void print_usage() {
     printf("usage: lambdamc -i map_source_dir [-o map_output_dir]\n");
}

int main(int argc, char** argv) {
    extern char *optarg;
    extern int optind;

    char* src_dir =NULL;
    char* dest_dir=NULL;
    char mapinfo_def_filename[PATH_MAX];
    int c = 0;

    if(argc==1) {
       print_usage();
       exit(0);
    }
    while ((c = getopt(argc, argv, "i:o:")) != -1) {
       switch(c) {
          case 'i':
             src_dir = optarg;
          break;

          case 'o':
             dest_dir = optarg;
          break;

          case '?':
             print_usage();
             exit(1);
          break;
       }
    }

    if(src_dir==NULL) {
        fprintf(stderr, "%s: missing -i option\n",argv[0]);
        print_usage();
        exit(1);
    }
    if(dest_dir==NULL) {
        dest_dir = strdup(src_dir);
    }
    
    snprintf(mapinfo_def_filename,PATH_MAX-1,"%s/%s",src_dir,"mapinfo.def");
    
    if(ini_parse(mapinfo_def_filename, def_handler, NULL) < 0) {
       fprintf(stderr,"Failed to load mapinfo.def\n");
    }
    
}
