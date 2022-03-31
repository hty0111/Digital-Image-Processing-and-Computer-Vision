//
// Created by hty on 3/13/22.
//

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "../include/Noise.h"
#include "../include/Filter.h"

void showImage(const cv::Mat & mat, const std::string & win_name, cv::Size size, int wait_key, const std::string & save_path="");

int main()
{
    // read image
    cv::Mat input_image = cv::imread("../../image/gate.jpg");
    if (!input_image.data)
    {
        std::cout << "Path error!" << std::endl;
        return -1;
    }

    // set image size for display
    cv::Size image_size = cv::Size(1200, 900);

    Noise noise;
    // Pepper noise
    showImage(input_image, "Input Image", image_size, 0);
    cv::Mat pepper_noise_image = noise.addPepperSaltNoise(input_image, input_image.cols * input_image.rows / 100);
    showImage(pepper_noise_image, "pepper noise",
              image_size, 0, "../../image/pepper_noise.png");
    // Gaussian noise
    cv::Mat gauss_noise_image = noise.addGaussianNoise(input_image, 2, 0, 10);
    showImage(gauss_noise_image, "Gaussian noise",
              image_size, 0, "../../image/gauss_noise.png");

    Filter filter;
    // Mean Filter
    cv::Mat mean_pepper_image = filter.meanFilter(pepper_noise_image, cv::Size(5, 5));
    showImage(mean_pepper_image, "Mean filter for pepper noise",
              image_size, 0, "../../image/mean_pepper.png");
    cv::Mat mean_gauss_image = filter.meanFilter(gauss_noise_image, cv::Size(5, 5));
    showImage(mean_gauss_image, "Mean filter for gaussian noise",
              image_size, 0, "../../image/mean_gauss.png");
    // Median Filter
    cv::Mat median_pepper_image = filter.medianFilter(pepper_noise_image, cv::Size(3, 3));
    showImage(median_pepper_image, "Median filter for pepper noise",
              image_size, 0, "../../image/median_pepper.png");
    cv::Mat median_gauss_image = filter.medianFilter(gauss_noise_image, cv::Size(3, 3));
    showImage(median_gauss_image, "Median filter for gaussian noise",
              image_size, 0, "../../image/median_gauss.png");
    // Bilateral Filter
//    cv::Mat bilateral_pepper_image = filter.bilateralFilter(pepper_noise_image);
    cv::Mat bilateral_pepper_image;
    filter.bilateralBlur(input_image, bilateral_pepper_image, 1, 1, 3);
    showImage(bilateral_pepper_image, "Bilateral filter for pepper image", image_size, 0);
//    cv::Mat bilateral_gauss_image = filter.bilateralFilter(gauss_noise_image);
//    showImage(bilateral_pepper_image, "Bilateral filter for gaussian image", image_size, 0);

    return 0;
}


void showImage(const cv::Mat & mat, const std::string & win_name, cv::Size size, int wait_key, const std::string & save_path)
{
    cv::namedWindow(win_name, cv::WINDOW_NORMAL);
    cv::resizeWindow(win_name, size);
    cv::imshow(win_name, mat);
    if (wait_key >= 0)
        cv::waitKey(wait_key);
    if (!save_path.empty())
        cv::imwrite(save_path, mat);
}
