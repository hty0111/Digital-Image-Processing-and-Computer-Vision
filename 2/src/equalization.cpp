//
// Created by hty on 3/13/22.
//

#include "../include/equalization.h"

Equalization::Equalization(cv::Mat &mat)
{
    image = mat;
    histogram = getHistMat(mat);
}

void Equalization::showImage(const cv::Mat & mat, const std::string & win_name, int wait_key)
{
    cv::namedWindow(win_name);
    cv::imshow(win_name, mat);
    if (wait_key >= 0)
        cv::waitKey(wait_key);
}

cv::Mat Equalization::getHistMat(const cv::Mat& mat)
{
    // 获取灰度数据
    const int channels = 0;
    const int histSize = 256;
    float range[] = {0, 256};
    const float * ranges[] = {range};
    calcHist(&mat, 1, &channels, cv::Mat(), hist_value, 1, &histSize, ranges, true, false);

    //　绘制原始图像的灰度直方图，背景为黑色，值为白色
    double maxValue = 0;
    minMaxLoc(hist_value, nullptr, &maxValue, nullptr, nullptr);
    cv::Mat histGrey((int) maxValue, 256, CV_8UC1, cv::Scalar::all(0));
    for (int i = 0; i < 256; i++)
        histGrey.col(i).rowRange(cv::Range((int) maxValue - (int)hist_value.at<float>(i, 0), (int) maxValue)) = 255;

    //　调整灰度图的大小
    cv::Mat histResize;
    resize(histGrey, histResize, cv::Size(500, 500));

    return histResize;
}

cv::Mat Equalization::equalize()
{
    double dpi = image.rows * image.cols;
    std::vector<double> pr(256, 0), sk(256, 0);
    for (int i = 0; i < 256; i++)
    {
        pr[i] = hist_value.at<float>(i, 0) / dpi;
        if (i == 0)
            sk[i] = pr[i];
        else
            sk[i] = sk[i-1] + pr[i];
    }

    for (int i = 0; i < 256; i++)
    {
        sk[i] = int(sk[i] * 255 + 0.5);
    }

    std::cout << std::endl << image.size << " " << image.rows << std::endl;
    cv::Mat equalized_image = image;
    for (int i = 0; i < image.rows; i++)
    {
        for (int j = 0; j < image.cols; j++)
        {
            equalized_image.at<uchar>(i, j) = sk[(int)image.at<uchar>(i, j)];
        }
    }
    return equalized_image;
}


