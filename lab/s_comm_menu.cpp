#include "s_comm_menu.h"

#include <iostream>
#include <iomanip>

using std::cin;
using std::cout;

bool s_comm_menu::run_menu(const s_comm_menu::td_list& comm_list)
{
	size_t index=0;
	for(auto& i:comm_list)
	{
		cout<<index<<i.text<<'\n';
		index++;
	}

	std::string s;
	if(!std::getline(cin,s))return true;
	if(s.empty() || s.find_first_not_of("0123456789")!=std::string::npos)
	{
		return true;
	}

	size_t i=std::strtoul(s.c_str(),0,10);
	if(i<comm_list.size())
	{
		const auto& comm=comm_list[i];
		if(comm.m_exit)
		{
			return false;
		}

		comm_list[i].func();
	}

	return true;
}
