/*
 * @Description:
 * @version: v1.0
 * @Author: HTY
 * @Date: 2022-05-28 14:07:45
 */

#include <iostream>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>

using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{
    // read images
    Mat img1 = imread("../../images/1.png");
    Mat img2 = imread("../../images/2.png");

    if (!img1.data or !img2.data)
    {
        cout << "Path error!" << endl;
        return -1;
    }
    else
        cout << "image size: " << img1.size << endl;

    Size image_size(1000, 400);

    /*
     * ORB
     */
    vector<KeyPoint> key_points1, key_points2;
    Mat descriptions1, descriptions2;
    Ptr<ORB> orb = ORB::create(500, 1.2f, 8, 31, 0, 2,
                               ORB::HARRIS_SCORE,31,20);

    // (1) 检测 Oriented FAST 角点位置
    orb->detect(img1, key_points1);
    orb->detect(img2, key_points2);

    // (2) 由角点位置计算 BRIEF 描述子
    orb->compute(img1, key_points1, descriptions1);
    orb->compute(img2, key_points2, descriptions2);

    Mat orb_img;
    drawKeypoints(img1, key_points1, orb_img, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
    imshow("ORB特征点", orb_img);

    // (3) 用 Hamming 距离匹配两帧中的 BRIEF
    vector<DMatch> matches;
    BFMatcher matcher(NORM_HAMMING);
    matcher.match(descriptions1, descriptions2, matches);

    // (4) 筛选配对点
    // 找出最大和最小距离
    double min_dist = 10000, max_dist = 0;
    for (int i = 0; i < descriptions1.rows; i++)
    {
        double dist = matches[i].distance;
        if (dist < min_dist)
            min_dist = dist;
        if (dist > max_dist)
            max_dist = dist;
    }

    // 描述子距离大于两倍最小值或经验值时，认为匹配有误
    vector<DMatch> good_matches;
    for (int i = 0; i < descriptions1.rows; i++)
        if (matches[i].distance <= max(2*min_dist, 30.0))
            good_matches.push_back(matches[i]);

    // (5) 绘制匹配结果
    Mat img_match, img_good_match;
    drawMatches(img1, key_points1, img2, key_points2, matches, img_match);
    drawMatches(img1, key_points1, img2, key_points2, good_matches, img_good_match);
    imshow("所有匹配点", img_match);
    imshow("优化后的匹配点", img_good_match);
    waitKey(0);

    /*
     * SIFT
     */
    key_points1.clear(); key_points2.clear();
    Ptr<SIFT> sift = SIFT::create(500, 3, 0.04, 10, 1.6);
    sift->detect(img1, key_points1);
    sift->detect(img2, key_points2);
    sift->compute(img1, key_points1, descriptions1);
    sift->compute(img2, key_points2, descriptions2);
    vector<DMatch> matches_1;
    BFMatcher matcher_1(NORM_L1);
    matcher_1.match(descriptions1, descriptions2, matches_1);
    Mat sift_img;
    drawKeypoints(img1, key_points1, sift_img, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
    imshow("SIFT特征点", sift_img);
    waitKey(0);
    destroyAllWindows();
    drawMatches(img1, key_points1, img2, key_points2, matches_1, img_match);
    imshow("所有匹配点", img_match);
    waitKey(0);

    return 0;
}

