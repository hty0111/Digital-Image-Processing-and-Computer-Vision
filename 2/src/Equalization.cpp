/*
 * @Description: Histogram Equalization
 * @version: v1.0
 * @Author: HTY
 * @Date: 2022-03-13 03:43:40
 */

#include "../include/Equalization.h"

void Equalization::getHistMat(cv::InputArray src, cv::OutputArray dst)
{
    // get Mat headers for input arrays
    cv::Mat src_mat = src.getMat();

    // get grayscale data
    cv::Mat hist_value;
    const int channels = 0;
    const int histSize = 256;
    float range[] = {0, 256};
    const float * ranges[] = {range};
    cv::calcHist(&src_mat, 1, &channels, cv::Mat(), hist_value,
                 1, &histSize, ranges, true, false);

    //　plot a grayscale histogram of the original image with a black background and white values
    double max_value = 0;
    cv::minMaxLoc(hist_value, nullptr, &max_value, nullptr, nullptr);
    cv::Mat histGrey((int) max_value, 256, CV_8UC1, cv::Scalar::all(0));
    for (int i = 0; i < 256; i++)
        histGrey.col(i).rowRange(cv::Range((int) max_value - (int)hist_value.at<float>(i, 0), (int) max_value)) = 255;
    cv::resize(histGrey, dst, cv::Size(500, 500));
}

void Equalization::equalize(cv::InputArray _src, cv::OutputArray _dst)
{
    // get Mat headers for input arrays
    cv::Mat src = _src.getMat();
    // create the output array so that it has the proper size and type
    _dst.create( src.size(), src.type() );
    cv::Mat dst = _dst.getMat();

    // get grayscale data
    const int channels = 0;
    cv::Mat hist_value;
    const int histSize = 256;
    float range[] = {0, 256};
    const float * ranges[] = {range};
    cv::calcHist(&src, 1, &channels, cv::Mat(), hist_value, 1, &histSize, ranges, true, false);

    // calculate probability distribution function and cumulative distribution function
    double dpi = src.rows * src.cols;
    std::vector<double> pr(256, 0), sk(256, 0);
    for (int i = 0; i < 256; i++)
    {
        pr[i] = hist_value.at<float>(i, 0) / dpi;
        if (i == 0)
            sk[i] = pr[i];
        else
            sk[i] = sk[i-1] + pr[i];
    }

    // denormalize to get the mapping function
    for (int i = 0; i < 256; i++)
        sk[i] = int(sk[i] * 255 + 0.5);

    // change pixel values of the original image according to the mapping function
    for (int i = 0; i < src.rows; i++)
        for (int j = 0; j < src.cols; j++)
            dst.at<uchar>(i, j) = sk[(int)src.at<uchar>(i, j)];
}


