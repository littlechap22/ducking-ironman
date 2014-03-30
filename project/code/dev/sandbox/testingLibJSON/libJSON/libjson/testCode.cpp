#include "libjson.h"   
#include <string>
#include<iostream>
using namespace std;

void ParseJSON( JSONNode  n){
	cout << "starting to parse" << endl;
	JSONNode::const_iterator i = n.begin();
	while (i != n.end()){
		// recursively call ourselves to dig deeper into the tree
		if (i -> type() == JSON_ARRAY || i -> type() == JSON_NODE){
			ParseJSON(*i);
		}
		// get the node name and value as a string
		std::string node_name = i -> name();
		cout << "name: " << i->name() << " value: " << i->as_string() <<"<br>"<<endl;
		++i;
	}
}
int main()
{
	std::string json = "{\"RootA\":\"Value in parent node\",\"ChildNode\":{\"ChildA\":\"String Value\",\"ChildB\":42}}";
	JSONNode node = libjson::parse(json);
	ParseJSON(node);
	return 0;
}
