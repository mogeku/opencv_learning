set HOME=%cd%
set EXTRA_MODULES_PATH="%HOME%\..\..\modules\opencv_contrib\modules"
set CMAKE_GENERATOR_OPTIONS=-G"Visual Studio 16 2019" -A "x64"
set CMAKE_OPTIONS=-DBUILD_PERF_TESTS=OFF ^
                  -DBUILD_TESTS=OFF ^
                  -DBUILD_DOCS=OFF  ^
                  -DWITH_CUDA=OFF ^
                  -DBUILD_EXAMPLES=OFF ^
                  -DINSTALL_CREATE_DISTRIB=ON

call cmake %CMAKE_GENERATOR_OPTIONS% %CMAKE_OPTIONS% ^
           -DOPENCV_EXTRA_MODULES_PATH=%EXTRA_MODULES_PATH% ^
           -B %HOME%\BuildOpencv ^
           %HOME%\..\..\modules\opencv
