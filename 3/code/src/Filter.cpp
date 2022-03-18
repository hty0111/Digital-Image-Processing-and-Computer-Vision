//
// Created by hty on 3/17/22.
//

#include "include/Filter.h"

cv::Mat Filter::meanFilter(const cv::Mat & input, cv::Size kernel_size)
{
    cv::Mat output = input.clone();
    cv::Mat ROI;

    // ignore edge pixel
    int center2edge = (kernel_size.height - 1 ) / 2;
    for (int i = center2edge; i < output.rows - center2edge; i++)
        for (int j = center2edge; j < output.cols - center2edge; j++)
            for (int k = 0; k < 3; k++)
            {
                // choose kernel area
                ROI = output(cv::Range(i - center2edge, i + center2edge + 1),
                             cv::Range(j - center2edge, j + center2edge + 1));
                // substitute for original pixel
                output.at<cv::Vec3b>(i, j)[k] = (int) cv::sum(ROI).val[k] / (kernel_size.height * kernel_size.width);
            }

    return output;
}

cv::Mat Filter::medianFilter(const cv::Mat & input, cv::Size kernel_size)
{
    cv::Mat output = input.clone();
    cv::Mat flattened, sorted, ROI, channel[3];

    // split channels
    cv::split(output, channel);

    // ignore edge pixel
    int center2edge = (kernel_size.height - 1 ) / 2;
    for (int i = center2edge; i < output.rows - center2edge; i++)
        for (int j = center2edge; j < output.cols - center2edge; j++)
            for (int k = 0; k < 3; k++)
            {
                // choose kernel area
                ROI = channel[k](cv::Range(i - center2edge, i + center2edge + 1),
                        cv::Range(j - center2edge, j + center2edge + 1));
                // ROI region is not continuous, but 'reshape' requires continuous matrix
                if (!ROI.isContinuous())
                    ROI = ROI.clone();
                // flatten and sort
                flattened = ROI.reshape(1, 1);
                cv::sort(flattened, sorted, cv::SORT_EVERY_ROW + cv::SORT_ASCENDING);
                // substitute for original pixel
                output.at<cv::Vec3b>(i, j)[k]= sorted.at<cv::Vec3b>(0, center2edge)[k];
            }

    return output;
}

cv::Mat Filter::bilateralFilter(const cv::Mat & input, cv::Size kernel_size)
{
    cv::Mat output = input.clone();

    return output;
}

