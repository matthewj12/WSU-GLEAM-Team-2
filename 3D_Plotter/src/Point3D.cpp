#pragma once

// #include "Viewport.cpp"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

class Point3D {
private:
	float m_x, m_y, m_z;
	float m_blit_x, m_blit_y;

public:
	Point3D(float p_x, float p_y, float p_z) {
		m_x = p_x;
		m_y = p_y;
		m_z = p_z;
	}

	Point3D(std::string p_text) {
		int first_comma_index = p_text.find(", ", 0);
		int second_comma_index = p_text.find(", ", first_comma_index+2);

		try {
			m_x = std::stof(p_text.substr(0, first_comma_index));
			m_y = std::stof(p_text.substr(first_comma_index+2, second_comma_index - first_comma_index - 2));
			m_z = std::stof(p_text.substr(second_comma_index+2));
		}
		catch (const std::exception& e) {
			std::cerr << "Error: " << e.what() << std::endl;
		}
	}

	float getX() { return m_x; }
	float getY() { return m_y; }
	float getZ() { return m_z; }

	std::vector<float> getXYZ() {
		std::vector<float> to_return;

		to_return.push_back(m_x);
		to_return.push_back(m_y);
		to_return.push_back(m_z);

		return to_return;
	}
	// std::vector<float> getXYZ() {
	// 	std::vector<float> to_return;
	// 	to_return.push_back(m_x);
	// 	to_return.push_back(m_y);
	// 	to_return.push_back(m_z);
	// 	return to_return;
	// }

	void setX(float p_x) { m_x = p_x; }
	void setY(float p_y) { m_y = p_y; }
	void setZ(float p_z) { m_z = p_z; }

	void printXYZ() {
		std::cout << m_x << ", " << m_y << ", " << m_z << std::endl;
	}

	// void render(SDL_Renderer* p_renderer, Viewport& p_viewport);
};