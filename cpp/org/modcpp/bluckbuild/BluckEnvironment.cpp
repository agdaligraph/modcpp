#include "org/modcpp/bluckbuild/BluckEnvironment.h"

#include <algorithm>
#include <cassert>
#include <cstring>
#include <fstream>
#include <string>
#include <unistd.h>

#include "org/modcpp/base/base.h"
#include "org/modcpp/bluckbuild/BluckBuild.h"
#include "org/jsoncpp/jsoncpp.h"
#include "org/modcpp/logging/Console.h"
#include "org/modcpp/string/Cstrings.h"

namespace org::modcpp::bluckbuild {

  using org::modcpp::logging::Console;
  using org::modcpp::string::Cstrings;
  using std::ifstream;
  using std::string;

  BluckEnvironment::BluckEnvironment() {
    char buffer[512];
    if (!getcwd(buffer, sizeof(buffer))) {
      Console::error("Cannot get working directory");
      std::exit(0);
    };

    Cstrings::removeTrailingSlash(buffer);
    workingPath = buffer;

    // TODO(saglam): implement a less hackish root rule
    cstringIterator root = Cstrings::findFirst(buffer, "/cpp");
    if (!root) {
      root = Cstrings::findFirst(buffer, "/java");
    }
    if (!root) {
      root = Cstrings::findFirst(buffer, "/html");
    }
    if (root) {
      Cstrings::trimRightAt(buffer, (mutableCstringIterator) root);
    }
    bluckRootPath = buffer;
  }

  void BluckEnvironment::readConfigFile() {
    readConfigFile(bluckRootPath + "/bluck.config");
  }

  void BluckEnvironment::readConfigFile(const string &fileName) {
    ifstream configFile;
    configFile.open(fileName);
    org::jsoncpp::Value config;
    org::jsoncpp::Reader reader;

    reader.parse(configFile, config);

    binFolderName = config.get("binFolderName", "/bluck-out").asString();
    dryRun = config.get("isDryRun", false).asBool();
    cppCompilerCommand = config.get("cppCompilerName", "g++").asString() + " "
       + config.get("cppCompilerFlags", "-std=c++1y -Wall").asString();
    cppLinkerFlags = config.get("cppLinkerFlags", "").asString();
    cCompilerCommand = config.get("cCompilerName", "gcc").asString() + " "
       + config.get("cCompilerFlags", "-Wall").asString();
    Console::info("Cpp compiler command: %s", cppCompilerCommand.c_str());
    Console::info("Cpp linker flags: %s", cppLinkerFlags.c_str());
  }

  string BluckEnvironment::translatePath(string path) const {
    if (path.back() == ':' || path.back() == '/') {
      path.erase(path.end() - 1);
    }
    if (Cstrings::startsWith(path.c_str(), "//")) {
      return path;
    }
    if (!Cstrings::startsWith(path.c_str(), "/")) {
      if (path == ".") {
        path = workingPath;
      } else {
        path.insert(0, workingPath + "/");
      }
    }
    auto result = std::mismatch(bluckRootPath.begin(), bluckRootPath.end(), path.begin());
    if (result.first == bluckRootPath.end()) {
      path.erase(path.begin(), result.second);
      path.insert(0, "/");
    } else {
      Console::error("Invalid path %s", path.c_str());
    }
    return path;
  }

}
