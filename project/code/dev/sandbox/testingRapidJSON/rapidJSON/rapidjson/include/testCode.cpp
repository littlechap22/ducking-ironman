#include "rapidjson/document.h"
#include <cstdio>

int main() {
	const char json[] = "{\"hello\" : \"world\" }";

	rapidjson::Document d;
	d.Parse<0>(json);

	printf("%s\n", d["hello"].GetString());

	return 0;
}
