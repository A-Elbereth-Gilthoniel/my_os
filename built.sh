#!/bin/bash
set -e

TARGET=i686-elf
PREFIX="$HOME/opt/cross"
PATH="$PREFIX/bin:$PATH"

echo "Updating system packages..."
sudo apt-get update
sudo apt-get -y install build-essential g++ make bison flex texinfo libgmp-dev libmpc-dev libmpfr-dev libisl-dev wget

mkdir -p $HOME/src && cd $HOME/src

# -------- binutils --------
if [ ! -d "binutils-2.42" ]; then
    echo "Downloading binutils..."
    wget https://ftp.gnu.org/gnu/binutils/binutils-2.42.tar.xz
    tar -xf binutils-2.42.tar.xz
fi

echo "Building binutils..."
mkdir -p build-binutils && cd build-binutils
../binutils-2.42/configure --target=$TARGET --prefix=$PREFIX --with-sysroot --disable-nls --disable-werror
make -j$(nproc)
make install
cd ..

# -------- gcc --------
if [ ! -d "gcc-10.2.0" ]; then
    echo "Downloading gcc..."
    wget https://ftp.gnu.org/gnu/gcc/gcc-10.2.0/gcc-10.2.0.tar.xz
    tar -xf gcc-10.2.0.tar.xz
fi

cd gcc-10.2.0
contrib/download_prerequisites
cd ..

echo "Building gcc..."
mkdir -p build-gcc && cd build-gcc
../gcc-10.2.0/configure --target=$TARGET --prefix=$PREFIX --disable-nls --enable-languages=c,c++,go --without-headers
make all-gcc -j$(nproc)
make all-target-libgcc -j$(nproc)
make install-gcc
make install-target-libgcc

echo ""
echo "✅ Кросс-компилятор $TARGET-gcc установлен в $PREFIX"
echo "📌 Добавь в .bashrc:"
echo "    export PATH=\"$PREFIX/bin:\$PATH\""
