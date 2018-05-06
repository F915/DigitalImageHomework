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
    waitKey(1);
    return answer_computer;
}

int main()
{
    int i;
    while (1)
    {
        scanf("%d", &i);
        showResult(i);
    }
    waitKey(0);
    return 0;
}