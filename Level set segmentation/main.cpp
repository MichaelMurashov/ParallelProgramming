#include <iostream>

#include "mouse.h"

using namespace std;

int main(int argc, char** argv) {
    Mat src = imread("img.jpg");
    const string windowName = "Source image";

    Rect roi = Mouse::drawRect(src, windowName);


    


    waitKey(0);
    return  0;
}