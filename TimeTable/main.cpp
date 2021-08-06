#include"inc.hpp"
#include"course_define.hpp"

int main() {
#define chk(w) cout<<#w<<"\t"<<w<<endl;
	chk(formlize(R"(dsada)"));
	chk(formlize(R"(ds
ada)"));
	chk(formlize(R"(dsa"da)"));
	chk(formlize(R"(dsa
"
da)"));
	stringstream ss;
	ss << R"(
"dsadasd" "1234" "ds""das""dsa"""
"l1
l2
l3
l4"" 
l5")";
	string us;
	while (1) {
		us = undoFormlize(ss);
		if (us == "")break;
		cout << us << endl;
	}
	return 0;
}