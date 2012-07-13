Roguelike
=========

Goal for this project is to make simple and clean roguelike-style game using C.

It is a coffeebreak roguelike, so don't expect to find complex in depth gameplay.

Build Notes
===========

## Requirements
* ncurses/pdcurses
* Any C compiler with c99 support. (Clang used on Linux builds)

Windows version is build using MinGW. Ncurses is replaced with pdcurses. Line 92 on curses.h needs to be commented out. stdbool.h is used for the boolean types instead.
Also add #include <stdbool.h> in that file. 

Author
======

Lauri Makinen
http://laurimakinen.net
lauri.sakari.makinen@gmail.com
