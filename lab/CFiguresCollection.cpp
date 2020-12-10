#include "CFiguresCollection.h"
#include <iostream>
#include <iomanip>

using std::cin;
using std::cout;

void CFiguresCollection::append_figure(std::unique_ptr<s_figure>&& f)
{
	m_figures.emplace_back(std::move(f));
}

rapidjson::Value& CFiguresCollection::toJson(rapidjson::Value& v,rapidjson::Document::AllocatorType& a)const
{
	v.SetObject();

	rapidjson::Value v_figures;v_figures.SetArray();
	for(auto& i:m_figures)
	{
		rapidjson::Value v_figure;
		i->toJson(v_figure,a);
		if(v_figure.IsObject() && v_figure.MemberCount())v_figures.PushBack(v_figure,a);
	}
	if(v_figures.IsArray() && v_figures.Size())v.AddMember("figures",v_figures,a);
	return v;
}

bool CFiguresCollection::fromJson(const rapidjson::Value& v)
{
	*this={};
	if(!v.IsObject())return false;

	auto& v_figures=v["figures"];
	if(!v_figures.IsArray())return false;
	m_figures.reserve(v_figures.Size());
	for(rapidjson::SizeType i=0; i<v_figures.Size(); i++)
	{
		if(auto f=s_figure::fromJson_public(v_figures[i]))
		{
			m_figures.emplace_back(std::move(f));
		}
	}

	return true;
}

void CFiguresCollection::RunConsolePlotter() const
{
    constexpr size_t cols=60;
	constexpr size_t rows=25;

	auto point_is_on_any_figure=[this](const size_t r, const size_t c)
	{
		for(const auto& f:m_figures)
		{
			//if(f->point_inside(r,c))return true;
			if(f->point_at_border(r,c))return true;
		}
		return false;
	};

	constexpr char wall='@';
	for(size_t c=0; c<cols; c++)
	{
		cout<<wall;
	}
	cout<<wall<<'\n';

	for(size_t r=0; r<rows; r++)
	{
		for(size_t c=0; c<cols; c++)
		{
			//определяем, попадает ли точка в какую-нибудь фигуру
			if(point_is_on_any_figure(r,c))
			{
				cout<<wall;
			}
			else
			{
				cout<<' ';
			}
		}
		cout<<wall<<'\n';
	}

	for(size_t c=0; c<cols; c++)
	{
		cout<<wall;
	}
	cout<<wall<<'\n';
}
