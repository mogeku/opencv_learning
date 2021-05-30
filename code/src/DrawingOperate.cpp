#include "SampleCommonHead.hpp"

#include <opencv2/opencv.hpp>

void TestDrawShapAndText()
{
    cv::Mat bg_img(1000, 1000, CV_8UC3);
    bg_img = cv::Scalar(0);

    //// line
    //{
    //    cv::Point p1(100, 100);
    //    cv::Point p2(500, 600);
    //    cv::Point p3(500, 700);
    //    cv::Scalar color(0, 0, 255);

    //    cv::line(bg_img, p1, p2, color, 1, cv::LINE_AA);
    //    cv::line(bg_img, p1, p3, color, 1, cv::LINE_4);
    //}

    //// rectangle
    //{
    //    cv::Rect rect(300, 300, 100, 200);
    //    cv::Scalar color(255, 0, 0);

    //    cv::rectangle(bg_img, rect, color, 1, cv::LINE_8);
    //}

    //// ellipse
    //{
    //    cv::Scalar color(0, 255, 0);
    //    cv::Point center(bg_img.rows / 2, bg_img.cols / 2);
    //    cv::Size axes(bg_img.rows / 4, bg_img.cols / 8);

    //    cv::ellipse(bg_img, center, axes, 90, 0, 300, color, 2);
    //}

    //// circle
    //{
    //    cv::Scalar color(0, 255, 255);
    //    cv::Point center(bg_img.rows / 2, bg_img.cols / 2);

    //    cv::circle(bg_img, center, bg_img.rows / 4, color, 2);
    //}

    // polygon
    {
        //cv::Point polygon1[]{ {100, 100}, {200, 150}, {100, 150}, {150, 300}, {50, 150} };
        //cv::Point polygon2[]{ {500, 550}, {580, 900}, {500, 800} };
        cv::Point polygon1[]{ {100, 100}, {200, 100}, {200, 300}, {100, 300} };
        cv::Point polygon2[]{ {500, 550}, {530, 580}, {520, 560} };
        cv::Point polygon3[]{ {700, 600}, {710, 630}, {690, 610}, {690, 590}, {695, 600} };
        cv::Point polygon4[]{ {400, 600}, {410, 630}, {390, 610}, {390, 590}, {395, 600} };
        const cv::Point* pts[]{polygon1, polygon2, polygon3, polygon4};
        int nts[] = { 4, 3 , 5, 5};
        cv::Scalar color(255, 255, 255);

        cv::fillPoly(bg_img, pts, nts, 4, color);

        std::vector<cv::Point> vec_pts1;
        vec_pts1.push_back({ 150, 150 });
        vec_pts1.push_back({ 160, 150 });
        vec_pts1.push_back({ 160, 170 });
        vec_pts1.push_back({ 150, 170 });
        //std::vector<cv::Point> vec_pts2;
        //vec_pts2.push_back({ 100, 100 });
        //vec_pts2.push_back({ 200, 100 });
        //vec_pts2.push_back({ 200, 400 });
        //vec_pts2.push_back({ 300, 400 });

        //cv::fillPoly(bg_img, std::vector<std::vector<cv::Point>>{ vec_pts1, vec_pts2 }, { 255, 25, 0 });
        cv::fillPoly(bg_img, vec_pts1, {0});
    }

    //// text
    //{
    //    cv::Point center(bg_img.rows / 2, bg_img.cols / 2);
    //    cv::Scalar color{ 0 };

    //    cv::putText(bg_img, "Hello Opencv", center, cv::FONT_HERSHEY_COMPLEX, 2.0, color, 2);
    //}

    cv::imshow("canvas", bg_img);
    //cv::imwrite(u8R"(../../../resource/image/2.jpg)", bg_img);
    cv::waitKey(0);
}

void TestRandomLine()
{
    cv::Mat bg_img(1000, 1000, CV_8UC3, { 255, 255, 255 });

    cv::RNG rng(1000);
    while (true)
    {
        cv::Point p1, p2;
        p1.x = rng.uniform(0, bg_img.rows);
        p1.y = rng.uniform(0, bg_img.cols);
        p2.x = rng.uniform(0, bg_img.rows);
        p2.y = rng.uniform(0, bg_img.cols);
        cv::Scalar color(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));

        if (-1 != cv::waitKey(5))
            break;

        cv::line(bg_img, p1, p2, color);
        cv::imshow("RandomLine", bg_img);
    }
}