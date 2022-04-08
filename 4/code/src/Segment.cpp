/*
 * @Description: 
 * @version: v1.0
 * @Author: HTY
 * @Date: 2022-04-08 11:00:10
 */

#include "include/Segment.h"

Segment::Canny_Data canny_data;
void Segment::cannyTrackbar(cv::Mat &src, const std::string &win_name)
{
    canny_data.src = src;
    canny_data.win_name = win_name;
    cv::createTrackbar("threshold 1", "Edge image", nullptr, 255,
                       cannyLowThrCallback, (void*)& canny_data);
    cv::createTrackbar("threshold 2", "Edge image", nullptr, 255,
                       cannyHighThrCallback, (void*)& canny_data);
}

void Segment::cannyLowThrCallback(int low_threshold, void *data)
{
    cv::Mat dst;

    cv::Canny(canny_data.src, dst, low_threshold, canny_data.high_threshold);
    canny_data.low_threshold = low_threshold;
    cv::imshow(canny_data.win_name, dst);
}

void Segment::cannyHighThrCallback(int high_threshold, void *data)
{
    cv::Mat dst;

    cv::Canny(canny_data.src, dst, canny_data.low_threshold, high_threshold);
    canny_data.high_threshold = high_threshold;
    cv::imshow(canny_data.win_name, dst);
}
