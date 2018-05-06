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
//2018.05.05    Create  Project
//2018.05.06    Restructuring codes





#include <iostream>
#include <cstdio>
#include <opencv/cv.hpp>

//输入流类型
#define IMAGE_FILE 0   //图片文件
#define VIDEO_FILE 1   //视频文件
#define VIDEO_STREAM 2 //视频流(摄像头)
bool video_flag = 0;

using namespace std;
using namespace cv;

void init_program(input_name, &input_type)
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
}

Mat getFlame(char *input_name, int input_type)
{
    Mat return_Mat;

    //输入流无法正常读入
    if (input_name == NULL)
    {
        printf("ERROR 0x10000001:Error input file's name.");
        return_Mat = 0;
    }
    if (input_type > 2 || input_type < 0)
    {
        printf("ERROR 0x10000002:Error input file's type.");
        return_Mat = 0;
    }
    if (input_type == 2 && (strlen(input_name) > 1 || *input_name < '0' || *input_name > '9'))
    {
        printf("Error 0x10000003:Unmatched input file's name and type.");
        return_Mat = 0;
    }

    switch (input_type)
    {
    case IMAGE_FILE:
        return_Mat = imread(input_name);
        if (return_Mat.empty())
        {
            printf("Error 0x10000004:Cannot open %s", input_name);
        }
        break;
    case VIDEO_FILE:
        video_flag = 1;
        VideoCapture video(input_name);
    }
}

int main(int argc, char *argv[])
{
    Mat origin;
    char *input_name;
    int input_type;
    init_program(input_name, &input_type);
    do
    {
        origin = get_flame("", "");

    } while (video_flag == 0 || exit_flag == 1);

    return 0;
}