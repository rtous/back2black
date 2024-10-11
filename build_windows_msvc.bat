REM cmake -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER=C:/MinGW/bin/g++.exe -DCMAKE_C_COMPILER=C:/MinGW/bin/gcc.exe -DBUILD_SHARED_LIBS=OFF -B build_windows -DCMAKE_PREFIX_PATH=C:/SDL -S .
REM trace mode: 
REM cmake -G "Visual Studio 17 2022" -DCMAKE_BUILD_TYPE=RelWithDebInfo --trace -DCMAKE_CXX_COMPILER="C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\MSBuild\Current\Bin\MSBuild.exe" --debug-find -DBUILD_SHARED_LIBS=OFF -B build_windows -DCMAKE_PREFIX_PATH=C:/SDL -S .

REM discarded -DCMAKE_BUILD_TYPE=RelWithDebInfo

REM trace: --trace
REM debug find packages: --debug-find

cmake -G "Visual Studio 17 2022" -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_CXX_COMPILER="C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\MSBuild\Current\Bin\MSBuild.exe" -DBUILD_SHARED_LIBS=OFF -B build_windows -DCMAKE_PREFIX_PATH=C:/SDL -S .

cmake --build build_windows
