#include <iostream>
#include <string>
#include <set>
#include <map>
#include <functional>
#include <vector>
#include <sstream>
#include <utility>
#include <filesystem>

// Predefined command return
enum CommandResult {
  COMMAND_SUCCESS,
  COMMAND_FAILURE,
  COMMAND_UNRECOGNIZED
};

// Funtion prototypes
// Helper function for commands
std::pair<std::string, std::vector<std::string>> parse_input(const std::string&);
std::vector<std::string> list_files_in_path(const std::string&);
std::pair<bool, std::string> exists_in_path(const std::string&);
// These funtions define command behaviour
CommandResult metaCommand();
CommandResult cmd__exit(const std::vector<std::string>&);
CommandResult cmd__okay();
CommandResult cmd__echo(const std::vector<std::string>&);
CommandResult cmd__type(const std::vector<std::string>&);

// Command map
std::map<std::string, std::function<CommandResult(const std::vector<std::string>&)>> command_map {
  {"", [](const std::vector<std::string>&){return COMMAND_SUCCESS;}}, // meta-command for blank input
  {"okay", [](const std::vector<std::string>&){return cmd__okay();}},
  {"exit", cmd__exit},
  {"echo", cmd__echo},
  {"type", cmd__type}
};


// helper funtions
std::pair<std::string, std::vector<std::string>> parse_input(const std::string& input){
  if (input.size() == 0 or input[0] == '\n') {
    return std::make_pair((std::string)"", std::vector<std::string>());
  }

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


std::vector<std::string> list_files_in_path(const std::string& path){
  std::vector<std::string> files;

  try {
    std::filesystem::path dir{path};
    for(auto const& dir_entry : std::filesystem::directory_iterator{dir}){
      files.push_back(dir_entry.path());
    }
  }
  catch (const std::exception& e) {
    files = {};
  }
  
  return files;
}


std::pair<bool, std::string> exists_in_path(const std::string& input) {
  // Get environment variable PATH
  const char* path_env_var = std::getenv("PATH");
  if(path_env_var == nullptr) std::exit(EXIT_FAILURE);

  // Convert char* to std::string
  std::string path(path_env_var);

  // Get all parts of PATH
  std::vector<std::string> parts;
  std::string part;
  std::istringstream stream(path);
  char delimiter = ':';

  while (std::getline(stream, part, delimiter)) {
    parts.push_back(part);
  }

  // Check if input exists in these PATH folders - list all path folders and match with content - linear search
  for (auto el : parts){
    std::string key = el + "/" + input;
    std::vector<std::string> files = list_files_in_path(el);
    for (auto el : files) {
      if (key.compare(el) == 0){
        return std::make_pair(true, key);
      }
    }
  }

  return std::make_pair(false, std::string(""));
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

CommandResult cmd__echo(const std::vector<std::string>& args){
  auto start = args.begin();
  auto stop = args.end();
  for (auto it = start; it != stop; ++it) {
    std::cout << *it;
    if (it + 1 != stop) std::cout << " ";
  }
  std::cout << '\n';

  return COMMAND_SUCCESS;
}


CommandResult cmd__type(const std::vector<std::string>& args){
  if (args.size() != 1){
    std::cout << "ArgumentError: type takes exactly 1 argument\n";
    return COMMAND_FAILURE;
  }
  else {
    if(command_map.find(args[0]) != command_map.end()){
      std::cout << args[0] << " is a shell builtin\n";
    }
    else {
      auto check_in_path = exists_in_path(args[0]);
      if(check_in_path.first) {
        std::cout << args[0] << " is " << check_in_path.second << "\n";
      }
      else std::cout << args[0] << ": not found\n";
    }
    return COMMAND_SUCCESS;
  }
}

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
