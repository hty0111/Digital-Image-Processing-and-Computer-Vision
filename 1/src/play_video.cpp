//
// Created by hty on 3/1/22.
//

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>


using namespace std;
using namespace cv;

int main()
{
    VideoCapture video("../../image/video.mp4");
    Mat image = imread("../../image/text.png");

    while (true)
    {
        Mat frame;
        video >> frame;

        Mat ROI = frame(Rect2i(100, 400, image.cols, image.rows));
        image.copyTo(ROI, image);

        namedWindow("play video");
        imshow("play video", frame);
        if (frame.empty())
            break;
        waitKey(15);
    }
}