#include "RenderWindow.hpp"

// #include "Entity.hpp"

RenderWindow::RenderWindow(const char* p_title, int p_w, int p_h) {
	m_w = p_w;
	m_h = p_h;

	m_window = SDL_CreateWindow("Minimal SDL program", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	// if (m_window == NULL) {
	// 	std::cout << "Window failed to render. LISTEN TO ME. Error: " << SDL_GetError() << std::endl;
	// }

	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

RenderWindow::~RenderWindow() {
	SDL_DestroyWindow(m_window);
	SDL_DestroyRenderer(m_renderer);
	IMG_Quit();
	TTF_Quit();
}

int RenderWindow::getX() { return 0; }
int RenderWindow::getY() { return 0; }
int RenderWindow::getW() { return m_w; }
int RenderWindow::getH() { return m_h; }
int RenderWindow::getFPS() { return m_fps; }
std::string RenderWindow::getWindowCaption() { return m_window_caption; }

void RenderWindow::clear() {
	SDL_RenderClear(m_renderer);
}

void RenderWindow::display() {
	SDL_RenderPresent(m_renderer);
}