#pragma once
#include "inc.hpp"
#include"type.hpp"
constexpr char spc = '"';
constexpr char splitChar = '\t';
#define dataFolder string("ser_data/")
inline string formlize(const string& s) {
	string ret;
	ret+= '"';
	for (auto c : s)
		if (c == spc)
			ret += spc, ret += spc;
		else
			ret += c;
	ret += spc;
	ret += splitChar;
	return ret;
}

inline string undoFormlize(stringstream &ss) {
	string ret;
	char c;
	ss >> ws >> c;
	if (!ss.good())return ret;
	_assert(c == '"');
	while (1) {
		if (ss.peek() != '"')
			ret += ss.get();
		else {
			ss.get();
			if (ss.peek() == '"')
				ret += ss.get();
			else
				return ret;
		}
	}
}

inline void create_directory(string path)
{
	//auto cmd = format(R"(rmdir /S /Q "{}" & mkdir "{}")", path, path);
	auto cmd = format(R"(if not exist "{}" mkdir "{}")", path, path);
	system(cmd.c_str());
}

inline void delete_directory(string path)
{
	auto cmd = format(R"(if exist "{}" rmdir /S /Q "{}")", path, path);
	system(cmd.c_str());
}

template<typename elemType>
void universal_insert(vector<elemType>& datas, vector<elemType>& to) {
	to.insert(to.end(), datas.begin(), datas.end());
}

template<typename T1, typename T2>
void universal_insert(vector< pair<remove_const_t<T1>, T2> >& datas, map<T1, T2>& to) {
	to.insert(datas.begin(), datas.end());
}
template<typename T1, typename T2>
void universal_insert(vector<pair<remove_const_t<T1>, T2>>& datas, multimap<T1, T2>& to) {
	to.insert(datas.begin(), datas.end());
}
template<typename T1, typename T2>
void universal_insert(vector< pair<add_const_t <T1>, T2> >& datas, map<T1, T2>& to) {
	to.insert(datas.begin(), datas.end());
}
template<typename T1, typename T2>
void universal_insert(vector<pair<add_const_t<T1>, T2>>& datas, multimap<T1, T2>& to) {
	to.insert(datas.begin(), datas.end());
}
////for simple typ
inline string  getFileContent(string fileName) {
	stringstream ret;
	const ifstream in(fileName);
	ret << in.rdbuf();
	auto p = ret.str();
	return p;
}
// boolalpha set ,with no test
template<typename type>
inline enable_if_t<!is_same_v<bool,type>> set_boolalpha_if_bool(stringstream& ss) {};
template<typename type>
inline enable_if_t<is_same_v<bool, type>> set_boolalpha_if_bool(stringstream& ss) { ss << boolalpha; };
template<typename type>
inline enable_if_t<!is_same_v<bool, type>> unset_boolalpha_if_bool(stringstream& ss) {};
template<typename type>
inline enable_if_t<is_same_v<bool, type>> unset_boolalpha_if_bool(stringstream& ss) { ss << noboolalpha; };


//denote the method to store and recover data
#define  chooseFile
#ifdef chooseFile
namespace dataManager {
	/*
	*  NOTICE
	* for a non-std::container object, should call the recover and store with stringstream 'ss'
	* and for others std::container object, call those funcs with string 'fileName'
	*
	*/

	/////////////
	// recover //
	/////////////
	//for simple data
	template<typename type>
	enable_if_t<is_built_in_type<type>, type >
		recover(stringstream& ss)
	{
		//for bool 
		set_boolalpha_if_bool<type>(ss);
		decay_t< type >ret;
		ss >> ret;
		unset_boolalpha_if_bool<type>(ss);
		return ret;

	};
	//for user define class ,but require recover implementation
	template<typename type>
	enable_if_t<is_same_v<type, decltype(type::recover(declval<stringstream&>()))>, type >
		recover(stringstream& ss)
	{
		return type::recover(ss);
	};
	/**********************************************************/
	/****************** for special type **********************/
	/**********************************************************/

	template<typename type>requires is_string_type<type>
	type recover(stringstream& ss) {
		return undoFormlize(ss);
	}
	template<typename type>requires is_pair_type<type>
	type recover(stringstream& ss) {
		//pair<_First, _Second> ret;
		decay_t<typename type::first_type> first = recover<decay_t< type::first_type>>(ss);//pair<int,int>::first_type
		decay_t<typename type::second_type> second = recover<type::second_type>(ss);
		return type({first, second});
	}
	//for contatiner, require range func begin() ....and value_type
	template<typename type> requires Container<type>
	 type	recover(stringstream &ss)
	{
		vector<typename type::value_type> tempVector;
		decay_t<type> ret;
		size_t sz;
		ss >> sz;
		tempVector.reserve(sz);
		for (int i = 0; i < sz; ++i)
			tempVector.push_back(recover<decay_t<typename type::value_type>>(ss));
		universal_insert(tempVector, ret);
		return ret;
	};
	/////////////
	//  store  //
	/////////////
	//for simple data
	template<typename type> requires is_built_in_type<type>
	stringstream&	store(stringstream& ss, const type &data) 
	{
		//for bool 
		set_boolalpha_if_bool<type>(ss);
		ss << data << '\t';
		unset_boolalpha_if_bool<type>(ss);
		return ss;
	};
	//for user define class,but require store implementation
	template<typename type>
	enable_if_t<is_same_v<stringstream,decay_t<decltype(type::store(declval<stringstream&>(),declval<type>()))>>, stringstream&>
		store(stringstream& ss, const type data)
	{
		return type::store(ss, data);
	};
	/**********************************************************/
	/****************** for special type **********************/
	/**********************************************************/
	//模板也有声明顺序的讲究，string须在pair前，pair须在container前
	//所以最好是声明和实现分开
	template<typename type>requires is_string_type<decay_t< type>>
	 stringstream & store(stringstream& ss, const type& data) {
		ss << formlize(data);
		return ss;
	}
	template<typename T> requires is_pair_type<T>
		stringstream& store(stringstream& ss, const T& data);
	//for contatiner, but require range func begin() and type value_type
	template<typename T>requires Container<T>
		stringstream& store(stringstream& ss, const T& con);
	/**********************************************************/
	/****************** store implementation ******************/
	/**********************************************************/
	template<typename T> requires is_pair_type<T>
		stringstream& store(stringstream& ss, const T& data) {
			store<T::first_type>(ss, data.first);
			store<T::second_type>(ss, data.second);
			return ss;
		}
	template<typename T>requires Container<T>
	stringstream&	store(stringstream &ss, const T& con) {
		using itType =typename T::value_type;
		//output content size
		ss << con.size() << endl;
		for (const itType &elem : con) {
			store<itType>(ss,elem);
		}
		return ss;
	}
};
#else

#endif


using data_task = function<void(void)>;
struct task_group {
	multimap<string,data_task>all_tasks;
	void do_all() {
		for (auto& tsk : all_tasks)
		{
			_assert(tsk.second != nullptr);
			tsk.second();
		}
	}
	void do_one(string name){
		auto p = all_tasks.find(name);
		while (p != all_tasks.end())
			p->second();
	}
};
extern task_group recover_group;
extern task_group store_group;
extern task_group clear_group;

//auto register data ,so that manage them would be easy
#define __auto_udf_register(type,name,reg,path) type name=(\
 reg##recover_group.all_tasks.insert(pair<string,data_task>(#name,[](){stringstream ss;ss<<getFileContent(path);name= dataManager:: recover<type>(ss);})),\
 reg##store_group.all_tasks.insert(pair<string,data_task>(#name,[](){stringstream ss; dataManager::store<type>(ss,name);ofstream out(path);out<<ss.str();})),\
 reg##clear_group.all_tasks.insert(pair<string,data_task>(#name,[](){name=type();})),\
 type());

#define auto_udf_register(type,name,reg) __auto_udf_register(type,name,reg,"data/"#name".ini")
#define auto_simple_global_register(type,name) __auto_udf_register(type,name,,"data/"#name".ini")