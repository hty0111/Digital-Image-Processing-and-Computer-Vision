/*
 * @Description: Homework of Machine Vision [histogram equalization]
 * @version: v1.0
 * @Author: HTY
 * @Date: 2022-03-14 23:05:10
 */

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "../include/Equalization.h"

void showImage(const cv::Mat & mat, const std::string & win_name, cv::Size size, int wait_key=0, const std::string & save_path="");

int main()
{
    // read image
    cv::Mat gate_image = imread("../../image/gate.jpg", cv::IMREAD_GRAYSCALE);
    cv::Mat arm_image = imread("../../image/arm.png", cv::IMREAD_GRAYSCALE);
    if (!gate_image.data && !arm_image.data)
    {
        std::cout << "Path error!" << std::endl;
        return -1;
    }

    // display original image
    showImage(gate_image, "Gate", cv::Size(600, 400),
              -1, "../../image/gate_grey.png");
    showImage(arm_image, "Arm", cv::Size(300, 400),
              0, "../../image/arm_grey.png");

    Equalization equalization;

    // display histogram of original image
    cv::Mat gate_hist, arm_hist;

    equalization.getHistMat(gate_image, gate_hist);
    equalization.getHistMat(arm_image, arm_hist);
    showImage(gate_hist, "Gate Histogram", cv::Size(500, 500),
              -1, "../../image/gate_hist.png");
    showImage(arm_hist, "Arm Histogram", cv::Size(500, 500),
              0, "../../image/arm_hist.png");

    // histogram equalize
    cv::Mat gate_equ, arm_equ;
    equalization.equalize(gate_image, gate_equ);
    equalization.equalize(arm_image, arm_equ);

    // display equalized image
    showImage(gate_equ, "Equalized Gate", cv::Size(1200, 900),
              -1, "../../image/gate_equ.png");
    showImage(arm_equ, "Equalized Arm", cv::Size(300, 400),
              0, "../../image/arm_equ.png");
    
    // display histogram of equalized image
    cv::Mat gate_equ_hist, arm_equ_hist;
    equalization.getHistMat(gate_equ, gate_equ_hist);
    equalization.getHistMat(arm_equ, arm_equ_hist);
    showImage(gate_equ_hist, "Equalized Gate Histogram", cv::Size(500, 500),
              -1, "../../image/gate_equ_hist.png");
    showImage(arm_equ_hist, "Equalized Arm Histogram", cv::Size(500, 500),
              0, "../../image/arm_equ_hist.png");

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