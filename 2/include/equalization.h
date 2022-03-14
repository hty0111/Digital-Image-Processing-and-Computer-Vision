//
// Created by hty on 3/13/22.
//

#ifndef INC_2_EQUALIZATION_H
#define INC_2_EQUALIZATION_H

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class Equalization {
private:
    cv::Mat hist_value;
public:
    cv::Mat image;
    cv::Mat histogram;
    explicit Equalization(cv::Mat & mat);
    cv::Mat getHistMat(const cv::Mat & mat);
    void showImage(const cv::Mat & mat, const std::string & win_name, cv::Size size = cv::Size(1200, 900), int wait_key = -1);
    cv::Mat equalize();
};


#endif //INC_2_EQUALIZATION_H
