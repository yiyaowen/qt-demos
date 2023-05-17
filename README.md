# Qt.TrayIcon.Demo

## 依赖

Qt5，要求 CMake 最低版本 3.10，支持 C++11 的编译器。

## 多平台构建

进入到项目文件夹，然后：

```
mkdir build
cd build
```

### Windows

Windows 上如果配置了环境变量，工具链都有了，直接：

```
cmake ..
make
```

但是一般会出错，需要手动写下路径，Windows 上一般用 MinGW 或者 MSVC，配置方法差不多。

#### MinGW

```
> cmake .. -DCMAKE_PREFIX_PATH=YOUR_Qt_PATH -DCMAKE_CXX_COMPILER=YOUR_g++.exe_PATH -DCMAKE_MAKE_PROGRAM=YOUR_make.exe_PATH -G "MinGW Makefiles"
> make 
```

make.exe 的具体名称可在 MinGW 的 bin 目录下找到，x86_64 的 MinGW 也许叫 mingw32-make.exe，32 位的一般就是 make.exe。

例如：Qt mingw 安装在 "D:/Qt/5.15.0/mingw"，g++ 在 "D:/MinGW/bin/g++.exe"，make 在 "D:/MinGW/bin/make.exe"，则执行如下步骤：（CMake 在 Windows 上也用 “/”）

```
> cmake .. -DCMAKE_PREFIX_PATH="D:/Qt/5.15.0/mingw" -DCMAKE_CXX_COMPILER="D:/MinGW/bin/g++.exe" -DCMAKE_MAKE_PROGRAM="D:/MinGW/bin/make.exe" -G "MinGW Makefiles"
```

#### MSVC

将上面的 g++ 编译器路径改为 MSVS 对应的编译器路径，make 路径改为对应的 make 路径（MSVC 的 make.exe 一般是 nmake.exe，名字稍有不同，在 VS 的路径下也可以找到），生成器不确定是什么，可以故意输错命令看一下 MSVC 对应的 Generator 是什么，比如用 cmake .. -G "asdf"，然后看一下错误输出里的提示，然后将 -G "MinGW Makefiles" 换成对应的生成器。

## Unix

macOS 和 Linux 的构建方式相同

```
cmake ..
make
```

一般工具链都配置好了，直接执行就行，如果报错再按照 Windows 一样配置路径就好了。
