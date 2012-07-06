@echo off

premake4 gmake

PATH=%PATH%;C:/MinGW/bin
mingw32-make
