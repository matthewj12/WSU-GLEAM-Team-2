#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include <string>

class RenderWindow {
private:
	int m_w, m_h, m_x, m_y, m_fps;
	std::string m_window_caption;
	SDL_Window* m_window;
	SDL_Renderer* m_renderer;

public:
	RenderWindow(const char* p_window_caption, int p_w, int p_h);
	~RenderWindow();

	int getX();
	int getY();
	int getW();
	int getH();
	int getFPS();
	std::string getWindowCaption();

	void clear();
	void display();

};