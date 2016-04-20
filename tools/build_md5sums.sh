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

if [ $platform == 'linux' ] || [ $platform == 'freebsd' ]; then
   find . -name '*.so' | xargs -n 1 -I{} sh -c 'md5sum {} >{}.md5'
elif [ $platform == 'darwin' ]; then
   find . -name '*.dylib' | xargs -n 1 -I{} sh -c 'md5sum {} >{}.md5'
fi
