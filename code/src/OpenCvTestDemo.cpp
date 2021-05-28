// OpenCvTestDemo.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "SampleCommonHead.hpp"

#include <iostream>
#include <opencv2/opencv.hpp>


int main()
{
    //TestDisplayImg(u8R"(D:\Project\git\opencv\TestResource\image\0.jpg)");
    //TestDisplayImgWithBlur(u8R"(D:\Project\git\opencv\TestResource\image\0.jpg)");
    //TestDisplayImgWithPyrDown(u8R"(D:\Project\git\opencv\TestResource\image\0.jpg)");
    //TestDisplayImgWithCanny(u8R"(D:\Project\git\opencv\TestResource\image\1.jpg)");
    //TestChangePixels(u8R"(D:\Project\git\opencv\TestResource\image\1.jpg)");

    //TestDisplayVideo(u8R"(D:\Project\git\opencv\TestResource\video\0.mp4)");
    //TestDisplayVideoEx(u8R"(D:\Project\git\opencv\TestResource\video\0.mp4)");
    TestDisplayCamera();
    std::cout << "Hello World!\n";
}
