//******************************************************//
//                Rock, Paper, Scissors                 //
//                                                      //
//                Digital Images Process                //
//                                                      //
//                                                      //
//                       F915                           //
//     https://github.com/F915/Rock-Paper-Scissors      //
//                     2018.05.05                       //
//******************************************************//

//Update Logs
//2018.05.05    Create project and test algorithms.
//2018.05.06    Restructuring codes.
//2018.05.07    Finish processFlame function.
//2018.05.08    Finish all functions.
//2018.05.16    Update deciding algorithms.

#include <iostream>
#include <cstdio>
#include <opencv/cv.hpp>
#include <math.h>

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

int initProgram(char *input_name, int *input_type);
Mat getFlame(char *input_name, int input_type);
Mat processFlame(Mat origin_image);
Mat processChannal(Mat single_channal, int channal_type);
int processContour(Mat binary_contour_image, double contours_square_lengtn[3][2]);
int decideReport(double contours_square_lengtn[3][2]);
int showResult(int report_result);

int main(int argc, char *argv[])
{
    Mat origin_image;
    Mat binary_contour_image;
    char input_name[100];
    int input_type = -1;
    int init_state = -1;
    int exit_flag = 0;
    int contour_result[3][2];
    int report_result;
    int contour_number;
    static double contours_square_lengtn[3][2] = {0};

    init_state = initProgram(input_name, &input_type);
    if (init_state != 0)
    {
        printf("\nERROR 0x00000000:Error initialize program.");
        exit_flag = 1;
    }
    do
    {
        if (exit_flag == 1)
        {
            break;
        }
        //Image or video reading.
        origin_image = getFlame(input_name, input_type);
        if (origin_image.empty())
        {
            printf("\nError 0x10000000:Cannot open input file or port: %s.", input_name);
            exit_flag = 1;
            break;
        }
        else
        {
            resize(origin_image, origin_image, Size(512, 512));
            imshow("Origin_image", origin_image);
            waitKey(1);
            //Image or video processing.
            binary_contour_image = processFlame(origin_image);
            memset(contours_square_lengtn, 0, sizeof(contours_square_lengtn));
            contour_number = processContour(binary_contour_image, contours_square_lengtn);
            if (contour_number != 0)
            {
                report_result = decideReport(contours_square_lengtn);
                showResult(report_result);
            }
            else
            {
                showResult(NO_ANSWER);
            }
        }

    } while (exit_flag == 0);
    waitKey(0);
    destroyAllWindows();
    return 0;
}

int initProgram(char *input_name, int *input_type)
{
    printf("//******************************************************//\n");
    printf("//                Rock, Paper, Scissors                 //\n");
    printf("//                                                      //\n");
    printf("//                Digital Images Process                //\n");
    printf("//                                                      //\n");
    printf("//                                                      //\n");
    printf("//                       F915                           //\n");
    printf("//     https://github.com/F915/Rock-Paper-Scissors      //\n");
    printf("//                     2018.05.05                       //\n");
    printf("//******************************************************//\n");

    int return_state = 0;
    char channal_name_origin[4][30] = {"H_channal", "S_channal", "V_channal", "Error"};
    char channal_name_binary[4][30] = {"H_channal_binary", "S_channal_binary", "V_channal_binary", "Error"};

    printf("\nPlease input the number of input file type.\n0 Image File\n1 Video File\n2 Video Stream\n");
    scanf("%d", input_type);
    if (*input_type > 2 || *input_type < 0)
    {
        printf("\nERROR 0x00000002:Error input file's type.");
        return_state = -1;
    }

    printf("\nPlease input the name of input file.\n");
    scanf("%s", input_name);
    if (input_name == NULL)
    {
        printf("\nERROR 0x00000001:Error input file's name.");
        return_state = -2;
    }

    if (*input_type == 2 && (strlen(input_name) > 1 || *input_name < '0' || *input_name > '9'))
    {
        printf("\nError 0x00000003:Unmatched input file's name with type.");
        return_state = -3;
    }

    namedWindow("Origin_image");
    namedWindow("Answer");
    namedWindow("Mix_contours");
    for (int i = 0; i < 2; i++)
    {
        namedWindow(channal_name_origin[i]);
        namedWindow(channal_name_binary[i]);
        createTrackbar("gauss_kernal_size", channal_name_origin[i], nullptr, 127);
        createTrackbar("erode_element_size", channal_name_origin[i], nullptr, 255);
        createTrackbar("dilate_element_size", channal_name_origin[i], nullptr, 255);
        createTrackbar("threshold_value_high", channal_name_origin[i], nullptr, 255);
        createTrackbar("threshold_value_low", channal_name_origin[i], nullptr, 255);
    }

    printf("\nInitialized program.");
    return return_state;
}

Mat getFlame(char *input_name, int input_type)
{
    static Mat return_mat;
    static int flame_number = 0;
    static VideoCapture video;

    if (video_flag == 0)
    {
        //Definitions are in #define.
        switch (input_type)
        {
        case IMAGE_FILE:
            return_mat = imread(input_name);
            if (return_mat.empty())
            {
                printf("\nError 0x10000001:Cannot open image: %s.", input_name);
            }
            break;
        case VIDEO_FILE:
            video_flag = 1;
            video.open(input_name);
            break;
        case VIDEO_STREAM:
            video_flag = 1;
            video.open(*input_name - '0');
            break;
        }
    }

    if (video_flag == 1)
    {
        video.read(return_mat);
        if (return_mat.empty())
        {
            printf("\nError 0x10000002:Cannot open video: %s.", input_name);
        }
    }
    printf("\nGot %d flame.", flame_number);
    flame_number++;
    //waitKey(1);
    return return_mat;
}

Mat processFlame(Mat origin_image)
{
    static Mat hsv_image;
    cvtColor(origin_image, hsv_image, COLOR_BGR2HSV);
    static Mat h_channal, s_channal, v_channal;
    vector<Mat> color_channals;
    split(hsv_image, color_channals);
    h_channal = color_channals.at(0);
    s_channal = color_channals.at(1);
    v_channal = color_channals.at(2);
    printf("\nSplit 3 channals OK");

    /*static Mat map_h_channal, map_s_channal, map_v_channal;
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
    //printf("\nProcess H channal OK\n");
    binary_s_channal = processChannal(s_channal, S_CHANNAL);
    //printf("\nProcess S channal OK\n");
    printf("\nMix 2 binary channals OK");
    return binary_h_channal & binary_s_channal;
    //If Kinect used.
    //Mat processDChannal(Mat origin_image);
}

Mat processChannal(Mat single_channal, int channal_type)
{
    int gauss_kernal_size = 0;
    int erode_element_size = 0;
    int dilate_element_size = 0;
    int threshold_value_high = 0;
    int threshold_value_low = 0;
    static Mat return_mat;
    static char channal_name_origin[4][30] = {"H_channal", "S_channal", "V_channal", "Error"};
    static char channal_name_binary[4][30] = {"H_channal_binary", "S_channal_binary", "V_channal_binary", "Error"};

    char *channal_name, *binary_channal_name;

    Mat map_channal;
    switch (channal_type)
    {
    case H_CHANNAL:
        channal_name = channal_name_origin[H_CHANNAL];
        binary_channal_name = channal_name_binary[H_CHANNAL];
        applyColorMap(single_channal, map_channal, COLORMAP_HSV);
        printf("\nRemap %s OK", channal_name);
        break;
    case S_CHANNAL:
        binary_channal_name = channal_name_binary[S_CHANNAL];
        channal_name = channal_name_origin[S_CHANNAL];
        applyColorMap(single_channal, map_channal, COLORMAP_JET);
        printf("\nRemap %s OK", channal_name);
        break;
    case V_CHANNAL:
        binary_channal_name = channal_name_binary[V_CHANNAL];
        channal_name = channal_name_origin[V_CHANNAL];
        applyColorMap(single_channal, map_channal, COLORMAP_JET);
        printf("\nRemap %s OK", channal_name);
        break;
    default:
        printf("\nError 0x21000001:Error channal type.");
        return_mat = 0;
        break;
    }

    if (!map_channal.empty())
    {
        imshow(channal_name, map_channal);

        gauss_kernal_size = getTrackbarPos("gauss_kernal_size", channal_name);
        erode_element_size = getTrackbarPos("erode_element_size", channal_name);
        dilate_element_size = getTrackbarPos("dilate_element_size", channal_name);
        threshold_value_high = getTrackbarPos("threshold_value_high", channal_name);
        threshold_value_low = getTrackbarPos("threshold_value_low", channal_name);

        /*
        setTrackbarPos("gauss_kernal_size", channal_name, gauss_kernal_size);
        setTrackbarPos("erode_element_size", channal_name, erode_element_size);
        setTrackbarPos("dilate_element_size", channal_name, dilate_element_size);
        setTrackbarPos("threshold_value_high", channal_name, threshold_value_high);
        setTrackbarPos("threshold_value_low", channal_name, threshold_value_low);
        */

        // gauss_kernal_size = 2 * gauss_kernal_size + 1;
        if (erode_element_size == 0)
        {
            erode_element_size = 1;
            setTrackbarPos("erode_element_size", channal_name, erode_element_size);
        }
        if (dilate_element_size == 0)
        {
            dilate_element_size = 1;
            setTrackbarPos("dilate_element_size", channal_name, dilate_element_size);
        }
        if (threshold_value_high == 0)
        {
            threshold_value_high = 255;
            setTrackbarPos("threshold_value_high", channal_name, threshold_value_high);
        }
        if (threshold_value_high < threshold_value_low)
        {
            threshold_value_high = threshold_value_low;
            setTrackbarPos("threshold_value_high", channal_name, threshold_value_high);
        }

        static Mat erode_element = getStructuringElement(MORPH_RECT, Size(erode_element_size, erode_element_size));
        static Mat dilate_element = getStructuringElement(MORPH_RECT, Size(dilate_element_size, dilate_element_size));

        GaussianBlur(single_channal, single_channal, Size(gauss_kernal_size * 2 + 1, gauss_kernal_size * 2 + 1), 0);
        threshold(single_channal, single_channal, threshold_value_high, 255, THRESH_TOZERO_INV);
        threshold(single_channal, single_channal, threshold_value_low, 255, THRESH_TOZERO);
        erode(single_channal, single_channal, erode_element);
        dilate(single_channal, single_channal, dilate_element);
        threshold(single_channal, single_channal, 0, 255, THRESH_BINARY_INV);

        return_mat = single_channal;
        if (!return_mat.empty())
        {
            imshow(binary_channal_name, return_mat);
        }
    }
    printf("\nProcess %s OK", channal_name);
    return return_mat;
}

int processContour(Mat binary_contour_image, double contours_square_lengtn[3][2])
{
    static Mat color_binary_contour_image;
    applyColorMap(binary_contour_image, color_binary_contour_image, COLORMAP_BONE);

    vector<vector<Point>> nearest_contours;
    findContours(binary_contour_image, nearest_contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);
    printf("\nFound %d contours", nearest_contours.size());

    vector<vector<Point>> contours_hull(nearest_contours.size());
    for (int i = 0; i < nearest_contours.size(); i++)
    {
        convexHull(Mat(nearest_contours[i]), contours_hull[i], true);
    }

    RotatedRect min_rectangle_points;
    Point2f single_rectangle_point[4];
    drawContours(color_binary_contour_image, nearest_contours, -1, Scalar(255, 0, 0), 3);
    drawContours(color_binary_contour_image, contours_hull, -1, Scalar(0, 255, 0), 3);
    static double current_contour_square, current_contour_length;

    for (int i = 0; i < nearest_contours.size(); i++)
    {
        min_rectangle_points = minAreaRect(nearest_contours[i]);
        min_rectangle_points.points(single_rectangle_point);
        current_contour_square = min_rectangle_points.size.area();
        current_contour_length = min_rectangle_points.size.height;
        current_contour_length += min_rectangle_points.size.width;
        current_contour_length *= 2;
        if (contours_square_lengtn[2][0] <= current_contour_square)
        {
            contours_square_lengtn[2][0] = current_contour_square;
        }
        if (contours_square_lengtn[2][1] <= current_contour_length)
        {
            contours_square_lengtn[2][1] = current_contour_length;
        }
        for (int j = 0; j < 4; j++)
        {
            line(color_binary_contour_image, single_rectangle_point[j], single_rectangle_point[(j + 1) % 4], Scalar(0, 0, 255), 3);
        }
    }
    printf("\nFound other contours OK");

    //Computing square and length

    for (int i = 0; i < nearest_contours.size(); i++)
    {
        current_contour_length = arcLength(nearest_contours[i], true);
        current_contour_square = contourArea(nearest_contours[i]);
        if (contours_square_lengtn[0][0] <= current_contour_square)
        {
            contours_square_lengtn[0][0] = current_contour_square;
        }
        if (contours_square_lengtn[0][1] <= current_contour_length)
        {
            contours_square_lengtn[0][1] = current_contour_length;
        }
    }
    for (int i = 0; i < contours_hull.size(); i++)
    {
        current_contour_length = arcLength(contours_hull[i], true);
        current_contour_square = contourArea(contours_hull[i]);
        if (contours_square_lengtn[1][0] <= current_contour_square)
        {
            contours_square_lengtn[1][0] = current_contour_square;
        }
        if (contours_square_lengtn[1][1] <= current_contour_length)
        {
            contours_square_lengtn[1][1] = current_contour_length;
        }
    }

    printf("\nNearest contour Square:%lf, Length:%lf", contours_square_lengtn[0][0], contours_square_lengtn[0][1]);
    printf("\nHull contour Square:%lf, Length:%lf", contours_square_lengtn[1][0], contours_square_lengtn[1][1]);
    printf("\nRectangle contour Square:%lf, Length:%lf", contours_square_lengtn[2][0], contours_square_lengtn[2][1]);
    imshow("Mix_contours", color_binary_contour_image);
    return nearest_contours.size();
}

int decideReport(double contours_square_lengtn[3][2])
{

    static char answer_name[4][10] = {"No Answer", "Rock", "Paper", "Scissors"};
    static int return_result;

    static double nearest_contour_lengtn, hull_contour_length;
    nearest_contour_lengtn = contours_square_lengtn[0][1];
    hull_contour_length = contours_square_lengtn[1][1];

    static double nearest_contour_square, hull_contour_square;
    nearest_contour_square = contours_square_lengtn[0][0];
    hull_contour_square = contours_square_lengtn[1][0];

    static double sub_hull_nearest_length, sub_hull_nearest_square;
    sub_hull_nearest_length = nearest_contour_lengtn - hull_contour_length;
    sub_hull_nearest_square = nearest_contour_square - hull_contour_square;

    printf("\nSubtract of length between hull and nearest in decideReport is %lf", sub_hull_nearest_length);
    printf("\nSubtract of square between hull and nearest in decideReport is %lf", sub_hull_nearest_square);

    static double decide_index;
    decide_index = (pow(sub_hull_nearest_length, 2) / sub_hull_nearest_square);

    if (decide_index > 50 && decide_index < 120)
    {
        return_result = ROCK;
        printf("\nHuman gives the answer %s", answer_name[ROCK]);
    }
    else if (decide_index >= 120 && decide_index < 600)
    {
        return_result = SCISSORS;
        printf("\nHuman gives the answer %s", answer_name[SCISSORS]);
    }
    else if (decide_index >= 600 && decide_index < 1500)
    {
        return_result = PAPER;
        printf("\nHuman gives the answer %s", answer_name[PAPER]);
    }
    else
    {
        return_result = NO_ANSWER;
        printf("\nHuman gives the answer %s", answer_name[NO_ANSWER]);
    }

    /*
    printf("\nNearest contour Square in decideReport:%lf, Length:%lf\n", contours_square_lengtn[0][0], contours_square_lengtn[0][1]);
    printf("\nHull contour Square in decideReport:%lf, Length:%lf\n", contours_square_lengtn[1][0], contours_square_lengtn[1][1]);
    printf("\nRectangle contour Square in decideReport:%lf, Length:%lf\n", contours_square_lengtn[2][0], contours_square_lengtn[2][1]);
    */

    //printf("\nHull contour square in decideReport is %lf\n", hull_contour_length);
    //printf("\nNearest contour square in decideReport is %lf\n", nearest_contour_lengtn);

    /*
    printf("\nSubtract of length between hull and nearest in decideReport is %lf", sub_hull_nearest_length);
    if (sub_hull_nearest_length > 50 && sub_hull_nearest_length < 120)
    {
        return_result = ROCK;
        printf("\nHuman gives the answer %s", answer_name[ROCK]);
    }
    else if (sub_hull_nearest_length >= 120 && sub_hull_nearest_length < 600)
    {
        return_result = SCISSORS;
        printf("\nHuman gives the answer %s", answer_name[SCISSORS]);
    }
    else if (sub_hull_nearest_length >= 600 && sub_hull_nearest_length < 1500)
    {
        return_result = PAPER;
        printf("\nHuman gives the answer %s", answer_name[PAPER]);
    }
    else
    {
        return_result = NO_ANSWER;
        printf("\nHuman gives the answer %s", answer_name[NO_ANSWER]);
    }
    */
    return return_result;
}

int showResult(int report_result)
{
    static char answer_name[4][10] = {"No Answer", "Rock", "Paper", "Scissors"};
    static Mat answer_image;
    static int answer_computer;

    //Definitions are in #define.
    switch (report_result)
    {
    case NO_ANSWER:
        answer_computer = NO_ANSWER;
        answer_image = imread("0.jpg");
        if (answer_image.empty())
        {
            printf("\nError 0x50000001:Cannot open answer image %s.", answer_name[answer_computer]);
        }
        break;
    case ROCK:
        answer_computer = PAPER;
        answer_image = imread("2.jpg");
        if (answer_image.empty())
        {
            printf("\nError 0x50000002:Cannot open answer image %s.", answer_name[answer_computer]);
        }
        break;
    case PAPER:
        answer_computer = SCISSORS;
        answer_image = imread("3.jpg");
        if (answer_image.empty())
        {
            printf("\nError 0x50000003:Cannot open answer image %s.", answer_name[answer_computer]);
        }
        break;
    case SCISSORS:
        answer_computer = ROCK;
        answer_image = imread("1.jpg");
        if (answer_image.empty())
        {
            printf("\nError 0x50000004:Cannot open answer image %s.", answer_name[answer_computer]);
        }
        break;
    default:
        answer_computer = NO_ANSWER;
        answer_image = imread("0.jpg");
        if (answer_image.empty())
        {
            printf("\nError 0x50000001:Cannot open answer image %s.", answer_name[answer_computer]);
        }
        printf("\nError 0x50000001:Cannot find suitable answer image.");
        break;
    }

    printf("\nComputer gives answer %s.", answer_name[answer_computer]);
    if (!answer_image.empty())
    {
        resize(answer_image, answer_image, Size(512, 512));
        imshow("Answer", answer_image);
    }
    //waitKey(1);
    return answer_computer;
}