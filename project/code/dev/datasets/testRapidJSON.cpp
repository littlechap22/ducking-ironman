#include "./rapidjson/document.h"
#include <cstdio>
#include <fstream>
#include <string>

#include<algorithm>
#include<cctype>
#include<cstring>
#include<functional>
#include<iomanip>
#include<iostream>
#include<limits>
#include<locale>
#include<math.h>
#include<sstream>
#include<stdlib.h>
#include<vector>

using namespace std;

int main() {
	//const char json[] = "{\"hello\" : \"world\" }";

	std::ifstream ifs("road.json");
	std:string content( (std::istreambuf_iterator<char>(ifs) ),
				(std::istreambuf_iterator<char>() ));


	rapidjson::Document d;
	//d.Parse<0>(json);
	d.Parse<0>(content.c_str());

	printf("%s\n", d["type"].GetString());

	return 0;
}
