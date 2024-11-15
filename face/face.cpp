#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <opencv2/video/video.hpp>

#include <dlib/opencv.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing/shape_predictor.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>

using namespace dlib;
using namespace std;

void draw_eyes_contour(full_object_detection shape, cv::Mat & res, cv::Scalar color){
    int x_plus_top = (shape.part(37).x() - shape.part(44).x())/4;
    int x_plus_bottom = (shape.part(46).x() - shape.part(41).x())/4;
    int y_plus_left = (shape.part(37).y() - shape.part(41).y())/5;
    int y_plus_right = (shape.part(44).y() - shape.part(46).y())/5;
    
    std::vector<cv::Point> eye_contour;
    eye_contour.push_back(cv::Point(shape.part(37).x() + x_plus_top, shape.part(37).y() + y_plus_left));
    eye_contour.push_back(cv::Point(shape.part(44).x() - x_plus_top, shape.part(44).y() + y_plus_right));
    eye_contour.push_back(cv::Point(shape.part(46).x() + x_plus_bottom, shape.part(46).y() - y_plus_right));
    eye_contour.push_back(cv::Point(shape.part(41).x() - x_plus_bottom, shape.part(41).y() - y_plus_right));
    eye_contour.push_back(cv::Point(shape.part(41).x() - x_plus_bottom, shape.part(41).y() - y_plus_left));
    eye_contour.push_back(cv::Point(shape.part(37).x() + x_plus_top, shape.part(37).y() + y_plus_left));

    cv::fillPoly(res, eye_contour, color);
}

void draw_pupils(full_object_detection shape, cv::Mat & res, cv::Scalar color){
    int radius = abs(shape.part(39).x() - shape.part(36).x()) / 4;
    int pupil_x = abs(shape.part(39).x() + shape.part(36).x()) / 2;
    int pupil_y = abs(shape.part(39).y() + shape.part(36).y()) / 2;
    cv::Point pupil = cv::Point(pupil_x, pupil_y);
    cv::circle(res, pupil, radius, color, -1); //-1 means fill

    int radius2 = abs(shape.part(42).x() - shape.part(45).x()) / 4;
    int pupil_x2 = abs(shape.part(42).x() + shape.part(45).x()) / 2;
    int pupil_y2 = abs(shape.part(42).y() + shape.part(45).y()) / 2;
    cv::Point pupil2 = cv::Point(pupil_x2, pupil_y2); 
    cv::circle(res, pupil2, radius2, color, -1); //-1 means fill*/
}

void draw_mouth(full_object_detection shape, cv::Mat & res, cv::Scalar color){
    std::vector<cv::Point> contour;
    for (int i=60; i<=67; i++) {
        contour.push_back(cv::Point(shape.part(i).x(), shape.part(i).y()));
    }
    cv::fillPoly(res, contour, color);
}

void draw_nose(full_object_detection shape, cv::Mat & res, cv::Scalar color){  
    std::vector<cv::Point> contour;
    contour.push_back(cv::Point(shape.part(31).x(), shape.part(31).y()));
    contour.push_back(cv::Point(shape.part(33).x(), shape.part(33).y()));
    contour.push_back(cv::Point(shape.part(33).x(), shape.part(33).y()));
    contour.push_back(cv::Point(shape.part(35).x(), shape.part(35).y()));
    contour.push_back(cv::Point(shape.part(35).x(), shape.part(35).y()));
    contour.push_back(cv::Point(shape.part(31).x(), shape.part(31).y()));
    cv::fillPoly(res, contour, color);
}

void face(cv::Mat img, cv::Mat & res, cv::Scalar color, cv::Scalar pupilsColor){
    frontal_face_detector detector = get_frontal_face_detector();
    shape_predictor pose_model;
    deserialize("checkpoints/shape_predictor_68_face_landmarks.dat") >> pose_model;
    // Turn OpenCV's Mat into something dlib can deal with.  Note that this just
    // wraps the Mat object, it doesn't copy anything.  So cimg is only valid as
    // long as temp is valid.  Also don't do anything to temp that would cause it
    // to reallocate the memory which stores the image as that will make cimg
    // contain dangling pointers.  This basically means you shouldn't modify temp
    // while using cimg.
    cv_image<bgr_pixel> cimg(img);

    // Detect faces 
    std::vector<rectangle> dets = detector(cimg);
    
    for (size_t i = 0; i < dets.size(); ++i) {
        cv::Point start_point = cv::Point(dets[i].left(), dets[i].top());
        cv::Point end_point = cv::Point(dets[i].right(), dets[i].bottom());
        full_object_detection shape;
        // Detect landmarks
        shape = pose_model(cimg, dets[i]);

        draw_eyes_contour(shape, res, color);
        draw_pupils(shape, res, pupilsColor);
        draw_mouth(shape, res, color);
        draw_nose(shape, res, color);
        
    }
}

