#include <exec.hh>

#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>

extern int errno;

bool CommandOutput::success() { return return_code == 0; }

void print_error_and_exit(int err) {
  std::cerr << "Error: " << strerror(err) << std::endl;
  exit(EXIT_FAILURE);
}

std::shared_ptr<CommandOutput> return_code(std::string command) {
  std::shared_ptr<CommandOutput> command_output =
      std::make_shared<CommandOutput>();

  int file_descriptors[2];
  if (pipe(file_descriptors) == -1)
    print_error_and_exit(errno);

  pid_t child_pid = fork();

  if (child_pid == -1)
    print_error_and_exit(errno);

  if (child_pid == 0) {
    // do  stuff
  } else {
    if (wait(&command_output->return_code) != child_pid)
      print_error_and_exit(errno);
    std::cout << "return: " << command_output->return_code << std::endl;
  }

  return command_output;
}
