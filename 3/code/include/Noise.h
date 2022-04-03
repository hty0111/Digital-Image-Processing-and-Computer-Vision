/*
 * @Description: 
 * @version: v1.0
 * @Author: HTY
 * @Date: 2022-03-17 16:55:58
 */

#ifndef CODE_Noise_H
#define CODE_Noise_H

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>

class Noise {
public:
    class Data
    {
    public:
        cv::Mat src;
        std::string win_name;
        int param[3] = {2, 0, 3};
    };

    void addPepperNoise(const cv::Mat & src, cv::Mat & dst, int num_noise);
    void addGaussNoise(const cv::Mat & src, cv::Mat & dst, double gain, double mu, double sigma);
    double generateGaussSeq(double mu, double sigma);

    static void pepperTrackbar(cv::Mat & src, const std::string & win_name);
    static void gaussTrackbar(cv::Mat & src, const std::string & win_name);
    static void pepperCallback(int pepper_num, void * data);
    static void gaussGainCallback(int gain, void * data);
    static void gaussMuCallback(int mu, void * data);
    static void gaussSigmaCallback(int sigma, void *data);
};


#endif //CODE_Noise_H
