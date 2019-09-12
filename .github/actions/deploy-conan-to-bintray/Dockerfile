FROM alpine:latest

RUN apk update
RUN apk add g++ gcc cmake python3 py3-pip python3-dev make

RUN pip3 install --upgrade pip
RUN pip3 install conan
COPY ./deploy-to-conan.sh /deploy-to-conan.sh

WORKDIR /home/bertrand

ENTRYPOINT [ "/deploy-to-conan.sh" ]