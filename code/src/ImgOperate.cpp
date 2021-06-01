#include "SampleCommonHead.hpp"

#include <iostream>
#include <random>
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
    //cv::blur(img, out, cv::Size(7, 7));
    cv::GaussianBlur(img, out, cv::Size(5, 5), 0.1, 0.1);
    //cv::GaussianBlur(out, out, cv::Size(5, 5), 3, 3);

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

void TestAddSaltNoise(const char* file_path)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    cv::Mat src = cv::imread(file_path, -1);
    if (src.empty())
    {
        printf("imread failed\n");
        return;
    }
    cv::imshow("src", src);

    int rows = src.rows;
    int cols = src.cols;
    int cn = src.channels();

    int noise_num = gen() % (rows * cols / 256);
    //int noise_num = 50;

    for (int i = 0; i < noise_num; ++i)
    {
        int row = gen() % rows;
        int col = gen() % cols;

        uchar* p = src.ptr<uchar>(row);
        for (int c = 0; c < (cn > 3 ? 3 : cn); ++c)
        {
            //p[col * cn + c] = gen() % 2 ? 255 : 0;
            p[col * cn + c] = 255;
        }
    }
    cv::imshow("out", src);

    std::string save_path(file_path);
    std::size_t dot_pos = save_path.rfind('.');
    save_path = save_path.substr(0, dot_pos) + "_SaltNoise" + save_path.substr(dot_pos);
    cv::imwrite(save_path, src);
    
    cv::waitKey(0);
}

void TestMedianFilter(const char* file_path)
{
    cv::Mat src = cv::imread(file_path, -1);
    if (src.empty())
    {
        printf("imread failed\n");
        return;
    }
    cv::imshow("src", src);

    cv::Mat dst;
    cv::medianBlur(src, dst, 5);

    cv::imshow("dst", dst);
    cv::waitKey(0);
}

void TestBilateralFilter(const char* file_path)
{
    cv::Mat src = cv::imread(file_path, -1);
    if (src.empty())
    {
        printf("imread failed\n");
        return;
    }
    cv::imshow("src", src);

    cv::Mat dst;

    cv::GaussianBlur(src, dst, cv::Size(5, 5), 10, 10);
    cv::imshow("Gaussian Blur", dst);

    cv::bilateralFilter(src, dst, 0, 100, 2);
    cv::imshow("Bilateral Filter", dst);

    cv::Matx<int, 3, 3> mask(0, -1, 0, -1, 5, -1, 0, -1, 0);
    cv::filter2D(dst, dst, dst.depth(), mask);
    cv::imshow("Result", dst);

    cv::waitKey(0);
}

void ElementTrackbarCallback(int pos, void* src)
{
    int s = pos * 2 + 1;
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(s, s));

    cv::Mat dst;
    cv::dilate(*(cv::Mat*)src, dst, kernel);
    cv::imshow("Dilate", dst);
    cv::erode(*(cv::Mat*)src, dst, kernel);
    cv::imshow("Erode", dst);
}
void TestDilateAndErode(const char* file_path)
{
    cv::Mat src = cv::imread(file_path, -1);
    if (src.empty())
    {
        printf("imread failed\n");
        return;
    }
    cv::imshow("src", src);

    int pos = 0;
    cv::namedWindow("Dilate", cv::WINDOW_AUTOSIZE);
    cv::createTrackbar("Element size", "Dilate", &pos, 20, ElementTrackbarCallback, &src);
    ElementTrackbarCallback(pos, &src);

    cv::waitKey(0);
}

void TestOpenAndClose(const char* file_path)
{
    cv::Mat src = cv::imread(file_path, -1);
    if (src.empty())
    {
        printf("imread failed\n");
        return;
    }
    cv::imshow("src", src);

    cv::Mat dst;
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(15, 15));

    cv::morphologyEx(src, dst, cv::MORPH_OPEN, kernel);
    cv::imshow("open", dst);

    cv::morphologyEx(src, dst, cv::MORPH_CLOSE, kernel);
    cv::imshow("close", dst);

    cv::waitKey(0);
}

void MorphGradintTrackbarCallback(int pos, void* src)
{
    int s = pos * 2 + 1;
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(s, s));

    cv::Mat dst;
    cv::morphologyEx(*(cv::Mat*)src, dst, cv::MORPH_GRADIENT, kernel);
    cv::imshow("Gradient", dst);
}
void TestMorphGradint(const char* file_path)
{
    cv::Mat src = cv::imread(file_path, -1);
    if (src.empty())
    {
        printf("imread failed\n");
        return;
    }
    cv::imshow("src", src);

    int pos = 0;
    MorphGradintTrackbarCallback(pos, &src);
    cv::createTrackbar("Size", "Gradient", &pos, 20, MorphGradintTrackbarCallback, &src);

    cv::waitKey(0);
}

void MorphTopAndBlackHatTrackbarCallback(int pos, void* src)
{
    int s = pos * 2 + 1;
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(s, s));

    cv::Mat dst;
    cv::morphologyEx(*(cv::Mat*)src, dst, cv::MORPH_TOPHAT, kernel);
    cv::imshow("Top Hat", dst);

    cv::morphologyEx(*(cv::Mat*)src, dst, cv::MORPH_BLACKHAT, kernel);
    cv::imshow("Black Hat", dst);
}
void TestMorphTopAndBlackHat(const char* file_path)
{
    cv::Mat src = cv::imread(file_path, -1);
    if (src.empty())
    {
        printf("imread failed\n");
        return;
    }
    cv::imshow("src", src);

    int pos = 0;
    MorphTopAndBlackHatTrackbarCallback(pos, &src);
    cv::createTrackbar("Size", "Top Hat", &pos, 20, MorphTopAndBlackHatTrackbarCallback, &src);

    cv::waitKey(0);
}

void TestExtractLineAndText(const char* file_path)
{
    cv::Mat src = cv::imread(file_path, -1);
    if (src.empty())
    {
        printf("imread failed\n");
        return;
    }
    cv::imshow("src", src);

    cv::Mat img_gray, img_bin, dst;
    cv::cvtColor(src, img_gray, cv::COLOR_BGR2GRAY);
    cv::imshow("gray", img_gray);

    cv::adaptiveThreshold(img_gray, img_bin, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY_INV, 7, 2);
    //cv::threshold(img_gray, img_bin, 127, 255, cv::THRESH_BINARY_INV);
    cv::imshow("binary", img_bin);

    cv::Mat hline = cv::getStructuringElement(cv::MORPH_RECT, { 7, 1 });
    cv::Mat vline = cv::getStructuringElement(cv::MORPH_RECT, { 1, 3 });
    cv::Mat rect = cv::getStructuringElement(cv::MORPH_RECT, { 3, 3 });

    //cv::erode(img_bin, dst, rect);
    //cv::dilate(dst, dst, vline);
    cv::morphologyEx(img_bin, dst, cv::MORPH_OPEN, rect);
    cv::bitwise_not(dst, dst);
    //cv::blur(dst, dst, { 3, 3 });
    cv::imshow("result", dst);

    cv::waitKey(0);
}

void TestPyrUpAndDown(const char* file_path)
{
    cv::Mat src = cv::imread(file_path, -1);
    if (src.empty())
    {
        printf("imread failed\n");
        return;
    }
    cv::imshow("src", src);

    cv::Mat dst;
    cv::pyrUp(src, dst);
    cv::imshow("PyrUp", dst);

    cv::pyrDown(src, dst);
    cv::imshow("PyrDown", dst);

    //Dog
    cv::cvtColor(dst, dst, cv::COLOR_BGR2GRAY);
    cv::Mat img_dog1, img_dog2;
    cv::GaussianBlur(dst, img_dog1, { 3, 3 }, 0, 0);
    cv::GaussianBlur(dst, img_dog2, { 5, 5 }, 0, 0);
    cv::subtract(img_dog1, img_dog2, dst);

    cv::normalize(dst, dst, 255, 0, cv::NORM_MINMAX);
    cv::imshow("DOG", dst);

    cv::waitKey(0);
}

struct ThresholdArgs
{
    cv::Mat* src;
    bool is_type;
};
void ThresholdTrackbarCallback(int pos, void* args)
{
    ThresholdArgs* param = (ThresholdArgs*)args;
    cv::Mat* src = param->src;
    static double thresh = 127;
    static int type = cv::THRESH_BINARY;

    if (param->is_type) type = pos; else thresh = pos;

    cv::Mat dst;
    cv::cvtColor(*src, dst, cv::COLOR_BGR2GRAY);
    //cv::threshold(dst, dst, thresh, 255, type);
    //cv::threshold(dst, dst, thresh, 255, type | cv::THRESH_OTSU);
    cv::threshold(dst, dst, thresh, 255, type | cv::THRESH_TRIANGLE);
    cv::imshow("result", dst);
}
void TestThreshold(const char* file_path)
{
    cv::Mat src = cv::imread(file_path, -1);
    if (src.empty())
    {
        printf("imread failed\n");
        return;
    }
    cv::imshow("src", src);

    cv::namedWindow("result", cv::WINDOW_AUTOSIZE);
    ThresholdArgs param1 = { &src, false};
    ThresholdArgs param2 = { &src, true };
    cv::createTrackbar("Thresh Value", "result", NULL, 255, ThresholdTrackbarCallback, &param1);
    cv::createTrackbar("Thresh Type", "result", NULL, 4, ThresholdTrackbarCallback, &param2);

    cv::waitKey(0);
}

void TestConvolution(const char* file_path)
{
    cv::Mat src = cv::imread(file_path, -1);
    if (src.empty())
    {
        printf("imread failed\n");
        return;
    }
    cv::imshow("src", src);

    cv::Matx<int, 2, 2> k_rebert_x(1, 0, 0, -1);
    cv::Matx<int, 2, 2> k_rebert_y(0, 1, -1, 0);
    cv::Matx<int, 3, 3> k_sobel_x(-1, 0, 1, -2, 0, 2, -1, 0, 1);
    cv::Matx<int, 3, 3> k_sobel_y(-1, -2, -1, 0, 0, 0, 1, 2, 1);
    cv::Matx<int, 3, 3> k_laplace(0, -1, 0, -1, 4, -1, 0, -1, 0);

    cv::Mat dst;
    cv::filter2D(src, dst, src.depth(), k_rebert_x);
    cv::imshow("Rebert x", dst);
    cv::filter2D(src, dst, src.depth(), k_rebert_y);
    cv::imshow("Rebert y", dst);
    cv::filter2D(src, dst, src.depth(), k_sobel_x);
    cv::imshow("Sobel x", dst);
    cv::filter2D(src, dst, src.depth(), k_sobel_y);
    cv::imshow("Sobel y", dst);
    cv::filter2D(src, dst, src.depth(), k_laplace);
    cv::imshow("Laplace", dst);

    int ksize = 0;
    int index = 0;
    while (true)
    {
        ksize = 4 + (index++ % 8) * 2 + 1;
        cv::Mat k_blur = cv::Mat::ones({ ksize, ksize }, CV_32F) / ((float)ksize * (float)ksize);
        cv::filter2D(src, dst, src.depth(), k_blur);
        cv::imshow("Custom blur", dst);

        if (27 == cv::waitKey(500))
            break;
    }
}

void TestDealBorder(const char* file_path)
{
    cv::Mat src = cv::imread(file_path, -1);
    if (src.empty())
    {
        printf("imread failed\n");
        return;
    }
    cv::imshow("src", src);

    cv::Mat dst;
    cv::copyMakeBorder(src, dst, 20, 20, 20, 20, cv::BORDER_DEFAULT);
    cv::imshow("Border default", dst);
    cv::copyMakeBorder(src, dst, 20, 20, 20, 20, cv::BORDER_REPLICATE);
    cv::imshow("Border replicate", dst);
    cv::copyMakeBorder(src, dst, 20, 20, 20, 20, cv::BORDER_CONSTANT, { 255, 100, 255 });
    cv::imshow("Border constant", dst);
    cv::copyMakeBorder(src, dst, 20, 20, 20, 20, cv::BORDER_WRAP);
    cv::imshow("Border wrap", dst);

    cv::waitKey(0);
}

void TestSobel(const char* file_path)
{
    cv::Mat src = cv::imread(file_path, -1);
    if (src.empty())
    {
        printf("imread failed\n");
        return;
    }
    cv::imshow("src", src);

    cv::Mat dst, sobel_x, sobel_y, sobel_xy;
    cv::GaussianBlur(src, dst, { 3, 3 }, 0, 0);
    cv::cvtColor(dst, dst, cv::COLOR_BGR2GRAY);

    cv::Sobel(dst, sobel_x, -1, 1, 0);
    cv::Sobel(dst, sobel_y, -1, 0, 1);
    cv::imshow("Sobel x", sobel_x);
    cv::imshow("Sobel y", sobel_y);

    //cv::Scharr(dst, sobel_x, CV_16S, 1, 0);
    //cv::Scharr(dst, sobel_y, CV_16S, 0, 1);
    cv::Sobel(dst, sobel_x, CV_16S, 1, 0);
    cv::Sobel(dst, sobel_y, CV_16S, 0, 1);
    cv::convertScaleAbs(sobel_x, sobel_x);
    cv::convertScaleAbs(sobel_y, sobel_y);
    cv::imshow("Dealed Sobel x", sobel_x);
    cv::imshow("Dealed Sobel y", sobel_y);

    cv::add(sobel_x, sobel_y, sobel_xy);
    cv::imshow("Final result1", sobel_xy);
    cv::addWeighted(sobel_x, 0.5, sobel_y, 0.5, 0, sobel_xy);
    cv::imshow("Final result2", sobel_xy);

    cv::waitKey(0);
}

void TestLaplacian(const char* file_path)
{
    cv::Mat src = cv::imread(file_path, -1);
    if (src.empty())
    {
        printf("imread failed\n");
        return;
    }
    cv::imshow("src", src);

    cv::Mat dst, laplacian;
    cv::GaussianBlur(src, dst, { 3, 3 }, 0, 0);
    cv::cvtColor(dst, dst, cv::COLOR_BGR2GRAY);

    cv::Laplacian(dst, laplacian, -1, 3);
    cv::imshow("Laplacian", laplacian);

    cv::Laplacian(dst, laplacian, CV_16S, 3);
    cv::convertScaleAbs(laplacian, laplacian);
    cv::threshold(laplacian, laplacian, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);
    cv::imshow("Dealed Laplacian", laplacian);

    cv::waitKey(0);
}

void CannyTrackbarCallback(int pos, void* data)
{
    cv::Mat* src = (cv::Mat*)data;

    cv::Mat dst, edges, gray;
    cv::blur(*src, gray, { 3, 3 });
    cv::cvtColor(*src, gray, cv::COLOR_BGR2GRAY);
    cv::Canny(gray, edges, pos, pos * 2);

    dst.create(src->size(), src->type());
    src->copyTo(dst, edges);
    cv::imshow("dst", dst);
}
void TestCanny(const char* file_path)
{
    cv::Mat src = cv::imread(file_path, -1);
    if (src.empty())
    {
        printf("imread failed\n");
        return;
    }
    cv::imshow("src", src);

    CannyTrackbarCallback(0, &src);
    cv::createTrackbar("Canny thresh", "dst", NULL, 255, CannyTrackbarCallback, &src);

    cv::waitKey(0);
}
