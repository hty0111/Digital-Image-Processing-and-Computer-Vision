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

## 4. 命名规则

- 变量一律使用下划线式命名。如`_image`表示原始灰度图，`_hist`表示原始直方图，`_equ`表示均衡化后图像，`_equ_hist`表示均衡化后直方图。（gray&grey有些混用，美英和英英不小心混了）
- 函数一律使用驼峰式命名。如`getHistMat`，`showImage`。
- 类名一律大写。如`Equalization`。
- 由于没有使用`cv,std`等命名空间，所以内置变量类型和函数都由其命名空间开头，与自己定义的很好区分。

## 5. 版本

| Version number | Updated time | Updated content                            |
| -------------- | ------------ | ------------------------------------------ |
| v1.2           | 2022.3.28    | 调整图片显示位置，使用`cv::InputArray`传参 |
| v1.1           | 2022.3.20    | 封装，规范化命名，添加保存图像操作         |
| v1.0           | 2022.3.14    | 完成直方图绘制和均衡化操作                 |

## 6. 作者

​		**HTY**	2022.3.30
