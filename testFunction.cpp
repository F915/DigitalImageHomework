#include <iostream>
#include <cstdio>
#include <opencv/cv.hpp>

#define IMAGE_FILE 0
#define VIDEO_FILE 1
#define VIDEO_STREAM 2
bool video_flag = 0;

#define NO_ANSWER 0
#define ROCK 1
#define PAPER 2
#define SCISSORS 3

#define H_CHANNAL 0
#define S_CHANNAL 1
#define V_CHANNAL 2

using namespace std;
using namespace cv;

Mat processChannal(Mat single_channal, int channal_type)
{
    int gauss_kernal_size = 0;
    int erode_element_size = 0;
    int dilate_element_size = 0;
    int threshold_value_high = 0;
    int threshold_value_low = 0;
    static Mat return_mat;
    char channal_name_origin[4][10] = {"H_channal", "S_channal", "V_channal", "Error"};
    char channal_name_binary[4][10] = {"H_channal_binary", "S_channal_binary", "V_channal_binary", "Error"};

    char *channal_name, *binary_channal_name;

    Mat map_channal;
    if (channal_type == H_CHANNAL)
    {
        channal_name = channal_name_origin[H_CHANNAL];
        binary_channal_name = channal_name_binary[H_CHANNAL];
        applyColorMap(single_channal, map_channal, COLORMAP_HSV);
    }
    else if (channal_type == S_CHANNAL)
    {
        binary_channal_name = channal_name_binary[S_CHANNAL];
        channal_name = channal_name_origin[S_CHANNAL];
        applyColorMap(single_channal, map_channal, COLORMAP_JET);
    }
    else if (channal_type == V_CHANNAL)
    {
        binary_channal_name = channal_name_binary[V_CHANNAL];
        channal_name = channal_name_origin[V_CHANNAL];
        applyColorMap(single_channal, map_channal, COLORMAP_JET);
    }
    else
    {
        printf("\nError 0x21000001:Error channal type.\n")
            return_mat = 0;
    }
    if (!map_channal.empty())
    {
        imshow(channal_name, map_channal);
        createTrackbar("gauss_kernal_size", channal_name, &gauss_kernal_size, 64);
        createTrackbar("erode_element_size", channal_name, &erode_element_size, 128);
        createTrackbar("dilate_element_size", channal_name, &dilate_element_size, 128);
        createTrackbar("threshold_value_high", channal_name, &threshold_value_high, 255);
        createTrackbar("threshold_value_low", channal_name, &threshold_value_low, 255);

        gauss_kernal_size = getTrackbarPos("gauss_kernal_size", channal_name);
        erode_element_size = getTrackbarPos("erode_element_size", channal_name);
        dilate_element_size = getTrackbarPos("dilate_element_size", channal_name);
        threshold_value_high = getTrackbarPos("threshold_value_high", channal_name);
        threshold_value_low = getTrackbarPos("threshold_value_low", channal_name);

        /*
        setTrackbar("gauss_kernal_size", channal_name, gauss_kernal_size);
        setTrackbar("erode_element_size", channal_name, erode_element_size);
        setTrackbar("dilate_element_size", channal_name, dilate_element_size);
        setTrackbar("threshold_value_high", channal_name, threshold_value_high);
        setTrackbar("threshold_value_low", channal_name, threshold_value_low);
        */

        gauss_kernal_size = 2 * gauss_kernal_size + 1;
        static Mat erode_element = getStructuringElement(MORPH_RECT, Size(erode_element_size, erode_element_size));
        static Mat dilate_element = getStructuringElement(MORPH_RECT, Size(dilate_element_size, dilate_element_size));

        GaussianBlur(single_channal, single_channal, Size(gauss_kernal_size, gauss_kernal_size), 0);
        erode(single_channal, single_channal, erode_element);
        dilate(single_channal, single_channal, dilate_element);
        threshold(single_channal, single_channal, threshold_value_high, 255, THRESH_TOZERO);
        threshold(single_channal, single_channal, threshold_value_low, 255, THRESH_TOZERO_INV);
        threshold(return_mat, single_channal, 0, 255, THRESH_BINARY);

        imshow(binary_channal_name, return_channal);
    }
    return return_mat;
}

Mat processFlame(Mat origin_image)
{
    static Mat hsv_image;
    cvtColor(origin_image, hsv_image, COLOR_BGR2HSV);
    static Mat h_channal, s_channal, v_channal;
    vector<Mat> color_channals;
    split(hsv_image, color_channals);
    h_channal = color_channels.at(0);
    s_channal = color_channels.at(1);
    v_channal = color_channels.at(2);
    printf("\nSplit 3 channals OK\n");

    /*
    static Mat map_h_channal, map_s_channal, map_v_channal;
    applyColorMap(h_channal, map_h_channal, COLORMAP_HSV);
    applyColorMap(s_channal, map_s_channal, COLORMAP_JET);
    applyColorMap(v_channal, map_v_channal, COLORMAP_JET);
    imshow("map_h_channal", map_h_channal);
    imshow("map_s_channal", map_s_channal);
    imshow("map_v_channal", map_v_channal);
    printf("\nRemap 3 channals OK\n");
    */

    static Mat binary_h_channal, binary_s_channal;
    binary_h_channal = processChannal(h_channal, H_CHANNAL);
    binary_s_channal = processChannal(s_channal, S_CHANNAL);
    //If Kinect used.
    //Mat processDChannal(Mat origin_image);
}

int main(int argc, char *argv[])
{
    Mat flame;
    int i = 0;
    Mat H, S, V;
    vector<Mat> channels;
    Mat mapH, mapS, mapV;
    Mat HSV_image;
    //VideoCapture capture(0);
    flame =
        while (1)
    {
        // capture >> flame;
        if (!flame.empty())
        {
            cvtColor(flame, HSV_image, COLOR_BGR2HSV);
            resize(HSV_image, HSV_image, Size(512, 512));
            split(HSV_image, channels);
            H = channels.at(0);
            S = channels.at(1);
            V = channels.at(2);
            processChannal(H, 0);
            //  processChannal(S, 1);
            // processChannal(V, 2);
            waitKey(1);
        }
    }
    return 0;
}