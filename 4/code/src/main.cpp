/*
 * @Description: 
 * @version: v1.0
 * @Author: HTY
 * @Date: 2022-04-07 11:50:46
 */

#include <iostream>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

#include "include/Segment.h"

void showImage(cv::Mat & mat, const std::string & win_name, cv::Size size, int wait_key=0, const std::string & save_path="",
               void (*pTrackbar)(cv::Mat & src, const std::string & win_name)=nullptr);

int main()
{
    // read image
    cv::Mat input_image = cv::imread("../../image/dragon.jpeg", cv::COLOR_BGR2GRAY);
    if (!input_image.data)
    {
        std::cout << "Path error!" << std::endl;
        return -1;
    }

    // set size for display
    cv::Size image_size = cv::Size(1500, 600);

    // original image
    showImage(input_image, "Original image", image_size);
    std::cout << "image size: " << input_image.size << std::endl;

    // binarize
    cv::Mat bin_image;
    cv::threshold(input_image, bin_image, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
    showImage(bin_image, "Binary image", image_size, 0, "../../result/bin_image.png");

    // edge
    cv::Mat edge_image;
    cv::Canny(input_image, edge_image, 100, 200);
    showImage(edge_image, "Edge image", image_size, 0, "../../result/edge_image.png");
    showImage(input_image, "Edge image", image_size, 0, "", Segment::cannyTrackbar);

    // contour
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(edge_image, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
    cv::Mat contour_image = cv::Mat::zeros(input_image.rows, input_image.cols, CV_8UC3);
    cv::drawContours(contour_image, contours, -1, cv::Scalar(100, 100, 100));
    showImage(contour_image, "Contour image", image_size, 0, "../../result/contour_image.png");

    // write to file
    std::ofstream outfile;
    outfile.open("../../result/contour_data.csv", std::ios::out);
    for (const auto & contour : contours)
    {
        for (auto point : contour)
        {
            outfile << "\"" << point << "\",";
        }
        outfile << std::endl;
    }

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

//    cv::destroyWindow(win_name);
}

