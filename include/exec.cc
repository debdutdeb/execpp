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

/** doesn't support shell quoting sorry */
[[nodiscard]] std::shared_ptr<std::vector<const char *>>
string_split(std::string source, char delimiter) {
  // not on heap haha
  std::shared_ptr<std::vector<const char *>> destination =
      std::make_shared<std::vector<const char *>>();

  size_t initial_pos = 0;

  for (size_t pos = 0;
       (pos = source.find(delimiter, initial_pos)) != std::string::npos;
       initial_pos = pos + 1)
    destination->push_back(
        source.substr(initial_pos, pos - initial_pos).c_str());

  destination->push_back(source.substr(initial_pos).c_str());
  destination->push_back(NULL);

  for (auto c : *destination)
    std::cout << c << '\n';

  return destination;
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

    auto c = string_split("echo hello", ' ');
    // const char **argv = c.data();
    // execvp(*argv, argv);
    for (auto cc : *c)
      std::cout << cc << '\n';
    // execvp(*argv, const_cast<char *const *>(argv));
  } else {

    if (wait(&command_output->return_code) != child_pid)
      print_error_and_exit(errno);
    std::cout << "return: " << command_output->return_code << std::endl;
  }

  return command_output;
}
