# 使用说明

## 1. 工程目录

​	|— `README.md`
​	|— `src` 
​      	  |— `main.cpp`
​			|— `Segment.cpp`
​	|— `include`
​			|— `Segment.h`	
​	|— `image`
​			|— `dragon.jpeg`
​	|— `result`
​			|— `contour_data.csv`
​	|— `build`
​			|— `bin`	
​					|— `segment`

## 2. 环境配置

1. 运行环境： `Ubuntu18.04`, `python3`, `gcc/g++`, `cmake3.2`
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
sudo chmod u+x ./filter
./filter
```

​		运行后按任意键显示下一幅图片，**注意：如果卡了请耐心等待**。

## 4. 命名规则

- 变量一律使用下划线式命名。
- 函数一律使用驼峰式命名。
- 类名一律大写。
- 由于没有使用`cv,std`等命名空间，所以内置变量类型和函数都由其命名空间开头，与自己定义的很好区分。

## 5. 版本

| Version number | Updated time | Updated content                      |
| -------------- | ------------ | ------------------------------------ |
| v1.0           | 2022.4.8     | 完成二值化、轮廓提取、链码提取等操作 |

## 6. 作者

​		**HTY**	2022.4.8

