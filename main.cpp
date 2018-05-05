#include <opencv/cv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
    // Mat input = imread("sample.jpg");
    //imshow("Origin",input);
    VideoCapture capture("1.mp4");
    Mat input, output;
    capture >> input;
    while (1)
    {
        capture >> input;
        if (input.empty())
        {
            break;
        }
        // imshow("input", input);
        waitKey(1);
        resize(input, output, Size(511, 511));
        Mat HSV_image;
        cvtColor(output, HSV_image, COLOR_BGR2HSV);
        Mat H, S, V;
        vector<Mat> channels;
        split(HSV_image, channels);
        H = channels.at(0);
        S = channels.at(1);
        V = channels.at(2);
        GaussianBlur(H, H, Size(63, 63), 0);
        GaussianBlur(S, S, Size(31, 31), 0);
        //threshold(g_grayImage,g_dstImage,g_nThresholdValue,255,g_nThresholdType);
        //imshow("H",H);
        //imshow("S",S);
        //imshow("V",V);
        //cout << H << " " << endl << endl;
        Mat op_H, op_S;
        Mat element_S = getStructuringElement(MORPH_RECT, Size(16, 16));
        Mat element_H = getStructuringElement(MORPH_RECT, Size(32, 32));
        //imshow("V",op_V);

        //  dilate(H, op_H, element_H);
        //  erode(S, op_S, element_S);
        //  dilate(op_S, op_S, element_S);
        //  erode(op_H, op_H, element_H);
        //threshold(op_H,op_H,100,255,THRESH_BINARY);
        //threshold(op_S,op_S,50,255,THRESH_BINARY);
        //dilate(op_H,op_H,element_H);
        //erode(op_S,op_S,element_S);
        //dilate(op_S,op_S,element_S);
        //erode(op_H,op_H,element_H);
        Mat HS = S + (255 - H) / 2;
        GaussianBlur(HS, HS, Size(31, 31), 0);
        threshold(HS, HS, 128, 255, THRESH_BINARY);
        Mat element_HS = getStructuringElement(MORPH_RECT, Size(8, 8));
        dilate(HS, HS, element_HS);
        erode(HS, HS, element_HS);
        vector<vector<Point> > contours;
        findContours(HS, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);
        drawContours(output, contours, -1, Scalar(255, 0, 0), 3);
        //imshow("H",op_H);
        //imshow("S",op_S);
        //imshow("S+H",HS);

        imshow("Res", output);
    }
    return 0;
}