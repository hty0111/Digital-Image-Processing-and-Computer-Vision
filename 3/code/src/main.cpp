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

    cv::Size image_size = cv::Size(1200, 900);

    Noise noise;
    showImage(input_image, "Input Image", image_size, 0);
    cv::Mat pepper_noise_image = noise.addPepperSaltNoise(input_image, input_image.cols * input_image.rows / 100);
    showImage(pepper_noise_image, "pepper_salt Noise", image_size, 0);

//    double sigma = 0;
    cv::Mat gauss_noise_image = noise.addGaussianNoise(input_image, 2, 0, 10);

//    cv::createTrackbar("Kernel Size", "imgOut", &sigma, 20, noise.gaussCallback);
    showImage(gauss_noise_image, "Gaussian Noise", image_size, 0);

    Filter filter;
    // Mean Filter
    cv::Mat mean_pepper_image = filter.meanFilter(pepper_noise_image, cv::Size(3, 3));
    showImage(mean_pepper_image, "Mean filter for pepper noise", image_size, 0);
    cv::Mat mean_gauss_image = filter.meanFilter(gauss_noise_image, cv::Size(3, 3));
    showImage(mean_gauss_image, "Mean filter for gaussian noise", image_size, 0);
    // Median Filter
    cv::Mat median_pepper_image = filter.medianFilter(pepper_noise_image, cv::Size(3, 3));
    showImage(median_pepper_image, "Median filter for pepper noise", image_size, 0);
    cv::Mat median_gauss_image = filter.medianFilter(gauss_noise_image, cv::Size(3, 3));
    showImage(median_gauss_image, "Median filter for gaussian noise", image_size, 0);
    // Bilateral Filter
    cv::Mat bilateral_pepper_image = filter.bilateralFilter(pepper_noise_image);
    showImage(bilateral_pepper_image, "Bilateral filter for pepper image", image_size, 0);
    cv::Mat bilateral_gauss_image = filter.bilateralFilter(gauss_noise_image);
    showImage(bilateral_pepper_image, "Bilateral filter for gaussian image", image_size, 0);

    return 0;
}
