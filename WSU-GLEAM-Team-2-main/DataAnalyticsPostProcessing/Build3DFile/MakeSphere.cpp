#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <vector>
#include <cmath>
#include <ctime>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

int getRandInt(int p_min, int p_max) {
    static constexpr double fraction { 1.0 / (RAND_MAX + 1.0) };
    return p_min + static_cast<int>((p_max - p_min + 1) * (rand() * fraction));
}

// TODO: put these in a file in such a way that the MakeShpere.cpp and the Arduio code can also read them (avoid redundancy).
int resolution = 5;
const int pan_min = -90;
const int pan_max = 90;
const int tilt_min = -45;
const int tilt_max = 85;

void createSphere(string output_path) {
	ofstream file(output_path);

	if (file.is_open()) {
		for (int pan = pan_min; pan <= pan_max; pan += resolution) {
			for (int tilt = tilt_min; tilt <= tilt_max; tilt += resolution) {
				// 90.0 to 100.0
				float dist = getRandInt(0, 100) / 10 + 90;

				file << dist << ", " << pan << ", " << tilt << endl;
			}

			file << endl;
		}
	}
	else {
		cout << "Could not create file '" << output_path << "'" << endl;
	}
}

int main() {
	srand(static_cast<unsigned int>(time(nullptr)));
	getRandInt(0, 6);

	createSphere("ScanData.txt");

	return 0;
}