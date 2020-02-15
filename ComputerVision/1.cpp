#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main() {
    Mat frame;
    VideoCapture cap("background.mp4");
    double fps = cap.get(CV_CAP_PROP_FPS);
    double time =0;
    while (time < 10000) {
        cap >> frame;
        time = cap.get(CAP_PROP_POS_MSEC);
        if(time<5000){
            continue;
        }
        for (int y = 0; y < frame.rows; y++) {
            for (int x = 50; x < 101; x++) {
                frame.at<Vec3b>(y, x)[0] = 0;
                frame.at<Vec3b>(y, x)[1] = 0;
                frame.at<Vec3b>(y, x)[2] = 255;
            }
        }
        putText(frame, format("%d/%d",frame.cols,frame.rows), Point(200,100), FONT_HERSHEY_SIMPLEX, 1, Scalar(0,255,255));
        imshow("Window", frame);
        waitKey(1000/fps);
    }
}
