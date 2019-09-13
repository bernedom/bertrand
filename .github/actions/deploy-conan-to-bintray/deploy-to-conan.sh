#!/bin/sh -l

export CONAN_LOGIN_USERNAME=$INPUT_CONAN_USER
export CONAN_PASSWORD=$INPUT_CONAN_PASS
echo "conan remote add custom ${INPUT_CONAN_REMOTE}"
conan remote add custom ${INPUT_CONAN_REMOTE}
conan create . ${INPUT_CONAN_CHANNEL}
conan upload ${INPUT_CONAN_PACKAGE}/*@${INPUT_CONAN_CHANNEL} --all -c -r custom
