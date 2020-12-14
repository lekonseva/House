#include <iostream>
#include <cstdlib>
#include <memory>
#include <stdexcept>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_main.h>


constexpr int WIDTH = 800, HEIGHT = 600;

std::shared_ptr<SDL_Window> win;//хранине указателя
std::shared_ptr<SDL_Renderer> ren;
using TexturePtr = std::shared_ptr<SDL_Texture>;
using WindowPtr = std::shared_ptr<SDL_Window>;
using RendererPtr = std::shared_ptr<SDL_Renderer>;
using TexturePtr = std::shared_ptr<SDL_Texture>;
TexturePtr bird;
TexturePtr fox;

inline void SDL_DIE(const char * where)
{
	throw std::runtime_error(
			std::string(where) +
			std::string(": ") +
			std::string(SDL_GetError()));
}

TexturePtr load_texture(const char * filename)
{
	SDL_Surface * surf = nullptr;
	surf = IMG_Load(filename); // Загрузка картинки
	if (surf == nullptr) SDL_DIE("IMG_Load()");
	// Здесь могла бы быть проверка на наличие прозрачности,
	// и если таковой нет - можно задать ключевой цвет
	TexturePtr result;
	result = TexturePtr(
			SDL_CreateTextureFromSurface(ren.get(), surf),
			SDL_DestroyTexture);
	if (result == nullptr) SDL_DIE("SDL_CreateTextureFromSurface()");
	SDL_FreeSurface(surf);

	return result;
}


void render_background()
{
	/*SDL_SetRenderDrawColor(ren.get(), 32, 64, 192, 255);
	SDL_RenderClear(ren.get());*/
	SDL_SetRenderDrawColor(ren.get(),65, 105, 255, 100);
	SDL_RenderClear(ren.get());
}

int bx = 100, by = 100, bw = 200, bh = 170;
int ba = 0;
int fy = 480,  fx = 100, fw = 150, fh = 100;
bool r = true;

void render_scene()
{
	//SDL_RenderCopy(ren.get(), bird.get(), nullptr, nullptr);

	SDL_Rect r_bird { bx, by, bw, bh };
	SDL_Point p_bird_center { bw/2, bh/2 };
	SDL_RenderCopyEx(
			ren.get(), bird.get(),
			nullptr, &r_bird,
			ba,						// Угол поворота
			&p_bird_center,			// Центр поворота (с.к. текстуры)
			SDL_FLIP_NONE);




	/*//фон
	SDL_SetRenderDrawColor(ren.get(),65, 105, 255, 100);
	SDL_RenderClear(ren.get());*/

	//земля
	SDL_SetRenderDrawColor(ren.get(), 139, 69, 19, 100);
	SDL_Rect z{0, HEIGHT-50, WIDTH, 50};
	SDL_RenderFillRect(ren.get(),&z);
	//SDL_RenderClear(ren.get());

	//стенка
	SDL_SetRenderDrawColor(ren.get(), 255, 165, 0, 100);
	SDL_Rect s{(WIDTH/2)-125, HEIGHT-300, 250, 250};
	SDL_RenderFillRect(ren.get(),&s);
	//SDL_RenderClear(ren.get());

	//труба
	SDL_SetRenderDrawColor(ren.get(), 128, 0, 0, 100);
	SDL_Rect t{450, 150, 50, 100};
	SDL_RenderFillRect(ren.get(),&t);

	//крыша
	for(int i = 1; i < 151; i++){
		SDL_SetRenderDrawColor(ren.get(), 184, 134, 11, 100);
		SDL_RenderDrawLine(ren.get(), 400-i, 150+i, 400+i,150+i);
	}

	//дверь
	SDL_SetRenderDrawColor(ren.get(), 205, 133, 63, 100);
	SDL_Rect d{WIDTH/2-100, HEIGHT-220, 80, 170};
	SDL_RenderFillRect(ren.get(),&d);

	//окно
	SDL_SetRenderDrawColor(ren.get(), 240, 248, 255, 100);
	SDL_Rect o{WIDTH/2+20, HEIGHT-220, 80, 80};
	SDL_RenderFillRect(ren.get(),&o);

	//fox
	/*SDL_Rect r_fox { fx, fy, fw, fh };
	SDL_RenderCopy(
		ren.get(), fox.get(),
		nullptr, &r_fox);*/

	//трава




}

void render_fox(){
	SDL_Rect r_fox { fx, fy, fw, fh };
	auto flip = r?SDL_FLIP_NONE:SDL_FLIP_HORIZONTAL;

	SDL_RenderCopyEx(
		ren.get(), fox.get(),
		nullptr, &r_fox, 0, nullptr, flip);

}

void main_loop()
{
	SDL_Event event;
	auto keys = SDL_GetKeyboardState(nullptr);
	for(;;) {
		// Обработка событий
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT: return;
			default: ;
			}
		}
		bx += 4;

		// Изменение состояния
		if (keys[SDL_SCANCODE_LEFT]) bx -= 4;
		if (keys[SDL_SCANCODE_RIGHT]) bx += 4;
		if (keys[SDL_SCANCODE_UP]) by -= 4;
		if (keys[SDL_SCANCODE_DOWN]) by += 4;

		if (keys[SDL_SCANCODE_W]) fy -= 4;
		if (keys[SDL_SCANCODE_S]) fy += 4;
		if (keys[SDL_SCANCODE_A]){
			fx -= 4;
			r = true;

		}
		if (keys[SDL_SCANCODE_D]){
			fx += 4;
			r = false;
		}
		/*if (keys[SDL_SCANCODE_A]) bh -= 4;
		if (keys[SDL_SCANCODE_Z]) bh += 4;
		if (keys[SDL_SCANCODE_Q]) bw -= 4;
		if (keys[SDL_SCANCODE_W]) bw += 4;
		if (keys[SDL_SCANCODE_E]) ba -= 1;
		if (keys[SDL_SCANCODE_R]) ba += 1;*/
		if (by < 0)
			by = 0;
		if(by >= 150)
			by = 149;
		if(bx >= WIDTH)
			bx = -64;
		if(fy < 0)
			fy = 0;
		if(fx >= WIDTH)
			fx = WIDTH-100;

		// Рисование
		render_background();
		render_scene();
		render_fox();
		SDL_RenderPresent(ren.get());
	}
}

int main(int,  char**){
	try {
			if (SDL_Init(SDL_INIT_EVERYTHING) != 0) SDL_DIE("SDL_Init()");

			win = WindowPtr(
					SDL_CreateWindow(
							"SDL window",
							SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
							WIDTH, HEIGHT,
							SDL_WINDOW_SHOWN),
					SDL_DestroyWindow);
			if (win == nullptr) SDL_DIE("SDL_CreateWindow()");

			ren = RendererPtr(
					SDL_CreateRenderer(win.get(), -1,
							SDL_RENDERER_ACCELERATED |
							SDL_RENDERER_PRESENTVSYNC),
					SDL_DestroyRenderer);
			if (ren == nullptr) SDL_DIE("SDL_CreateRenderer()");

			// Тут будет загрузка всяких ресурсов
			bird = load_texture("bird.png");
			fox = load_texture("fox.png");

			main_loop();

		} catch (const std::exception& e) {
			std::cerr << "Произошла ошибка:\n" <<
					e.what() << std::endl;
			return 1;
		} catch (...) {
			std::cerr << "Произошла какая-то ошибка." << std::endl;
			return 2;
		}

		return 0;


}

