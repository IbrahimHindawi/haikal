@echo off
mkdir build
pushd build
cl  /nologo /Za /Zi ..\source\main.c
popd build
