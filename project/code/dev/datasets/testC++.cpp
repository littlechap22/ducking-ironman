#include<algorithm>
#include<cctype>
#include<cstring>
#include<fstream>
#include<functional>
#include<iomanip>
#include<iostream>
#include<limits>
#include<locale>
#include<math.h>
#include<sstream>
#include<stdlib.h>
#include<string>
#include<vector>

#include</var/www/project/datasets/headers/UTM.h>
using namespace std;

//TODO: move to string library
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {

	std::stringstream ss(s);
	std::string item;
	while(std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

//trim from start
static inline std::string &ltrim(std::string &s) {
	
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

//trim from end
static inline std::string &rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

//trim from both sides
static inline std::string &trim(std::string &s) {
	return ltrim(rtrim(s));
}

int main() {
        cout << std::setprecision(std::numeric_limits<long double>::digits10);
        cout << std::setprecision(std::numeric_limits<double>::digits10);

	ifstream file;
	char gpsFilename[25] = "gps_data.txt";
	file.open(gpsFilename);

	if(!file) {
		cout << "Error: No input file: " << gpsFilename << "\n";
		return 1;
	}

	long double gpsLatLng[10000][2];
	double gpsUTM[10000][2];

	int col = 0;
	double lat;
	double lng;

	char UTMZone[50];

	const char *n;
	char **endPtr = NULL;
	string num;
	string line;

	int i = 0;
	
	///while(!infile.eof()) {
	for(i = 0; i < 7531; i++) {
		getline(file, line);
		std::vector<std::string> x = split(line, '\t');

		for(int j = 2; j < 4; j++) {
			n = x[j].c_str();
			gpsLatLng[i][col] = strtold(n, endPtr);
			col++;
		}
		UTM::LLtoUTM(gpsLatLng[i][0], gpsLatLng[i][1], gpsUTM[i][0], gpsUTM[i][1], UTMZone);
		col = 0;
	}
	int size_gpsUTM = i;

	file.close();


	char roadFilename[25] = "road_network.txt";
	file.open(roadFilename);

	if(!file) {
		cout << "Error: No input file: " << roadFilename << "\n";
		return 1;
	}

	char unwantedChars[] = "LINESTRING()";
	string segment;
	
	//TODO: create dynamic memory allocation
	double roadSegments[100000][4];
	int index = 0;

	//for(int i = 0; i < 158167; i++) {
	for(int i = 0; i < 10000; i++) {
		getline(file, line);
		std::vector<std::string> x = split(line, '\t');

		segment = x[6];
		for(int j = 0; j < strlen(unwantedChars); j++) {
			segment.erase(std::remove(segment.begin(), segment.end(), unwantedChars[j]), segment.end());
		}
		std::vector<std::string> smallerSegments = split(segment, ',');

		for(int j = 0; j < smallerSegments.size() - 1; j++) {
			std::vector<std::string> startLngLat = split(trim(smallerSegments[j]), ' ');
			std::vector<std::string> endLngLat = split(trim(smallerSegments[j+1]), ' ');

			roadSegments[index][0] = atof(startLngLat[1].c_str());
			roadSegments[index][1] = atof(startLngLat[0].c_str());
			roadSegments[index][2] = atof(endLngLat[1].c_str());
			roadSegments[index][3] = atof(endLngLat[0].c_str());
			index++;
		}
	}
	file.close();
	int size_roadSegments = index;

	//cout << size_roadSegments << "\n";

	double roadSegmentsUTM[10000][4];

	for(int i = 0; i < size_roadSegments && i < 10000; i++) {
	      //cout << roadSegments[i][0] << " " << roadSegments[i][1] << " " << roadSegments[i][2] << " " <<  roadSegments[i][3] << "\n";
	      UTM::LLtoUTM(roadSegments[i][0], roadSegments[i][1], roadSegmentsUTM[i][0], roadSegmentsUTM[i][1], UTMZone);

	      UTM::LLtoUTM(roadSegments[i][2], roadSegments[i][3], roadSegmentsUTM[i][2], roadSegmentsUTM[i][3], UTMZone);

	      //for(int k = 0; k < 4; k++) {
	      //        cout << roadSegments[i][k] << " ";
	      //}
	      //cout << "\n";

	      //for(int k = 0; k < 4; k++) {
	      //        cout << roadSegmentsUTM[i][k] << " ";
	      //}
	      //cout << "\n";
	}

	int size_roadSegmentsUTM = i;

	double x0, y0, x1, y1, x2, y2, p, q;

	double dist;
	double m;
	double b;

	double projection[100000][3];
	long int min = 10000000;

	for(int i = 0; i < size_gpsUTM; i++) {

		p = gpsUTM[i][0];
		q = gpsUTM[i][1];

		for(int j = 0; j < size_roadSegmentsUTM; j++) {

			x1 = roadSegmentsUTM[j][0];
			y1 = roadSegmentsUTM[j][1];

			x2 = roadSegmentsUTM[j][2];
			y2 = roadSegmentsUTM[j][3];

			m = (y2 - y1)/ (x2 - x1);
			b = (x2*y1 - x1*y2)/ (x2 - x1);

			x0 = (m*q + p - m*b)/ (m*m + 1);
			y0 = (m*m*q + m*p + b) / (m*m + 1);

			dist = abs(q - m*p - b) / sqrt(m*m + 1);


			if(dist < min) {
				min = dist;
				projection[i][0] = j;
				projection[i][1] = x0;
				projection[i][2] = y0;
			}
		}
		min = 10000000;
	}

	double output[10000][2];

	for(int i = 0; i < size_gpsUTM; i++) {
		
		//cout << projection[i][1] << " " << projection[i][2] << "\n";
		UTM::UTMtoLL(projection[i][1], projection[i][2], UTMZone, output[i][0], output[i][1]);
	}
	
	//cout << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	//cout << "<kml xmlns=\"http://earth.google.com/kml/2.0\">\n";
	//cout << "<Document>\n";
	//cout << "\n";

	//int oldIndex = -1;

	//for(int i = 0; i < size_gpsUTM; i++) {
	//	int index = projection[i][0];
	//	if(index != oldIndex) {
	//		cout << "<Placemark><name>" << "roadSegment" << rand() % 1000 << "</name><Point><coordinates>" << roadSegments[index][1] << "," << roadSegments[index][0] << "</coordinates></Point></Placemark>\n";
	//		cout << "<Placemark><name>" << "roadSegment" << rand() % 1000 << "</name><Point><coordinates>" << roadSegments[index][3] << "," << roadSegments[index][2] << "</coordinates></Point></Placemark>\n";
	//	}
	//	oldIndex = index;
	//}

	//cout << "\n";
	//cout << "</Document>\n";
	//cout << "</kml>\n";

	cout << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	cout << "<kml xmlns=\"http://earth.google.com/kml/2.0\">\n";
	cout << "<Document>\n";
	cout << "\n";

	for(int i = 0; i < size_gpsUTM; i++) {
		cout << "<Placemark><name>" << "gps_" << gpsLatLng[i][1] << "," << gpsLatLng[i][0]  << "</name><Point><coordinates>" << gpsLatLng[i][1] << "," << gpsLatLng[i][0] << "</coordinates></Point></Placemark>\n";
		//cout << "<Placemark><name>" << "gps" << rand() % 1000 << "</name><Point><coordinates>" << output[i][1] << "," << output[i][0] << "</coordinates></Point></Placemark>\n";
	}
	
	cout << "\n";
	cout << "</Document>\n";
	cout << "</kml>\n";
	
	//ofstream outputFile;
	//outputFile.open("gpsCorrected.kml");
	//std::cout.precision(10);	
	//for(int i = 0; i < size_gpsUTM; i++) {

	//	outputFile << "<Placemark><name>" << rand() % 1000 << "</name><Point><coordinates>" << gpsLatLng[i][0] << "," << gpsLatLng[i][1] << "</coordinates></Point></Placemark>\n";
	//	outputFile << "<Placemark><name>" << rand() % 1000 << "</name><Point><coordinates>" << output[i][0] << "," << output[i][1] << "</coordinates></Point></Placemark>\n";
	//}
	//outputFile.close();
	return 0;
}
