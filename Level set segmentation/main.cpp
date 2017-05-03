#include <iostream>

#include <opencv2/highgui.hpp>

using namespace cv;
using namespace std;

uint getIntensity(const Mat& img, int x, int y) {
    Vec3b color = img.at<Vec3b>(x, y);
    return (uint)(0.11 * color.val[0] + 0.53 * color.val[1] + 0.36 * color.val[2]);
}

int main(int argc, char** argv) {
    Mat src = imread("../images/5.jpg");
    Mat result = src.clone();
    imshow("src", src);

    uint threshold = getIntensity(src, 0, 0);
    uint range = 50;

    const clock_t start = clock();

    for (int i = 0; i < result.rows; i++)
        for (int j = 0; j < result.cols; j++) {
            uint intensity = getIntensity(result, i, j);

            //// allocate object
            if ((threshold - range) > intensity || intensity > (threshold + range))
                result.at<Vec3b>(i, j) = Vec3b(0, 0, 255);
        }

    imshow("result", result);

    const double stop = static_cast<double>(clock() - start) / CLOCKS_PER_SEC;
    cout << "time - " << stop;

    waitKey(0);
    return 0;
}
