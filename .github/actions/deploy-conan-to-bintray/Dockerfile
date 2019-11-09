FROM conanio/gcc8:1.20.2

# Running as a dedicated user is needed,
# because else conan fails to find conan.conf ind the home directory
RUN sudo useradd -ms /bin/bash conanuser
USER conanuser

COPY ./deploy-to-conan.sh /deploy-to-conan.sh

ENTRYPOINT [ "/deploy-to-conan.sh" ]