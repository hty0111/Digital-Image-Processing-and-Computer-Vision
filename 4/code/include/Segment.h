/*
 * @Description: 
 * @version: v1.0
 * @Author: HTY
 * @Date: 2022-04-08 11:00:10
 */

#ifndef CODE_SEGMENT_H
#define CODE_SEGMENT_H

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>

class Segment {
public:
    class Canny_Data
    {
    public:
        cv::Mat src;
        std::string win_name;
        int low_threshold = 0, high_threshold = 0;
    };

    static void cannyTrackbar(cv::Mat & src, const std::string & win_name);
    static void cannyLowThrCallback(int low_threshold, void * data);
    static void cannyHighThrCallback(int high_threshold, void *data);

    void getFreeman(const std::vector<cv::Point>& contours, std::vector<int>& freeman_code);
};


#endif //CODE_SEGMENT_H
