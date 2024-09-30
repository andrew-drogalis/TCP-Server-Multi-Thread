FROM alpine:latest
USER root

RUN echo "http://dl-cdn.alpinelinux.org/alpine/edge/testing/" >> /etc/apk/repositories

LABEL maintainer="Andrew Drogalis <108765079+andrew-drogalis@users.noreply.github.com>"

# Split up dependencies for improved Docker caching
RUN apk update && \
    apk add --no-cache \
        gcc \
        g++ \
        make \
        cmake \
        gdb \
        git \
        bash \
        ca-certificates \
        sudo

RUN apk update && \
    apk add --no-cache \
        ninja-build \
        gperftools-dev \
        llvm17-dev \
        clang17-dev \
        clang17-extra-tools

RUN apk update && \
    apk add --no-cache \
        valgrind-dev \
        python3-dev \
        py3-pip \
        eigen-dev \
        boost-dev \
        abseil-cpp \
        benchmark-dev

RUN apk update && \
    apk add --no-cache \
        gtest-dev \
        gmock \
        cppcheck \
        include-what-you-use 

# Project specific dependencies
RUN apk update && \
    apk add --no-cache \
        openssl-dev libsecret-dev libmicrohttpd-dev
    
RUN python3 -m venv .venv && \
    source .venv/bin/activate && \
        python3 -m pip install conan lizard cmakelang codespell gcovr && \
        rm -rf /var/cache/apk/*

ENV CC "gcc"
ENV CXX "g++"
ENV CMAKE "cmake"
ENV MAKE "make"

RUN adduser \
    -s /bin/bash \
    -D \
    vscode \
  && addgroup sudo \
  && adduser vscode sudo \
  && echo -e "\n# Allow sudo without password\n%sudo ALL=(ALL) NOPASSWD:ALL\n" >> /etc/sudoers

USER vscode
ENTRYPOINT [ "/bin/bash", "-l" ]
