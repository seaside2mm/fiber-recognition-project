//
// Created by kubota on 6/10/18.
//

#include "../include/scene.h"
#include "../include/frame.h"

Scene::Scene(){}

Scene::Scene(Mat &image)
{
    this->image = image;
}

void Scene::noCircle()
{
    cout <<"*************输入图像缺少两个圆***************\n" << endl;
}

void Scene::oneCircle()
{
    Frame frame(Scene::image);
    double sharpness = frame.computeShapeness();
    cout <<"\n*************输入图像缺少一个圆*************";
    cout <<"\n图像清晰度为：" << sharpness << endl;
}

void Scene::twoCircle()
{
    Frame frame(Scene::image);
    double sharpness = frame.computeShapeness();
    vector<Vec3f> circles;
    frame.detectCircles(circles);
    if( frame.isOverlapped(circles) )
    {
        cout<<"\n*************输入图像两个圆重叠*************\n";
    }
    else
    {
        cout<<"\n*************输入图像两个圆不重叠*************";
        cout << "\n图像清晰度：" << sharpness;
        cout << "\n圆1--->圆心坐标：(" << circles[0][0] << " " << circles[0][1] << ")  圆直径像素数： "<< circles[0][2];
        cout << "\n圆2--->圆心坐标：(" << circles[1][0] << " " << circles[1][1] << ")  圆直径像素数： "<< circles[1][2] << endl;
    }
}

void Scene::moreCircle()
{
    cout << "\n*************输入图像多于两个圆*************\n";
}
