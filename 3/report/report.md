# 添加噪声并滤波

​	**姓名**： 胡天扬

​	**学号**： 3190105708

​	**专业**： 自动化（控制）

​	**课程**： 数字图像处理与机器视觉	

​	**指导教师**： 姜伟



## 一、题目要求

​		任意选取1张图像，添加高斯噪声和椒盐噪声。实现均值滤波、中值滤波和双边滤波等去噪方法，比较各方法去噪结果，并分析各去噪方法的特点。



## 二、原图

​		原图是一张三通道的彩色图，像素为`1080 × 1440。

！！图片



## 三、添加噪声

​		有关噪声的函数封装在了`Noise`类中。

### 3.1 椒盐噪声

​		椒盐噪声即黑色和白色的像素点。使用`rand()`函数随机选取像素点，并根据传入的噪声像素点数量`num_noise`分别添加黑点和白点。

```cpp
cv::Mat Noise::addPepperSaltNoise(const cv::Mat & input, int num_noise)
{
    cv::Mat output = input.clone();

    // add black and white dot
    for (int i = 0; i < num_noise; i++) 
    {
        int x = rand() % input.rows;
        int y = rand() % input.cols;
        if (i % 2 == 0)     // half for black
            for (int j = 0; j < 3; j++)
                output.at<cv::Vec3b>(x, y)[j] = 0;
        else                // half for white
            for (int j = 0; j < 3; j++)
                output.at<cv::Vec3b>(x, y)[j] = 255;
    }
    return output;
}
```

！！图片

### 3.2 高斯噪声

​		高斯分布的概率密度函数为 $p(z)=\frac{1}{\sqrt{2 \pi} \sigma} e^{-(z-\bar{z})^{2} / 2 \sigma^{2}}$，这里用 $Box-Muller$ 变换来构建服从高斯分布的随机变量。具体描述为：选取两个服从`[0, 1]`上均匀分布的随机变量 $U_1、U_2$，则有
$$
X=\cos \left(2 \pi U_{1}\right) \sqrt{-2 \ln U_{2}} \\
Y=\sin \left(2 \pi U_{1}\right) \sqrt{-2 \ln U_{2}}
$$
$X、Y$服从均值为0、方差为1的高斯分布。具体的数学证明就略过了。

​		代码如下，首先根据上述公式生成服从指定均值和方差的高斯分布序列，然后对每个像素点添加高斯噪声。

```cpp
double Noise::generateGaussianSeq(double miu, double sigma) {
    // Box-Muller transform
    double u1, u2, std_norm_result;
    do {
        u1 = (double) rand() / RAND_MAX;
        u2 = (double) rand() / RAND_MAX;
        std_norm_result = cos(2 * CV_PI * u1) * sqrt(-2.0 * log(u2));
    } while (std_norm_result > 1.0 || std_norm_result == 0.0);
    return miu + sigma * std_norm_result;
}

cv::Mat Noise::addGaussianNoise(const cv::Mat &input, int gain, double miu, double sigma)
{
    cv::Mat output = input.clone();

    for (int i = 0; i < output.rows; i++)
        for (int j = 0; j < output.cols; j++)
            for (int channel = 0; channel < 3; channel++)
                output.at<cv::Vec3b>(i, j)[channel] += gain * generateGaussianSeq(miu, sigma);

    return output;
}
```

！！图片



## 四、滤波

### 4.1 均值滤波

​		均值滤波里面的种类也有很多，比如算术均值、几何均值、谐波均值、逆谐波均值，这里就用最常规的算术均值滤波，公式如下：
$$
f(x, y)=\frac{1}{m n} \sum_{(s, t) \in S_{x, y}} g(s, t)
$$
​		首先计算出待求像素到核边缘的距离`center2edge`，从而忽略边缘点的处理。为了代码的简洁考虑，在为`kernel`区域的像素值求和时，使用了`ROI`区域+`cv::sum()`的方法，可能时间复杂度会增加，但实际运行时没有影响。

```cpp
cv::Mat Filter::meanFilter(const cv::Mat & input, cv::Size kernel_size)
{
    cv::Mat output = input.clone();
    cv::Mat ROI;

    // ignore edge pixel
    int center2edge = (kernel_size.height - 1 ) / 2;
    for (int i = center2edge; i < output.rows - center2edge; i++)
        for (int j = center2edge; j < output.cols - center2edge; j++)
            for (int k = 0; k < 3; k++)
            {
                // choose kernel area
                ROI = output(cv::Range(i - center2edge, i + center2edge + 1),
                             cv::Range(j - center2edge, j + center2edge + 1));
                // substitute for original pixel
                output.at<cv::Vec3b>(i, j)[k] = (int) cv::sum(ROI).val[k] / (kernel_size.height * kernel_size.width);
            }

    return output;
}
```

！！图片



### 4.2 中值滤波

​		中值滤波就是把区域中的中值作为新的像素值，这部分代码略长，因为`opencv`中的排序函数`cv::sort()`只能针对行或列，不能同时针对整片区域，所以实际操作时先根据`kernel`取出`ROI`区域，`flatten`之后再交给`cv::sort`处理，但是有三点需要注意：

1. `cv::sort()`只能针对单通道，所以需要先拆分三通道。
2. `reshape()`只能针对连续值矩阵，而`ROI`恰好不是连续的，所以需要`clone()`出新的连续矩阵。
3. `cv::Mat`变量的定义要放在最外层循环外，否则会导致运行效率非常低。

```cpp
cv::Mat Filter::medianFilter(const cv::Mat & input, cv::Size kernel_size)
{
    cv::Mat output = input.clone();
    cv::Mat flattened, sorted, ROI, channel[3];

    // split channels
    cv::split(output, channel);

    // ignore edge pixel
    int center2edge = (kernel_size.height - 1 ) / 2;
    for (int i = center2edge; i < output.rows - center2edge; i++)
        for (int j = center2edge; j < output.cols - center2edge; j++)
            for (int k = 0; k < 3; k++)
            {
                // choose kernel area
                ROI = channel[k](cv::Range(i - center2edge, i + center2edge + 1),
                        cv::Range(j - center2edge, j + center2edge + 1));
                // ROI region is not continuous, but 'reshape' requires continuous matrix
                if (!ROI.isContinuous())
                    ROI = ROI.clone();
                // flatten and sort
                flattened = ROI.reshape(1, 1);
                cv::sort(flattened, sorted, cv::SORT_EVERY_ROW + cv::SORT_ASCENDING);
                // substitute for original pixel
                output.at<cv::Vec3b>(i, j)[k]= sorted.at<cv::Vec3b>(0, center2edge)[k];
            }
    
    return output;
}
```



### 4.3 双边滤波







## 六、总结

​		虽说直方图均衡化可以增强对比度，但同时也使得图像蕴含的信息减少。当原始图像的灰度分布比较均衡时，增强对比度的效果较好，只会损失小部分信息；而当原始图像本身的灰度值都集中在低灰度区时，根据均衡化公式的定义，由于低灰度区的频率很大，在计算分布函数时会使得后续的频率也增大，使得映射函数将其映射到高灰度区，图像也就会发白发亮，对于这种情况，其实更应该用**直方图规定化**来处理。