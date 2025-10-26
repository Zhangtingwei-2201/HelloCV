#include<opencv2/imgcodecs.hpp> 
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/opencv.hpp>
#include<iostream>
using namespace cv;
using namespace std;

// 检测交通信号灯颜色
string detectTrafficLightColor(Mat& frame) {
    Mat hsv, mask_red1, mask_red2, mask_red, mask_yellow, mask_green;
    
    // 转换到HSV颜色空间
    cvtColor(frame, hsv, COLOR_BGR2HSV);
    
    // 颜色范围 - 调整以更好地匹配红绿灯
    inRange(hsv, Scalar(0, 150, 150), Scalar(10, 255, 255), mask_red1);
    inRange(hsv, Scalar(170, 150, 150), Scalar(180, 255, 255), mask_red2);
    inRange(hsv, Scalar(20, 120, 120), Scalar(30, 255, 255), mask_yellow);
    inRange(hsv, Scalar(50, 100, 100), Scalar(85, 255, 255), mask_green);
    
    // 合并红色掩码
    mask_red = mask_red1 | mask_red2;
    
    // 形态学操作 - 使用更大的核来连接红绿灯区域
    Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(25, 25));
    morphologyEx(mask_red, mask_red, MORPH_CLOSE, kernel);
    morphologyEx(mask_yellow, mask_yellow, MORPH_CLOSE, kernel);
    morphologyEx(mask_green, mask_green, MORPH_CLOSE, kernel);
    
    // 检测轮廓
    vector<vector<Point>> contours_red, contours_yellow, contours_green;
    findContours(mask_red, contours_red, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    findContours(mask_yellow, contours_yellow, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    findContours(mask_green, contours_green, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    
    // 检测红色
    for (size_t i = 0; i < contours_red.size(); i++) {
        double area = contourArea(contours_red[i]);
        // 使用较大的面积阈值，只检测大的红绿灯区域
        if (area > 1500) {
            // 获取外接矩形
            Rect bbox = boundingRect(contours_red[i]);
            
            // 计算宽高比，圆形区域的宽高比应该接近1
            double aspect_ratio = (double)bbox.width / bbox.height;
            
            // 只识别接近正方形的区域（宽高比在0.7-1.3之间）
            if (aspect_ratio > 0.7 && aspect_ratio < 1.3) {
                rectangle(frame, bbox, Scalar(0, 0, 255), 3);
                putText(frame, "Red Light", Point(bbox.x, bbox.y - 10),
                        FONT_HERSHEY_SIMPLEX, 0.7, Scalar(0, 0, 255), 2);
                return "Red";
            }
        }
    }
    
    // 检测黄色
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
    
    // 检测绿色
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
    // 读取视频文件
    VideoCapture cap("Resources/jnaTrafficLight.mp4");
    if (!cap.isOpened()) {
        cout << "Error: Could not open video file" << endl;
        return -1;
    }
    
    // 获取视频属性
    int frame_width = cap.get(CAP_PROP_FRAME_WIDTH);
    int frame_height = cap.get(CAP_PROP_FRAME_HEIGHT);
    double fps = cap.get(CAP_PROP_FPS);
    
            // 创建视频写入对象
    VideoWriter writer("result.avi", 
                      VideoWriter::fourcc('M','J','P','G'), 
                      fps, 
                      Size(frame_width, frame_height));
    
    Mat frame;
    
    // 创建缩小一半的显示窗口
    namedWindow("Traffic Light Detection", WINDOW_NORMAL);
    resizeWindow("Traffic Light Detection", frame_width/2, frame_height/2);
    
    while (true) {
        cap >> frame;
        if (frame.empty()) {
            break;
        }
        
        // 检测交通信号灯颜色
        string color = detectTrafficLightColor(frame);
        
        // 在左上角显示检测结果
        putText(frame, "Light: " + color, Point(20, 40), 
                FONT_HERSHEY_SIMPLEX, 1.2, Scalar(255, 255, 255), 3);
        
        // 写入输出视频
        writer.write(frame);
        
        // 显示实时结果
        imshow("Traffic Light Detection", frame);
        
        // 减少等待时间以加速视频播放
        if (waitKey(1) == 27) {
            break;
        }
    }
    
    // 释放资源
    cap.release();
    writer.release();
    destroyAllWindows();
    
    cout << "Processing completed! Output saved as result.avi" << endl;
    
    return 0;
}