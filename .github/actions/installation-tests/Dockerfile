FROM conanio/gcc8:1.20.2

# Running as a dedicated user is needed,
# because else conan fails to find conan.conf ind the home directory
RUN sudo useradd -ms /bin/bash conanuser

RUN sudo apt update
RUN sudo apt install shunit2 -y
USER conanuser

CMD $INPUT_SCRIPT