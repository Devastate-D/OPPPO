#pragma once
#include <cmath>

inline constexpr double eps{0.0000001};//точность при сравнении вещественных чисел
inline bool isAlmostEqual(const double a,const double b)
{
	return std::fabs(a-b)<=eps;
}
