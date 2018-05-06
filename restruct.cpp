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

#define NO_ANSWER 0
#define ROCK 1
#define PAPER 2
#define SCISSORS 3

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
    static Mat return_Mat;
    static int flame_number = 0;
    static VideoCapture video;

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
    //waitKey(1);
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

int decideReport(int *contour_result)
{
    ;
}

int showResult(int report_result)
{
    static const char answer_name[4][10] = {"No Answer", "Rock", "Paper", "Scissors"};
    static Mat answer_image;
    static int answer_computer;
    switch (report_result)
    {
    case NO_ANSWER:
        answer_computer = NO_ANSWER;
        answer_image = imread("0.jpg");
        if (answer_image.empty())
        {
            printf("\nError 0x50000001:Cannot open answer image %s.\n", answer_name[answer_computer]);
        }
        break;
    case ROCK:
        answer_computer = PAPER;
        answer_image = imread("1.jpg");
        if (answer_image.empty())
        {
            printf("\nError 0x50000002:Cannot open answer image %s.\n", answer_name[answer_computer]);
        }
        break;
    case PAPER:
        answer_computer = SCISSORS;
        answer_image = imread("2.jpg");
        if (answer_image.empty())
        {
            printf("\nError 0x50000003:Cannot open answer image %s.\n", answer_name[answer_computer]);
        }
        break;
    case SCISSORS:
        answer_computer = ROCK;
        answer_image = imread("3.jpg");
        if (answer_image.empty())
        {
            printf("\nError 0x50000004:Cannot open answer image %s.\n", answer_name[answer_computer]);
        }
        break;
    default:
        answer_computer = NO_ANSWER;
        answer_image = imread("0.jpg");
        if (answer_image.empty())
        {
            printf("\nError 0x50000001:Cannot open answer image %s.\n", answer_name[answer_computer]);
        }
        printf("\nError 0x50000001:Cannot find suitable answer image.\n");
        break;
    }
    printf("\nComputer gives answer %s.\n", answer_name[answer_computer]);
    if (!answer_image.empty())
    {
        resize(answer_image, answer_image, Size(512, 512));
        imshow("Answer", answer_image);
    }
    //waitKey(1);
    return answer_computer;
}

//******************************************************//
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
            imshow("origin_image", origin_image);
            waitKey(1);
            //Image or video processing.
            /* 
            binary_contour_image = processFlame(origin_image);
            contour_result = processContour(binary_contour_image);

            report_result = decideReport(contour_result);
            showResult(report_result); //bingo
            */
        }

    } while (video_flag == 1 && exit_flag == 0);
    waitKey(0);
    return 0;
}
