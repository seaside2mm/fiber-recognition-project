//
// Created by kubota on 6/1/18.
//

#ifndef CONTROL_H
#define CONTROL_H

#include "common_include.h"
#include "frame.h"
#include "scene.h"
#include "video.h"

class Control
{
public:
/*
 * 检测视频中清晰度最好的帧
 */
    Mat computeBestFrame(string &video_dir, bool show_best_frame_info = true);

/*
 * 判断不同场景
 */
    void detectScene(string &video_dir);

};

#endif //CONTROL_H
