#include <exec.hh>

#include <array>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <vector>

using namespace std::string_literals;

extern int errno;

bool CommandOutput::success() { return return_code == 0; }

void print_error_and_exit(int err) {
  std::cerr << "Error: " << strerror(err) << std::endl;
  exit(EXIT_FAILURE);
}

[[nodiscard]] char *const *get_arguments(std::string command) {
  /**
   * we want for each word
   * to allocate a c string
   * then create a c style array of strings
   */
  std::vector<const char *> components;
  size_t pos = 0;
  while ((pos = command.find(" ")) != std::string::npos) {
    components.push_back(command.substr(0, pos).c_str());
    command.erase(0, pos + 1);
  }

  char *args = (char *)malloc(sizeof(char *) * components.size());
  if (args == nullptr)
    print_error_and_exit(errno);

  for (auto c : components)
    std::cout << c << '\n';

  return nullptr;
}

std::shared_ptr<CommandOutput> run_command(std::string command) {
  std::shared_ptr<CommandOutput> command_output =
      std::make_shared<CommandOutput>();

  int file_descriptors[2];
  if (pipe(file_descriptors) == -1)
    print_error_and_exit(errno);

  pid_t child_pid = fork();

  if (child_pid == -1)
    print_error_and_exit(errno);

  if (child_pid == 0) {
    // if (dup2(file_descriptors[1], STDOUT_FILENO) == -1)
    //   print_error_and_exit(errno);
    //
    // if (dup2(file_descriptors[0], STDERR_FILENO) == -1)
    //   print_error_and_exit(errno);
    //
    // close(STDOUT_FILENO);
    // close(STDERR_FILENO);

    (void)get_arguments("echo hello world"s);
    // execvp(*argv, const_cast<char *const *>(argv));
  } else {

    if (wait(&command_output->return_code) != child_pid)
      print_error_and_exit(errno);
    std::cout << "return: " << command_output->return_code << std::endl;
  }

  return command_output;
}
