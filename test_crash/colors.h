#ifndef __COLORS_H__
#define __COLORS_H__

#include <tuple>
#include <vector>

//typedef std::tuple<int, int, int> t_color;
//typedef std::vector< t_color > t_colors;

typedef struct{int r, g, b;} t_color;
typedef std::vector< t_color > t_colors;

t_colors make_colors_vector();

#endif