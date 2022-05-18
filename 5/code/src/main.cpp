/*
 * @Description: 
 * @version: v1.0
 * @Author: HTY
 * @Date: 2022-05-13 23:07:45
 */

#include <iostream>
#include <string>
#include "boost/format.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include "include/Morphology.h"

void showImage(cv::Mat & mat, const std::string & win_name, cv::Size size, int wait_key=0, const std::string & save_path="",
               void (*pTrackbar)(cv::Mat & src, const std::string & win_name)=nullptr);

int main(int argc, char* argv[])
{
    // read images
    boost::format read_fmt("../../images/%s.%s");
    boost::format save_fmt("../../images/%s_%s.png");
    std::string image_name, image_type;
    if (argc == 2)
    {
        image_name = argv[1];
        image_type = "png";
    }
    if (argc == 3)
    {
        image_name = argv[1];
        image_type = argv[2];
    }
    else
    {
        image_name = "dragon";
        image_type = "jpeg";
    }

    cv::Mat input_image = cv::imread((read_fmt % image_name % image_type).str(), cv::IMREAD_GRAYSCALE);
    if (!input_image.data)
    {
        std::cout << "Path error!" << std::endl;
        return -1;
    }
    else
        std::cout << "image size: " << input_image.size << std::endl;

    cv::Size image_size(1000, 400);
    showImage(input_image, "input images", image_size, 0,
              (save_fmt % image_name % "grey").str());

    Morphology morphology;
    // OSTU
    cv::Mat binary_image;
    morphology.OSTU(input_image, binary_image);
    showImage(binary_image, "binary images", image_size, 0,
              (save_fmt % image_name % "binary").str());

    // erosion
    cv::Mat erode_image;
    morphology.erode(binary_image, erode_image);
    showImage(erode_image, "erode images", image_size, 0,
              (save_fmt % image_name % "erode").str());

    // dilation
    cv::Mat dilate_image;
    morphology.dilate(binary_image, dilate_image);
    showImage(dilate_image, "dilate images", image_size, 0,
              (save_fmt % image_name % "dilate").str());

    // open operation
    cv::Mat open_image;
    morphology.open(binary_image, open_image, 5, 5);
    showImage(open_image, "open images", image_size, 0,
              (save_fmt % image_name % "open").str());

    // close operation
    cv::Mat close_image;
    morphology.close(binary_image, close_image, 5, 5);
    showImage(close_image, "close images", image_size, 0,
              (save_fmt % image_name % "close").str());

    // HMT
    cv::Mat hmt_image;
    morphology.HMT(close_image, hmt_image);
    showImage(hmt_image, "HMT images", image_size, 0,
              (save_fmt % image_name % "hmt").str());

    return 0;
}

void showImage(cv::Mat & mat, const std::string & win_name, cv::Size size, int wait_key,
               const std::string & save_path, void (*pTrackbar)(cv::Mat& src, const std::string & win_name))
{
    if (!size.empty() and size != cv::Size(0, 0))
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

//    cv::destroyWindow(win_name);
}
