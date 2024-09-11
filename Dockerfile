FROM ubuntu:24.04
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y vim git wget cmake 

RUN apt-get update && apt-get install -y build-essential \
    libzmq3-dev \
    tree \
    autotools-dev automake autoconf libtool \
    net-tools
