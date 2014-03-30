#include "libjson.h"   
#include <string>
int main()
{
	std::string json = "{\"RootA\":\"Value in parent node\",\"ChildNode\":{\"ChildA\":\"String Value\",\"ChildB\":42}}";
	JSONNode n = libjson::parse(json);
	return 0;
}
