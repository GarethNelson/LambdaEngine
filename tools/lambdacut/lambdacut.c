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
     printf("usage: lambdacut -w tile_width -h tile_height -i filename -o output_dir [-v]\n");
}

int main(int argc, char** argv) {
    extern char *optarg;
    extern int optind;

    char* input_filename     = NULL;
    char* output_path        = NULL;
    unsigned int tile_w      = 0;
    unsigned int tile_h      = 0;
    unsigned int image_w     = 0;
    unsigned int image_h     = 0;

    int c = 0;
    int verbose = 0;

    if(argc==1) {
       print_usage();
       exit(0);
    }
    while ((c = getopt(argc, argv, "w:h:x:y:i:o:v")) != -1) {
       switch(c) {
          case 'i':
             input_filename = strdup(optarg);
          break;

          case 'o':
             output_path = strdup(optarg);
          break;

          case 'w':
             tile_w = atoi(optarg);
          break;

          case 'h':
             tile_h = atoi(optarg);
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

    if(input_filename==NULL) {
       fprintf(stderr,"%s: missing -i option\n",argv[0]);
       print_usage();
       exit(1);
    }
    if(output_path==NULL) {
       fprintf(stderr,"%s: missing -o option\n",argv[0]);
       print_usage();
       exit(1);
    }
    if(tile_w==0) {
       fprintf(stderr,"%s: missing -w option\n",argv[0]);
       print_usage();
       exit(1);
    }
    if(tile_h==0) {
       fprintf(stderr,"%s: missing -h option\n",argv[0]);
       print_usage();
       exit(1);
    }

    MagickWand *mw = NULL;
    MagickWandGenesis();
    mw = NewMagickWand();   

    MagickReadImage(mw,input_filename);

    image_w = MagickGetImageWidth(mw);
    image_h = MagickGetImageHeight(mw);

    unsigned int tiles_x    = image_w/tile_w;
    unsigned int tiles_y    = image_h/tile_h;
    unsigned int tile_count = tiles_x*tiles_y;
    unsigned int tile_x     = 0;
    unsigned int tile_y     = 0;
    unsigned int i          = 0;

    if(verbose) {
       printf("Loaded %s[%d * %d], %d tiles [%d * %d]\n",input_filename,image_w,image_h,tile_count,tiles_x,tiles_y);
    }
    
    MagickWand** tileset = malloc(sizeof(MagickWand*)*tile_count);
    unsigned int* is_dup = malloc(sizeof(unsigned int)*tile_count);

    for(i=0; i< tile_count; i++) {
        tile_x = i / tiles_x;
        tile_y = i % tiles_x;
        if(verbose) {
          printf("Tile %d located at [%d, %d]\n",i,tile_x*tile_w,tile_y*tile_h);

        }
        tileset[i] = CloneMagickWand(mw);
        MagickCropImage(tileset[i],tile_w,tile_h,tile_x*tile_w,tile_y*tile_h);
    }

    if(verbose) {
       printf("Beginning tileset comparisions\n");
    }
    unsigned int x=0;
    double diff;
    for(i=0; i< tile_count; i++) {
        for(x=0; x< tile_count; x++) {
            if(x==i) continue;
            MagickCompareImages(tileset[i],tileset[x],1,&diff);
            if(diff==0) {
               is_dup[i]=x;
               if(verbose) {
                  printf("%d is a duplicate of %d\n",i,x);
               }
               break;
            }
        }
    }

    if(mw) {
       mw = DestroyMagickWand(mw);
    }

    MagickWandTerminus();
 
}
