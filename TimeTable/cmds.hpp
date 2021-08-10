/*
 * 几个简单的命令
 */
#pragma once
#include"inc.hpp"
#include"cmd_parse.hpp"
#include<unordered_map>
inline string undoParas(const vector<string> vs, int begin = 0)
{
	string ret;
	if (begin >= vs.size())return ret;
	while (begin < vs.size())
	{
		ret += ' "';
		for (auto c : vs[begin])
			if (c == '"')
				ret += "\\\"";
			else
				ret += c;
		ret += '"';
		begin++;
	}
	return ret;
}
inline unordered_map<string, bool> getSpecialCmdParas(const vector<string>& paras)
{
	unordered_map<string, bool >ret;
	for (auto p : paras)
		if (!p.empty() && (p[0] == '-' || p[0] == '/'))
			ret[p] = true;
	return ret;
}
#define paras const vector<string>& vs,istream&ins,ostream&ous,ostream&errs

enum  retIndicates { shell_exit = 1000, shell_direct_leave, any_success, any_fail };
inline int sys(const vector<string>& vs, istream& ins, ostream& ous, ostream& errs)
{
	if (vs.size() < 2)return 0;
	string syscmd = vs[1];
	auto restPms = undoParas(vs, 2);
	syscmd += " " + restPms;
	return system(syscmd.c_str());
}
inline int echo(const vector<string>& vs, istream& ins, ostream& ous, ostream& errs)
{
	static bool echoOn = true;
	bool outputEndEnter = true;
	if (vs.size() < 2)return 0;
	auto sps = getSpecialCmdParas(vs);
	if (sps["-n"] || sps["/n"])outputEndEnter = false;
	if (sps["-off"])echoOn = false;
	if (sps["-on"])echoOn = true;

	if (echoOn) {
		for (int i = 1; i < vs.size(); ++i)
		{
			if (!sps[vs[i]])
			{
				ous << vs[i];
				if (i != vs.size() - 1)
					cout << ' ';
			}
		}
		if (outputEndEnter)
			ous << endl;
	}
	return 0;
}
inline int stupid_shell(const vector<string>& vs, istream& ins, ostream& ous, ostream& errs)
{
	command_parser cp;
	int ret;
	cp({ "echo", "\nTimeTable>","-n" });
	while (1)
	{
		if (ins.peek() == EOF || ins.eof())break;
		//todo ：more specific specification
		auto pms = getOneCommand(ins);
		if (pms.size() > 0)
			ret = cp(pms);
		switch (ret)
		{
		case retIndicates::shell_direct_leave:
			return shell_direct_leave;
		case retIndicates::shell_exit:
			return 0;
		}
		cp({ "echo", "\nTimeTable> ","-n" });
	}
	return 0;
}
inline vector<pair<const string, command_task_type>> noneNameCmds = {
make_pair<string,command_task_type>("exit",[](const vector<string>& vs,istream& ins,ostream& ous,ostream& errs)
{
	return int(retIndicates::shell_exit);
}),
make_pair<string,command_task_type>("dexit",[](const vector<string>& vs,istream& ins,ostream& ous,ostream& errs)
{
	return int(retIndicates::shell_direct_leave);
})
};

inline void default_initial_cmds(map<string, command_task_type>& ms)
{
	ms.insert(noneNameCmds.begin(), noneNameCmds.end());
	ms["sys"] = sys;
	ms["echo"] = echo;
}