#include "main.h"
#include "CFiguresCollection.h"
#include "s_comm_menu.h"
#include "rapidjson/document.h"
//#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/istreamwrapper.h"
#include <string>
#include <iostream>
#include <iomanip>
#include <functional>
#include <vector>

using std::cin;
using std::cout;

//1 - нужно будет вводить с клавиатуры вручную
//0 - используются заранее проставленные значения
#define USE_HAND_INPUT 0

inline static const std::string file_name="figures.txt";

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;


	CFiguresCollection FiguresCollection;

	auto fill_default_figs=[&]
	{
		{
			auto f=std::make_unique<s_circle>();
			f->m_color=s_figure::e_color::green;
			f->m_center={25,10};
			f->m_r=7;
			f->SetDateAsCurrent();

			FiguresCollection.append_figure(std::move(f));
		}

		{
			auto f=std::make_unique<s_circle>();
			f->m_color=s_figure::e_color::red;
			f->m_center={45,8};
			f->m_r=9;
			//f->SetDateAsCurrent();
			FiguresCollection.append_figure(std::move(f));
		}

		{
			auto f=std::make_unique<s_rectangle>();
			f->m_color=s_figure::e_color::yellow;
			f->m_lef_top={10,5};
			f->m_rig_bot={27,20};
			f->SetDateAsCurrent();
			FiguresCollection.append_figure(std::move(f));
		}
	};

	auto menu_show_figures_list=[&]
	{
		cout<<'\n';
		cout<<"figures list:"<<'\n';
		size_t index=0;
		for(const auto& i:FiguresCollection.m_figures)
		{
			cout<<"  "<<index<<": "<<i->InfoForScreen();
			cout<<'\n';
			index++;
		}
		cout<<'\n';
	};

	auto menu_load_file=[&]
	{
		cout<<'\n';

		std::ifstream file(file_name);
		if(!file.is_open())
		{
			cout<<"  can't open file \'"<<file_name<<"\'"<<'\n';
			return;
		}

		rapidjson::Document doc;
		rapidjson::IStreamWrapper sw(file);
		if(doc.ParseStream(sw).HasParseError())
		{
			cout<<"  can't load from file \'"<<file_name<<"\'"<<'\n';
			return;
		}

		if(!FiguresCollection.fromJson(doc))
		{
			cout<<"  can't load from file \'"<<file_name<<"\'"<<'\n';
			return;
		}
		cout<<"loaded from file \'"<<file_name<<"\'"<<'\n';

		cout<<'\n';
	};

	auto menu_save_file=[&]
	{
		cout<<'\n';

		rapidjson::Document doc;
		FiguresCollection.toJson(doc,doc.GetAllocator());

		rapidjson::StringBuffer sb;
		//rapidjson::Writer<rapidjson::StringBuffer> writer(sb);//без форматирования
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);//с форматированием
		if(!doc.Accept(writer))
		{
			cout<<"  false call doc.Accept file \'"<<file_name<<"\'"<<'\n';
			return;
		}
		auto s=sb.GetString();

		std::ofstream file(file_name);
		if(!file.is_open())
		{
			cout<<"  can't open file \'"<<file_name<<"\'"<<'\n';
			return;
		}

		if(!(file<<s))
		{
			cout<<"  can't save to file \'"<<file_name<<"\'"<<'\n';
			return;
		}


		cout<<"saved to file \'"<<file_name<<"\'"<<'\n';
		cout<<'\n';
	};

	auto menu_edit_figures_list=[&]
	{
		auto to_add=[&]
		{
			auto menu_add_circle=[&]
			{
				cout<<"------------------------"<<'\n';
				if(auto f=s_circle::input_from_console())
				{
					f->m_edit_date=std::time(0);
					FiguresCollection.m_figures.emplace_back(std::move(f));
				}
				cout<<'\n';
			};

			auto menu_add_rectangle=[&]
			{
				cout<<"------------------------"<<'\n';
				if(auto f=s_rectangle::input_from_console())
				{
					f->m_edit_date=std::time(0);
					FiguresCollection.m_figures.emplace_back(std::move(f));
				}
				cout<<'\n';
			};

			while(1)
			{
				cout<<"------------------------"<<'\n';
				cout<<"Choose figure to add:"<<'\n';
				if(!s_comm_menu::run_menu({
					{nullptr  ," - exit",1},
					{menu_show_figures_list," - show figures list ("+std::to_string(FiguresCollection.m_figures.size())+")"},
					{menu_add_circle   ," - circle "  },
					{menu_add_rectangle," - rectangle"},
				}))break;
			}
		};

		auto to_delete=[&]
		{
			auto& arr=FiguresCollection.m_figures;
			if(!arr.size())return;

			cout<<"------------------------"<<'\n';
			cout<<"Enter index to delete:"<<'\n';
			size_t index{}; cin>>index;
			std::getchar();//чистка от '\n'
			if(index>=arr.size())
			{
				cout<<"index must be 0..."<<arr.size()-1<<":"<<'\n';
				return;
			}

			arr.erase(arr.begin()+index);
			cout<<"figure with index "<<index<<" deleted"<<'\n';
		};

		cout<<"menu_add_figure"<<'\n';
		while(1)
		{
			cout<<"------------------------"<<'\n';
			cout<<"Choose action:"<<'\n';
			if(!s_comm_menu::run_menu({
				{nullptr  ," - exit",1},
				{menu_show_figures_list," - show figures list ("+std::to_string(FiguresCollection.m_figures.size())+")"},
				{to_add   ," - to add "       },
				{to_delete," - to delete"     },
			}))break;
		}
	};

	auto menu_delete_all_figs=[&]
	{
		FiguresCollection.m_figures.clear();
		cout<<'\n';
		cout<<"all figures have been deleted"<<'\n';
		cout<<'\n';
	};

	auto menu_draw_scene=[&]
	{
		cout<<'\n';
		FiguresCollection.RunConsolePlotter();
		cout<<'\n';
	};

	auto menu_main=[&]
	{
		while(1)
		{
			cout<<"------------------------"<<'\n';
			cout<<"Choose action:"<<'\n';
			if(!s_comm_menu::run_menu({
				{nullptr               ," - exit",1},
				{menu_show_figures_list," - show figures list ("+std::to_string(FiguresCollection.m_figures.size())+")"},
				{menu_draw_scene       ," - draw scene"               },
				{menu_load_file        ," - load file"                },
				{menu_save_file        ," - save file"                },
				{menu_edit_figures_list," - edit figures list"        },
				{menu_delete_all_figs  ," - delele all figures"       },
				{fill_default_figs     ," - fill default figures list"},
			}))break;
		}
	};

	//-------------------------
	fill_default_figs();
	menu_main();

	cout<<"done!"<<'\n';
	return 0;
}
