/*
 * @Description:
 * @version: v1.0
 * @Author: HTY
 * @Date: 2022-06-01 11:36:18
 */

#include <iostream>
#include <fstream>
#include <string>
#include "boost/format.hpp"
#include <opencv2/opencv.hpp>

using namespace std;

bool isSave = true;
void showImage(const std::string & win_name, cv::Mat & mat, cv::Size size, int wait_key=0, const std::string & save_path="");


int main(int argc, char **argv)
{
    string image_root = "../../images/calibration_target/";    // image root of calibration target
    boost::format save_fmt("../../images/corners/%s_corner.png");  // image names of corners
    ifstream fin("../../images/file_names.txt");    // file names of images in calibration target
    if (!fin)
    {
        cout << "Cannot find files of image names!" << endl;
        return -1;
    }
    ofstream fout("../../calibration_result.txt");  // results

    /* (1) corner detection */
    int image_number = 0;       // image numbers
    cv::Size image_size;        // image size
    int points_per_row = 8;
    int points_per_col = 6;
    cv::Size corner_size = cv::Size(points_per_row, points_per_col);
    vector<cv::Point2f> points_per_image;
    vector<vector<cv::Point2f>> points_all_images;

    string image_name;
    while (getline(fin, image_name))
    {
        image_number++;
        cv::Mat image_raw = cv::imread(image_root + image_name);
        if (image_raw.empty())
        {
            cout << "Path error!" << endl;
            return -1;
        }
        if (image_number == 1)
        {
            image_size.width = image_raw.cols;
            image_size.height = image_raw.rows;
            cout << "image_size.width = " << image_size.width << endl;
            cout << "image_size.height = " << image_size.height << endl;
        }

        cv::Mat image_gray;
        cv::cvtColor(image_raw, image_gray, cv::COLOR_BGR2GRAY);

        // find corners
        bool success = cv::findChessboardCorners(image_gray, corner_size, points_per_image);
        if (!success)
        {
            cout << "Cannot find the corners for image " << image_name << endl;
            image_number--;
        }
        else
        {
            // subpixel corner detection
            cv::find4QuadCornerSubpix(image_gray, points_per_image, cv::Size(5, 5));
            points_all_images.push_back(points_per_image);
            
            // visualize and save image
            cv::drawChessboardCorners(image_raw, corner_size, points_per_image, success);
            showImage("Corner detection", image_raw, cv::Size(), -1, (save_fmt % image_name).str());
        }
    }
    cv::destroyAllWindows();
    cout << "image_numbers = " << (int) points_all_images.size() << endl;

    // calibration
    cv::Size block_size(3, 3);  // size of each grid
    cv::Mat K(3, 3, CV_32FC1, cv::Scalar::all(0));   // inner param
    cv::Mat dist_coef(1, 5, CV_32FC1, cv::Scalar::all(0)); // distortion coefficients
    vector<cv::Mat> R;  // rotation matrix
    vector<cv::Mat> T;  // translation matrix

    vector<cv::Point3f> points3D_per_image;
    for (int i = 0; i < corner_size.height; i++)
        for (int j = 0; j < corner_size.width; j++)
            points3D_per_image.emplace_back(block_size.width * j, block_size.height * i, 0);

    vector<vector<cv::Point3f>> points3D_all_images(image_number,points3D_per_image);   // 3d coordinates
    
    // calibration
    cv::calibrateCamera(points3D_all_images, points_all_images, image_size, K, dist_coef, R, T, 0);

    // validation
    cout.precision(4);
    fout.precision(4);
    cout.setf(ios::fixed);
    fout.setf(ios::fixed);
    double err = 0.0, total_err = 0.0;
    vector<cv::Point2f> points_reproject;
    cout << "\nCalibration error of each image:\n";
    fout << "Calibration error of each image:\n";
    for (int i = 0; i < image_number; i++)
    {
        points3D_per_image = points3D_all_images[i];
        // reprojection by calibrated params
        cv::projectPoints(points3D_per_image, R[i], T[i], K, dist_coef, points_reproject);
        
        // calculate error
        vector<cv::Point2f> detect_points = points_all_images[i];
        cv::Mat detect_points_Mat = cv::Mat(1, (int) detect_points.size(), CV_32FC2);
        cv::Mat reproject_points_Mat = cv::Mat(1, (int) points_reproject.size(), CV_32FC2);
        for (int j = 0; j < detect_points.size(); j++)
        {
            detect_points_Mat.at<cv::Vec2f>(0, j) = cv::Vec2f(detect_points[j].x, detect_points[j].y);
            reproject_points_Mat.at<cv::Vec2f>(0, j) = cv::Vec2f(points_reproject[j].x, points_reproject[j].y);
        }
        err = cv::norm(reproject_points_Mat, detect_points_Mat, cv::NormTypes::NORM_L2);
        total_err += err /= corner_size.area();
        cout << "Mean pixel error of image " << i << ": " << err << endl;
        fout << "Mean pixel error of image " << i << ": " << err << endl;
    }
    cout << "\nMean pixel error of whole images: " << total_err << endl;
    fout << "\nMean pixel error of whole images: " << total_err << endl;

    // save params
    cv::Mat rotation = cv::Mat(3, 3, CV_32FC1, cv::Scalar::all(0));
    cout << "\ncamera intrinsics:" << endl;
    cout << K << endl;
    fout << "\ncamera intrinsics:" << endl;
    fout << K << endl;
    cout << "\ndistortion coefficients:" << endl;
    cout << dist_coef << endl << endl;
    fout << "\ndistortion coefficients:" << endl;
    fout << dist_coef << endl << endl;

    for (int i = 0; i < image_number; i++)
    {
        cv::Rodrigues(R[i], rotation);    // convert rotation vector to rotation matrix
        fout << "rotation matrix of image " << i << ":" << endl;
        fout << rotation << endl;
        fout << "translation matrix of image " << i << ":" << endl;
        fout << T[i] << endl;
    }
    fout.close();

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