#!/bin/sh

find . -name *.so | xargs -n 1 -I{} sh -c 'md5sum {} >{}.md5'
