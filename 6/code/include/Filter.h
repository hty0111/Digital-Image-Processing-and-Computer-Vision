/*
 * @Description: 
 * @version: v1.0
 * @Author: HTY
 * @Date: 2022-05-18 14:37:20
 */


#ifndef CODE_FILTER_H
#define CODE_FILTER_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <cmath>

class Filter {
private:
    int angle;
    int offset;
public:
    Filter(int angle, int offset);
    void wiener(const cv::Mat & src, cv::Mat & dst);
    void getPSF(const cv::Mat &src, cv::Mat & PSF);
};


#endif //CODE_FILTER_H
