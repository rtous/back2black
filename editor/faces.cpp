/*
 * Utils related to the video 
 */

#include <stdio.h>
#include "video.h"
#include <opencv2/core/utils/filesystem.hpp>
#include <opencv2/video/video.hpp>
#include "common1.h"

#include <dlib/opencv.h>
#include <opencv2/highgui/highgui.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>

using namespace dlib;
using namespace std;

//from https://stackoverflow.com/questions/42223991/how-to-use-shape-predictor-68-face-landmarks-dat-in-landmark-extraction-in-ope
void face(){
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
    cv::Mat img = cv::imread("img.jpg", cv::IMREAD_COLOR);
    cv_image<bgr_pixel> cimg(img);
    // Detect faces 
    std::vector<rectangle> faces = detector(cimg);
    // Find the pose of each face.
    std::vector<full_object_detection> shapes;
    for (unsigned long i = 0; i < faces.size(); ++i)
        shapes.push_back(pose_model(cimg, faces[i]));
}