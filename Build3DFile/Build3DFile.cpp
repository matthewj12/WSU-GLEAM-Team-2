#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <vector>
#include <cmath>
#include <stdio.h>

using namespace std;
// TODO: put these in a file in such a way that the Arduio code can also read them (avoid redundancy).
const int pan_steps = 10;
const int tilt_steps = 10;

float degToRad(float p_deg) {
	return p_deg * 3.14159265 / 180;
}

struct vector3f {
	float x, y, z;

	vector3f(string p_text) {
		int first_comma_index = p_text.find(", ", 0);
		int second_comma_index = p_text.find(", ", first_comma_index + 2);
		float pan, tilt, dist;

		try {
			// Each line is:
			// distance pan_angle tilt_angle
			dist = stof(p_text.substr(0, first_comma_index));
			pan = stof(p_text.substr(first_comma_index+2, second_comma_index-first_comma_index-2));
			tilt = stof(p_text.substr(second_comma_index+2));

			tilt = degToRad(tilt);
			pan = degToRad(pan);

			// Math magic
			x = dist * cos(tilt) * cos(pan);
			y = dist * cos(tilt) * sin(pan);
			z = dist * sin(tilt);
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
	vector3f a, b, c;
};

vector<vector3f> loadScanData(string p_path) {
	vector<vector3f> points;

	ifstream file(p_path);
	file.clear();
	file.seekg(0, ios::beg);

	if (file.is_open()) {
		while (!file.eof()) {
			string cur_line;
			getline(file, cur_line);

			if (cur_line != "") {
				points.push_back(vector3f(cur_line));
			}
		}
	}
	else {
		cout << "Could not open file '" << p_path << "'" << endl;
	}

	file.close();

	return points;
}

vector<Triangle> makeMeshFromPoints(vector<vector3f>& p_points) {
	vector<Triangle> to_return;

	for (int i = tilt_steps; i < p_points.size(); i++) {
		// First triangle in tilt-column
		if (i % tilt_steps == 0) {
			to_return.push_back(Triangle{
				p_points[i-tilt_steps], 
				p_points[i], 
				p_points[i-tilt_steps+1]
			});
		}
		// Last triangle in tilt-column
		else if ((i + 1) % tilt_steps == 0) {
			to_return.push_back(Triangle{
				p_points[i-tilt_steps], 
				p_points[i-1], 
				p_points[i]
			});
		}
		// In-between triangles in tilt-column
		else {
			to_return.push_back(Triangle{
				p_points[i-tilt_steps], 
				p_points[i-1], 
				p_points[i]
			});
			to_return.push_back(Triangle{
				p_points[i-tilt_steps], 
				p_points[i], 
				p_points[i-tilt_steps+1]
			});
		}
	}

	return to_return;
}

void meshToSTL(vector<Triangle>& p_tris, string p_output_path) {
	/*
	
	facet normal nx ny nz
		outer loop
			vertex v1x v1y v1z
			vertex v2x v2y v2z
			vertex v3x v3y v3z
		endloop
	endfacet
	
	3D Modeling/Viewing programs automatically fill-in the data for the normal 
	(nx, ny, nz), which signifies the side from which the triangle is visible
	en.wikipedia.org/wiki/Normal_(geometry)
	
	*/

	ofstream file(p_output_path);

	if (file.is_open()) {
		file << "solid <mesh>" << endl;

		for (Triangle t : p_tris) {
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

using mesh3d = vector<Triangle>;

int main() {
	vector<vector3f> dists_and_angles = loadScanData("ScanData.txt");

	mesh3d mesh = makeMeshFromPoints(dists_and_angles);

	meshToSTL(mesh, "output.stl");
}
