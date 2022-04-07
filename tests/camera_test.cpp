#include <camera/DisplayImage.h>
#include <stdio.h>
#include "camera/opencv2/opencv.hpp"
#include "camera/opencv2/highgui/highgui.hpp"
#include "camera/opencv2/imgproc/imgproc.hpp"
#include "camera/opencv2/imgproc/imgproc.hpp"


void test_camera(void){
    cv::VideoCapture cap(0);
    if(!cap.isOpened()){return;}
    cv::Mat image;

    // while(!){

    // }

    cap>>image;
    cv::imwrite("frame.jpg",image);
}

int main(int argc, char const *argv[]){
  
    test_camera();
    return 0;
}