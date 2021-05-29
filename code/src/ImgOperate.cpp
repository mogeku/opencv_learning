#include "SampleCommonHead.hpp"

#include <iostream>
#include <opencv2/opencv.hpp>

void TestDisplayImg(const char* file_path)
{
    //cv::Mat img = cv::imread(file_path, cv::ImreadModes::IMREAD_UNCHANGED);
    //cv::Mat img = cv::imread(file_path, cv::ImreadModes::IMREAD_GRAYSCALE);
    cv::Mat img = cv::imread(file_path, cv::ImreadModes::IMREAD_COLOR);
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
    cv::Mat img_rgb, img_gry, img_cny, img_pyr, img_gry2;

    img_rgb = cv::imread(file_path);

    cv::cvtColor(img_rgb, img_gry, cv::COLOR_BGR2GRAY);
    cv::cvtColor(img_rgb, img_gry2, cv::COLOR_RGB2GRAY);
    cv::imshow("Rgb", img_rgb);
    cv::imshow("Gray", img_gry);
    cv::imshow("Gray2", img_gry2);

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

void TestMaskModifyImg(const char* file_path)
{
    cv::Mat img = cv::imread(file_path, cv::ImreadModes::IMREAD_UNCHANGED);
    if (img.empty())
    {
        printf("Image read failed\n");
        return;
    }
    cv::imshow("origin", img);

    double t = cv::getTickCount();
    int cols = img.cols;
    int rows = img.rows;
    int channals = img.channels();

    cv::Mat out(rows, cols, CV_8UC4);
    for (int row = 1; row < rows - 1; ++row)
    {
        const uchar* pre = img.ptr<uchar>(row - 1);
        const uchar* cur = img.ptr<uchar>(row);
        const uchar* next = img.ptr<uchar>(row + 1);

        uchar* out_cur = out.ptr<uchar>(row);
        for (int col = 1 * channals; col < (cols - 1) * channals; col += channals)
        {
            const uchar* up = pre + col;
            const uchar* left = cur + (col - 1 * channals);
            const uchar* right = cur + (col + 1 * channals);
            const uchar* down = next + col;
            const uchar* center = cur + col;
           
            uchar* out_center = out_cur + col;

            for (int channal = 0; channal < (channals > 3 ? 3 : channals); ++channal)
            {
                out_center[channal] = cv::saturate_cast<uchar>(5 * center[channal] - (up[channal] + left[channal] + right[channal] + down[channal]));
            }
            out_center[3] = center[3];
        }
    }
    t = (cv::getTickCount() - t) / cv::getTickFrequency();
    printf("My modify consume time: %.2f\n", t);

    cv::imshow("modified", out);

    t = cv::getTickCount();
    cv::Mat out_by_filter2D;
    cv::filter2D(img, out_by_filter2D, img.depth(), cv::Matx33f{ 0, -1, 0, -1, 5, -1, 0, -1, 0 });
    t = (cv::getTickCount() - t) / cv::getTickFrequency();
    printf("filter2D consume time: %.2f\n", t);

    cv::imshow("filter2D", out_by_filter2D);

    cv::waitKey(0);
}

void TestCreateMat(const char* file_path)
{
    cv::Mat img = cv::imread(file_path, -1);
    if (img.empty())
    {
        printf("imread failed\n");
        return;
    }
    cv::imshow("origin", img);

    cv::Mat mat = cv::Mat::zeros(img.size(), img.type());
    //mat = cv::Scalar(255, 0, 255);
    //mat = img.clone();
    img.copyTo(mat);
    //mat = cv::Mat::eye(10, 10, CV_8UC3);
    //std::cout << mat << std::endl;
    cv::imshow("created", mat);

    cv::Vec4b* pixel = mat.ptr<cv::Vec4b>(0);
    printf("%d, %d, %d\n", (*pixel)[0], (*pixel)[1], (*pixel)[2]);
    pixel = &mat.at<cv::Vec4b>(10);
    auto vec4f = mat.at<cv::Vec4f>(10);
    printf("sizeof(Vec4f)=%d\n", sizeof(cv::Vec4f));
    printf("sizeof(Vec4b)=%d\n", sizeof(cv::Vec4b));
    printf("%d, %d, %d\n", (*pixel)[0], (*pixel)[1], (*pixel)[2]);

    cv::waitKey(0);
}

void TestReverseColor(const char* file_path)
{
    cv::Mat src = cv::imread(file_path, 1);
    if (src.empty())
    {
        printf("imread failed\n");
        return;
    }
    cv::imshow("origin", src);

    cv::Mat dst = cv::Mat(src.size(), src.type());
    int rows = src.rows;
    int cols = src.cols;
    int channals = src.channels();

    cv::Vec3b pixel;
    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < cols; ++col)
        {
            pixel = src.at<decltype(pixel)>(row, col);
            pixel[0] = 255 - pixel[0];
            pixel[1] = 255 - pixel[1];
            pixel[2] = 255 - pixel[2];
            //pixel[0] = pixel[2];
            //pixel[1] = pixel[2];
            dst.at<decltype(pixel)>(row, col) = pixel;
        }
    }

    cv::imshow("output", dst);
    
    cv::bitwise_not(src, dst);
    cv::imshow("bitwise_not", dst);

    cv::waitKey(0);
}

void TestBlendImg(const char* file_path1, const char* file_path2)
{
    cv::Mat src1, src2;
    src1 = cv::imread(file_path1, 1);
    src2 = cv::imread(file_path2, 1);
    if (src1.empty() || src2.empty())
    {
        printf("imread failed\n");
        return;
    }
    cv::imshow("origin1", src1);
    cv::imshow("origin2", src2);

    cv::Mat dst;
    double alpha = 0.5;
    if (src1.rows == src2.rows && src1.cols == src2.cols && src1.type() == src2.type())
    {
        cv::addWeighted(src1, alpha, src2, (1.0 - alpha), 0, dst);
    }
    else
    {
        printf("Image is not same, can not blend\n");
        return;
    }

    cv::imshow("output", dst);
    cv::waitKey(0);
}

void TestAdjContrastAndLight(const char* file_path)
{
    cv::Mat src = cv::imread(file_path, 1);
    if (src.empty())
    {
        printf("imread failed\n");
        return;
    }
    cv::imshow("origin", src);

    src.convertTo(src, CV_32F);

    int rows = src.rows;
    int cols = src.cols;
    int cn = src.channels();
    cv::Mat dst = cv::Mat(src.size(), src.type());
    dst.convertTo(dst, CV_32F);

    float alpha = 1.3;
    float beta = -10;
    
    cv::Vec3f pixel;
    for (int row = 0; row < rows; ++row)
    {
        for (int col = 0; col < cols; ++col)
        {
            if (cn == 3)
            {
                pixel = src.at<decltype(pixel)>(row, col);
                pixel[0] = cv::saturate_cast<float>(pixel[0] * alpha + beta);
                pixel[1] = cv::saturate_cast<float>(pixel[1] * alpha + beta);
                pixel[2] = cv::saturate_cast<float>(pixel[2] * alpha + beta);
                dst.at<decltype(pixel)>(row, col) = pixel;
            }
            else if (cn == 1)
            {
                dst.at<uchar>(row, col) = src.at<uchar>(row, col) * alpha + beta;
            }

        }
    }

    dst.convertTo(dst, CV_8UC3);
    cv::imshow("output", dst);
    cv::waitKey(0);
}
