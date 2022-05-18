/*
 * @Description: 
 * @version: v1.0
 * @Author: HTY
 * @Date: 2022-05-16 19:50:49
 */


#include "include/Morphology.h"

void Morphology::OSTU(const cv::Mat & src, cv::Mat & dst)
{
    dst = src.clone();

    // calculate probability of each grey value
    int size = src.cols * src.rows;
    double p[256] = {0};
    for (int i = 0; i < src.rows; i++) {
        auto row_ptr = src.ptr<uchar>(i);
        for (int j = 0; j < src.cols; j++)
            p[row_ptr[j]] += 1.0;
    }
    for (double & i : p)
        i /= size;

    // calculate threshold according to formula of OSTU
    int threshold;
    double max_delta = 0, cur_delta;
    for (int i = 0; i < 256; i++)
    {
        double w0 = 0, w1 = 0, mu0 = 0, mu1 = 0;
        for (int j = 0; j < i; j++)
            w0 += p[j];
        w1 = 1 - w0;
        for (int j = 0; j < 256; j++)
            if (j < i)
                mu0 += p[j] * (j+1) / w0;
            else
                mu1 += p[j] * (j+1) / w1;
        cur_delta = w0 * w1 * pow(mu1-mu0, 2);
        if (cur_delta > max_delta)
        {
            max_delta = cur_delta;
            threshold = i;
        }
    }
    std::cout << "OSTU threshold: " << threshold << std::endl;

    // binarize the images
    for (int i = 0; i < dst.rows; i++)
    {
        auto row_ptr = dst.ptr<uchar>(i);
        for (int j = 0; j < dst.cols; j++)
            row_ptr[j] = ((row_ptr[j] < threshold) ? 0 : 255);
    }
}


void Morphology::erode(const cv::Mat & src, cv::Mat & dst, int kernel_size)
{
    dst = cv::Mat::zeros(src.size(), src.type());
    int c2e = (int) (kernel_size / 2);    // distance of kernel center to edge

    // traverse points on input images
    for (int i = c2e; i < src.rows - c2e; i++)
        for (int j = c2e; j < src.cols - c2e; j++)
        {
            // extract ROI
            int flag = 0;   // flag == 1 means erosion
            // traverse points on kernel
            for (int x = -c2e; x < c2e; x++)
            {
                for (int y = -c2e; y < c2e; y++)
                    if (src.at<uchar>(i+x, j+y) == 0)
                    {
                        flag = 1;
                        break;
                    }
                if (flag)
                    break;
            }
            dst.at<uchar>(i, j) = (flag ? 0 : 255);
        }
}


void Morphology::dilate(const cv::Mat &src, cv::Mat &dst, int kernel_size)
{
    dst = cv::Mat::zeros(src.size(), src.type());
    int c2e = (int) (kernel_size / 2);    // distance of kernel center to edge

    // traverse points on input images
    for (int i = c2e; i < src.rows - c2e; i++)
        for (int j = c2e; j < src.cols - c2e; j++)
            if (src.at<uchar>(i, j) == 255)
                // traverse points on kernel
                for (int x = -c2e; x < c2e; x++)
                    for (int y = -c2e; y < c2e; y++)
                        dst.at<uchar>(i+x, j+y) = 255;
}


void Morphology::open(cv::Mat &src, cv::Mat &dst, int erode_size, int dilate_size)
{
    cv::Mat erode_output;
    erode(src, erode_output, erode_size);
    dilate(erode_output, dst, dilate_size);
}


void Morphology::close(cv::Mat &src, cv::Mat &dst, int erode_size, int dilate_size)
{
    cv::Mat dilate_output;
    dilate(src, dilate_output, dilate_size);
    erode(dilate_output, dst, erode_size);
}


void Morphology::HMT(const cv::Mat &src, cv::Mat &dst)
{
    int kernel_size = (int) HMT_kernel[0][0].size();
    int c2e = (int) (kernel_size / 2);    // distance of kernel center to edge

    cv::Mat input = src.clone(), output = src.clone();
    cv::Mat roi = cv::Mat::zeros(cv::Size(kernel_size, kernel_size), src.type());

    int iter = 0;
    while (true)
    {
        // traverse templates to HMT
        for (auto kernel : HMT_kernel)
        {
            std::vector<cv::Point> points;  // points failed to hit
            // traverse points on input images
            for (int i = 0; i < input.rows - 2*c2e; i++)
                for (int j = 0; j < input.cols - 2*c2e; j++)
                {
                    bool flag = true;
                    // traverse points on kernel
                    for (int x = 0; x < kernel_size; x++)
                        for (int y = 0; y < kernel_size; y++)
                            if ((kernel[x][y] == 1 && input.at<uchar>(i+x, j+y) == 0) ||
                                (kernel[x][y] == -1 && input.at<uchar>(i+x, j+y) == 255))
                                flag = false;
                    if (flag)
                        points.emplace_back(i+1, j+1);
                }
            for (auto point : points)
                output.at<uchar>(point.x, point.y) = 0;
        }

        iter++;
        // conditions on termination
        if ((iter >= max_iter) || (cv::countNonZero((output != input)) == 0))
        {
            dst = output.clone();
            std::cout << "HTM iter times: " << iter << std::endl;
            break;
        }
        input = output.clone();
    }
}
