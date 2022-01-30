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

// steps = number of distinct rotational positions for points
const int pan_steps = 10;
const int tilt_steps = 5;
const int pan_min = 45;
const int pan_max = 135;
const int tilt_min = -45;
const int tilt_max = 90;
const string output_path = "ScanData.txt";

const int pan_intvl = (pan_max - pan_min) / (pan_steps - 1);
const int tilt_intvl = (tilt_max - tilt_min) / (tilt_steps - 1);

void createSphere() {
	ofstream file(output_path);

	if (file.is_open()) {
		for (int pan = pan_min; pan <= pan_max; pan += pan_intvl) {
			for (int tilt = tilt_min; tilt <= tilt_max; tilt += tilt_intvl) {
				// 90.0 to 100.0
				float radius = getRandInt(0, 100) / 10 + 90;

				file << radius << ", " << pan << ", " << tilt << endl;
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

	createSphere();

	return 0;
}