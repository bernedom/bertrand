FROM conanio/gcc11:1.48.1

USER root
# Running as a dedicated user is needed
# because else conan fails to find conan.conf ind the home directory
RUN useradd -ms /bin/bash conanuser

RUN apt update
RUN apt install shunit2 -y
USER conanuser

CMD $INPUT_SCRIPT