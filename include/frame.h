//
// Created by kubota on 6/1/18.
//

#ifndef FRAME_H
#define FRAME_H

#include "common_include.h"


/*
 * 针对单独视频帧作出处理的函数，封装传给control模块
 */
class Frame
{
public:
    //构造函数
    Frame();
    Frame(Mat& image);

/*
 * 评价视频帧的清晰度函数，注意结果越小，清晰度越好
 */
    double computeShapeness(int ksize = 3);

/*
 *圆心的屏幕坐标，圆心与方框图在上下、左右两方向的正负像素差值。
 */
    void computeReferenceDistance(const vector<Vec3f> &circles, const vector<cv::Point2f>& reference_point);

/*
 * 检测图像中的圆
 */
    void detectCircles(vector<Vec3f>& circles);

/*
 * 画出检测到的圆
 */
    void drawCircles();

/*
 * 输入所有帧的清晰度，得到最优清晰度的帧索引
 */
    int getBestFrameIndex(vector<double>& values);

/*
 * 计算检测到的圆是否重叠
 */
    bool isOverlapped(const vector<Vec3f>& circles);

/*
 * 得到方框图中心坐标
 */
    vector<Point2f> getReferencePoint();

    //数据成员
    Mat image;
    vector<Vec3f> circles;
};

#endif //FRAME_H
