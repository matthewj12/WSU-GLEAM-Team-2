#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <vector>
#include <cmath>
#include <stdio.h>

using namespace std;
// TODO: put these in a file in such a way that the Arduio code can also read them (avoid redundancy).
const int pan_steps = 18;
const int tilt_steps = 2;

float degToRad(float p_deg) {
	return p_deg * 3.14159265 / 180;
}

struct Point3D {
	float x, y, z;

	Point3D(string p_text) {
		int first_comma_index = p_text.find(", ", 0);
		int second_comma_index = p_text.find(", ", first_comma_index + 2);
		float pan, tilt, dist;

		try {
			dist = stof(p_text.substr(0, first_comma_index));
			pan = stof(p_text.substr(first_comma_index + 2, second_comma_index - first_comma_index - 2));
			tilt = stof(p_text.substr(second_comma_index + 2));

			tilt = degToRad(tilt);
			pan = degToRad(pan);

			float xy_dist = cos(tilt) * dist; // xy component of dist

			x = cos(pan) * xy_dist;
			y = sin(pan) * xy_dist;
			z = sin(tilt) * xy_dist;
		}
		catch (const exception& e) {
			cerr << "Error: " << e.what() << endl;
		}
	}

	void printXYZ() {
		cout << x << ", " << y << ", " << z << endl;
	}
};

struct Triangle {
	Point3D a, b, c;
};

vector<Triangle> makeMeshFromPoints(vector<Point3D>& p_points) {
	vector<Triangle> to_return;

	for (int i = tilt_steps; i < p_points.size(); i++) {

		if (i % tilt_steps == 0) {
			to_return.push_back(Triangle{
				p_points[i-tilt_steps], 
				p_points[i], 
				p_points[i - tilt_steps + 1]
			});
		}
		else if ((i + 1) % tilt_steps == 0) {
			to_return.push_back(Triangle{
				p_points[i-tilt_steps], 
				p_points[i-1], 
				p_points[i]
			});
		}
		else {
			to_return.push_back(Triangle{
				p_points[i-tilt_steps], 
				p_points[i], 
				p_points[i-tilt_steps+1]
			});
			to_return.push_back(Triangle{
				p_points[i-tilt_steps], 
				p_points[i-1], 
				p_points[i]
			});
		}
	}

	return to_return;
}

vector<Point3D> loadScanData(string p_path) {
	vector<Point3D> points;

	ifstream file(p_path);
	file.clear();
	file.seekg(0, ios::beg);

	if (file.is_open()) {
		while (!file.eof()) {
			string cur_line;
			getline(file, cur_line);

			if (cur_line != "") {
				points.push_back(Point3D(cur_line));
			}
		}
	}
	else {
		cout << "Could not open file '" << p_path << "'" << endl;
	}

	file.close();

	return points;
}

void meshToSTL(vector<Triangle>& p_tris, string p_output_path) {
	//facet normal nx ny nz
	//	outer loop
	//		vertex v1x v1y v1z
	//		vertex v2x v2y v2z
	//		vertex v3x v3y v3z
	//	endloop
	//endfacet

	ofstream file(p_output_path);

	if (file.is_open()) {
		file << "solid <mesh>" << endl;

		for (Triangle t : p_tris) {
			/*Point3D norm, vec1, vec2;

			vec1.x = t.b.x - t.a.x;
			vec1.y = t.b.y - t.a.y;
			vec1.z = t.b.z - t.a.z;

			vec2.x = t.c.x - t.a.x;
			vec2.y = t.c.y - t.a.y;
			vec2.z = t.c.z - t.a.z;

			norm.x = vec1.y*vec2.z - vec1.z*vec2.y;
			norm.y = vec1.z*vec2.x - vec1.x*vec2.z;
			norm.z = vec1.x*vec2.y - vec1.y*vec2.x;

			float norm_len = sqrtf(norm.x * norm.x + norm.y * norm.y + norm.z * norm.z);
			norm.x /= norm_len;
			norm.y /= norm_len;
			norm.z /= norm_len;*/

			file << "facet normal 0 0 0" << endl;
			file << "\touter loop" << endl;
			file << "\t\t vertex " << t.a.x << " " << t.a.y << " " << t.a.z << endl;
			file << "\t\t vertex " << t.b.x << " " << t.b.y << " " << t.b.z << endl;
			file << "\t\t vertex " << t.c.x << " " << t.c.y << " " << t.c.z << endl;
			file << "\tendloop" << endl;
			file << "endfacet" << endl;
		}

		file << "endsolid <mesh>" << endl;
	}
	else {
		cout << "Could not create file '" << p_output_path << "'" << endl;
	}
}

int main() {
	vector<Point3D> points = loadScanData("ScanData.txt");

	vector<Triangle> tris = makeMeshFromPoints(points);

	meshToSTL(tris, "output.stl");
}