#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <opencv2/video/video.hpp>

#include <dlib/opencv.h>
//#include <opencv2/highgui/highgui.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing/shape_predictor.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>

#include "face.h"

using namespace dlib;
using namespace std;

void dlib_test(cv::Mat img, cv::Mat & res){
    // Load face detection and pose estimation models.
    frontal_face_detector detector = get_frontal_face_detector();
    shape_predictor pose_model;
    deserialize("checkpoints/shape_predictor_68_face_landmarks.dat") >> pose_model;
    // Turn OpenCV's Mat into something dlib can deal with.  Note that this just
    // wraps the Mat object, it doesn't copy anything.  So cimg is only valid as
    // long as temp is valid.  Also don't do anything to temp that would cause it
    // to reallocate the memory which stores the image as that will make cimg
    // contain dangling pointers.  This basically means you shouldn't modify temp
    // while using cimg.
    //cv::Mat img = cv::imread("img.jpg", cv::IMREAD_COLOR);
    cv_image<bgr_pixel> cimg(img);

    // Detect faces 
    std::vector<rectangle> dets = detector(cimg);

    for (size_t i = 0; i < dets.size(); ++i) {
        printf("face detected\n");
        //cv::rectangle(res, cv::Rect(dets[i].left(), dets[i].top(), dets[i].right(), dets[i].bottom()), cv::Scalar(255,255,255), 2);//Using circle()function to draw the line//
        cv::Point start_point = cv::Point(dets[i].left(), dets[i].top());
        cv::Point end_point = cv::Point(dets[i].right(), dets[i].bottom());
        //cv::rectangle(res, cv::Rect(dets[i].left(), dets[i].top(), dets[i].right(), dets[i].bottom()), cv::Scalar(255,255,255), 2);//Using circle()function to draw the line//
        cv::rectangle(res, start_point, end_point, cv::Scalar(255,255,255), 2);//Using circle()function to draw the line//
        printf("face detected\n");
        printf("detecting ladnmarks for face %d\n", i);
        // Detect landmarks for this face
        full_object_detection shape;
        shape = pose_model(cimg, dets[i]);

        // Draw landmarks on the image
        for (size_t j = 0; j < shape.num_parts(); ++j) {
            int x = shape.part(j).x();
            int y = shape.part(j).y();
            printf("landmark num %d = %d,%d\n", j, x, y);
            // cv::circle(img, cv::Point(x, y), 2, cv::Scalar(0, 255, 0), -1); // Green color
            // Draw a circle at the specified point (x, y) with a radius of 2 pixels
            //draw_solid_circle(cimg, point(x, y), 2, rgb_pixel(0, 255, 0));
            cv::circle(res, cv::Point(x,y), 2, cv::Scalar(255,255,255), 2);//Using circle()function to draw the line//
        }
    }


}

int main()
{
	std::cerr << "Test tool for DLIB face landmarks detector.!\n";
	printf("trying to open and image with opencv...\n");
    std::string image_path = cv::samples::findFile("img3.jpg");
    cv::Mat img = cv::imread(image_path);
    if(img.empty())
    {
        std::cout << "Could not read the image: " << image_path << std::endl;
        return 1;
    }
    cv::imshow("Display window", img);
    int k = cv::waitKey(0); // Wait for a keystroke in the window
    
    //cv::Mat res = cv::Mat::zeros(cv::Size(img.cols,img.rows), CV_8UC1);
    cv::Mat res = img.clone();
    dlib_test(img, res);
    cv::imshow("Display window", res);
    k = cv::waitKey(0); // Wait for a keystroke in the window
    
    //cv::Mat res2 = cv::Mat::zeros(cv::Size(img.cols,img.rows), CV_8UC1);
    //cv::Mat res2 = img.clone();
    cv::Mat res2 = cv::Mat(cv::Size(img.cols,img.rows), CV_8UC4, cv::Scalar(0,0,0,0));

    face(img, res2, cv::Scalar(118, 113, 168, 255), cv::Scalar(61, 71, 118, 255));
    cv::imshow("Display window", res2);
    k = cv::waitKey(0); // Wait for a keystroke in the window

    cv::imwrite("borrar.png", res2);

    return 0;
}
