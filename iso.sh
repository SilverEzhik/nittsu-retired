#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/nittsu.kernel isodir/boot/nittsu.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "nittsu" {
	multiboot /boot/nittsu.kernel
}
EOF
grub2-mkrescue -o nittsu.iso isodir
