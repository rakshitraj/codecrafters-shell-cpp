#include <iostream>
#include <string>
#include <set>
#include <map>
#include <functional>

// Predefined command return
enum CommandResult {
  COMMAND_SUCCESS,
  COMMAND_FAILURE,
  COMMAND_UNRECOGNIZED
};

// Functions that define command behaviour

CommandResult cmd__exit(const std::string& input){
  std::exit(EXIT_SUCCESS);
}

CommandResult cmd__okay(){
  std::cout<< "All okay!\n";
  return COMMAND_SUCCESS;
}

// Map of known commands and their implementaion
std::map<std::string, std::function<CommandResult(const std::string&)>> command_map {
  {"exit", cmd__exit},
  {"okay", [](const std::string& input){return cmd__okay();}}
};

// REPL

void print_prompt() {
  std::cout << "$ ";
}


void read_input(std::string& input){
  std::getline(std::cin, input);
}


CommandResult do_command(const std::string& input){
  std::map<std::string, std::function<CommandResult(const std::string&)>>::iterator it = command_map.find(input);
  if (it != command_map.end()) {
    it->second(input);
  }
  else {
    return COMMAND_UNRECOGNIZED;
  }
}


int main() {
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  std::string input;

  while (true){
    print_prompt();
    read_input(input);
    
    switch (do_command(input))
    {
      case COMMAND_SUCCESS:
        continue;
      case COMMAND_UNRECOGNIZED:
        std::cout << input << ": command not found\n";
        continue;
      default:
        std::cout << "unknown state\n";
        std::exit(EXIT_FAILURE);
    }
  }
}
