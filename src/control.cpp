//
// Created by kubota on 6/1/18.
//

#include "../include/control.h"
#include "../include/config.h"

Mat Control::computeBestFrame(string &video_dir, bool show_best_frame_info)
{
    int time = Config::get<int>("time");

    //视频输入，帧提取
    VideoCapture capture;
    capture.open(video_dir);
    if( !capture.isOpened() )
    {
        cout << "No input Video";
    }

    double total_frames = capture.get(CV_CAP_PROP_FRAME_COUNT);      //计算视频总共帧数

    //找出最佳帧
    vector<double> sharpness_of_frames;  //储存每一帧清晰度

    cout << "\n\n#################    每一帧信息    ##################";
    Frame frame;
    while(true)
    {
        Mat image;
        capture >> image;
        if(image.empty())
            break;

        frame.image = image;
        double sharpness = frame.computeShapeness();
        sharpness_of_frames.push_back(sharpness);

        //检测圆
        vector<Vec3f> circles;
        frame.detectCircles(circles);
        frame.drawCircles();

        //计算圆心偏离距离
        vector<Point2f> reference_point = frame.getReferencePoint();
        frame.computeReferenceDistance(circles, reference_point);

        imshow("image", image);
        waitKey(time);
    }

    //得到最佳帧
    int best_frame_index = frame.getBestFrameIndex(sharpness_of_frames);
    capture.set(CV_CAP_PROP_POS_FRAMES, best_frame_index);  //设置视频中最佳帧位置
    Mat best_frame;
    capture.read(best_frame);

    if (show_best_frame_info)
    {
        Frame frame(best_frame);
        vector<Vec3f> circles;
        frame.detectCircles(circles);
        frame.drawCircles();

        vector<Point2f> reference_point = frame.getReferencePoint();

        cout << "\n\n#################    最佳帧信息    ##################";
        cout << "\n\n视频全部帧数目: " << total_frames << "，最佳帧位置: " << best_frame_index;
        frame.computeReferenceDistance(circles, reference_point);
        imshow("best frame", best_frame);
        waitKey(time);
    }
    return best_frame;
}


void Control::detectScene(string &video_dir)
{
    int time = Config::get<int>("time");

    VideoCapture capture;
    capture.open(video_dir);
    if (!capture.isOpened())
    {
        cout << "No input Video";
    }

    while (true) {
        //输出每一帧
        Mat image;
        capture >> image;

        if (image.empty())
            break;

        //得到帧中的圆
        Frame frame(image);
        vector<Vec3f> circles;
        frame.detectCircles(circles);
        frame.drawCircles();

        // 测试检测各个场景
        Scene scene(image);
        if( circles.size() == 0 )
        {
            scene.noCircle();
        }
        else if( circles.size() == 1 )
        {
            scene.oneCircle();    //输入另一个圆的清晰度评价值
        }
        else if( circles.size() == 2 )
        {
            scene.twoCircle();
        }
        else
        {
            scene.moreCircle();
        }
        imshow("image", image);
        waitKey(time);
    }
}


