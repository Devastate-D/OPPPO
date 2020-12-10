#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <functional>

struct s_comm_menu
{
	std::function<void(void)> func{};
	std::string text;
	bool m_exit{};

	using td_list=std::vector<s_comm_menu>;

	//если вернёт false, это выход
	static bool run_menu(const td_list& comm_list);
};

