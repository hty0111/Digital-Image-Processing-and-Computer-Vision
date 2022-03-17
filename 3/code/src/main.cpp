//
// Created by hty on 3/13/22.
//

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "../include/Noise.h"
#include "../include/Filter.h"

void showImage(const cv::Mat & mat, const std::string & win_name, cv::Size size, int wait_key) {
    cv::namedWindow(win_name, cv::WINDOW_NORMAL);
    cv::resizeWindow(win_name, size);
    cv::imshow(win_name, mat);
    if (wait_key >= 0)
        cv::waitKey(wait_key);
}

int main()
{
    // read image
//    cv::Mat input_image = imread("../../image/manipulator.png", cv::IMREAD_GRAYSCALE);
    cv::Mat input_image = cv::imread("../../image/gate.jpg");
    if (!input_image.data)
    {
        std::cout << "Path error!" << std::endl;
        return -1;
    }

    Noise noise;
    showImage(input_image, "Input Image", cv::Size(1200, 900), 0);
    cv::Mat pepper_noise_image = noise.addPepperSaltNoise(input_image, input_image.cols * input_image.rows / 100);
    showImage(pepper_noise_image, "pepper_salt Noise", cv::Size(1200, 900), 0);

//    double sigma = 0;
    cv::Mat gauss_noise_image = noise.addGaussianNoise(input_image, 2, 0, 10);

//    cv::createTrackbar("Kernel Size", "imgOut", &sigma, 20, noise.gaussCallback);
    showImage(gauss_noise_image, "Gaussian Noise", cv::Size(1200, 900), 0);

    Filter filter;


    return 0;
}
