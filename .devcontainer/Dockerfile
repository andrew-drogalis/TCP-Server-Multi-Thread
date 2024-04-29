FROM fedora:latest
USER root

LABEL maintainer="Andrew Drogalis <108765079+andrew-drogalis@users.noreply.github.com>"

# Split up dependencies for improved caching
RUN dnf -y update && \
    dnf -y install \
        gcc \
        g++ \
        cmake \
        gdb \
        git

RUN dnf -y install \
        ninja-build \
        kernel-tools \
        google-perftools \
        llvm-devel \
        clang-devel

RUN dnf -y install \
        valgrind \
        python3 \
        python3-pip \
        eigen3 \
        boost-devel \
        abseil-cpp

RUN dnf -y install \
        clang-tools-extra \
        gtest-devel \
        gmock-devel \
        doxygen \
        hotspot \
        cppcheck 

# Project specific dependancies
RUN dnf -y install \
        openssl-devel libsecret-devel libmicrohttpd-devel
    
RUN pip install conan lizard cmakelang codespell gcovr && \
        dnf clean all && \
        rm -rf /var/cache/dnf

ENV CC "gcc"
ENV CXX "g++"
ENV CMAKE "cmake"
ENV MAKE "make"

RUN git clone https://github.com/google/benchmark.git && \
   mkdir -p benchmark/build && \
   cd benchmark/build && cmake .. -DCMAKE_BUILD_TYPE=Release -DBENCHMARK_ENABLE_TESTING=OFF && \
   make && make install && \
   rm -rf /benchmark

# Install include-what-you-use
ENV IWYU /home/iwyu
ENV IWYU_BUILD ${IWYU}/build
ENV IWYU_SRC ${IWYU}/include-what-you-use
RUN LLVM_VER=$( \
    clang --version | \
    grep -o '[0-9][0-9]' | head -1 \
    ) \
    && echo LLVM_VER \
    && mkdir -p ${IWYU_BUILD} && \
    git clone --branch clang_$LLVM_VER \
        https://github.com/include-what-you-use/include-what-you-use.git \
        ${IWYU_SRC} \
    && CC=clang-$LLVM_VER CXX=clang++-$LLVM_VER cmake -S ${IWYU_SRC} \
        -B ${IWYU_BUILD} \
        -G "Unix Makefiles" -DCMAKE_PREFIX_PATH=/usr/lib/llvm-$LLVM_VER && \
    cmake --build ${IWYU_BUILD} -j && \
    cmake --install ${IWYU_BUILD} \
    && mkdir -p $(include-what-you-use -print-resource-dir 2>/dev/null) \
    && ln -s $(readlink -f /usr/lib/clang/$LLVM_VER/include) \
    $(include-what-you-use -print-resource-dir 2>/dev/null)/include

# Per https://github.com/include-what-you-use/include-what-you-use#how-to-install:
# `You need to copy the Clang include directory to the expected location before
#  running (similarly, use include-what-you-use -print-resource-dir to learn
#  exactly where IWYU wants the headers).`

RUN useradd -m -s /bin/bash vscode

ENTRYPOINT [ "bash", "-c" ]
