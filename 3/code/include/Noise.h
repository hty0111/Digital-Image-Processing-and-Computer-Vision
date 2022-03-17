//
// Created by hty on 3/17/22.
//

#ifndef CODE_Noise_H
#define CODE_Noise_H

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>

class Noise {
public:
    cv::Mat addPepperSaltNoise(const cv::Mat & input, int num_noise);
    cv::Mat addGaussianNoise(const cv::Mat & input, int gain, double miu, double sigma);
    double generateGaussianSeq(double miu, double sigma);
    void showImage(const cv::Mat & mat, const std::string & win_name, cv::Size size, int wait_key);
    void gaussCallback(int pos, void *);
};


#endif //CODE_Noise_H
