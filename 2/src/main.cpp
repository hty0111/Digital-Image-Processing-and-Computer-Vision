//
// Created by hty on 3/13/22.
//

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "../include/equalization.h"

int main()
{
    // 读取图像
//    cv::Mat input_image = imread("../../image/manipulator.png", cv::IMREAD_GRAYSCALE);
    cv::Mat input_image = imread("../../image/gate.jpg", cv::IMREAD_GRAYSCALE);
    if (!input_image.data)
    {
        std::cout << "Path error!" << std::endl;
        return -1;
    }

    // 显示原始图像和灰度图
    Equalization input(input_image);
    input.showImage(input.image, "Input Image");
    input.showImage(input.histogram, "Input Histogram", cv::Size(500, 500), 0);

    // 对图像实施均衡化操作
    cv::Mat result = input.equalize();
    Equalization output(result);

    // 显示均衡化后的图像和灰度图
    output.showImage(output.image, "Equalized Image");
    output.showImage(output.histogram, "Equalized Histogram", cv::Size(500, 500), 0);

    return 0;
}
