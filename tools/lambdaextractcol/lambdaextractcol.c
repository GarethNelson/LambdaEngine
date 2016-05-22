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
//     Lambda collision tiles extractor
//
//-----------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

#include <wand/magick_wand.h>

void print_usage() {
     fprintf(stderr,"usage: lambdaextractcol -r red_color -g green_color -b blue_color -o output_dir [-f filename] [-v]\n");
}

int main(int argc, char** argv) {
    extern char *optarg;
    extern int optind;

    char* input_filename     = NULL;
    char* output_path        = NULL;
    unsigned int red_c       = 0;
    unsigned int green_c     = 0;
    unsigned int blue_c      = 0;

    int c = 0;
    int verbose = 0;

    if(argc==1) {
       print_usage();
       exit(0);
    }
    while ((c = getopt(argc, argv, "r:g:b:o:f:v")) != -1) {
       switch(c) {
          case 'f':
             input_filename = strdup(optarg);
          break;

          case 'o':
             output_path = strdup(optarg);
          break;

          case 'r':
             red_c = atoi(optarg);
          break;

          case 'g':
             green_c = atoi(optarg);
          break;

          case 'b':
             blue_c = atoi(optarg);
          break;

          case 'v':
             verbose = 1;
          break;

          case '?':
             print_usage();
             exit(1);
          break;
       }
    }

    FILE* input_fd;

    if(input_filename==NULL) {
       input_fd=stdin;
    } else {
       input_fd=fopen(input_filename,"r");
    }

    if(input_fd==NULL) {
       fprintf(stderr,"%s: Unable to open input file \"%s\" - %s\n",argv[0],input_filename,strerror(errno));
       exit(1);
    }
    if(output_path==NULL) {
       fprintf(stderr,"%s: missing -o option\n",argv[0]);
       print_usage();
       exit(1);
    }

    MagickWandGenesis();

    MagickWandTerminus();

}
