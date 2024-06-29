#include <iostream>
#include <string>
#include <set>
#include <map>
#include <functional>
#include <vector>
#include <sstream>
#include <utility>

// Predefined command return
enum CommandResult {
  COMMAND_SUCCESS,
  COMMAND_FAILURE,
  COMMAND_UNRECOGNIZED
};

// helper funtions
std::pair<std::string, std::vector<std::string>> parse_input(const std::string& input){
  std::vector<std::string> parts;
  std::istringstream stream(input);
  std::string word;

  while (stream >> word){
    parts.push_back(word);
  }
  
  // Command
  std::string command = parts[0];
  std::vector<std::string> arguments;

  // Remove command to get list of arguments
  if (!parts.empty()) {
    parts.erase(parts.begin());
    arguments = parts;
  }
  else {
    arguments = {};
  }

  std::pair<std::string, std::vector<std::string>> parsed;
  parsed.first = command;
  parsed.second = arguments;

  return parsed;
}

// Functions that define command behaviour

CommandResult cmd__exit(const std::vector<std::string>& args){
  if (args.size() != 1){
    std::cout << "ArgumentError: exit takes exactly 1 argument\n";
    return COMMAND_FAILURE;
  }
  int args__exit_status = std::stoi(args[0]);
  std::exit(args__exit_status);
}

CommandResult cmd__okay(){
  std::cout << "All okay!\n";
  return COMMAND_SUCCESS;
}

// Map of known commands and their implementaion
std::map<std::string, std::function<CommandResult(const std::vector<std::string>&)>> command_map {
  {"exit", cmd__exit},
  {"okay", [](const std::vector<std::string>&){return cmd__okay();}}
};

// REPL

void print_prompt() {
  std::cout << "$ ";
}


void read_input(std::string& input){
  std::getline(std::cin, input);
}


CommandResult do_command(const std::string& input){
  std::pair<std::string, std::vector<std::string>> parsed_pair = parse_input(input);

  std::string command = parsed_pair.first;
  std::vector<std::string> arguments = parsed_pair.second;

  std::map<std::string, std::function<CommandResult(const std::vector<std::string>&)>>::iterator it = command_map.find(command);
  
  if (it != command_map.end()) {
    return it->second(arguments);
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
      case COMMAND_FAILURE:
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
