# GCC support can be specified at major, minor, or micro version
# (e.g. 8, 8.2 or 8.2.0).
# See https://hub.docker.com/r/library/gcc/ for all supported GCC
# tags from Docker Hub.
# See https://docs.docker.com/samples/library/gcc/ for more on how to use this image
FROM gcc:latest

RUN apt-get update && apt-get install -y \
    cmake \
    git \
    gdb \
    sudo \
    locales \
    locales-all \
    lcov \
    && rm -rf /var/lib/apt/lists/*

RUN git clone https://github.com/google/googletest.git \
    && cd googletest \
    && mkdir build && cd build \
    && cmake .. -DBUILD_SHARED_LIBS=ON -DINSTALL_GTEST=ON -DCMAKE_INSTALL_PREFIX:PATH=/usr \
    && make -j8 \
    && make install \
    && ldconfig

ENV LC_ALL en_US.UTF-8 
ENV LANG en_US.UTF-8  
ENV LANGUAGE en_US:en

ARG USERNAME=samurai
# On Linux, replace with your actual UID, GID if not the default 1000
ARG USER_UID=1000
ARG USER_GID=$USER_UID

RUN groupadd --gid $USER_GID $USERNAME \
   && useradd --uid $USER_UID --gid $USER_GID -m $USERNAME -s /bin/bash \
   && mkdir -p /home/$USERNAME/.vscode-server /home/$USERNAME/.vscode-server-insiders \
   && chown ${USER_UID}:${USER_GID} /home/$USERNAME/.vscode-server* \
   && echo $USERNAME ALL=\(root\) NOPASSWD:ALL > /etc/sudoers.d/$USERNAME \
   && chmod 0440 /etc/sudoers.d/$USERNAME

USER $USERNAME

WORKDIR /workspaces/kata-bootstrap
COPY build.sh .

LABEL Name=kata-bootstrap Version=0.0.1
