#include <iostream>
#include <string>

#include "./jsoncpp-src-0.5.0/include/json/json.h"
//#include "jsoncpp/json.h"

int main()
{

	std::string example = "{\"array\":[\"item1\", \"item2\"], \"not an array\":\"asdf\"}";
	Json::Value value;
	Json::Reader reader;

	bool parsed = reader.parse(example, value, false);
	std::cout << parsed;
	return 0;
}
