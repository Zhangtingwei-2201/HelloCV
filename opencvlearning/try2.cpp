#include<opencv2/imgcodecs.hpp> 
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/opencv.hpp>
#include<iostream>
using namespace cv;
using namespace std;

//语雀文档中有详细说明和解释

string detectTrafficLightColor(Mat& frame) {
    Mat hsv, mask_red1, mask_red2, mask_red, mask_yellow, mask_green;
    cvtColor(frame, hsv, COLOR_BGR2HSV);
    inRange(hsv, Scalar(0, 150, 150), Scalar(10, 255, 255), mask_red1);
    inRange(hsv, Scalar(170, 150, 150), Scalar(180, 255, 255), mask_red2);
    inRange(hsv, Scalar(20, 120, 120), Scalar(30, 255, 255), mask_yellow);
    inRange(hsv, Scalar(50, 100, 100), Scalar(85, 255, 255), mask_green);
    mask_red = mask_red1 | mask_red2;
    Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(25, 25));
    morphologyEx(mask_red, mask_red, MORPH_CLOSE, kernel);
    morphologyEx(mask_yellow, mask_yellow, MORPH_CLOSE, kernel);
    morphologyEx(mask_green, mask_green, MORPH_CLOSE, kernel);
    vector<vector<Point>> contours_red, contours_yellow, contours_green;
    findContours(mask_red, contours_red, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    findContours(mask_yellow, contours_yellow, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    findContours(mask_green, contours_green, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE)
    for (size_t i = 0; i < contours_red.size(); i++) {
        double area = contourArea(contours_red[i]);
        if (area > 1500) {
            Rect bbox = boundingRect(contours_red[i]);
            double aspect_ratio = (double)bbox.width / bbox.height;
            if (aspect_ratio > 0.7 && aspect_ratio < 1.3) {
                rectangle(frame, bbox, Scalar(0, 0, 255), 3);
                putText(frame, "Red Light", Point(bbox.x, bbox.y - 10),
                        FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 255), 2);
                return "Red";
            }
        }
    }
    
    for (size_t i = 0; i < contours_yellow.size(); i++) {
        double area = contourArea(contours_yellow[i]);
        if (area > 1500) {
            Rect bbox = boundingRect(contours_yellow[i]);
            double aspect_ratio = (double)bbox.width / bbox.height;
            if (aspect_ratio > 0.7 && aspect_ratio < 1.3) {
                rectangle(frame, bbox, Scalar(0, 255, 255), 3);
                putText(frame, "Yellow Light", Point(bbox.x, bbox.y - 10),
                        FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 255, 255), 2);
                return "Yellow";
            }
        }
    }
    
    for (size_t i = 0; i < contours_green.size(); i++) {
        double area = contourArea(contours_green[i]);
        if (area > 1500) {
            Rect bbox = boundingRect(contours_green[i]);
            double aspect_ratio = (double)bbox.width / bbox.height;
            if (aspect_ratio > 0.7 && aspect_ratio < 1.3) {
                rectangle(frame, bbox, Scalar(0, 255, 0), 3);
                putText(frame, "Green Light", Point(bbox.x, bbox.y - 10),
                        FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 255, 0), 2);
                return "Green";
            }
        }
    }
    return "None";
}

int main() {
    VideoCapture cap("Resources/jnaTrafficLight.mp4");
    if (!cap.isOpened()) {
        cout << "Error: Could not open video file" << endl;
        return -1;
    }
    int frame_width = cap.get(CAP_PROP_FRAME_WIDTH);
    int frame_height = cap.get(CAP_PROP_FRAME_HEIGHT);
    double fps = cap.get(CAP_PROP_FPS);
    VideoWriter writer("result.avi", VideoWriter::fourcc('M','J','P','G'),fps,Size(frame_width, frame_height));
    Mat frame;
    namedWindow("Traffic Light Detection", WINDOW_NORMAL);
    resizeWindow("Traffic Light Detection", frame_width/2, frame_height/2);
    while (true) {
        cap >> frame;
        if (frame.empty()) {
            break;
        }
        string color = detectTrafficLightColor(frame);
        putText(frame, "Light: " + color, Point(20, 40), 
                FONT_HERSHEY_SIMPLEX, 1.2, Scalar(255, 255, 255), 3);
        writer.write(frame);
        imshow("Traffic Light Detection", frame);
        if (waitKey(1) == 27) {
            break;
        }
    }
    cap.release();
    writer.release();
    destroyAllWindows();
    cout << "Processing completed! Output saved as result.avi" << endl;
    return 0;
}
