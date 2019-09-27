#include <string>

class Iview {
  public:
	virtual void init() = 0;
	virtual void refresh_screen() = 0;
	virtual void current_keypress() = 0;
	virtual std::string prompt_user(std::string) = 0;
	virtual void message_user(std::string) = 0;
};
