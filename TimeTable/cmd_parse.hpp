#pragma once
#include"inc.hpp"
#include"type.hpp"
struct command_parser;
struct detail_command_task_type;
struct extend_detail_command_task_type;
using command_task_type = detail_command_task_type;
using extend_command_task_type = extend_detail_command_task_type;// function<int(const command_parser&cp,const vector<string>&, istream&, ostream&, ostream&)>;
using msc = map<string, command_task_type>;
using msec = map<string, extend_command_task_type>;
inline_reference_var_helper(msc, reg_commands);
inline_reference_var_helper(msec, reg_extend_commands);

struct detail_command_task_type :function<int(const vector<string>&, istream&, ostream&, ostream&)>
{
	using My_Base = function<int(const vector<string>&, istream&, ostream&, ostream&)>;
	string specification = "";
	detail_command_task_type() = default;
	detail_command_task_type(void*):My_Base(nullptr){}
	template<non_can_be_string_type _first , typename ...argTypes>
	detail_command_task_type( _first _f,argTypes...args) :My_Base(_f,args...), specification() {}
	template<typename ...argTypes>
	detail_command_task_type(string spe,argTypes...args) : My_Base(args...), specification(spe) {}
};
struct extend_detail_command_task_type :function<int(const command_parser& cp, const vector<string>&, istream&, ostream&, ostream&)>
{
	using My_Base = function<int(const command_parser& cp, const vector<string>&, istream&, ostream&, ostream&)>;
	string specification = "";
	extend_detail_command_task_type() = default;
	extend_detail_command_task_type(void*) :My_Base(nullptr) {}
	template<non_can_be_string_type _first, typename ...argTypes>
	extend_detail_command_task_type(_first _f, argTypes...args) : My_Base(_f, args...), specification() {}
	template<typename ...argTypes>
	extend_detail_command_task_type(string spe, argTypes...args) : My_Base(args...), specification(spe) {}
};
inline vector<string> getOneCommand(istream& in)
{
	vector<string >ret;
	bool inStart = true;
	bool inRef = false;
	while (1)
	{
		if (in.peek()==EOF)break;
		if (inStart) {
			//new param
		new_param:
			while (in.peek() == ' ' || in.peek() == '\t')
				in.get();
			if (!in.eof())
			{
				inStart = false;
				char c = in.get();
				if (c == '\n')
					return ret;
				inRef = c == '"';
				if (c != '"')
					ret.emplace_back(string(1, c));
				else
					ret.emplace_back();
			}
			else
				return ret;
		}
		else
		{

			char c = in.get();
			switch (c)
			{
			case '\n':
				if (!inRef)
					return ret;
				else
					ret.back() += c;
				break;
			case '"':
				if (ret.back().size() > 0 && ret.back().back() == '\\')
				{
					// \"  to  "
					ret.back().back() = c;
					break;
				}
				else
				{
					if (inRef)
					{
						goto new_param;
					}
					{
						ret.push_back(string());
						inRef = true;
					}
				}
				break;
			case ' ':
			case '\t':
				if (inRef)
					ret.back() += c;
				else
					goto new_param;
				break;
			//case EOF:
			//	return ret;
			case '\\':
			default:
				ret.back() += c;
			}

		}
	}
	return ret;
}
inline command_task_type find_task(msc& cms, const vector<string>&pms)
{
	if (pms.empty())return nullptr;
	for (auto sc : cms)
		if (sc.first == pms.front())
			return sc.second;
	//todo: more parse rule
	string curCmdName = pms.front();
	for(int i=1;i<pms.size();++i)
	{
		curCmdName += " "+pms[i];
		for (auto sc : cms)
			if (sc.first == curCmdName)
				return sc.second;
	}
	return nullptr;
}
struct command_parser
{
	//todo: if there need more attribute
	mutable msc cms;
	command_parser(msc& cms = reg_commands,const msec &ecms=reg_extend_commands) : cms(cms)
	{
		//Ìî³äÀ©Õ¹ÃüÁî
		for (auto& ecm : ecms)
			//this->cms.insert(pair<string, command_task_type>(ecm.first, (command_task_type)bind(ecm.second, (const command_parser&)(*this), placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4)));
			this->cms[ecm.first] = { ecm.second.specification,[this, &ecm](const vector<string>& vs, istream& ins, ostream& ous, ostream& errs) {return ecm.second(*this, vs, ins, ous, errs); } };
	}
	int operator()(const vector<string>& pms,istream& ins = cin, ostream& ous = cout, ostream& errs = cerr)const
	{
		auto task = find_task(cms, pms);
		if (task == nullptr) {
			errs << "not found command:" << pms[0];
			return -1;
		}
		return task(pms, ins, ous, errs);
	}
};