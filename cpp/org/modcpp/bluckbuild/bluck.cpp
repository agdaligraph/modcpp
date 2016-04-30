#include <string>

#include "org/modcpp/base/base.h"
#include "org/modcpp/bluckbuild/BluckBuild.h"
#include "org/modcpp/logging/Console.h"
#include "org/modcpp/string/Cstrings.h"

using org::modcpp::bluckbuild::BluckBuild;
using org::modcpp::logging::Console;
using org::modcpp::string::Cstrings;
using std::string;

void printHelp() {
  puts("bluck (built: " __TIME__ ", " __DATE__ ")\n\n"
    "Usage: bluck [command] target\n\n"
    "Commands:\n"
    "\tbuild\n"
    "\trun\n"
    "\ttest\n"
    "\tclean\n");
}

int main(int argc, char **argv) {
  if (argc != 3) {
    printHelp();
    return 1;
  }

  BluckBuild bluckBuild;
  string path = argv[2];
  BluckBuild::Result result = BluckBuild::Result::Fail;
  if (Cstrings::isEqual("build", argv[1])) {
    result = bluckBuild.buildTarget(path);
    switch (result) {
      case BluckBuild::Result::Fresh:
        Console::info("Target is up to date.");
        break;
      case BluckBuild::Result::Success:
        Console::info("Target succesfully built.");
        break;
      case BluckBuild::Result::Fail:
        Console::error("Build error.");
        break;
    }
  } else if (Cstrings::isEqual("run", argv[1])) {
    result = bluckBuild.runTarget(path);
  } else if (Cstrings::isEqual("test", argv[1])) {
    result = bluckBuild.testTarget(path);
    switch (result) {
      case BluckBuild::Result::Success:
        Console::info("Passed.");
        break;
      default:
        Console::error("Failed.");
        break;
    }
  } else if (Cstrings::isEqual("clean", argv[1])) {
    result = bluckBuild.cleanTarget(path);
  } else {
    Console::error("Unrecognized command %s", argv[1]);
  }
  return result == BluckBuild::Result::Success ? 0 : -1;
}
