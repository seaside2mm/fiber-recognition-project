//
// Created by kubota on 6/2/18.
//

#include "../include/frame.h"
#include "../include/config.h"

Frame::Frame(){}

Frame::Frame(Mat& image)
{
    this->image = image;
}

double Frame::computeShapeness(int ksize)
{
    cv::Mat Gx, Gy;
    cv::Sobel(Frame::image, Gx, CV_64F, 1, 0, ksize);
    cv::Sobel(Frame::image, Gy, CV_64F, 0, 1, ksize);

    cv::Mat FM = Gx.mul(Gx) + Gy.mul(Gy);

    double sharpness = cv::mean(FM).val[0];
    return sharpness;
}


void Frame::detectCircles(vector<Vec3f> &circles)
{
    Mat gray;
    cvtColor(Frame::image, gray, CV_BGR2GRAY);
    GaussianBlur(gray, gray, Size(3,3), 2, 2);
    HoughCircles(gray, circles, CV_HOUGH_GRADIENT, 1, gray.rows/8, 21, 55, 100, 300);
    Frame::circles = circles;
}



void Frame::drawCircles()
{
    for(int i=0; i<Frame::circles.size(); i++)
    {
        circle(Frame::image, Point(Frame::circles[i][0],Frame::circles[i][1]), 2, Scalar(0,255,0), 3); //
        circle(Frame::image, Point(Frame::circles[i][0], Frame::circles[i][1]), Frame::circles[i][2], Scalar(0,0,255), 2, 2 );  //Scalar(0,0,255)
    }
}



void Frame::computeReferenceDistance(const vector<Vec3f>& circles, const vector<cv::Point2f>& reference_point)
{

    vector<Point2f> distance;
    Point2f left_distance;
    Point2f right_distance;

    Point2f left_center;
    Point2f right_center;

    Point2f left_reference = Point(reference_point[0].x,reference_point[0].y);
    Point2f right_reference = Point(reference_point[1].x,reference_point[1].y);

    //两个圆
    if (circles.size() == 2 )
    {
        cout << "\n\n***************此帧有两个圆***************" << endl;
        if ( circles[0][0] < circles[1][0] )  //第一个为左圆
        {
            left_distance.x = circles[0][0] - reference_point[0].x;
            left_distance.y = circles[0][1] - reference_point[0].y;

            right_distance.x = circles[0][0] - reference_point[1].x;
            right_distance.y = circles[0][1] - reference_point[1].y;

            left_center = Point(circles[0][0],circles[0][1]);
            right_center = Point(circles[1][0],circles[1][1]);
        }
        else  //第一个为右圆
        {
            left_distance.x = circles[1][0] - reference_point[0].x;
            left_distance.y = circles[1][1] - reference_point[0].y;

            right_distance.x = circles[0][0] - reference_point[1].x;
            right_distance.y = circles[0][1] - reference_point[1].y;

            left_center = Point(circles[1][0],circles[1][1]);
            right_center = Point(circles[0][0],circles[0][1]);
        }
        cv::line(Frame::image, left_center, left_reference, (0, 200, 255), 2, 3);
        cv::line(Frame::image, right_center, right_reference, (0, 200, 255), 2, 3);

        cout << "左圆圆心坐标：(" << circles[0][0] << " " << circles[0][1] << "), 圆直径： "<< circles[0][2] <<
             "; \n右圆圆心坐标：(" << circles[1][0] << " " << circles[1][1] << "), 圆直径： "<< circles[1][2] << endl;
        printf("\n圆心距离方框中心距离：\n左圆：x = %.1f, y = %.1f, \n右圆： x = %.1f, y = %.1f", left_distance.x, left_distance.y, right_distance.x, right_distance.y);
    }
    else if (circles.size() == 1)
    {
        cout << "\n\n***************此帧有一个圆***************"<< endl;
        cout << "圆心坐标：(" << circles[0][0] << " " << circles[0][1] << "), 圆直径数："<< circles[0][2] << endl;
        if ((circles[0][1] - reference_point[0].x) < (circles[0][1] - reference_point[1].x))
        {
            left_distance.x = circles[0][0] - reference_point[0].x;
            left_distance.y = circles[0][1] - reference_point[0].y;
            left_center = Point(circles[0][0],circles[0][1]);
            cv::line(Frame::image, left_center, left_reference, (0, 200, 255), 2, 3);
            printf("\n圆心距离方框中心距离：\n左圆：x = %.1f, y = %.1f", left_distance.x, left_distance.y);
        }
        else
        {
            right_distance.x = circles[0][0] - reference_point[1].x;
            right_distance.y = circles[0][1] - reference_point[1].y;
            right_center = Point(circles[0][0],circles[0][1]);
            cv::line(Frame::image, right_center, right_reference, (0, 200, 255), 2, 3);
            printf("\n圆心距离方框中心距离：\n右圆： x = %.1f, y = %.1f", right_distance.x, right_distance.y);
        }
    }
    else
    {
        cout << "没有圆或者超出两个圆";
    }
}


int Frame::getBestFrameIndex(vector<double>& values)
{
    vector<double>::iterator min_value = min_element(values.begin(), values.end());
    int index = distance(values.begin(), min_value);
    return index;
}

bool Frame::isOverlapped(const vector<Vec3f>& circles)
{
    double x = circles[0][0] - circles[1][0];
    double y = circles[0][1] - circles[1][1];
    double l = sqrt((x*x + y*y));
    double d = circles[0][2] + circles[1][2];
    if( d >= l )
        return true;
    else
        return false;
}

vector<Point2f> Frame::getReferencePoint()
{
    Config::setParameterFile("../config/default.yaml");
    float lx = Config::get<float>("left_point.x");
    float ly = Config::get<float>("left_point.y");
    float rx = Config::get<float>("right_point.x");
    float ry = Config::get<float>("right_point.y");

    Point2f left_point(lx,ly);
    Point2f right_point(rx,ry);
    vector<Point2f> reference_point;
    reference_point.push_back(left_point);
    reference_point.push_back(right_point);

    return reference_point;
}