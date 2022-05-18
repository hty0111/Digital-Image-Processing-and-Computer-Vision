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
#include "include/Restore.h"

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
        image_name = "input_image";
        image_type = "bmp";
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
    showImage(input_image, "input images", image_size);


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
