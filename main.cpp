#include<stdio.h>
#include<iostream>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"

using namespace std;

cv::VideoCapture openWebcam() {
    const char* pipe = "autovideosrc device=/dev/video0  ! appsink";
    cv::VideoCapture webcam(pipe);

    if (!webcam.isOpened()) {
        cout << "Connection to a camera failed" << endl;
    }

    return webcam;
}

int detectFace() {
    cv::Mat frame, grayFrame;
    cv::Size windowSize(1000, 1000);
    cv::namedWindow("Example", cv::WINDOW_NORMAL);
    cv::resizeWindow("Example", windowSize);
    cv::VideoCapture webcam = openWebcam();
    vector<cv::Rect> face, eye;

    cv::String faceCascadeFile = "haarcascade_frontalface_alt.xml";
    cv::String eyeCascadeFile = "haarcascade_eye_tree_eyeglasses.xml";
    cv::CascadeClassifier faceCascade;
    cv::CascadeClassifier eyeCascade;
    if (!faceCascade.load(faceCascadeFile)) {
        cout << "Face Cascade File Load Error" << endl;
        return -1;
    }
    if (!eyeCascade.load(eyeCascadeFile)) {
        cout << "Eye Cascade File Load Error" << endl;
        return -1;
    }

    for (;;) {
        webcam >> frame;
        if (frame.empty()) {
            cout << "Frame Error" << endl;
            return -1;
        }

        cv::cvtColor(frame, grayFrame, cv::COLOR_BGR2GRAY);
//        cv::equalizfeHist(grayFrame, grayFrame);
        faceCascade.detectMultiScale(grayFrame, face,1.1, 2, 0, cv::Size(30,30));
        for( size_t i = 0; i < face.size(); i++ ) {
            cv::Point center( face[i].x + face[i].width*0.5, face[i].y + face[i].height*0.5 );
            cv::ellipse(frame, center, cv::Size(face[i].width*0.7, face[i].height *0.7), 0, 0, 360, cv::Scalar( 255, 255, 255 ), 4, 8, 0 );
        }

        eyeCascade.detectMultiScale(grayFrame, eye, 1.1, 2, 0, cv::Size(30,30));
        for (size_t i = 0; i < eye.size(); i++) {
            cv::Point center(eye[i].x + eye[i].width*0.5, eye[i].y + eye[i].height*0.5);
            cv::circle(frame, center, eye[i].width*0.5, cv::Scalar(0,0,0), 2, 8, 0);
        }

        cv::imshow("Example", frame);
        if (cv::waitKey(33) > 0) return -1;
    }



}

int main(int argc, char *argv[])
{
    detectFace();
    return 0;
}

