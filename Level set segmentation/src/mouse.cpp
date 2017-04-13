//
// Created by Михаил Мурашов on 04.04.17.
//

#include "mouse.h"

void Mouse::onMouse(int event, int x, int y, int, void*) {
    switch (event) {
        case EVENT_LBUTTONDOWN :
            mouse.is_selection_started = true;
            mouse.is_selection_finished = false;
            mouse.point_first = Point(x, y);
            break;

        case EVENT_LBUTTONUP :
            mouse.is_selection_started = false;
            mouse.is_selection_finished = true;
            mouse.point_second = Point(x, y);
            break;

        case EVENT_MOUSEMOVE :
            if (mouse.is_selection_started && !mouse.is_selection_finished)
                mouse.point_second = Point(x, y);
    }
}

Rect Mouse::drawRect(const Mat src, const string windowName) {
    namedWindow(windowName, WINDOW_NORMAL);
    setMouseCallback(windowName, Mouse::onMouse, 0);

    imshow(windowName, src);
    waitKey(1);

    Rect rect;
    mouse.is_selection_started = false;
    mouse.is_selection_finished = false;

    while (!mouse.is_selection_finished) {
        if (mouse.is_selection_started) {
            Mat src_cpy;
            src.copyTo(src_cpy);

            rect.x = mouse.point_first.x;
            rect.y = mouse.point_first.y;
            rect.width = mouse.point_second.x - mouse.point_first.x;
            rect.height = mouse.point_second.y - mouse.point_first.y;

            rectangle(src_cpy, rect, Scalar(0));
            imshow(windowName, src_cpy);
        }

        waitKey(30);
    }

    return rect;
}
