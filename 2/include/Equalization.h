/*
 * @Description: Histogram Equalization
 * @version: v1.0
 * @Author: HTY
 * @Date: 2022-03-13 03:43:40
 */

#ifndef INC_2_EQUALIZATION_H
#define INC_2_EQUALIZATION_H

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class Equalization {
public:
    void getHistMat(cv::InputArray src, cv::OutputArray dst);
    void equalize(cv::InputArray src, cv::OutputArray dst);
};


#endif //INC_2_EQUALIZATION_H
