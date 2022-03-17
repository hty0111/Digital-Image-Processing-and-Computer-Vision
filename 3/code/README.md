# 使用说明

## 1. 工程目录

​	|— `README.md`
​	|— `src` 
​      	  |— `main.cpp`
​			|— `equalization.cpp`
​	|— `include`
​			|— `equalization.h`
​	|— `build`
​			|— `bin`	
​					|— `histogram_equalization`
​	|— `image`
​			|— `gate.jpg`
​			|— `manipulator.png`

## 2. 环境配置

1. 运行环境： `python3`, `gcc/g++`, `cmake3.2`
2. 使用的库： `opencv`

## 3. 编译运行

​		已保留可执行文件，可在 `/build/bin` 目录下直接运行。

```shell
# 根据CMakeLists.txt进行编译
cd build
cmake ..
make
# 在binary目录下运行可执行文件，否则会出现路径错误
cd bin
./<executable file>	
```

