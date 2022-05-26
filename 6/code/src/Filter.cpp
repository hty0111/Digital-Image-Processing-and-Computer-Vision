/*
 * @Description: 
 * @version: v1.0
 * @Author: HTY
 * @Date: 2022-05-18 14:37:19
 */

// TODO: FFT & iFFT

#include "include/Filter.h"
using namespace std;

Filter::Filter(int angle, int offset)
{
    this->angle = angle;
    this->offset = offset;
}

void Filter::wiener(const cv::Mat &src, cv::Mat &dst)
{
    dst = cv::Mat::zeros(src.size(), src.type());

    cv::Mat PSF;
    getPSF(src, PSF);

}

void Filter::getPSF(const cv::Mat &src, cv::Mat &PSF)
{
    PSF = cv::Mat::zeros(src.size(), src.type());
    int x_center = (src.rows - 1) / 2;
    int y_center = (src.cols - 1) / 2;

    for (int i = 0; i < offset; i++)
    {
        int x_offset = (int) round(sin(angle * M_PI / 180) * i);
        int y_offset = (int) round(cos(angle * M_PI / 180) * i);
        PSF.at<uchar>(x_center-x_offset, y_center+y_offset) = 255;
    }

    int sum = 0, cnt = 0;
    for (int i = 0; i < PSF.rows; i++)
        for (int j = 0; j < PSF.cols; j++)
            if (PSF.at<uchar>(i, j) == 255)
            {
                sum += 255;
                cnt++;
            }

    for (int i = 0; i < PSF.rows; i++)
        for (int j = 0; j < PSF.cols; j++)
            PSF.at<uchar>(i, j) = round(PSF.at<uchar>(i, j) / cnt);

    cv::imshow("psf", PSF);
    cv::waitKey(0);
}


