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
#include <opencv2/xfeatures2d.hpp>

using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;

bool isSave = true;
void showImage(const std::string & win_name, cv::Mat & mat, cv::Size size, int wait_key=0, const std::string & save_path="");


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
    showImage("ORB特征点", orb_img, image_size, 0, "../../images/ORB_features.png");

    // (3) 用 Hamming 距离匹配两帧中的 BRIEF
    vector<DMatch> matches_orb;
    BFMatcher matcher_orb(NORM_HAMMING);
    matcher_orb.match(descriptions1, descriptions2, matches_orb);

    // (4) 筛选配对点
    // 找出最大和最小距离
    double min_dist = 10000, max_dist = 0;
    for (int i = 0; i < descriptions1.rows; i++)
    {
        double dist = matches_orb[i].distance;
        if (dist < min_dist)
            min_dist = dist;
        if (dist > max_dist)
            max_dist = dist;
    }

    // 描述子距离大于两倍最小值或经验值时，认为匹配有误
    vector<DMatch> good_matches_orb;
    for (int i = 0; i < descriptions1.rows; i++)
        if (matches_orb[i].distance <= max(2*min_dist, 30.0))
            good_matches_orb.push_back(matches_orb[i]);

    // (5) 绘制匹配结果
    Mat img_match, img_good_match;
    drawMatches(img1, key_points1, img2, key_points2, matches_orb, img_match);
    drawMatches(img1, key_points1, img2, key_points2, good_matches_orb, img_good_match);
    showImage("ORB所有匹配点", img_match, image_size, 0, "../../images/ORB_all_matches.png");
    showImage("ORB优化后的匹配点", img_good_match, image_size, 0, "../../images/ORB_good_matches.png");
    destroyAllWindows();

    /*
     * SIFT
     */
    key_points1.clear(); key_points2.clear();
    Ptr<SIFT> sift = SIFT::create(500, 3, 0.04, 10, 1.6);
    sift->detect(img1, key_points1);
    sift->detect(img2, key_points2);
    sift->compute(img1, key_points1, descriptions1);
    sift->compute(img2, key_points2, descriptions2);

    vector<DMatch> matches_sift;
    FlannBasedMatcher matcher_sift;
    matcher_sift.match(descriptions1, descriptions2, matches_sift);

    min_dist = 10000;
    for (int i = 0; i < descriptions1.rows; i++)
    {
        double dist = matches_sift[i].distance;
        if (dist < min_dist)
            min_dist = dist;
    }
    vector<DMatch> good_matches_sift;
    for (int i = 0; i < descriptions1.rows; i++)
        if (matches_sift[i].distance <= 4 * min_dist)
            good_matches_sift.push_back(matches_sift[i]);

    Mat sift_img;
    drawKeypoints(img1, key_points1, sift_img, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
    showImage("SIFT特征点", sift_img, image_size, 0, "../../images/SIFT_features.png");
    drawMatches(img1, key_points1, img2, key_points2, matches_sift, img_match);
    drawMatches(img1, key_points1, img2, key_points2, good_matches_sift, img_good_match);
    showImage("SIFT所有匹配点", img_match, image_size, 0, "../../images/SIFT_all_matches.png");
    showImage("SIFT优化后的匹配点", img_good_match, image_size, 0, "../../images/SIFT_good_matches.png");
    destroyAllWindows();

    /*
     * SURF
     */
    key_points1.clear(); key_points2.clear();
    Ptr<SURF> surf = SURF::create(100, 4, 3);
    surf->detect(img1, key_points1);
    surf->detect(img2, key_points2);
    surf->compute(img1, key_points1, descriptions1);
    surf->compute(img2, key_points2, descriptions2);

    vector<DMatch> matches_surf;
    FlannBasedMatcher matcher_surf;
    matcher_surf.match(descriptions1, descriptions2, matches_surf);

    min_dist = 10000;
    for (int i = 0; i < descriptions1.rows; i++)
    {
        double dist = matches_surf[i].distance;
        if (dist < min_dist)
            min_dist = dist;
    }
    vector<DMatch> good_matches_surf;
    for (int i = 0; i < descriptions1.rows; i++)
        if (matches_surf[i].distance <= 3 * min_dist)
            good_matches_surf.push_back(matches_surf[i]);

    Mat surf_img;
    drawKeypoints(img1, key_points1, surf_img, Scalar::all(-1), DrawMatchesFlags::DEFAULT);
    showImage("SURF特征点", surf_img, image_size, 0, "../../images/SURF_features.png");
    drawMatches(img1, key_points1, img2, key_points2, matches_surf, img_match);
    drawMatches(img1, key_points1, img2, key_points2, good_matches_surf, img_good_match);
    showImage("SURF所有匹配点", img_match, image_size, 0, "../../images/SURF_all_matches.png");
    showImage("SURF优化后的匹配点", img_good_match, image_size, 0, "../../images/SURF_good_matches.png");

    return 0;
}


void showImage(const std::string & win_name, cv::Mat & mat, cv::Size size, int wait_key, const std::string & save_path)
{
    if (!size.empty() and size != cv::Size(0, 0))
    {
        cv::namedWindow(win_name, cv::WINDOW_NORMAL);
        cv::resizeWindow(win_name, size);
    }
    else
        cv::namedWindow(win_name, cv::WINDOW_AUTOSIZE);

    cv::imshow(win_name, mat);
    if (wait_key >= 0)
        cv::waitKey(wait_key);
    if (!save_path.empty() && isSave)
        cv::imwrite(save_path, mat);
}
