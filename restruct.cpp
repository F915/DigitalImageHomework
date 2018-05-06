//******************************************************//
//                Rock, Paper, Scissors                 //
//                                                      //
//                Digital Images Process                //
//                                                      //
//                                                      //
//                       F915                           //
//          https://github.com/F915/FingersGame         //
//                     2018.05.05                       //
//******************************************************//

//Update Logs
//2018.05.05    Create project and test algorithms.
//2018.05.06    Restructuring codes.

#include <iostream>
#include <cstdio>
#include <opencv/cv.hpp>

#define IMAGE_FILE 0   
#define VIDEO_FILE 1   
#define VIDEO_STREAM 2 
bool video_flag = 0;

using namespace std;
using namespace cv;

int initProgram(char *input_name, int *input_type)
{
    printf("//******************************************************//\n");
    printf("//                Rock, Paper, Scissors                 //\n");
    printf("//                                                      //\n");
    printf("//                Digital Images Process                //\n");
    printf("//                                                      //\n");
    printf("//                                                      //\n");
    printf("//                       F915                           //\n");
    printf("//          https://github.com/F915/FingersGame         //\n");
    printf("//                     2018.05.05                       //\n");
    printf("//******************************************************//\n");

    int return_state = 0;

    printf("\nPlease input the number of input file type.\n0 Image File\n1 Video File\n2 Video Stream\n");
    scanf("%d", input_type);
    if (*input_type > 2 || *input_type < 0)
    {
        printf("\nERROR 0x00000002:Error input file's type.\n");
        return_state = -1;
    }

    printf("\nPlease input the name of input file.\n");
    scanf("%s", input_name);
    if (input_name == NULL)
    {
        printf("\nERROR 0x00000001:Error input file's name.\n");
        return_state = -2;
    }

    if (*input_type == 2 && (strlen(input_name) > 1 || *input_name < '0' || *input_name > '9'))
    {
        printf("\nError 0x00000003:Unmatched input file's name with type.\n");
        return_state = -3;
    }
    printf("\ninitialized program.\n");
    return return_state;
}

Mat getFlame(char *input_name, int input_type)
{
    Mat return_Mat;
    static int flame_number = 0;
    VideoCapture video;

    if (video_flag == 0)
    {
        //Definitions are in #define.
        switch (input_type)
        {
        case IMAGE_FILE:
            return_Mat = imread(input_name);
            if (return_Mat.empty())
            {
                printf("\nError 0x10000001:Cannot open image: %s.\n", input_name);
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
        video.read(return_Mat);
        if (return_Mat.empty())
        {
            printf("\nError 0x10000002:Cannot open video: %s.\n", input_name);
        }
    }
    printf("\nGot %d flame.\n", flame_number);
    flame_number++;
    return return_Mat;
}

Mat processFlame(Mat origin_image)
{
    ;
}

int *processContour(Mat binary_contour_image)
{
    ;
}

int main(int argc, char *argv[])
{
    Mat origin_image;
    Mat binary_contour_image;
    char input_name[100];
    int input_type = -1;
    int init_state = -1;
    int exit_flag = 0;
    int contour_result[3][2];

    init_state = initProgram(input_name, &input_type);
    if (init_state != 0)
    {
        printf("\nERROR 0x00000000:Error initialize program.\n");
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
            printf("\nError 0x10000000:Cannot open input file or port: %s.\n", input_name);
            exit_flag = 1;
            break;
        }
        else
        {
            //Image or video processing.
            binary_contour_image = processFlame(origin_image);
            contour_result = processContour(binary_contour_image);
        }

    } while (video_flag == 1 && exit_flag == 0);

    return 0;
}