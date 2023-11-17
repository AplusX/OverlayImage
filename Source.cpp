#include "highgui/highgui.hpp"    
#include "opencv.hpp"    
#include <core/core.hpp>  
#include <iostream>
#include <cmath>

using namespace std;

int main() {
    // Read the image
    cv::Mat img = cv::imread("./FinalBuffer.png",1);
    cv::Mat Mask = cv::imread("./Alpha.png", 0);
    img.convertTo(img, CV_32FC3);
    Mask.convertTo(Mask, CV_32FC1);

    if (img.empty()) {
        std::cerr << "Error: Unable to read the image." << std::endl;
        return -1;
    }
    cv::Mat output = cv::Mat(img.size(), CV_32FC4);

    for (int i = 0; i < img.rows; i++) {
        for (int j = 0; j < img.cols; j++) {
            for (int k = 0; k < 3; k++) {
                output.at<cv::Vec4f>(i, j)[k] = img.at<cv::Vec3f>(i, j)[k];
            }
           output.at<cv::Vec4f>(i, j)[3] = Mask.at<float>(i, j);
        }
    }
    cv::imwrite("Output.png", output);
    cv::Mat Forward = cv::imread("./Output.png",-1);
    Forward.convertTo(Forward, CV_32FC4);
    cv::Mat background = cv::imread("./background.png",1);
    background.convertTo(background, CV_32FC3);
    cv::Mat Final = cv::imread("./background.png",1);
    Final.convertTo(Final, CV_32FC3);

    float scale = 0.7f;
    cv::resize(Forward, Forward, cv::Size(), scale, scale);

    cv::Point insert_loc = cv::Point(-150, 50);

    for (int i = insert_loc.y; i < insert_loc.y + Forward.rows; i++) {
        for (int j = insert_loc.x; j < insert_loc.x + Forward.cols; j++) {
            if (i > 0 && j > 0 && i < background.rows && j < background.cols) {
                for (int k = 0; k < 3; k++) {
                    float alpha = Forward.at<cv::Vec4f>(i - insert_loc.y, j - insert_loc.x)[3] / 255.0;
                    Final.at<cv::Vec3f>(i, j)[k] = Forward.at<cv::Vec4f>(i- insert_loc.y, j- insert_loc.x)[k] * alpha + background.at<cv::Vec3f>(i, j)[k] * (1.0 - alpha);
                }
            }
        }
    }
    Final.convertTo(Final, CV_8UC3);
    // Show the image
    cv::imwrite("Final.png", Final);
    std::cout << "Finish";

    return 0;
}
