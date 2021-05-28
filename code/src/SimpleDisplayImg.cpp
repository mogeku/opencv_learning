#include "SampleCommonHead.hpp"

#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

void TestDisplayImg(const char* file_path)
{
    cv::Mat img = cv::imread(file_path, -1);
    if (img.empty())
        printf("imread failed\n");
    cv::namedWindow("Example1", cv::WINDOW_AUTOSIZE);
    cv::imshow("Example1", img);
    cv::waitKey(0);
    cv::destroyAllWindows();
}

void TestDisplayImgWithBlur(const char* file_path)
{

    cv::Mat img = cv::imread(file_path, cv::ImreadModes::IMREAD_UNCHANGED);
    if (img.empty())
        printf("imread failed\n");
    cv::namedWindow("Example2_5-in", cv::WINDOW_AUTOSIZE);
    cv::namedWindow("Example2_5-out", cv::WINDOW_AUTOSIZE);

    cv::imshow("Example2_5-in", img);

    cv::Mat out;
    cv::GaussianBlur(img, out, cv::Size(5, 5), 3, 3);
    cv::GaussianBlur(out, out, cv::Size(5, 5), 3, 3);

    cv::imshow("Example2_5-out", out);

    cv::waitKey(0);
    cv::destroyAllWindows();
}

void TestDisplayImgWithPyrDown(const char* file_path)
{
    cv::Mat img = cv::imread(file_path, -1);
    cv::Mat out;

    cv::pyrDown(img, out);

    cv::imshow("In", img);
    cv::imshow("Out", out);

    cv::waitKey(0);
    cv::destroyAllWindows();
}

void TestDisplayImgWithCanny(const char* file_path)
{
    cv::Mat img_rgb, img_gry, img_cny, img_pyr;

    img_rgb = cv::imread(file_path);

    cv::cvtColor(img_rgb, img_gry, cv::COLOR_RGB2GRAY);
    cv::imshow("Gray", img_gry);

    cv::pyrDown(img_gry, img_pyr);
    cv::pyrDown(img_pyr, img_pyr);
    cv::Canny(img_gry, img_cny, 10, 100, 3, true);
    cv::imshow("Canny", img_cny);

    cv::waitKey(0);
}

void TestChangePixels(const char* file_path)
{
    cv::Mat img_rgb, img_gry, img_cny, img_pyr;

    img_rgb = cv::imread(file_path);

    cv::cvtColor(img_rgb, img_gry, cv::COLOR_RGB2GRAY);

    cv::pyrDown(img_gry, img_pyr);
    cv::pyrDown(img_pyr, img_pyr);
    cv::Canny(img_gry, img_cny, 10, 100, 3, true);

    int x = 16, y = 32;
    cv::Vec3b intensity = img_rgb.at<cv::Vec3b>(y, x);

    printf("At (%d, %d) : (%d, %d, %d)\n", x, y, intensity[0], intensity[1], intensity[2]);
    printf("Gray pixel there is: %d\n", img_gry.at<uchar>(y, x));

    x /= 4; y /= 4;
    printf("Pyramid2 pixel there is: %d\n", img_pyr.at<uchar>(y, x));
    img_cny.at<uchar>(x, y) = 128;

    cv::imshow("Canny", img_cny);
    cv::imshow("Gray", img_gry);

    cv::waitKey(0);
}
