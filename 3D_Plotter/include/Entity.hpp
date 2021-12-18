#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

class Entity {
private:
	int m_x, m_y, m_w, m_h;
	SDL_Rect m_rect;
	SDL_Texture* m_tex;

public:
	Entity(int p_x, int p_y, int p_w, int p_h);
	int getX();
	int getY();
	int getW();
	int getH();
	SDL_Texture* getTex();
	void render(SDL_Renderer* p_renderer);
};