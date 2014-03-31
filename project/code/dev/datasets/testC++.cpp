#include<algorithm>
#include<cctype>
#include<cstdio>
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

#include</var/www/project/code/dev/datasets/headers/UTM.h>

#include</var/www/project/code/dev/datasets/headers/document.h>
#include</var/www/project/code/dev/datasets/headers/filestream.h>
#include</var/www/project/code/dev/datasets/headers/prettywriter.h>
#include</var/www/project/code/dev/datasets/headers/rapidjson.h>
#include</var/www/project/code/dev/datasets/headers/reader.h>
#include</var/www/project/code/dev/datasets/headers/stringbuffer.h>
#include</var/www/project/code/dev/datasets/headers/writer.h>

using namespace rapidjson;
using namespace std;

bool IsPointOnSegment( double x1, double y1, double x2, double y2, double x0, double y0);

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

void ConvertRoadSegmentsDataToUTM(double roadSegments[10000][4], int roadSegmentsSize, double roadSegmentsUTM[10000][4], char UTMZone[50]) {

	for(int i = 0; i < roadSegmentsSize; i++) {
	      UTM::LLtoUTM(roadSegments[i][0], roadSegments[i][1], roadSegmentsUTM[i][0], roadSegmentsUTM[i][1], UTMZone);
	      UTM::LLtoUTM(roadSegments[i][2], roadSegments[i][3], roadSegmentsUTM[i][2], roadSegmentsUTM[i][3], UTMZone);
	}
}

void ConvertToUTM(long double gpsLatLng[10000][2], int gpsLatLngSize, double gpsUTM[10000][2], char UTMZone[50]) {
	
	for(int i = 0; i < gpsLatLngSize; i++) {
		UTM::LLtoUTM(gpsLatLng[i][0], gpsLatLng[i][1], gpsUTM[i][0], gpsUTM[i][1], UTMZone);
	}
}

bool IsPointOnSegment(double start_x, double start_y, double end_x, double end_y,
		double point_x, double point_y) {
	double dx = end_x - start_x;
	double dy = end_y - start_y;
	double innerProduct = (point_x - start_x)*dx + (point_y - start_y)*dy;
	return 0 <= innerProduct && innerProduct <= dx*dx + dy*dy;
}

void MapMatchGPSPoints(double gpsUTM[10000][2], int gpsLatLngSize, double roadSegmentsUTM[10000][4], 
						int roadSegmentsSize, double projection[10000][3]) {
	long double min = 10000000;
	double x0, y0;
	double x1, y1;
	double x2, y2;
	double p;
	double q;

	double dist;
	double m;
	double b;

	for(int i = 0; i < gpsLatLngSize; i++) {

		p = gpsUTM[i][0];
		q = gpsUTM[i][1];

		for(int j = 0; j < roadSegmentsSize; j++) {

			x1 = roadSegmentsUTM[j][0];
			y1 = roadSegmentsUTM[j][1];

			x2 = roadSegmentsUTM[j][2];
			y2 = roadSegmentsUTM[j][3];

			m = (y2 - y1)/ (x2 - x1);
			b = (x2*y1 - x1*y2)/ (x2 - x1);

			x0 = (m*q + p - m*b)/ (m*m + 1);
			y0 = (m*m*q + m*p + b) / (m*m + 1);

			dist = abs(q - m*p - b) / sqrt(m*m + 1);

			bool isPointValid = IsPointOnSegment(x1, y1, x2, y2, x0, y0);

			if((dist < min) && isPointValid) {
				min = dist;
				projection[i][0] = j;
				projection[i][1] = x0;
				projection[i][2] = y0;
			}
		}
		min = 10000000;
	}
}

void DisplayOutputAsKML(double a[10000][2], int aSize, bool isInput) {
	
	cout << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	cout << "<kml xmlns=\"http://earth.google.com/kml/2.0\">\n";
	cout << "<Document>\n";
	cout << "\n";

	for(int i = 0; i < aSize; i++) {
		cout << "<Placemark>";
		cout << "<Style><IconStyle><Icon><href>";
		if(isInput) {
			cout << "http://maps.google.com/mapfiles/marker_white.png";
		} else {
			cout << "http://maps.google.com/mapfiles/marker_green.png";
		}
		cout << "</href></Icon></IconStyle></Style>";
		cout << "<name>" << "gps_" << a[i][1] << "," << a[i][0] << "</name><Point><coordinates>" << a[i][1] << "," << a[i][0] << "</coordinates></Point></Placemark>\n";
	}
	
	cout << "\n";
	cout << "</Document>\n";
	cout << "</kml>\n";
}

void DisplayOutputAsKML(long double a[10000][2], int aSize, bool isInput) {

	cout << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	cout << "<kml xmlns=\"http://earth.google.com/kml/2.0\">\n";
	cout << "<Document>\n";
	cout << "\n";

	for(int i = 0; i < aSize; i++) {
		cout << "<Placemark>";
		cout << "<Style><IconStyle><Icon><href>";
		if(isInput) {
			cout << "http://maps.google.com/mapfiles/marker_white.png";
		} else {
			cout << "http://maps.google.com/mapfiles/marker_green.png";
		}
		cout << "</href></Icon></IconStyle></Style>";
		cout << "<name>" << "gps_" << a[i][1] << "," << a[i][0] << "</name><Point><coordinates>" << a[i][1] << "," << a[i][0] << "</coordinates></Point></Placemark>\n";
	}
	
	cout << "\n";
	cout << "</Document>\n";
	cout << "</kml>\n";
}

void DisplayRoadSegmentsAsKML(double a[10000][4], int aSize) {

	cout << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	cout << "<kml xmlns=\"http://earth.google.com/kml/2.0\">\n";
	cout << "<Document>\n";
	cout << "\n";

	for(int i = 0; i < aSize; i++) {
		cout << "<Placemark>";
		cout << "<Style><IconStyle><Icon><href>";
		cout << "http://maps.google.com/mapfiles/marker_white.png";
		cout << "</href></Icon></IconStyle></Style>";
		cout << "<name>" << "gps_" << a[i][1] << "," << a[i][0] << "</name><Point><coordinates>" << a[i][1] << "," << a[i][0] << "</coordinates></Point></Placemark>\n";
		
		cout << "<Placemark>";
		cout << "<Style><IconStyle><Icon><href>";
		cout << "http://maps.google.com/mapfiles/marker_white.png";
		cout << "</href></Icon></IconStyle></Style>";
		cout << "<name>" << "gps_" << a[i][3] << "," << a[i][2] << "</name><Point><coordinates>" << a[i][3] << "," << a[i][2] << "</coordinates></Point></Placemark>\n";
		cout << "\n";
	}
	
	cout << "\n";
	cout << "</Document>\n";
	cout << "</kml>\n";
}

int ReadGPSDataIntoArray(long double gpsLatLng[10000][2], char filename[25]) {

	ifstream file;
	file.open(filename);

	if(!file) {
		cout << "Error: No input file: " << filename << "\n";
		return 1;
	}

	int col = 0;
	int i = 0;
	string line;
	const char *n;
	char **endPtr = NULL;

	for(i = 0; i < 7531; i++) {
		getline(file, line);
		std::vector<std::string> x = split(line, '\t');

		for(int j = 2; j < 4; j++) {
			n = x[j].c_str();
			gpsLatLng[i][col] = strtold(n, endPtr);
			col++;
		}
		col = 0;
	}
	file.close();
	return i;

}

int ReadRoadSegmentsDataIntoArray(double roadSegments[100000][4], char filename[25], string format) {

	int index = 0;
	if(format == "text") {
		ifstream file;
		file.open(filename);

		if(!file) {
			cout << "Error: No input file: " << filename << "\n";
			return 1;
		}

		char unwantedChars[] = "LINESTRING()";
		string line;
		string segment;
		
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

	} else if (format == "json") {
	
		std::ifstream ifs(filename);
		std:string json( (std::istreambuf_iterator<char>(ifs) ), (std::istreambuf_iterator<char>() ));

		rapidjson::Document jsonObj;
		jsonObj.Parse<0>(json.c_str());

		const Value& features = jsonObj["features"];

		for(SizeType p = 0; p < features.Size(); p++) {
			const Value& feature = features[p]["geometry"];
			const Value& nodes = feature["coordinates"];
		
			for(SizeType q = 0; q < nodes.Size()-1; q++) {
				const Value& nodeLngLatStart = nodes[q];
				const Value& nodeLngLatEnd = nodes[q+1];
				roadSegments[index][0] = nodeLngLatStart[1].GetDouble();
				roadSegments[index][1] = nodeLngLatStart[SizeType(0)].GetDouble();
				roadSegments[index][2] = nodeLngLatEnd[1].GetDouble();
				roadSegments[index][3] = nodeLngLatEnd[SizeType(0)].GetDouble();

				index++;
			}
		}	
	}
	return index;
}

void SetPrecisionLimits() {
	cout << std::setprecision(std::numeric_limits<long double>::digits10);
        cout << std::setprecision(std::numeric_limits<double>::digits10);
}


int main() {

	SetPrecisionLimits();

	long double gpsLatLng[10000][2];	
	char filename[25] = "gps_data.txt";
	int gpsLatLngSize = ReadGPSDataIntoArray(gpsLatLng, filename);


	double gpsUTM[10000][2];
	char UTMZone[50];
	ConvertToUTM(gpsLatLng, gpsLatLngSize, gpsUTM, UTMZone);


	double roadSegments[100000][4];
	string format = "json";
	char roadFilename[25] = "road.json";
	int roadSegmentsSize = ReadRoadSegmentsDataIntoArray(roadSegments, roadFilename, format);


	double roadSegmentsUTM[10000][4];
	ConvertRoadSegmentsDataToUTM(roadSegments, roadSegmentsSize, roadSegmentsUTM, UTMZone);


	double projection[100000][3];
	MapMatchGPSPoints(gpsUTM, gpsLatLngSize, roadSegmentsUTM, roadSegmentsSize, projection);


	double output[10000][2];
	for(int i = 0; i < gpsLatLngSize; i++) {
		UTM::UTMtoLL(projection[i][1], projection[i][2], UTMZone, output[i][0], output[i][1]);
	}
	
	DisplayOutputAsKML(output, gpsLatLngSize, false);
	
	return 0;
}
