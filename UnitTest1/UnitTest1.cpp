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
// Box1���ػ�
template <typename _CharT>
struct std::formatter<Box1, _CharT> : std::formatter<bool, _CharT>
{	// ���ﲻһ��Ҫ�̳�bool,������������Ҳ����, ���� : std::formatter<double, _CharT>
	template <typename _FormatContext>
	typename _FormatContext::iterator format(const Box1& v, _FormatContext& format_context)
	{	// ����format�����ش����ͣ�����ʹ��auto����
		// ��Ϊ���Ǽ̳���std::formatter<bool, _CharT>�����Կ�ֱ�ӵ��ø����format
		typename _FormatContext::iterator Ite
			= std::formatter<bool, _CharT>::format(v.bool_value, format_context);
		Ite = '\n';
		// �������������͵���Ҫ������ʱ����������format
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
