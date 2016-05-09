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
//     Lambda tile cut tool
//
//-----------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>

#include <wand/magick_wand.h>

void print_usage() {
     printf("usage: lambdacut -w tile_width -h tile_height [-x tile_x -y tile_y] -i filename [-o output_dir | -o output_filename]\n");
}

int main(int argc, char** argv) {
    extern char *optarg;
    extern int optind;

    char* input_filename     = NULL;
    char* output_path        = NULL;
    unsigned int tile_width  = 0;
    unsigned int tile_height = 0;
    unsigned int tile_x      = 0;
    unsigned int tile_y      = 0;

    int c = 0;

    if(argc==1) {
       print_usage();
       exit(0);
    }
    while ((c = getopt(argc, argv, "w:h:x:y:i:o:")) != -1) {
       switch(c) {
          case 'i':
             input_filename = strdup(optarg);
          break;

          case 'o':
             output_path = strdup(optarg);
          break;

          case 'w':
             tile_width = atoi(optarg);
          break;

          case 'h':
             tile_height = atoi(optarg);
          break;

          case 'x':
             tile_x = atoi(optarg);
          break;

          case 'y':
             tile_y = atoi(optarg);
          break;

          case '?':
             print_usage();
             exit(1);
          break;
       }
    }

    if(input_filename==NULL) {
       fprintf(stderr,"%s: missing -i option\n",argv[0]);
       print_usage();
       exit(1);
    }
    if(output_path==NULL) {
       fprintf(stderr,"%s: missing -o option\n",argv[0]);
    }
    
}
