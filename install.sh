#!/bin/bash
# Make a cross-compiler environment for i686 32-bit Intel architecture
export CC=/usr/bin/gcc
export LD=/usr/bin/gcc
export PREFIX="/usr/local/i686elfgcc"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

# binutils
mkdir /tmp/src
cd /tmp/src
curl -O https://ftp.gnu.org/gnu/binutils/binutils-2.34.tar.gz # If the link 404's, look for a more recent version
tar xf binutils-2.34.tar.gz
mkdir binutils-build
cd binutils-build
../binutils-2.34/configure --target=$TARGET --enable-interwork --enable-multilib --disable-nls --disable-werror --prefix=$PREFIX 2>&1 | tee configure.log
make all install 2>&1 | tee make.log

# gcc
cd /tmp/src
curl -O https://ftp.gnu.org/gnu/gcc/gcc-9.3.0/gcc-9.3.0.tar.gz
tar xf gcc-9.3.0.tar.gz
mkdir gcc-build
cd gcc-build
../gcc-9.3.0/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --disable-libssp --enable-languages=c --without-headers
make all-gcc 
make all-target-libgcc 
make install-gcc 
make install-target-libgcc