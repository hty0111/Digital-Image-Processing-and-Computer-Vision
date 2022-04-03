/*
 * @Description: 
 * @version: v1.0
 * @Author: HTY
 * @Date: 2022-03-17 20:02:06
 */

#include "include/Filter.h"

void Filter::meanFilter(const cv::Mat & src, cv::Mat & dst, int kernel_size)
{
    dst = src.clone();
    cv::Mat ROI;

    if (kernel_size % 2 == 0)
        kernel_size += 1;

    // ignore edge pixel
    if (kernel_size > 1)
    {
        int c2e = (int) (kernel_size / 2);  // center2edge
        for (int i = c2e; i < dst.rows - c2e; i++)
            for (int j = c2e; j < dst.cols - c2e; j++)
                for (int k = 0; k < 3; k++)
                {
                    // choose kernel area
                    ROI = dst(cv::Range(i - c2e, i + c2e + 1),
                              cv::Range(j - c2e, j + c2e + 1));
                    // substitute for original pixel
                    dst.at<cv::Vec3b>(i, j)[k] = (int) cv::sum(ROI).val[k] / pow(kernel_size, 2);
                }
    }
}

void Filter::medianFilter(const cv::Mat & src, cv::Mat & dst, int kernel_size)
{
    dst = src.clone();
    cv::Mat flattened, sorted, ROI, channels[3];

    // split channels
    cv::split(dst, channels);
    if (kernel_size % 2 == 0)
        kernel_size += 1;

    // ignore edge pixel
    if (kernel_size > 1)
    {
        int c2e = (int) (kernel_size / 2);  // center2edge
        for (int i = c2e; i < dst.rows - c2e; i++)
            for (int j = c2e; j < dst.cols - c2e; j++)
                for (int ch = 0; ch < 3; ch++)
                {
                    // choose kernel area
                    ROI = channels[ch](cv::Range(i - c2e, i + c2e + 1),
                                     cv::Range(j - c2e, j + c2e + 1));
                    // ROI region is not continuous, but 'reshape' requires continuous matrix
                    if (!ROI.isContinuous())
                        ROI = ROI.clone();
                    // flatten and sort
                    flattened = ROI.reshape(1, 1);
                    cv::sort(flattened, sorted, cv::SORT_EVERY_ROW + cv::SORT_ASCENDING);
                    // substitute for original pixel
                    dst.at<cv::Vec3b>(i, j)[ch]= sorted.at<cv::Vec3b>(0, c2e)[ch];
                }
    }
}


void Filter::bilateralFilter(const cv::Mat & src, cv::Mat & dst, int kernel_size, double sigma_domain, double sigma_range)
{
    dst = src.clone();

    double weight_domain[kernel_size][kernel_size], weight_range[kernel_size][kernel_size];
    if (kernel_size % 2 == 0)
        kernel_size += 1;
    // center pixel point index 'i' & 'j'
    int i = (int) kernel_size / 2, j = i;
    // surrounding pixel point index 'k' & 'l'
    int k, l;

    // calculate domain kernel, only up to kernel size
    for (k = 0; k < kernel_size; k++)
        for (l = 0; l < kernel_size; l++)
            weight_domain[k][l] = exp(-(pow(i-k,2) + pow(j-l,2)) / (2 * pow(sigma_domain, 2)));

    // change pixel value
    if (kernel_size > 1)
    {
        double sum_numerator = 0, sum_denominator = 0;
        int c2e = (int) (kernel_size / 2);  // center2edge
        for (i = c2e; i < dst.rows - c2e; i++)
            for (j = c2e; j < dst.cols - c2e; j++)
            {
                for (int ch = 0; ch < 3; ch++)
                {
                    // calculate range kernel
                    for (k = 0; k < kernel_size; k++)
                        for (l = 0; l < kernel_size; l++)
                        {
                            weight_domain[k][l] =
                                    exp(-pow(src.at<cv::Vec3b>(i, j)[ch] -
                                            src.at<cv::Vec3b>(i-c2e+k, j-c2e+l)[ch], 2)
                                    / (2 * pow(sigma_range, 2)));
                        }
                    for (k = 0; k < kernel_size; k++)
                        for (l = 0; l < kernel_size; l++)
                        {
                            sum_numerator += src.at<cv::Vec3b>(i-c2e+k, j-c2e+l)[ch] *
                                    weight_domain[k][l] * weight_range[k][l];
                            sum_denominator += weight_domain[k][l] * weight_range[k][l];
                        }
                    dst.at<cv::Vec3b>(i, j)[ch] = sum_numerator / sum_denominator;
                    sum_numerator = sum_denominator = 0;
                 }
            }
    }
}


Filter::Data mean_data;
void Filter::meanTrackbar(cv::Mat &src, const std::string &win_name)
{
    mean_data.src = src;
    mean_data.win_name = win_name;
    cv::createTrackbar("kernel size", win_name, nullptr, 15,
                       meanCallback, (void*)& mean_data);
    cv::setTrackbarPos("kernel size", win_name, 3);
}

Filter::Data median_data;
void Filter::medianTrackbar(cv::Mat &src, const std::string &win_name)
{
    median_data.src = src;
    median_data.win_name = win_name;
    cv::createTrackbar("kernel size", win_name, nullptr, 15,
                       medianCallback, (void*)& median_data);
    cv::setTrackbarPos("kernel size", win_name, 3);
}

void Filter::meanCallback(int kernel_size, void *data)
{
    Data* extracted_data = (Data*) data;
    cv::Mat dst, src = extracted_data->src;
    std::string win_name = extracted_data->win_name;

    Filter filter;
    filter.meanFilter(src, dst, kernel_size);
    cv::imshow(win_name, dst);
}

void Filter::medianCallback(int kernel_size, void *data)
{
    Data* extracted_data = (Data*) data;
    cv::Mat dst, src = extracted_data->src;
    std::string win_name = extracted_data->win_name;

    Filter filter;
    filter.medianFilter(src, dst, kernel_size);
    cv::imshow(win_name, dst);
}

