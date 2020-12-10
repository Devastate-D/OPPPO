#include "s_figure.h"
#include "main.h"
#include <iostream>
#include <iomanip>

using std::cin;
using std::cout;

rapidjson::Value& s_figure::toJson(rapidjson::Value& v,rapidjson::Document::AllocatorType& a) const
{
	v.SetObject();

	//общая часть
	const e_type type=GetType();
	v.AddMember("type" ,toVariant(type   ),a);
	v.AddMember("color",toVariant(m_color),a);

	if(m_edit_date)
	{
		v.AddMember("edit_date",(int64_t)*m_edit_date,a);
	}

	rapidjson::Value v_childclass;
	//наследственная часть
	switch(type)
	{
		case e_type::none       :{}break;
		case e_type::s_figure   :{}break;
		case e_type::s_circle   :{dynamic_cast<const s_circle   *>(this)->toJson(v_childclass,a);}break;
		case e_type::s_rectangle:{dynamic_cast<const s_rectangle*>(this)->toJson(v_childclass,a);}break;
	}
	if(v_childclass.IsObject() && v_childclass.MemberCount())
	{
		v.AddMember("childclass",v_childclass,a);
	}

	return v;
}

bool s_figure::fromJson_private(const rapidjson::Value& v)
{
	*this={};
	if(!v.IsObject())return false;

	//общая часть
	auto type=(e_type)v["type"].GetInt64();

	m_color=(e_color)v["color"].GetUint();

	if(const auto& v_edit_date=v["edit_date"]; !v_edit_date.IsNull())
	{
		std::time_t t=v_edit_date.GetInt64();
		m_edit_date=t;
	}

	//наследственная часть
	const auto& v_childclass=v["childclass"];
	bool ok=false;
	switch(type)
	{
		case e_type::none       :
		{
		}break;
		case e_type::s_figure   :
		{
			ok=true;
		}break;
		case e_type::s_circle   :
		{
			if(auto* f=dynamic_cast<s_circle*>(this))
			{
				ok=f->fromJson(v_childclass);
			}
		}break;
		case e_type::s_rectangle:
		{
			if(auto* f=dynamic_cast<s_rectangle*>(this))
			{
				ok=f->fromJson(v_childclass);
			}
		}break;
	}

	return ok;
}

std::unique_ptr<s_figure> s_figure::fromJson_public(const rapidjson::Value& v)
{
	if(!v.IsObject())return {};

	auto type=(e_type)v["type"].GetInt64();

	std::unique_ptr<s_figure> f;
	switch(type)
	{
		case e_type::none       :
		{
		}break;
		case e_type::s_figure   :
		{
			f=std::make_unique<s_figure>();
		}break;
		case e_type::s_circle   :
		{
			f=std::make_unique<s_circle>();
		}break;
		case e_type::s_rectangle:
		{
			f=std::make_unique<s_rectangle>();
		}break;
	}

	if(f)
	{
		f->fromJson_private(v);
	}

	return f;
}

rapidjson::Value& s_circle::toJson(rapidjson::Value& v,rapidjson::Document::AllocatorType& a) const
{
	v.SetObject();
	rapidjson::Value v2;

	v.AddMember("center",m_center.toJson(v2,a),a);
	v.AddMember("r",m_r,a);

	return v;
}

bool s_circle::fromJson(const rapidjson::Value& v)
{
	//*this={};-тут не надо из-за наследования
	if(!v.IsObject())return false;

	if(!m_center.fromJson(v["center"]))return false;
	m_r=v["r"].GetUint();

	return true;
}

rapidjson::Value& s_rectangle::toJson(rapidjson::Value& v,rapidjson::Document::AllocatorType& a) const
{
	v.SetObject();
	rapidjson::Value v2;

	v.AddMember("lef_top",m_lef_top.toJson(v2,a),a);
	v.AddMember("rig_bot",m_rig_bot.toJson(v2,a),a);

	return v;
}

bool s_rectangle::fromJson(const rapidjson::Value& v)
{
	//*this={};-тут не надо из-за наследования
	if(!v.IsObject())return false;

	if(!m_lef_top.fromJson(v["lef_top"]))return false;
	if(!m_rig_bot.fromJson(v["rig_bot"]))return false;

	return true;
}

bool s_figure::input_from_console(s_figure& f)
{
	cout<<" + inpute data for '"<<f.name()<<"' :"<<'\n';
	//cout<<"   enter "

	return true;
}

std::unique_ptr<s_circle> s_circle::input_from_console()
{
	auto f=std::make_unique<s_circle>();
	if(!s_figure::input_from_console(*f))return {};

	cout<<"   enter center x: ";cin>>f->m_center.x; if(!cin)return {};
	cout<<"   enter center y: ";cin>>f->m_center.y; if(!cin)return {};
	cout<<"   enter radius  : ";cin>>f->m_r;        if(!cin)return {};

	std::getchar();//чистка от '\n'
	return f;
}

std::unique_ptr<s_rectangle> s_rectangle::input_from_console()
{
	auto f=std::make_unique<s_rectangle>();
	if(!s_figure::input_from_console(*f))return {};

	cout<<"   enter left   side (x1): ";cin>>f->m_lef_top.x; if(!cin)return {};
	cout<<"   enter right  side (x2): ";cin>>f->m_rig_bot.x; if(!cin)return {};
	cout<<"   enter top    side (y1): ";cin>>f->m_lef_top.y; if(!cin)return {};
	cout<<"   enter botton side (y2): ";cin>>f->m_rig_bot.y; if(!cin)return {};

	std::getchar();//чистка от '\n'
	return f;
}

bool s_circle::point_inside(const size_t y, const size_t x) const
{
	double r_test=(x-m_center.x)*(x-m_center.x)+(y-m_center.y)*(y-m_center.y);
	double r_this=m_r*m_r;
	return r_test+9.0<r_this;
}

bool s_circle::point_at_border(const size_t y, const size_t x) const
{
	double r_test=(x-m_center.x)*(x-m_center.x)+(y-m_center.y)*(y-m_center.y);
	double r_this=m_r*m_r;
	return r_test-9.0<r_this && r_test+9.0>r_this;
}

bool s_rectangle::point_inside(const size_t y, const size_t x) const
{
	return(x>=m_lef_top.x && x<=m_rig_bot.x && y>=m_lef_top.y && y<=m_rig_bot.y);
}

bool s_rectangle::point_at_border(const size_t y, const size_t x) const
{
	if(x==m_lef_top.x)
	{
		return y>=m_lef_top.y && y<=m_rig_bot.y;
	}

	if(x==m_rig_bot.x)
	{
		return y>=m_lef_top.y && y<=m_rig_bot.y;
	}

	if(y==m_lef_top.y)
	{
		return x>=m_lef_top.x && x<=m_rig_bot.x;
	}

	if(y==m_rig_bot.y)
	{
		return x>=m_lef_top.x && x<=m_rig_bot.x;
	}

	return false;
}

