#pragma once
#include"inc.hpp"
struct command_parser;
using command_task_type = function<int(const vector<string>&, istream&, ostream&, ostream&)>;
using extend_command_task_type = function<int(const command_parser&cp,const vector<string>&, istream&, ostream&, ostream&)>;
using msc = map<string, command_task_type>;
using msec = map<string, extend_command_task_type>;
inline_reference_var_helper(msc, reg_commands);
inline_reference_var_helper(msec, reg_extend_commands);
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
	return nullptr;
}
struct command_parser
{
	//istream& ins;
	//ostream& ous, &errs;
	msc& cms;
	//command_parser(msc& cms = reg_commands,istream& ins=cin,ostream&ous=cout,ostream&errs=cerr) : cms(cms),ins(ins) ,ous(ous),errs(errs) {}
	command_parser(msc& cms = reg_commands) : cms(cms){}
	int operator()(const vector<string>& pms,istream& ins = cin, ostream& ous = cout, ostream& errs = cerr)const
	{
		auto task = find_task(cms, pms);
		if (task == nullptr)return -1;
		return task(pms, ins, ous, errs);
	}
};