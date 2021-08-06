#include "pch.h"
#include "CppUnitTest.h"
//#include"../TimeTable/course_define.hpp"
#include<format>
#include<iostream>
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
struct Box1 {
	bool bool_value;
	int int_value;
	const char* str;
};
// Box1的特化
template <typename _CharT>
struct std::formatter<Box1, _CharT> : std::formatter<bool, _CharT>
{	// 这里不一定要继承bool,其他内置类型也可以, 比如 : std::formatter<double, _CharT>
	template <typename _FormatContext>
	typename _FormatContext::iterator format(const Box1& v, _FormatContext& format_context)
	{	// 所有format均返回此类型，以下使用auto代替
		// 因为我们继承自std::formatter<bool, _CharT>，所以可直接调用父类的format
		typename _FormatContext::iterator Ite
			= std::formatter<bool, _CharT>::format(v.bool_value, format_context);
		Ite = '\n';
		// 而其他内置类型的需要构建临时对象来调用format
		Ite = std::formatter<int, _CharT>().format(v.int_value, format_context);
		Ite = '\n';
		Ite = std::formatter<const char*, _CharT>().format(v.str, format_context);
		return Ite;
	}
};
namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			/*course c { string("math"),uint32_t(32) };
			auto p = format("{}", c);
			std::cout << p;*/
			Box1 b = { true,12,"Dasd" };
			auto p = format("{}", b);
			cout << p;
		}
	};
}
