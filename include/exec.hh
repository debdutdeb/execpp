#pragma once

#include <memory>
#include <string>

struct [[nodiscard]] CommandOutput {
  int return_code;
  std::string stdout;
  std::string stderr;

  CommandOutput() = default;

  bool success();
};

[[nodiscard]] std::shared_ptr<CommandOutput> run_command(std::string);
