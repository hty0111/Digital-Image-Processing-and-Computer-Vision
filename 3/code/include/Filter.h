//
// Created by hty on 3/17/22.
//

#ifndef CODE_FILTER_H
#define CODE_FILTER_H

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>

class Filter {
    cv::Mat meanFilter(const cv::Mat & input, cv::Size kernel_size);
    cv::Mat medianFilter(const cv::Mat & input, cv::Size kernel_size);
    cv::Mat bilateralFilter(const cv::Mat & input, cv::Size kernel_size);

};


#endif //CODE_FILTER_H
