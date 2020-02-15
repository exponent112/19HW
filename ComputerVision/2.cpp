#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main() {
    Mat image, dst;
    image = imread("apple.jpg");
    Rect rect(Point(160, 140), Point(340, 370));
    rectangle(image,rect,Scalar(255,0,0),4,8,0);
    threshold(image(rect), image(rect), 128, 255, THRESH_BINARY);
    for (int y = 0; y < 140; y++) {
        for (int x = 0; x < image.cols; x++) {
            int test = (image.at<Vec3b>(y, x)[0]+image.at<Vec3b>(y, x)[1]+image.at<Vec3b>(y, x)[2])/3;
            image.at<Vec3b>(y, x)[0] = test;
            image.at<Vec3b>(y, x)[1] = test;
            image.at<Vec3b>(y, x)[2] = test;
        }
    }
    for (int y = 0; y < 370; y++) {
        for (int x = 0; x < 160; x++) {
            int test = (image.at<Vec3b>(y, x)[0]+image.at<Vec3b>(y, x)[1]+image.at<Vec3b>(y, x)[2])/3;
            image.at<Vec3b>(y, x)[0] = test;
            image.at<Vec3b>(y, x)[1] = test;
            image.at<Vec3b>(y, x)[2] = test;
        }
    }
    for (int y = 140; y < 370; y++) {
        for (int x = 340; x < image.cols; x++) {
            int test = (image.at<Vec3b>(y, x)[0]+image.at<Vec3b>(y, x)[1]+image.at<Vec3b>(y, x)[2])/3;
            image.at<Vec3b>(y, x)[0] = test;
            image.at<Vec3b>(y, x)[1] = test;
            image.at<Vec3b>(y, x)[2] = test;
        }
    }
    for (int y = 370; y < image.rows; y++) {
        for (int x = 0; x < image.cols; x++) {
            int test = (image.at<Vec3b>(y, x)[0]+image.at<Vec3b>(y, x)[1]+image.at<Vec3b>(y, x)[2])/3;
            image.at<Vec3b>(y, x)[0] = test;
            image.at<Vec3b>(y, x)[1] = test;
            image.at<Vec3b>(y, x)[2] = test;
        }
    }
    
    imshow("2", image);
    waitKey();
}
