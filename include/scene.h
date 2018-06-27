//
// Created by kubota on 6/10/18.
//

#ifndef FIBER_PROJECT_SCENE_H
#define FIBER_PROJECT_SCENE_H

#include "common_include.h"

/*TODO
 * 增加类说明
 */

class Scene
{
public:

    Scene();
    Scene(Mat &image);

    void noCircle();
    void oneCircle();
    void twoCircle();
    void moreCircle();

    Mat image;
};

#endif //FIBER_PROJECT_SCENE_H

