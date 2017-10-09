#include "Signal.hpp"
#include <iostream>

class Button {
 public:
  Signal<> on_click;
};

class Message {
 public:
  void display() const {
    std::cout << "Hello World!" << std::endl;
  }
};

int main() {
	Button  button;
	Message message;

	button.on_click.connect_member(&message, &Message::display);
	button.on_click.emit();

	return 0;
}

