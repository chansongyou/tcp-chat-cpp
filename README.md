# tcp-chat-cpp
TCP Chat Server and Client using Boost.Asio


## Prerequisites

The followings must be installed:
* gcc
* g++
* make
* gdb (optional for debugging in linux and windows)
* lldb (optional for debugging in osx)

### How to install boost lib

Refer to [Getting Started on Unix Variants](https://www.boost.org/doc/libs/1_84_0/more/getting_started/unix-variants.html#get-boost)
1. `cd /usr/lib`
2. `sudo wget sudo wget https://boostorg.jfrog.io/artifactory/main/release/1.84.0/source/boost_1_84_0.tar.gz`
3. `sudo tar -xvf boost_1_84_0.tar.gz`
4. `cd boost_1_84_0`
5. `sudo ./bootstrap.sh --with-toolset=gcc`
6. `sudo ./b2 install`