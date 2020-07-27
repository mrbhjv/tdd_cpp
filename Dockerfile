# GCC support can be specified at major, minor, or micro version
# (e.g. 8, 8.2 or 8.2.0).
# See https://hub.docker.com/r/library/gcc/ for all supported GCC
# tags from Docker Hub.
# See https://docs.docker.com/samples/library/gcc/ for more on how to use this image
FROM gcc:latest

RUN apt-get update && apt-get install -y \
    cmake \
    git \
    && rm -rf /var/lib/apt/lists/*

RUN git clone https://github.com/google/googletest.git \
    && cd googletest \
    && mkdir build && cd build \
    && cmake .. -DBUILD_SHARED_LIBS=ON -DINSTALL_GTEST=ON -DCMAKE_INSTALL_PREFIX:PATH=/usr \
    && make -j8 \
    && make install \
    && ldconfig

WORKDIR /usr/src/dockertest
COPY build.sh .
CMD ["./build.sh"]

LABEL Name=kata-bootstrap Version=0.0.1
