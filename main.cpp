#include <string>
#include <chrono>
#include <iostream>
#include <iterator>
#include <vector>
#include <fstream>
#include "MResizer.hpp"

std::vector<char> read_raw(std::string &filename)
{
    std::ifstream ifs(filename, std::ios::binary);
    std::vector<char> result{std::istreambuf_iterator<char>(ifs),
                             std::istreambuf_iterator<char>()};
    return result;
}

int main(int argc, char **argv)
{
    if (argc != 5)
    {
        std::cout << "Usage is: ./Resizer [filename] [width] [height] [number of threads]" << std::endl;
        return -1;
    }
    std::string imgName = argv[1];
    int width = std::stoi(argv[2]);
    int height = std::stoi(argv[3]);
    int threadNum = std::stoi(argv[4]);
    auto srcRaw = read_raw(imgName);
    std::vector<char> dstRaw;
    auto begin = std::chrono::high_resolution_clock::now();
    resize(srcRaw, dstRaw, width, height, threadNum);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Multithread resize time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << std::endl;
    std::ofstream fout("result_new.jpg", std::ios::out | std::ios::binary);
    std::ostream_iterator<char> ofit(fout);
    std::copy(dstRaw.begin(), dstRaw.end(), ofit);
    return 0;
}