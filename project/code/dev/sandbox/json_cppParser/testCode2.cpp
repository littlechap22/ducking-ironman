#include<iostream>
#include "jsoncpp/json.h"

int main(){

	std::string input_json = "{\n \"mynestedvalue\" : {\n   \"value2\" : \"A string\" \n },\n \"myinteger\" : 42, \n \"mystring\" : \"John\" \n}\n";

	std::cout << input_json << std::endl;

	Json::Value values;
	Json::Reader reader;
	reader.parse( input_json, values);

	// second argument to get(a,b) is a default value that will be assigned in case of a parsing error
	std::string mystring_value = values.get("mystring","defaultstring").asString();
	std::cout << "Mystring: " << mystring_value << std::endl;
	int myinteger_value = values.get("myinteger", Json::Value(0) ).asInt();
	std::cout << "Myinteger: " << myinteger_value << std::endl; 

	Json::Value mynested_value = values.get("mynestedvalue", Json::Value(0) );
	std::string value2_string = mynested_value.get("value2","default").asString();

	std::cout << "value2: " << value2_string << std::endl; 

	return 0;
}
