//
// Created by hty on 3/13/22.
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "../include/equalization.h"

using namespace cv;
using namespace std;

int main()
{

//    Mat input = imread("../../image/manipulator.png");
    Mat gate = imread("../../image/gate.jpg", 0);
//    cout << input.size << endl;
    if (!gate.data)
    {
        cout << "Path error!" << endl;
        return -1;
    }

    Equalization input(gate);
    input.showImage(input.image, "Gate Image");
    input.showImage(input.histogram, "Gate Histogram", 0);

    Mat result = input.equalize();
    Equalization output(result);

    output.showImage(output.image, "Equalized Image");
    output.showImage(output.histogram, "Equalized Histogram", 0);

    return 0;
}
