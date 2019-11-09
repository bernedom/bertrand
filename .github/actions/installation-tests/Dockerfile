FROM conanio/gcc8:1.20.2

USER root
# Running as a dedicated user is needed,
# because else conan fails to find conan.conf ind the home directory
RUN useradd -ms /bin/bash conanuser

RUN apt update
RUN apt install shunit2 -y
USER conanuser

CMD $INPUT_SCRIPT