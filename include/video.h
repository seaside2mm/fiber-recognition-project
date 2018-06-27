//
// Created by kubota on 6/2/18.
//

#ifndef FIBER_PROJECT_VIDEO_H
#define FIBER_PROJECT_VIDEO_H

#include "common_include.h"
#include "../include/frame.h"

class Video
{
public:

    bool computeAssociation(string dir);

private:
    double getPSNR(const Mat& I1, const Mat& I2);
    Scalar getMSSIM(const Mat& i1, const Mat& i2);   //图像关联度函数
};
#endif //FIBER_PROJECT_VIDEO_H
