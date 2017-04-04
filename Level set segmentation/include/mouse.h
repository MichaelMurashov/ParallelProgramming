//
// Created by Михаил Мурашов on 04.04.17.
//

#ifndef LEVEL_SET_MOUSE_H
#define LEVEL_SET_MOUSE_H

#include <iostream>

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace cv;
using namespace std;

struct MousePosition {
    bool is_selection_started;
    bool is_selection_finished;
    Point point_first;
    Point point_second;
};

namespace Mouse {
    void onMouse(int event, int x, int y, int, void*);
    Rect drawRect(const Mat src, const string windowName);

    static MousePosition mouse;
}

#endif //LEVEL_SET_MOUSE_H
