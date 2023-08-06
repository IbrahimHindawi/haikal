@echo off

mkdir build
pushd build

cmake .. "-DCMAKE_TOOLCHAIN_FILE=C:\devel\vcpkg\scripts\buildsystems\vcpkg.cmake"
powershell -Command ..\scripts\clang-build.ps1 -export-jsondb
rem python ..\scripts\finalize.py

popd build
