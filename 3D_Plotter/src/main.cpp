#include "RenderWindow.hpp"
#include "Point3d.cpp"
#include "Matrix2D.cpp"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <iostream>
#include <fstream>
#include <string>
#include <array>
#include <vector>
#include <random>
#include <unordered_map>
// #include <ctime>

void limitTPS(int p_ms_between_frames) {
	static int delta_time = 0;

	delta_time = SDL_GetTicks() - delta_time;
	int time_to_wait = p_ms_between_frames - delta_time;

	if (time_to_wait > 0) {
		SDL_Delay(time_to_wait);
	}
}

int getRandInt(int p_min, int p_max) {
	static constexpr double frac = 1 / (RAND_MAX + 1);
	return p_min + static_cast<int>(rand() * frac * (p_max - p_min + 1));
}

// void saveData(std::string p_path, std::unordered_map<std::string, int>& p_data) {
// 	std::ofstream file(p_path, std::ios::binary);

// 	file.seekp(0);

// 	for (auto it = p_data.begin(); it < p_data.end(); it++) {
// 		std::string key = it->first();
// 		int val = it->second();

// 		file.write((char*) key, sizeof(std::string));
// 		file.write((char*) val, sizeof(int));
// 	}

// 	file.close();
// }

void loadPoints(std::string p_path, std::vector<Point3D>& p_points) {
	std::ifstream file(p_path);

	file.clear();
	file.seekg(0, std::ios::beg);

	if (file.is_open()) {
		while (!file.eof()) {
			std::string cur_line;
			std::getline(file, cur_line);

			p_points.push_back(Point3D(cur_line));

			// std::cout << cur_line << std::endl;;
		}

	}
	else {
		std::cout << "Could not open file '" << p_path << "'" << std::endl;
	}

	file.close();
}

int main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_EVERYTHING);

	srand(static_cast<unsigned int>(SDL_GetTicks()));

	RenderWindow render_window = RenderWindow("3D Plotter", 800, 600);
	SDL_Event event;
	bool game_running = true;

	std::vector<Point3D> points;
	loadPoints("res/points.txt", points);

	std::cout << "Read " << points.size() << " points from file." << std::endl;

	Matrix2D m = Matrix2D(3, 3, points);

	std::cout << "Matrix width = " << m.getW() << ", height = " << m.getH() << std::endl;

	Matrix2D m_inv = m.getInverse();

	std::cout << "Original: " << std::endl;
	m.printVals();
	std::cout << std::endl;

	std::cout << "Projected: " << std::endl;
	m_inv.printVals();
	std::cout << std::endl;

	while (game_running) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				game_running = false;
			}
		}

		render_window.display();
		limitTPS(60);
	}

	return 0;
}
