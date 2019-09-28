#include <string>

class iview {
  	public:
		int key = '?';
		bool running = false;

		virtual void init() = 0;
		virtual void destroy() = 0;
		virtual void draw_screen(int **board, int size) = 0;
		virtual std::string prompt_user(std::string) = 0;
		virtual void message_user(std::string) = 0;
};
