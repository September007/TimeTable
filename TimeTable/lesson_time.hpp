//课程时间属性
#pragma once
#include"inc.hpp"
#include"serialize.hpp"
template<typename T>
struct inherit_from
{
	T data;
	template<typename ...argsT>
	inherit_from(argsT&&...args) :data(args...) {}
	operator T&() { return data; }
}; 

struct lesson_attribute:inherit_from<string>
{
	using My_Base = inherit_from<string>;  
	template<typename ...argsT>
	lesson_attribute(argsT&&...args) :My_Base(args...) {}

	static auto recover(stringstream&ss)
	{
		lesson_attribute ret;
		ret.data = dataManager::recover<string>(ss);
		return ret;
	}
	static auto store(stringstream &ss,const lesson_attribute&data)
	{
		return std::ref(dataManager::store(ss, data.data));
	}
};
using vl = vector<lesson_attribute>;
inline_reference_var_helper(vl, all_lesson_attributes);