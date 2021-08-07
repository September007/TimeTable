#include "pch.h"
#include"../TimeTable/serialize.hpp"
#include"../TimeTable/course_define.hpp"
#include"../TimeTable/serialize.cpp"
#include<format>
const string test_prefix_folder = "test/";
using  namespace dataManager;

using psi = pair<string, int>;
using vs = vector<string>;
using msi = map<string, int>;
using mmsi = multimap<string, int>;
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
TEST(serialize, is_built_in_type) {
	using psi = pair < string, int>;
	EXPECT_EQ(is_built_in_type<int>, true);
	EXPECT_EQ(is_built_in_type<char>, true);
	EXPECT_EQ(is_built_in_type<float>, true);
	EXPECT_EQ(is_built_in_type<int*>, true);
	EXPECT_EQ(is_built_in_type<string>, false);
	EXPECT_EQ(is_built_in_type<vector<int>>, false);
	EXPECT_EQ(is_built_in_type<psi>, false);
	EXPECT_EQ(is_built_in_type<int[]>, false);
	EXPECT_EQ(is_built_in_type<string[]>, false);
}
TEST(serialize, universal_insert) {
	vector<int> expVi = { 1,2,3,4 }, tVi;
	universal_insert(expVi, tVi);
	EXPECT_EQ(tVi, expVi);
	using pic = pair<int, char>;
	map<int, char> expMic={ {1,'a'},{2,'b'},{3,'c'} }, tMic;
	auto temp = vector<pic>({ {1,'a'},{2,'b'},{3,'c'} });
	universal_insert(temp, tMic);
	EXPECT_EQ(tMic, expMic);
	mmsi expmmsi = { {"aa",1},{"bb",2},{"cc",0},{"cc",4} }, tmmsi;
	auto tempmmsi = vector<pair<string, int>>{ {"aa",1},{"bb",2},{"cc",0},{"cc",4} };
	universal_insert(tempmmsi, tmmsi);
	EXPECT_EQ(expmmsi, tmmsi);
}
TEST(serialize, getFileContent) {
	create_directory("test");
	string con = R"(dadsad
132425364!#$@%$#^$%&#^$=46897/79973
"D:M<>?ADSFVCL::""AX:Asdlq})";
	ofstream out(test_prefix_folder + "tempFile");
	out << con;
	out.close();
	auto  tempStr = getFileContent(test_prefix_folder + "tempFile");
	EXPECT_EQ(con, tempStr);
	delete_directory("test");
}
TEST(serialize, recover_store_simple) {
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
		tCv[i] = recover<course>(ss);
	for (int i = 0; i < 2; ++i)
		EXPECT_EQ(exp[i], tCv[i]);
}

TEST(serialize, recover_store_special_type) {
	using pic = pair<int, char>;
	pic exp = make_pair(1, 'a'), t;
	stringstream ss;
	store(ss,exp);
	t = recover<pic>(ss);
	EXPECT_EQ(exp, t);

	using pcii = pair<const int, int>;
	pcii exp2{ 1,2 };
	store(ss, exp2);
	auto t2 = recover<pcii>(ss);
	EXPECT_EQ(exp2, t2);
}
TEST(serialize, recover_store_container) {
	vector<course> vexp = { {"ababa",123},{R"(ab"
"
aba)",123} ,
		{R"("""""""""d""""""""")",1234567890} }, tVv;
	stringstream ss; 
	store<vector<course>>(ss, vexp);
	//is_pair_type < vector<course>>;
	tVv= recover<vector<course>>(ss);
	EXPECT_EQ(vexp, tVv);
	
	 map<string, int> mexp = { {"ababa",123},{R"(ab"
"
aba)",123} ,
		{R"("""""""""d""""""""")",1234567890} }, tMv;
	store(ss, mexp);
	tMv= recover<map<string, int>>(ss);
	EXPECT_EQ(mexp, tMv);
	
	vector<string> expvs = { "ababa","dadas","DAdasa" }, tvs;
	store(ss, expvs);
	tvs = recover<vector<string>>(ss);
	EXPECT_EQ(expvs, tvs);
}

 //store(ss,exp##test_agr_##type); exp##test_agr_##type=recover<type>(ss); 
#define agr(type)  __auto_udf_register(type,test_agr_##type,,"data/test/"#type".ini")// auto_simple_global_register(type,test_agr_##type)
#define assign_and_set_expect(type) type exp##test_agr_##type=test_agr_##type 
#define check_eq(type) EXPECT_EQ(test_agr_##type,exp##test_agr_##type)

agr(int);
agr(char	);
agr(float	);
agr(string	);
agr(psi		);
agr(vs		);
agr(msi		);
agr(mmsi	);

TEST(serialize, __auto_udf_register) {
	create_directory("data/test");
	stringstream ss;
	assign_and_set_expect(int		) = 1;
	assign_and_set_expect(char		) = 'a';
	assign_and_set_expect(float		) = 1.234;
	assign_and_set_expect(string	) = "balabala";
	assign_and_set_expect(psi		) = pair<string,int>("wulala",123);
	assign_and_set_expect(vs		) = {"wulala01","wulala02","wulala03"};
	assign_and_set_expect(msi,		) = { {"aa",1},{"bb",2},{"cc",0} };
	assign_and_set_expect(mmsi,		) = { {"aa",1},{"bb",2},{"cc",0},{"cc",4} };
	store_group.do_all();
	clear_group.do_all();
	recover_group.do_all();
	check_eq(int);
	check_eq(char);
	check_eq(float);
	check_eq(string);
	check_eq(psi);
	check_eq(vs);
	check_eq(msi);
	check_eq(mmsi);

	delete_directory("data/test");
}