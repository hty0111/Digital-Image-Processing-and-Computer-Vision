/*
 * @Description: 
 * @version: v1.0
 * @Author: HTY
 * @Date: 2022-03-17 20:02:06
 */

#ifndef CODE_FILTER_H
#define CODE_FILTER_H

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>

class Filter {
public:
    class Data
    {
    public:
        cv::Mat src;
        std::string win_name;
    };

    void meanFilter(const cv::Mat & src, cv::Mat & dst, int kernel_size);
    void medianFilter(const cv::Mat & src, cv::Mat & dst, int kernel_size);
    void bilateralFilter(const cv::Mat & src, cv::Mat & dst, int kernel_size, double sigmaR, double sigmaD);

    static void meanTrackbar(cv::Mat & src, const std::string & win_name);
    static void meanCallback(int kernel_size, void * data);
    static void medianTrackbar(cv::Mat & src, const std::string & win_name);
    static void medianCallback(int kernel_size, void * data);
};


#endif //CODE_FILTER_H
