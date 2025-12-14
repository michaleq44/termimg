#!/usr/bin/sh
pacman -Syu --noconfirm
pacman -S git ncurses sudo --noconfirm
cd /termimg/pkg || exit 1
useradd -M michaleq || true
runuser -l michaleq -c 'makepkg -d'
rm -rf pkg src PKGBUILD archonubuntu.sh