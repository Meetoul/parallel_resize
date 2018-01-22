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
    if (argc != 4)
    {
        std::cout << "Usage is: ./Resizer [filename] [scale factor] [number of threads]" << std::endl;
        return -1;
    }
    std::string imgName = argv[1];
    double scaleFactor = std::stod(argv[2]);
    int threadNum = std::stoi(argv[3]);
    auto srcRaw = read_raw(imgName);
    std::vector<char> dstRaw;
    auto begin = std::chrono::high_resolution_clock::now();
    resize(srcRaw, dstRaw, scaleFactor, threadNum);
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << "Multithread resize time: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << std::endl;
    std::ofstream fout("result_new.jpg", std::ios::out | std::ios::binary);
    std::ostream_iterator<char> ofit(fout);
    std::copy(dstRaw.begin(), dstRaw.end(), ofit);
    size_t rawSize = dstRaw.size();

    return 0;
}