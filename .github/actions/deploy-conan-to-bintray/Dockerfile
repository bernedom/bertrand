FROM alpine:latest

RUN apk update
RUN apk add g++ gcc cmake python3 py3-pip python3-dev

RUN pip3 install --upgrade pip
RUN pip3 install conan

CMD echo ::set-putput name=msg::"hello world"