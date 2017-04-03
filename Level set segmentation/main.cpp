#include <iostream>

#include "src/level.cpp"

using std::cout;
using std::endl;

int main(int argc, char** argv) {
    Mat img = imread("img.jpg");

    imshow("circle", img);

    waitKey(0);
    return  0;
}