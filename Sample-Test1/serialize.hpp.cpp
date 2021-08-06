#include "pch.h"
#include"../TimeTable/serialize.hpp"
#include"../TimeTable/course_define.hpp"
const string test_prefix_folder = "test/";
using  namespace dataManager;
TEST(serialize, formalize)
{
	EXPECT_EQ(formlize(R"()"), R"(""	)");
	EXPECT_EQ(formlize(R"(asdf)"), R"("asdf"	)");
	EXPECT_EQ(formlize(R"(aa""aa)"), R"("aa""""aa"	)");
	EXPECT_EQ(formlize(R"(a
b
c)"), R"("a
b
c"	)");
	EXPECT_EQ(formlize(R"(a
b"
c)"), R"("a
b""
c"	)");
}
TEST(serialize,undoFormalize){

	stringstream ss;
	ss << R"(
"dsadasd" "1234" "ds""das""dsa"""
"l1
l2
l3 
l4"" 
l5")";
	EXPECT_EQ(undoFormlize(ss), R"(dsadasd)");
	EXPECT_EQ(undoFormlize(ss), R"(1234)");
	EXPECT_EQ(undoFormlize(ss), R"(ds"das"dsa")");
	EXPECT_EQ(undoFormlize(ss), R"(l1
l2
l3 
l4" 
l5)");
}
TEST(serialize, is_simple_type) {
	using psi = pair < string, int>;
	EXPECT_EQ(is_simple_type<int>, true);
	EXPECT_EQ(is_simple_type<char>, true);
	EXPECT_EQ(is_simple_type<float>, true);
	EXPECT_EQ(is_simple_type<int*>, true);
	EXPECT_EQ(is_simple_type<string>, false);
	EXPECT_EQ(is_simple_type<vector<int>>, false);
	EXPECT_EQ(is_simple_type<psi>, false);
	EXPECT_EQ(is_simple_type<int[]>, false);
	EXPECT_EQ(is_simple_type<string[]>, false);
}
TEST(serialize, universal_insert) {
	vector<int> expVi = { 1,2,3,4 }, tVi;
	universal_insert(expVi, tVi);
	EXPECT_EQ(tVi, expVi);
	using pic = pair<int, char>;
	map<int, char> expMic={ {1,'a'},{2,'b'},{3,'c'} }, tMic;
	universal_insert(vector<pic>({ {1,'a'},{2,'b'},{3,'c'} }), tMic);
	EXPECT_EQ(tMic, expMic);
}
TEST(serialize, getFileContent) {
	system("rmdir /S /Q test & mkdir test");
	string con = R"(dadsad
132425364!#$@%$#^$%&#^$=46897/79973
"D:M<>?ADSFVCL::""AX:Asdlq})";
	ofstream out(test_prefix_folder + "tempFile");
	out << con;
	out.close();
	auto & tempStr = getFileContent(test_prefix_folder + "tempFile");
	EXPECT_EQ(con, tempStr);
}
TEST(serilize, recover_store_simple) {
	stringstream ss;
	//ss << R"(123 12 1.2 a 12345678900 true false)";
	store(ss, 123);
	store(ss, 12);
	store(ss, 1.2);
	store(ss, 'a');
	store(ss, 12345678900);
	store(ss, true);
	store(ss, false);
	EXPECT_EQ(123, dataManager::recover<int>(ss));
	//unit8_t is unsigned char,if pass uint8_t,answer should be "1"
	EXPECT_EQ(12, dataManager::recover<int>(ss));
	EXPECT_FLOAT_EQ(1.2, dataManager::recover<float>(ss));
	EXPECT_EQ('a', dataManager::recover<char>(ss));
	EXPECT_EQ(12345678900, dataManager::recover<long long>(ss));
	//bool recover require boolalpha manipulate
	//PS. 
	//msvc is strange,for "12asddaf",noboolalpha read to bool +string,gets wrong
	//for "1adsada",noboolalpha read to bool +string,gets right
	//i think,read activation take int to transe to bool,but for non"0"|"1" ,the bool gets right,but the stream set fail very time
	//any way, set boolalpha to recover and store  may be more safe
	EXPECT_EQ(true, dataManager::recover<bool>(ss));
	EXPECT_EQ(false, dataManager::recover<bool>(ss));
	store(ss, 123);
	const int ci = recover<const int>(ss);
}
TEST(serialize, recover_store_user_define_class) {
	course exp[] = { {"ababa",123},{R"(ab"
"
aba)",123} }, tCv[2];
	stringstream ss;
	for (int i = 0; i < 2; ++i)
		course::store(ss, exp[i]);
	for (int i = 0; i < 2; ++i)
		tCv[i] = course::recover(ss);
	for (int i = 0; i < 2; ++i)
		EXPECT_EQ(exp[i], tCv[i]);
}
TEST(serialize, recover_store_container) {
	vector<course> vexp = { {"ababa",123},{R"(ab"
"
aba)",123} ,
		{R"("""""""""d""""""""")",1234567890} }, tVv;
	map<string, int> mexp = { {"ababa",123},{R"(ab"
"
aba)",123} ,
		{R"("""""""""d""""""""")",1234567890} }, tMv;
	stringstream ss; 
	//store<vector, course>(ss, vexp);
	//tVv= recover<vector, course>(ss);
	//EXPECT_EQ(vexp, tVv);
	using itType = decltype(mexp)::value_type;

	//store(ss, itType("aba", 12321));
	//auto p = recover<pair,string,int>(ss);
	//int i = 0;

	//store<map, string, int>(ss, mexp);
	//auto tempV = recover<vector, pair<string, int>>(ss);
	//tMv.insert(tempV.begin(), tempV.end());
	////decltype(tMv)::value_type;
	//EXPECT_EQ(mexp, tMv);
}