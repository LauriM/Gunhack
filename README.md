Roguelike
=========

Goal for this project is to make simple and clean roguelike-style game using C.

It is a coffeebreak roguelike, so don't expect to find complex in depth gameplay.

Project page with binaries and screenshots is on my [website](http://www.laurimakinen.net/projects/gunhack.html).

This game is "finished". Meaning that I will not actively develop it. However, if you make a patch, I am happy to include it if its good.

Build Notes
===========

## Requirements
* ncurses/pdcurses
* premake4
* Any C compiler with c99 support. (Clang used on Linux builds)

Windows version is build using MinGW. Ncurses is replaced with pdcurses. Line 92 on curses.h needs to be commented out. stdbool.h is used for the boolean types instead.
Also add #include <stdbool.h> in that file. 

First run premake and then make/whatever build system you decide to use. Windows libraries are hardcoded so you may need to fix that in the premake config.

License
=======

See the LICENSE.txt for full license.

Author
======

* Lauri Makinen
* http://laurimakinen.net
* lauri.sakari.makinen@gmail.com
