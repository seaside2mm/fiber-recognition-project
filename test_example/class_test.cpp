//
// Created by kubota on 6/23/18.
//

#include "../include/common_include.h"
#include "../include/control.h"
#include "../include/scene.h"
#include "../include/frame.h"

void testScene()
{
    /*******************测试Scene模块****************/

    Mat no_circle = imread("../data/no_circle.bmp");
    Mat one_circle = imread("../data/one_circle.bmp");
    Mat two_circle_overlapped = imread("../data/two_circle_overlapped.bmp");
    Mat two_circle_no_overlapped = imread("../data/two_circle_no_overlapped.bmp");

    Scene scene1(no_circle);
    Frame frame1(no_circle);
    vector<Vec3f> circles1;
    frame1.detectCircles(circles1);
    frame1.drawCircles();
    scene1.noCircle();
    imshow("no_circle", no_circle);
    waitKey(0);

    Scene scene2(one_circle);
    Frame frame2(one_circle);
    vector<Vec3f> circles2;
    frame2.detectCircles(circles2);
    frame2.drawCircles();
    scene2.oneCircle();
    imshow("one_circle", one_circle);
    waitKey(0);

    Scene scene3(two_circle_overlapped);
    Frame frame3(two_circle_overlapped);
    vector<Vec3f> circles3;
    frame3.detectCircles(circles3);
    frame3.drawCircles();
    scene3.twoCircle();
    imshow("two_circle_overlapped", two_circle_overlapped);
    waitKey(0);

    Scene scene4(two_circle_no_overlapped);
    Frame frame4(two_circle_no_overlapped);
    vector<Vec3f> circles4;
    frame4.detectCircles(circles4);
    frame4.drawCircles();
    scene4.twoCircle();
    imshow("two_circle_no_overlapped", two_circle_no_overlapped);
    waitKey(0);
}

void testFrame()
{
    /*******************测试frame模块****************/
    Mat image = imread("../data/left/1.bmp");
    Frame frame(image);

    //测试清晰度检测
    double  sharpness = frame.computeShapeness();

    //测试圆检测
    vector<Vec3f> circles;
    frame.detectCircles(circles);
    frame.drawCircles();
    cout << sharpness;

    // 测试测试中心距离方框中心基准距离
    vector<Point2f> reference_point = frame.getReferencePoint();
    frame.computeReferenceDistance(circles, reference_point);

    imshow("image", image);
    waitKey(0);
}

//int main()
//{
//    testFrame();
//    testScene();
//}