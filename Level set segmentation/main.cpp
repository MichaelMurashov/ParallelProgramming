#include <iostream>

#include "mouse.h"

using namespace std;

uint getIntensity(const Mat& img, int x, int y) {
    Vec3b color = img.at<Vec3b>(x, y);
    return (uint)(0.11 * color.val[0] + 0.53 * color.val[1] + 0.36 * color.val[2]);
}

int main(int argc, char** argv) {
    Mat src = imread("../images/square.jpg");
    Rect roi = Mouse::drawRect(src, "Select rectangle");

    Mat result = src.clone();
    uint threshold = getIntensity(src, roi.tl().x, roi.tl().y);
    uint range = 10;

    int startX = roi.tl().x;
    int startY = roi.tl().y;
    int endX = roi.br().x;
    int endY = roi.br().y;

    const clock_t start = clock();

    for (int i = startX; i < endX; i++)
        for (int j = startY; j < endY; j++) {
            uint intensity = getIntensity(result, j, i);

            //// allocate background
//            if ((threshold - range) < intensity || intensity < (threshold + range))
//                result.at<Vec3b>(j, i) = Vec3b(0, 255, 0);

            //// allocate object
            if ((threshold - range) > intensity || intensity > (threshold + range))
                result.at<Vec3b>(j, i) = Vec3b(0, 0, 255);

            //// allocate borders (ALMOST WORKS)
//            if ((threshold - range) > intensity || intensity > (threshold + range)) {
//                threshold = getIntensity(result, j, i);
//                result.at<Vec3b>(j, i) = Vec3b(0, 0, 0);
//            }
        }

    const double stop = static_cast<double>(clock() - start) / CLOCKS_PER_SEC;
    cout << "time - " << stop;

    imshow("Result", result);

    waitKey(0);
    return 0;
}
