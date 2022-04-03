/*
 * @Description: 
 * @version: v1.0
 * @Author: HTY
 * @Date: 2022-03-17 16:49:16
 */

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "../include/Noise.h"
#include "../include/Filter.h"

void showImage(cv::Mat & mat, const std::string & win_name, cv::Size size, int wait_key=0, const std::string & save_path="",
               void (*pTrackbar)(cv::Mat & src, const std::string & win_name)=nullptr);

int main()
{
    // read image
    cv::Mat input_image = cv::imread("../../image/gate.jpg");
    if (!input_image.data)
    {
        std::cout << "Path error!" << std::endl;
        return -1;
    }

    // set size for display
    cv::Size image_size = cv::Size(1200, 1200);

    // original image
    showImage(input_image, "Original image", image_size, 0);


    Noise noise;
    // Pepper noise
    cv::Mat pepper_noise_image;
    noise.addPepperNoise(input_image, pepper_noise_image, input_image.cols * input_image.rows / 100);
    showImage(input_image, "Pepper noise", image_size, 0, "", Noise::pepperTrackbar);

    // Gauss noise
    cv::Mat gauss_noise_image;
    noise.addGaussNoise(input_image, gauss_noise_image, 2, 0, 5);
    showImage(input_image, "Gauss noise", image_size, 0, "", Noise::gaussTrackbar);


    Filter filter;
    // Mean Filter
    cv::Mat mean_pepper_image, mean_gauss_image;
    filter.meanFilter(pepper_noise_image, mean_pepper_image, 3);
    filter.meanFilter(gauss_noise_image, mean_gauss_image, 3);
    showImage(pepper_noise_image, "Mean filter for pepper noise", image_size, 0, "", Filter::meanTrackbar);
    showImage(gauss_noise_image, "Mean filter for Gauss noise",image_size, 0, "", Filter::meanTrackbar);

    // Median Filter
    cv::Mat median_pepper_image, median_gauss_image;
    filter.medianFilter(pepper_noise_image, median_pepper_image, 3);
    filter.medianFilter(gauss_noise_image, median_gauss_image, 3);
    showImage(pepper_noise_image, "Median filter for pepper noise", image_size, 0, "", Filter::medianTrackbar);
    showImage(gauss_noise_image, "Median filter for Gauss noise", image_size, 0, "", Filter::medianTrackbar);

    // Bilateral Filter
    cv::Mat bilateral_pepper_image, bilateral_gauss_image;
    filter.bilateralFilter(pepper_noise_image, bilateral_pepper_image, 7, 20, 20);
    showImage(bilateral_pepper_image, "Bilateral filter for pepper image", image_size);
    filter.bilateralFilter(gauss_noise_image, bilateral_gauss_image, 7, 20, 20);
    showImage(bilateral_gauss_image, "Bilateral filter for Gauss image", image_size);

    return 0;
}


void showImage(cv::Mat & mat, const std::string & win_name, cv::Size size, int wait_key,
               const std::string & save_path, void (*pTrackbar)(cv::Mat& src, const std::string & win_name))
{
    if (!size.empty())
    {
        cv::namedWindow(win_name, cv::WINDOW_NORMAL);
        cv::resizeWindow(win_name, size);
    }
    else
        cv::namedWindow(win_name, cv::WINDOW_AUTOSIZE);

    cv::imshow(win_name, mat);
    if (pTrackbar != nullptr)
        (*pTrackbar)(mat, win_name);
    if (wait_key >= 0)
        cv::waitKey(wait_key);
    if (!save_path.empty())
        cv::imwrite(save_path, mat);

    cv::destroyWindow(win_name);
}
