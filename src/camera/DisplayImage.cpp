#include <stdio.h>
#include "./opencv2/opencv.hpp"
#include "./opencv2/highgui/highgui.hpp"
#include "./opencv2/imgproc/imgproc.hpp"
#include "./opencv2/imgproc/imgproc.hpp"

//#include <opencv2/opencv.hpp>


// static void processFramesFromCamera();

static void processFramesFromCamera(){
    cv::VideoCapture cap(0);
    if(!cap.isOpened()){return;}
    cv::Mat image;

    // while(!){

    // }

    cap>>image;
    cv::imwrite("frame.jpg",image);


}

void init_camera(void)
{
    // cv::Mat image;
    // image = cv::imread( "lena.jpeg", 1 );
    // if ( !image.data )
    // {
    //     printf("No image data \n");
    //     exit(-1);
    // }
    // cv::namedWindow("Display Image", cv::WINDOW_AUTOSIZE );
    // imshow("Display Image", image);
    // cv::waitKey(0);


    processFramesFromCamera();
}