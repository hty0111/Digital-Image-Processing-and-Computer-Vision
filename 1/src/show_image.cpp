//
// Created by hty on 2/23/22.
//

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;

int main()
{
    Mat image = imread("../../image/image.png");
    namedWindow("Original Manipulator");
    imshow("Original Manipulator", image);

    Mat result;
    cvtColor(image, result, COLOR_BGR2GRAY );
    namedWindow("Mono_colored Manipulator");
    imshow("Mono_colored Manipulator", result);

    waitKey(0);

    return 0;
}
