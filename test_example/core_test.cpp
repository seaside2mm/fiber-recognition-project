//
// Created by kubota on 6/23/18.
//

//测试，看看valgrind这个工具

#include "../include/common_include.h"
#include "../include/control.h"
#include "../include/config.h"

void test_computeBestFrame()
{
    //读入参数文件，读入视频
    string video_dir = Config::get<string>("video_dir");

    //测试检测最优帧
    Control control;
    control.computeBestFrame(video_dir, true);  //true为输出最佳帧信息，默认输出
}


void test_detectScene() {
    //读入参数文件，读入视频
    string video_dir = Config::get<string>("video_dir");

    // 测试检测各个场景
    Control control;
    control.detectScene(video_dir);
}

int main()
{
    Config::setParameterFile("../config/default.yaml");
    test_computeBestFrame();
    //test_detectScene();
    return 0;
}