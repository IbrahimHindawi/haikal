@echo off
mkdir build
pushd build
cl  /nologo /Za /Zi ..\source\main.c ..\source\list.c ..\source\Array.c ..\source\xstring.c
popd build
