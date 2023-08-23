@echo off

rem WARNING:
rem Must be executed from project root directory using `scripts\bbuild.bat`
rem TODO(Ibrahim): Implement Debug and Release to CompileCommand.

rem UserParameters:
set CMakeToolchainFile="C:\devel\vcpkg\scripts\buildsystems\vcpkg.cmake"

rem Setup Script Variables
for %%I in (.) do set ProjectName=%%~nxI
rem echo %ProjectName%
set CompileCommand=msbuild build\%ProjectName%.sln -nologo -m -v:m /property:Configuration=Debug /property:VcpkgEnabled=false

rem Program Begin
echo Use -h to display available commands.
echo:
goto GETOPTS

:Help
echo -b to build solution. 
echo -c to compile. 
echo -cr to compile and run.
echo -r to run exe.
echo -s to open solution.
echo -x to clean up.
goto :eof

:Build
mkdir build
pushd build
cmake .. "-DCMAKE_TOOLCHAIN_FILE=%CMakeToolchainFile%" "-DCMAKE_BUILD_TYPE=Debug" "-DProjectNameParam:STRING=%ProjectName%"
powershell -Command ..\scripts\clang-build.ps1 -export-jsondb
popd build
goto :eof

:Compile
%CompileCommand%
goto :eof

:CompileRun
%CompileCommand%
build\Debug\%ProjectName%.exe
goto :eof

:Run
build\Debug\%ProjectName%.exe
goto :eof

:CleanUp
echo Destroy build folder.
rmdir /S /Q build
goto :eof

:Solution
build\%ProjectName%.sln
goto :eof

:GETOPTS
if /I "%1" == "-h" call :Help
if /I "%1" == "-b" call :Build
if /I "%1" == "-c" call :Compile
if /I "%1" == "-cr" call :CompileRun
if /I "%1" == "-r" call :Run
if /I "%1" == "-s" call :Solution
if /I "%1" == "-x" call :CleanUp
shift
if not "%1" == "" call :Epilogue
:Epilogue
goto :eof
