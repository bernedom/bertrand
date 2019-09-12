#!/bin/sh -l

conan remote add bernedom https://api.bintray.com/conan/bernedom/conan
conan create . $1
#conan upload bertrand/*@$1 --all -c -r bernedom
