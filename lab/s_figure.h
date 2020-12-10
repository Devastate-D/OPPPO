#pragma once

#include "rapidjson/document.h"
#include <cstdint>
#include <string>
#include <ctime>
#include <optional>
#include <fstream>
#include <sstream>
#include <memory>

template<typename T>
struct s_point
{
	T x{0};
	T y{0};

	rapidjson::Value& toJson(rapidjson::Value& v,rapidjson::Document::AllocatorType& a) const
	{
		v.SetObject();
		if constexpr(std::is_same_v<T, float> || std::is_same_v<T, double>)
		{
			v.AddMember("x",x,a);
			v.AddMember("y",y,a);
		}
		else
		{
			v.AddMember("x",x,a);
			v.AddMember("y",y,a);
		}
		return v;
	}

	bool fromJson(const rapidjson::Value& v)
	{
		*this={};
		if(!v.IsObject())return false;

		if constexpr(std::is_same_v<T, float> || std::is_same_v<T, double>)
		{
			x=v["x"].GetDouble();
			y=v["y"].GetDouble();
		}
		else
		{
			x=v["x"].GetUint();
			y=v["y"].GetUint();
		}

		return true;
	}
};

struct s_figure
{
	enum class e_type:uint16_t
	{
		none,
		s_figure,
		s_circle,
		s_rectangle,
	};
	static_assert(       sizeof(e_type)==      sizeof(uint16_t));
	friend auto toVariant(const e_type& item){return (uint16_t)item;}

	enum class e_color:uint8_t
	{
		red,//красный
		orange,//оранжевый
		yellow,//желтый
		green,//зеленый
		light_blue,//голубой
		deep_blue,//синий
		violet,//фиолетовый
	};
	static_assert(       sizeof(e_color)==      sizeof(uint8_t));
	friend auto toVariant(const e_color& item){return (uint8_t)item;}

	friend std::string toString(const e_color& v)
	{
		switch(v)
		{

			case e_color::red       :return "red"       ;break;
			case e_color::orange    :return "orange"    ;break;
			case e_color::yellow    :return "yellow"    ;break;
			case e_color::green     :return "green"     ;break;
			case e_color::light_blue:return "light_blue";break;
			case e_color::deep_blue :return "deep_blue" ;break;
			case e_color::violet    :return "violet"    ;break;
		}

		return "?";
	};

	virtual e_type GetType()const
	{
		return e_type::s_figure;
	}
	virtual std::string name()const
	{
		return "figure";
	}

	e_color m_color{};
	std::optional<std::time_t> m_edit_date{};//Дата последнего редактирования фигуры: ДД:ММ:ГГГГ;

	virtual ~s_figure()=default;

	static constexpr char m_ctime_format[]="%d:%m:%Y";
	std::string GetDate()
	{
		if(!m_edit_date)return "?";
		char s[64]{};
		{
			//https://en.cppreference.com/w/cpp/chrono/c/strftime
			std::strftime(s,std::size(s)-1,m_ctime_format, std::localtime(&(*m_edit_date)));
		}
		return s;
	}

	void SetDateAsCurrent()
	{
		m_edit_date=std::time(0);
	}

	rapidjson::Value& toJson(rapidjson::Value& v,rapidjson::Document::AllocatorType& a)const;
private:
	bool fromJson_private(const rapidjson::Value& v);
public:
	static std::unique_ptr<s_figure> fromJson_public(const rapidjson::Value& v);

	virtual std::string InfoForScreen()
	{
		std::stringstream ss;
		ss<<name()<<", ";
		ss<<"color="<<toString(m_color)<<", ";
		ss<<"edit date= "<<GetDate();
		return ss.str();
	}

	virtual bool point_inside(const size_t y, const size_t x)const
	{
		(void)x;
		(void)y;
		return false;
	}
	virtual bool point_at_border(const size_t y, const size_t x)const
	{
		(void)x;
		(void)y;
		return false;
	}

	static bool input_from_console(s_figure& f);
};

struct s_circle:public s_figure
{
	e_type GetType()const override
	{
		return e_type::s_circle;
	}
	std::string name()const override
	{
		return "circle";
	}

	s_point<uint32_t> m_center;
	uint32_t          m_r{0};

	rapidjson::Value& toJson(rapidjson::Value& v,rapidjson::Document::AllocatorType& a)const;
	bool fromJson(const rapidjson::Value& v);

	std::string InfoForScreen()override
	{
		std::stringstream ss;
		ss<<s_figure::InfoForScreen()<<", ";
		ss<<"("<<m_center.x<<","<<m_center.y<<")"<<", ";
		ss<<"(R="<<m_r<<")"<<", ";
		return ss.str();
	}

	bool point_inside(const size_t y, const size_t x)const override;
	bool point_at_border(const size_t y, const size_t x)const override;

	static std::unique_ptr<s_circle> input_from_console();
};

struct s_rectangle:public s_figure
{
	e_type GetType()const override
	{
		return e_type::s_rectangle;
	}
	std::string name()const override
	{
		return "rectangle";
	}

	s_point<float> m_lef_top;
	s_point<float> m_rig_bot;

	rapidjson::Value& toJson(rapidjson::Value& v,rapidjson::Document::AllocatorType& a)const;
	bool fromJson(const rapidjson::Value& v);

	std::string InfoForScreen()override
	{
		std::stringstream ss;
		ss<<s_figure::InfoForScreen()<<", ";
		ss<<"("<<m_lef_top.x<<","<<m_lef_top.y<<")"<<", ";
		ss<<"("<<m_rig_bot.x<<","<<m_rig_bot.y<<")"<<", ";
		return ss.str();
	}

	bool point_inside(const size_t y, const size_t x)const override;
	bool point_at_border(const size_t y, const size_t x)const override;

	static std::unique_ptr<s_rectangle> input_from_console();
};
