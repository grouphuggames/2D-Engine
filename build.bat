@echo off

cd build
cmake ../
msbuild 2DEngine.sln /property:Configuration=Release
cd Release
Engine.exe
cd ../../