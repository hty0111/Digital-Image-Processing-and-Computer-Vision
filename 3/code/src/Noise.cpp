//
// Created by hty on 3/17/22.
//

#include "include/Noise.h"
#include <cstdlib>

cv::Mat Noise::addPepperSaltNoise(const cv::Mat & input, int num_noise)
{
    cv::Mat output = input.clone();

    // add black and white dot
    for (int i = 0; i < num_noise; i++) {
        int x = rand() % input.rows;
        int y = rand() % input.cols;
        if (i % 2 == 0)     // half for black
            for (int j = 0; j < 3; j++)
                output.at<cv::Vec3b>(x, y)[j] = 0;
        else                // half for white
            for (int j = 0; j < 3; j++)
                output.at<cv::Vec3b>(x, y)[j] = 255;
    }
    return output;
}

cv::Mat Noise::addGaussianNoise(const cv::Mat &input, int gain, double miu, double sigma)
{
    cv::Mat output = input.clone();

    for (int i = 0; i < output.rows; i++)
    {
        for (int j = 0; j < output.cols; j++)
        {
            for (int channel = 0; channel < 3; channel++)
            {
                output.at<cv::Vec3b>(i, j)[channel] += gain * generateGaussianSeq(miu, sigma);
            }
        }
    }

    return output;
}

double Noise::generateGaussianSeq(double miu, double sigma) {
    // Box-Muller transform
    double u1, u2, std_norm_result;
    do {
        u1 = (double) rand() / RAND_MAX;
        u2 = (double) rand() / RAND_MAX;
        std_norm_result = cos(2 * CV_PI * u1) * sqrt(-2.0 * log(u2));
//        std::cout << u1 << " " << u2 << std::endl;
//        std::cout << std_norm_result << std::endl;
    } while (std_norm_result > 1.0 || std_norm_result == 0.0);
    return miu + sigma * std_norm_result;
}

//void Noise::gaussCallback(int pos, void *)
//{
//    if (pos < 0)
//        return;
//
//}





