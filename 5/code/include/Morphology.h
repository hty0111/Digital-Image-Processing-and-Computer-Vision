/*
 * @Description: 
 * @version: v1.0
 * @Author: HTY
 * @Date: 2022-05-16 19:50:49
 */


#ifndef CODE_MORPHOLOGY_H
#define CODE_MORPHOLOGY_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

class Morphology {
private:
    int max_iter = 100;
    std::vector< std::vector< std::vector<int> > > HMT_kernel = {
            {{-1,-1,-1}, {0,1,0}, {1,1,1}},
            {{0,-1,-1}, {1,1,-1}, {1,1,0}},
            {{1,0,-1}, {1,1,-1}, {1,0,-1}},
            {{1,1,0}, {1,1,-1}, {0,-1,-1}},
            {{1,1,1}, {0,1,0}, {-1,-1,-1}},
            {{0,1,1}, {-1,1,1}, {-1,-1,0}},
            {{-1,0,1}, {-1,1,1}, {-1,0,1}},
            {{-1,-1,0}, {-1,1,1}, {0,1,1}},
    };

public:
    void OSTU(const cv::Mat &src, cv::Mat &dst);
    void erode(const cv::Mat & src, cv::Mat & dst, int kernel_size=3);
    void dilate(const cv::Mat & src, cv::Mat & dst, int kernel_size=3);
    void open(cv::Mat & src, cv::Mat & dst, int erode_size=3, int dilate_size=3);
    void close(cv::Mat & src, cv::Mat & dst, int erode_size=3, int dilate_size=3);
    void HMT(const cv::Mat & src, cv::Mat & dst);
};


#endif //CODE_MORPHOLOGY_H
