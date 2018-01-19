#include <opencv2/opencv.hpp>
#include <string>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>
#include <future>

void my_resize(cv::Mat *src, cv::Mat *dst, int y, int h, double scaleFactor)
{
    int dst_y = y * scaleFactor;
    int dst_h = h * scaleFactor;
    cv::resize((*src)(cv::Rect{0, y, src->cols, h}), (*dst)(cv::Rect{0, dst_y, dst->cols, dst_h}), cv::Size(), scaleFactor, scaleFactor);
}

void mt_resize(cv::Mat &src, cv::Mat &dst, double scaleFactor, int threadNum = 0)
{
    if (threadNum == 0)
        threadNum = std::thread::hardware_concurrency();
    int partHeight = src.rows / threadNum;
    std::vector<std::future<void>> futures;
    dst = cv::Mat(cv::Size(src.cols * scaleFactor, src.rows * scaleFactor), CV_8UC3, cv::Scalar(0, 0, 0));
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < threadNum; i++)
    {
        futures.emplace_back(std::async(my_resize, &src, &dst, partHeight * i, partHeight, scaleFactor));
    }
    for (auto &f : futures)
    {
        f.get();
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Only resize time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "\n";
}

int main(int argc, char **argv)
{
    if (argc != 4)
    {
        printf("Usage is: ./Resizer [filename] [scale factor] [number of threads]");
        return -1;
    }
    std::string imgName = argv[1];
    double scaleFactor = std::stod(argv[2]);
    int thread_num = std::stoi(argv[3]);

    cv::Mat src = cv::imread(imgName.c_str(), CV_LOAD_IMAGE_COLOR);
    cv::Mat dst;
    auto begin = std::chrono::high_resolution_clock::now();
    cv::resize(src, dst, cv::Size(), scaleFactor, scaleFactor);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Single thread resize time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "\n";

    cv::Mat dst2;
    begin = std::chrono::high_resolution_clock::now();
    mt_resize(src, dst2, scaleFactor, thread_num);
    end = std::chrono::high_resolution_clock::now();
    std::cout << "Multi thread resize time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "\n";
    cv::imwrite("resullt.jpg", dst2);
    return 0;
}