FROM debian:jessie

RUN REPO=http://cdn-fastly.deb.debian.org && \
  echo "deb $REPO/debian jessie main\ndeb $REPO/debian jessie-updates main\ndeb $REPO/debian-security jessie/updates main" > /etc/apt/sources.list
RUN echo "deb http://ftp.us.debian.org/debian unstable main contrib non-free" >> /etc/apt/sources.list.d/unstable.list

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update --yes
RUN apt-get install --yes \
  automake \
  autogen \
  bash \
  build-essential \
  git \
  libgl1-mesa-dev \
  x11proto-core-dev \
  libx11-dev
RUN apt-get install --yes -t unstable gcc-5 g++-5
RUN echo 'Yes, do as I say!' | apt-get install libsdl2-dev
RUN apt-get clean --yes

# clone and build dependencies
RUN git clone git://github.com/bkaradzic/bx.git && \
  git clone git://github.com/bkaradzic/bimg.git && \
  git clone git://github.com/bkaradzic/bgfx.git

RUN cd bx && make linux-release64
RUN cd bimg && make build CXX="g++-5" CC="gcc-5"
RUN cd bgfx && make linux-release64
RUN cd bgfx && make tools

COPY . /bgfx-PlanetShader
RUN mkdir -p /bgfx-PlanetShader/shaders/glsl
RUN cd bgfx-PlanetShader && make
