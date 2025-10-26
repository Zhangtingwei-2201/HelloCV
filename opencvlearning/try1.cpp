#include<opencv2/imgcodecs.hpp> 
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/opencv.hpp>
#include<iostream>
using namespace cv;
using namespace std;

#include <opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;

int main() {
    // 1. 初始化视频读取对象（两种方式任选）
    // 方式1：读取本地视频文件（替换为你的视频路径，支持 MP4/AVI 等格式）
    VideoCapture cap("Resources/test_video.mp4");
    // 方式2：读取摄像头（0 表示默认摄像头，1 表示外接摄像头）
    // VideoCapture cap(0);

    // 检查视频/摄像头是否成功打开
    if (!cap.isOpened()) {
        cout << "无法打开视频/摄像头，请检查路径或设备！" << endl;
        return -1;
    }

    // 2. 逐帧读取并处理
    Mat frame;          // 存储原始帧
    Mat gray_frame;     // 存储灰度化后帧
    Mat filtered_frame; // 存储滤波后帧

    while (true) {
        // 读取一帧（成功返回 true，视频结束返回 false）
        bool ret = cap.read(frame);
        if (!ret) {
            cout << "视频已播放完毕或读取失败！" << endl;
            break;
        }

        // ---------------------- 帧处理步骤（可自定义）----------------------
        // 步骤1：将原始 BGR 帧转为灰度帧
        cvtColor(frame, gray_frame, COLOR_BGR2GRAY);

        // 步骤2：对灰度帧进行高斯滤波（降噪，核大小需为奇数，如 5x5）
        GaussianBlur(gray_frame, filtered_frame, Size(5, 5), 0);
        // -------------------------------------------------------------------

        // 3. 显示处理前后的帧
        imshow("原始帧（BGR）", frame);         // 显示原始帧
        imshow("灰度化+高斯滤波后帧", filtered_frame); // 显示处理后帧

        // 4. 控制播放速度（25ms 对应 ~40fps，按 ESC 键退出）
        if (waitKey(25) == 27) { // 27 是 ESC 键的 ASCII 码
            cout << "用户手动退出！" << endl;
            break;
        }
    }

    // 5. 释放资源（必须执行，避免内存泄漏）
    cap.release();       // 释放视频读取对象
    destroyAllWindows(); // 关闭所有显示窗口

    return 0;
}