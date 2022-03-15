# 使用说明

## 1. 工程目录

​	|— `src` 
​      	  |— `show_image.cpp`
​	    	|— `play_video.cpp`	
​	|— `build`
​			|— `bin`	
​					|— `shoe_image`
​					|— `play_video`
​	|— `image`
​			|— `image.jpg`
​			|— `video.mp4`

## 2. 编译运行

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

