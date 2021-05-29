#include "SampleCommonHead.hpp"

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

void TestDisplayVideo(const char* file_path)
{
    cv::namedWindow("Example2", cv::WINDOW_AUTOSIZE);
    cv::VideoCapture cap;
    cap.open(file_path);

    cv::Mat frame;
    while (1)
    {
        cap >> frame;
        if (frame.empty())
            break;
        cv::imshow("Example2", frame);
        int ret = cv::waitKey(33);
        printf("%d\n", ret);
        if (ret >= 0)
            break;
    }
}

int g_slider_position = 0;
int g_run = 1, g_dontset = 0;   //start out in single step mode
cv::VideoCapture g_cap;
void onTrackbarSlide(int pos, void *)
{
    g_cap.set(cv::CAP_PROP_POS_FRAMES, pos);
    if (!g_dontset)
        g_run = 1;
    g_dontset = 0;
}
void TestDisplayVideoEx(const char* file_path)
{
    cv::namedWindow("Example2_4", cv::WINDOW_AUTOSIZE);
    g_cap.open(file_path);
    int frames = (int)g_cap.get(cv::CAP_PROP_FRAME_COUNT);
    int tmpw = (int)g_cap.get(cv::CAP_PROP_FRAME_WIDTH);
    int tmph = (int)g_cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    printf("Video has %d frames of dimensions(%d, %d).\n", frames, tmpw, tmph);

    cv::createTrackbar("Position", "Example2_4", &g_slider_position, frames, onTrackbarSlide);

    cv::Mat frame;
    while (1)
    {
        if (g_run != 0)
        {
            g_cap >> frame;
            if (frame.empty())
                break;
            int current_pos = (int)g_cap.get(cv::CAP_PROP_POS_FRAMES);
            g_dontset = 1;

            cv::setTrackbarPos("Position", "Example2_4", current_pos);
            cv::imshow("Example2_4", frame);

            g_run -= 1;
        }

        char c = (char)cv::waitKey(10);
        if (c == 's')
        {
            g_run = 1;
            printf("Single step, run = %d\n", g_run);
        }
        if (c == 'r')
        {
            g_run = -1;
            printf("Rum mode, run = %d\n", g_run);
        }
        if (c == 27)
            break;
    }
    cv::destroyAllWindows();
}

void TestDisplayCamera()
{
    cv::VideoCapture cap;
    cap.open(-1);
    if (!cap.isOpened())
    {
        printf("Error: Couldn't open capture.\n");
        return;
    }

    cv::Mat frame;
    while (1)
    {
        cap >> frame;
        if (frame.empty())
            break;

        cv::imshow("Camera", frame);

        if (-1 != cv::waitKey(33))
            break;
    }
}
