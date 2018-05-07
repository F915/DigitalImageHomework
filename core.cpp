#include <opencv/cv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
    // Mat input = imread("sample.jpg");
    //imshow("Origin",input);
    VideoCapture capture(0);
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
        waitKey(20);
        resize(input, output, Size(1024, 1024));
        Mat HSV_image;
        cvtColor(output, HSV_image, COLOR_BGR2HSV);
        Mat H, S, V;
        Mat mapH, mapS, mapV;
        vector<Mat> channels;
        split(HSV_image, channels);
        H = channels.at(0);
        S = channels.at(1);
        V = channels.at(2);
        applyColorMap(H, mapH, COLORMAP_HSV);
        applyColorMap(S, mapS, COLORMAP_JET);
        applyColorMap(V, mapV, COLORMAP_JET);
        imshow("S", mapS);
        imshow("V", mapV);
        imshow("H", mapH);
        GaussianBlur(H, H, Size(9, 9), 0);
        GaussianBlur(S, S, Size(9, 9), 0);
        GaussianBlur(V, V, Size(9, 9), 0);
        //threshold(g_grayImage,g_dstImage,g_nThresholdValue,255,g_nThresholdType);

        //imshow("S",S);
        //imshow("V",V);
        //cout << H << " " << endl << endl;
        // cout << S << " " << endl << endl;
        Mat op_H, op_S;
        Mat element_S = getStructuringElement(MORPH_RECT, Size(16, 16));
        Mat element_H = getStructuringElement(MORPH_RECT, Size(32, 32));
        //imshow("V",op_V);
        erode(H, op_H, element_H);
        dilate(op_H, op_H, element_H);
        //  erode(S, op_S, element_S);
        //  dilate(op_S, op_S, element_S);

        //threshold(op_H,op_H,100,255,THRESH_BINARY);
        //threshold(op_S,op_S,50,255,THRESH_BINARY);
        //dilate(op_H,op_H,element_H);
        //erode(op_S,op_S,element_S);
        //dilate(op_S,op_S,element_S);
        //erode(op_H,op_H,element_H);

        Mat HS = H;
        GaussianBlur(HS, HS, Size(31, 31), 0);
        threshold(HS, HS, 128, 255, THRESH_BINARY);
        //threshold(HS, HS, 128, 255, THRESH_BINARY);
        imshow("HS", HS);
        Mat element_HS = getStructuringElement(MORPH_RECT, Size(8, 8));
        dilate(HS, HS, element_HS);
        erode(HS, HS, element_HS);
        vector<vector<Point> > contours;
        findContours(HS, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);
        drawContours(output, contours, -1, Scalar(255, 0, 0), 1);
        //imshow("H",op_H);
        //imshow("S",op_S);
        //imshow("S+H",HS);
        vector<vector<Point> > hull(contours.size());
        for (int i = 0; i < contours.size(); i++)
        {
            convexHull(Mat(contours[i]), hull[i], false);
        }
        drawContours(output, hull, -1, Scalar(0, 255, 0), 1);

        RotatedRect rectPoint;
        for (int i = 0; i < contours.size(); i++)
        {
            rectPoint = minAreaRect(contours[i]);
            Point2f fourPoint2f[4];
            rectPoint.points(fourPoint2f);
            for (int j = 0; j < 4; j++)
            {
                line(output, fourPoint2f[j], fourPoint2f[(j + 1) % 4], Scalar(0, 0, 255), 1);
            }
        } //定义一个存储以上四个点的坐标的变量
        //Point2f fourPoint2f[4];
        //将rectPoint变量中存储的坐标值放到 fourPoint的数组中
        //for (int i = 0; i < contours.size();i++)
        //{
        //rectPoint[i].points(fourPoint2f);
        //  for (int j = 0; j < 4; j++)
        //  {
        //    //  line(output, fourPoint2f[j], fourPoint2f[(j + 1) % 4], Scalar(0, 0, 255), 3);
        //  }
        //} //rectPoint.points(fourPoint2f);
        ////for (int i = 0; i < 4; i++)
        ////{
        ////    line(output, fourPoint2f[i], fourPoint2f[(i + 1)%4], Scalar(0,0,255), 3);
        //}

        imshow("Res", output);
    }
    return 0;
}