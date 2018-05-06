#include <iostream>
#include <opencv/cv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
    Mat flame;
    int i = 0;
    Mat H, S, V;
    vector<Mat> channels;
    Mat mapH, mapS, mapV;
    Mat HSV_image;
    VideoCapture capture(0);
    while (1)
    {
        capture >> flame;
        if (!flame.empty())
        {
            resize(flame, flame, Size(512, 512));
            imshow("origin", flame);
            cvtColor(flame, HSV_image, COLOR_BGR2HSV);
            split(HSV_image, channels);
            H = channels.at(0);
            S = channels.at(1);
            V = channels.at(2);
            applyColorMap(H, mapH, COLORMAP_HSV);
            applyColorMap(S, mapS, COLORMAP_JET);
            applyColorMap(V, mapV, COLORMAP_JET);
            imshow("H", mapH);
            imshow("S", mapS);
            imshow("V", mapV);
            waitKey(1);
        }
    }
    return 0;
}