#pragma once
#include <cstdint>
#include <vector>

//кольцевая однонаправленная очередь
template<typename T>
class CLoopQueue:public std::vector<T>
{
};
