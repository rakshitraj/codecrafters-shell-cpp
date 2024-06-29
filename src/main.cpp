#include <iostream>

void print_prompt() {
  std::cout << "$ ";
}

void read_input(std::string& input){
  std::getline(std::cin, input);
}

void command(const std::string& input){
  // Command dictionary does not exist yet
  std::cout << input << " : command not found\n";
}

int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  std::string input;

  while (true){
    print_prompt();
    read_input(input);
    command(input);
  }

}
