#!/bin/bash

platform='unknown'
unamestr=`uname`
if [[ "$unamestr" == 'Linux' ]]; then
   platform='linux'
elif [[ "$unamestr" == 'FreeBSD' ]]; then
   platform='freebsd'
elif [[ "$unamestr" == 'Darwin' ]]; then
   platform='darwin'
fi

find . -name '*.dll' | xargs -n 1 -I{} sh -c 'md5sum {} >{}.md5'
find . -name '*.so' | xargs -n 1 -I{} sh -c 'md5sum {} >{}.md5'
find . -name '*.dylib' | xargs -n 1 -I{} sh -c 'md5sum {} >{}.md5'
