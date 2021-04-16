// Copyright 2021 Vedrukov Pavel

#include <iostream>
#include "../../../modules/task_2/vedrukov_p_linear_filtering_omp/linear_filtering.h"

std::vector<std::vector<unsigned int>>
get_random_matrix(unsigned int w, unsigned int h) {
    std::vector<std::vector<unsigned int>> res(h);
    std::mt19937 gen(time(0));
    std::uniform_int_distribution<> uid(0, 255);
    for (unsigned int i = 0; i < h; ++i) {
        res[i].resize(w);
        for (unsigned int j = 0; j < w; ++j) {
            res[i][j] = uid(gen);
        }
    }
    return res;
}

std::vector<std::vector<unsigned int>> gauss_filtering(
std::vector<std::vector<unsigned int>> Image,
int radius, double sigma) {
    std::vector<std::vector<unsigned int>> res = Image;
    int h = Image.size();
    int w = Image[0].size();
    omp_set_num_threads(4);
    #pragma omp parallel for shared(Image, res)
        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                res[i][j] = 0;
                for (int y = -radius; y <= radius; ++y) {
                    for (int x = -radius; x <= radius; ++x) {
                        if ((j + x) > 0 && (j + x) < w &&
                            (i + y) > 0 && (i + y) < h) {
                            res[i][j] +=
                            static_cast<unsigned int>(Image[i + y][j + x] *
                                exp(-(x * x + y * y) / 2 / (sigma * sigma)) /
                                (2 * sigma * sigma * 3.14159265359));
                        }
                    }
                }
                res[i][j] = (res[i][j] < 0) ?
                0 : (res[i][j] > 255 ? 255 : res[i][j]);
            }
        }
    return res;
}