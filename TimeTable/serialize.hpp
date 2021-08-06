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
template<class Type>
constexpr bool is_simple_type = !is_class_v<Type> && !is_array_v<Type>;
template<typename elemType>
void universal_insert(vector<elemType>& datas, vector<elemType>& to) {
	to.insert(to.end(), datas.begin(), datas.end());
}
template<typename T1,typename T2>
void universal_insert(vector<pair<T1,T2>>& datas, map<T1,T2>& to) {
	to.insert( datas.begin(), datas.end());
}
////for simple type
//template<typename Type>
//enable_if_t<!is_class_v<Type>&&!is_array_v<Type>,Type>
//	universal_recover(stringstream& ss) {
//	Type ret;
//	ss >> ret;
//	return ret;
//}
////for classes had recover() implementation
//template<typename Type>
//enable_if_t<is_same_v<Type,decltype(Type::recover(declval<stringstream&>()))>,
//	Type>
//	universal_recover(stringstream& ss) {
//	return Type::recover(ss);
//}
inline string  getFileContent(string fileName) {
	stringstream ret;
	ifstream in(fileName);
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

//for pair
//template<typename Type>

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
	//template<typename type>
	//type recover(stringstream&) {};
	//for simple data
	template<typename type>
	enable_if_t<is_simple_type<type>, type >
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
	enable_if_t<is_same_v<type, decltype(type::recover(declval<stringstream>()))>, type >
		recover(stringstream& ss)
	{
		return type::recover(ss);
	};
	//for contatiner, require range func begin() ....and value_type
	template<template<typename _First,typename ..._Rest>typename cont,typename _First,typename ..._Rest>
	enable_if_t<is_class_v<decltype(declval<cont<_First, _Rest ...>>().begin())>
		&&!is_same_v<string,cont<_First,_Rest...>>,vector<typename cont<_First, _Rest ...>::value_type>>
		recover(stringstream &ss)
	{
		vector<typename cont<_First, _Rest ...>::value_type> ret;
		size_t sz;
		ss >> sz;
		ret.reserve(sz);
		for (int i = 0; i < sz; ++i)
			ret.push_back(recover<decay_t<typename cont<_First, _Rest ...>::value_type>>(ss));
		return ret;
	};
	/**********************************************************/
	/****************** for special type **********************/
	/**********************************************************/
	//template<template<typename _First,typename ..._Rest>typename cont, typename _First,typename _Second>
	//enable_if_t<is_same_v<cont<_First,_Second>,pair<_First,_Second>>, cont<_First, _Second>>recover<cont<_First,_Second>>
	//template<typename _First,typename _Second>
	//pair<_First,_Second> recover<pair<_First,_Second>>
	//	(stringstream& ss) {
	//	//pair<_First, _Second> ret;
	//	decay_t<_First> first = recover<_First>(ss);
	//	decay_t<_Second> second = recover<_Second>(ss);
	//	return pair<_First, _Second>({first, second});
	//}

	template<typename type>
	enable_if_t<is_same_v<decay_t<type>,string>, type>recover(stringstream& ss) {
		return undoFormlize(ss);
	}
	/////////////
	//  store  //
	/////////////
	//template<typename type>
	//stringstream& store(stringstream& ss, type data) { static_assert(false); };

	//for simple data
	template<typename type>
	enable_if_t<is_simple_type<type>, stringstream&>
		store(stringstream& ss, const type &data) 
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
	//for contatiner, but require range func begin() and type value_type
	template<template < typename _First,typename..._Rest>typename cont, typename _First,typename ..._Rest >
	enable_if_t<is_class_v<decltype(declval<cont<_First,_Rest...>>().begin())>
		&& !is_same_v<string, cont<_First, _Rest...>>,stringstream&>
		store(stringstream &ss, const cont<_First, _Rest...>& con) {
		using itType =typename cont<_First, _Rest...>::value_type;
		//output content size
		ss << con.size() << endl;
		for (auto& elem : con) {
			store(ss,elem);
		}
		return ss;
	}
	/**********************************************************/
	/****************** for special type **********************/
	/**********************************************************/
	template<typename _First,typename _Second>
	stringstream& store(stringstream& ss, const pair<_First, _Second>& data) {
		store(ss, data.first);
		store(ss, data.second);
		return ss;
	}
	template<typename type>
	enable_if_t<is_same_v<string, decay_t<type>>, stringstream &>store(stringstream& ss, const type& data) {
		ss << formlize(data);
		return ss;
	}
};
#else

#endif