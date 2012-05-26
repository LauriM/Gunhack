Major todo
==========

* better world generation
* ingame message system
* item colors
* item dropping/applying
* item trigger coding (a,b,c etc)

Malloc todo
===========
* save system rebuild

Battle system
=============
* change meleeDmg to be range, not static
* drops from enemies

Minor todo
==========

* Save system is not portable
* more FALSE TRUE stuff
* HUD improvements
* ColorData doesn't respect visdata
* npcs won't block pathfind

Gameplay
========

* Explosives to dig trough the dungeon
* Brutal blood effects

Plan
====

* Clean and solid C-code
* Simple roguelike implementation with rapid gameplay
* Gameplay should be possible to be developed forward without much challenge
* Overall plan: get that "one-game" out before the army ends

Notes
=====

Windows version is build using MinGW. Ncurses is replaced with pdcurses. Line 92 on curses.h needs to be commented out. stdbool.h is used for the boolean types instead.
