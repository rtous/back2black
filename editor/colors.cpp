#include "colors.h"

#define NUM_COLORS 256

t_colors make_colors_vector() {
    t_colors v;
    for (int i = 0; i < NUM_COLORS; ++i) {
        const int r = rand() % 256;
        const int g = rand() % 256;
        const int b = rand() % 256;

        //t_color c(r,g,b);
        t_color c = {r, g, b};
        v.push_back(c);
    }
    return v;
}

/*std::vector<std::tuple<int, int, int>>  make_colors_vector() {
    std::vector<std::tuple<int, int, int>> v;
    for (int i = 0; i < NUM_COLORS; ++i) {
        const int r = (125 + i * 50) % 256;
        const int g = (50 + i * 50) % 256;
        const int b = (200 + i * 50) % 256;
        std::tuple<int, int, int> c(r,g,b);
        v.push_back(c);
    }
    return v;
}*/