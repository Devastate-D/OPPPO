#pragma once
#include "s_figure.h"
#include "CLoopQueue.h"
#include <cstdint>
#include <array>
#include <memory>
#include <fstream>

class CFiguresCollection
{
public:
	CLoopQueue<std::unique_ptr<s_figure>> m_figures;

	void append_figure(std::unique_ptr<s_figure>&& f);
	rapidjson::Value& toJson(rapidjson::Value& v,rapidjson::Document::AllocatorType& a)const;
	bool fromJson(const rapidjson::Value& v);

	void RunConsolePlotter()const;
};
