cmake -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER=C:/MinGW/bin/g++.exe -DCMAKE_C_COMPILER=C:/MinGW/bin/gcc.exe -DBUILD_SHARED_LIBS=OFF -B build_windows -DCMAKE_PREFIX_PATH=C:/SDL -S .
REM cmake -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER=C:/MinGW/bin/g++.exe -DCMAKE_C_COMPILER=C:/MinGW/bin/gcc.exe -DBUILD_SHARED_LIBS=OFF -DSDL2_PATH="C:/SDL" -B build_windows -S .
cmake --build build_windows
