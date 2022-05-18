# 使用说明

## 1. 工程目录

​	|— `README.md`

​	|— `src` 

​      	  |— `main.cpp`

​			|— `Morphology.cpp`

​	|— `include`

​			|— `Morphology.h`	

​	|— `images`

​			|— `original_image.jpeg`

​			|— `binary_image.png`

​			|— `erosion_image.png`

​			|— `dilation_image.png`

​			|— `open_image.png`

​			|— `close_image.png`

​			|— `hmt_image.png`

​	|— `build`

​			|— `bin`	

​					|— `morphology_filter`

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
sudo chmod u+x ./morphology_filter
./morphology_filter [--image_name] [--image_type]
```

​		默认读入读片为`dragon.jpeg`，当只输入一个参数`image_name`时，默认的图片后缀为`png`。	

​		运行后按任意键显示下一幅图片，**注意：HMT变换需要一定时间，请耐心等待**。

## 4. 命名规则

- 变量一律使用下划线式命名。
- 函数一律使用驼峰式命名。
- 类名一律大写。
- 由于没有使用`cv,std`等命名空间，所以内置变量类型和函数都由其命名空间开头，与自己定义的很好区分。

## 5. 版本

| Version number | Updated time | Updated content                       |
| -------------- | ------------ | ------------------------------------- |
| v1.0           | 2022.5.13    | 大津法二值化                          |
| v2.0           | 2022.5.16    | 腐蚀、膨胀、开闭运算                  |
| v3.0           | 2022.5.17    | HMT变换                               |
| v3.1           | 2022.5.17    | 加入`boost::format`格式化输入输出文件 |
| v3.2           | 2022.5.18    | 加入命令行参数设置图像名称和后缀      |

## 6. 作者

​		**HTY**	2022.5.18

