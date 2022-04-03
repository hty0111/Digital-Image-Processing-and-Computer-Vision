/*
 * @Description: 
 * @version: v1.0
 * @Author: HTY
 * @Date: 2022-03-17 16:55:58
 */

#include "include/Noise.h"
#include <cstdlib>

void Noise::addPepperNoise(const cv::Mat & src, cv::Mat & dst, int num_noise)
{
    dst = src.clone();

    // add black and white dot
    for (int i = 0; i < num_noise; i++)
    {
        int x = rand() % src.rows;
        int y = rand() % src.cols;
        if (i % 2 == 0)     // half for black
            for (int j = 0; j < 3; j++)
                dst.at<cv::Vec3b>(x, y)[j] = 0;
        else                // half for white
            for (int j = 0; j < 3; j++)
                dst.at<cv::Vec3b>(x, y)[j] = 255;
    }
}

void Noise::addGaussNoise(const cv::Mat & src, cv::Mat & dst, double gain, double mu, double sigma)
{
    dst = src.clone();

    int pixel_value;
    for (int i = 0; i < dst.rows; i++)
        for (int j = 0; j < dst.cols; j++)
        {
            pixel_value = gain * generateGaussSeq(mu, sigma);
            for (int channel = 0; channel < 3; channel++)
            {
                // limit range to [0, 255]
                if (dst.at<cv::Vec3b>(i, j)[channel] + pixel_value > 255)
                    dst.at<cv::Vec3b>(i, j)[channel] = 255;
                else if (dst.at<cv::Vec3b>(i, j)[channel] + pixel_value < 0)
                    dst.at<cv::Vec3b>(i, j)[channel] = 0;
                else
                    dst.at<cv::Vec3b>(i, j)[channel] += pixel_value;
            }
        }
}

double Noise::generateGaussSeq(double mu, double sigma) {
    // Box-Muller transform
    double u1, u2, std_norm_result;
    do {
        u1 = (double) rand() / RAND_MAX;
        u2 = (double) rand() / RAND_MAX;
        std_norm_result = cos(2 * CV_PI * u1) * sqrt(-2.0 * log(u2));
    } while (std_norm_result > 1.0 || std_norm_result == 0.0);
    return mu + sigma * std_norm_result;
}

Noise::Data pepper_data;
void Noise::pepperTrackbar(cv::Mat & src, const std::string & win_name)
{
    pepper_data.src = src;
    pepper_data.win_name = win_name;
    int max_pos = src.cols * src.rows / 10;
    int cur_pos = src.cols * src.rows / 100;
    cv::createTrackbar("pepper number", win_name, nullptr, max_pos,
                       pepperCallback, (void*)& pepper_data);
    cv::setTrackbarPos("pepper number", win_name, cur_pos);
}

Noise::Data gauss_data;
void Noise::gaussTrackbar(cv::Mat & src, const std::string & win_name)
{
    gauss_data.src = src;
    gauss_data.win_name = win_name;
    cv::createTrackbar("gain", win_name, &gauss_data.param[0], 10,
                       gaussGainCallback, (void*)& gauss_data);
    cv::createTrackbar("mu", win_name, &gauss_data.param[1], 20,
                       gaussMuCallback, (void*)& gauss_data);
    cv::createTrackbar("sigma", win_name, &gauss_data.param[2], 20,
                       gaussSigmaCallback, (void*)& gauss_data);
}

void Noise::pepperCallback(int pepper_num, void * data)
{
    Data* extracted_data = (Data*) data;
    cv::Mat dst, src = extracted_data->src;
    std::string win_name = extracted_data->win_name;

    Noise noise;
    noise.addPepperNoise(src, dst, pepper_num);
    cv::imshow(win_name, dst);
}

void Noise::gaussGainCallback(int gain, void *data)
{
    Data* extracted_data = (Data*) data;
    cv::Mat dst, src = extracted_data->src;
    std::string win_name = extracted_data->win_name;

    Noise noise;
    noise.addGaussNoise(src, dst, gain, gauss_data.param[1], gauss_data.param[2]);
    gauss_data.param[0] = gain;
    cv::imshow(win_name, dst);
}

void Noise::gaussMuCallback(int mu, void *data)
{
    Data* extracted_data = (Data*) data;
    cv::Mat dst, src = extracted_data->src;
    std::string win_name = extracted_data->win_name;

    Noise noise;
    noise.addGaussNoise(src, dst, gauss_data.param[0], mu, gauss_data.param[2]);
    gauss_data.param[1] = mu;
    cv::imshow(win_name, dst);
}

void Noise::gaussSigmaCallback(int sigma, void *data)
{
    Data* extracted_data = (Data*) data;
    cv::Mat dst, src = extracted_data->src;
    std::string win_name = extracted_data->win_name;

    Noise noise;
    noise.addGaussNoise(src, dst, gauss_data.param[0], gauss_data.param[1], sigma);
    gauss_data.param[2] = sigma;
    cv::imshow(win_name, dst);
}

