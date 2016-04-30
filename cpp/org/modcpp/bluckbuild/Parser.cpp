#include "org/modcpp/bluckbuild/Parser.h"

#include <algorithm>
#include <string>
#include <vector>

#include "org/modcpp/bluckbuild/Target.h"
#include "org/modcpp/logging/Console.h"
#include "org/modcpp/string/Cstrings.h"

namespace org::modcpp::bluckbuild {
  using org::modcpp::logging::Console;
  using org::modcpp::string::Cstrings;
  using std::string;
  using std::vector;

  Parser::Parser(string fileName) : fileName(fileName) {}

  Parser::~Parser() {}

  static Target::Type getTargetType(const char *targetType) {
    if (Cstrings::isEqual(targetType, "cpp_library")) {
      return Target::Type::CppLibrary;
    } else if (Cstrings::isEqual(targetType, "cpp_binary")) {
      return Target::Type::CppBinary;
    } else if (Cstrings::isEqual(targetType, "cpp_test")) {
      return Target::Type::CppTest;
    }
    Console::error("Unsupported target type %s", targetType);
    exit(0);
  }

  void Parser::expandLocalDeps(Target &target) {
    for (string &dep : target.deps) {
      if (dep.at(0) == ':') {
        dep = target.package + dep;
      }
    }
  }

  void Parser::toStandardForm(Target &target) {
    std::sort(target.srcs.begin(), target.srcs.end());
    std::sort(target.deps.begin(), target.deps.end());
  }

  /**
   * @brief read double quote enclosed literal from f
   * @return true if successful
   */
  bool Parser::parseLiteral(FILE *f, char buffer[]) {
    if (fscanf(f, " %511[\"]", buffer) <= 0) {
      Console::error("Expecting a double quoted literal");
      return false;
    }
    if (fscanf(f, "%511[^\"]\"", buffer) <= 0) {
      Console::error("Cannot parse literal after");
      return false;
    }
    return true;
  }

  /**
   * @brief read a Python array from file f
   * @return true if an array is successfully parsed
   */
  bool Parser::parseArray(FILE *f, char buffer[], vector<string> &literals) {
    if (fscanf(f, " [") < 0) {
      Console::error("Expecting an array");
      return false;
    }

    while (parseLiteral(f, buffer)) {
      literals.push_back(buffer);
      if (fscanf(f, " %1[]]", buffer) > 0) {
        return true;
      }
      if (fscanf(f, " %1[,]", buffer) <= 0) {
        Console::error("Missing comma after item %s", buffer);
        return false;
      }
      if (fscanf(f, " %1[]]", buffer) > 0) {
        return true;
      }
    }
    return fscanf(f, " %1[]]", buffer) > 0;
  }

  /**
   * @brief populate a {@link Target} from a BLUCK file
   * @return true if file is well-formed and contains the target sought
   */
  bool Parser::populateTarget(Target &target) const {
    char buffer[512];
    FILE *f = fopen(fileName.c_str(), "r");

    if (!f) {
      Console::error("No BLUCK file at %s", fileName.c_str());
      return false;
    }

    bool targetFound = false;
    bool isMalformed = false;

    // Read a target
    while (!isMalformed && !targetFound && fscanf(f, " %511[a-z_]", buffer) > 0) {
      target.type = getTargetType(buffer);
      isMalformed |= fscanf(f, " %1[(]", buffer) <= 0;

      // Read target declarations
      while (fscanf(f, " %511[a-z_]", buffer) > 0) {
        if (Cstrings::isEqual(buffer, "name")) {
          isMalformed |= fscanf(f, " %1[=]", buffer) <= 0;
          if (isMalformed) {
            Console::error("Missing assignment operator after %s", buffer);
            break;
          }
          isMalformed |= !parseLiteral(f, buffer);

          // Not the target we seek. Skip past the end of the current target
          if (target.name != Cstrings::trim(buffer, "\"")) {
            isMalformed |= fscanf(f, "%*[^)])") < 0;
            break;
          } else {
            targetFound = true;
          }
        } else if (Cstrings::isEqual(buffer, "srcs")) {
          isMalformed |= fscanf(f, " %1[=]", buffer) <= 0;
          isMalformed |= !parseArray(f, buffer, target.srcs);
        } else if (Cstrings::isEqual(buffer, "deps")) {
          isMalformed |= fscanf(f, " %1[=]", buffer) <= 0;
          isMalformed |= !parseArray(f, buffer, target.deps);
        } else if (Cstrings::isEqual(buffer, "includes")) {
          isMalformed |= fscanf(f, " %1[=]", buffer) <= 0;
          isMalformed |= !parseArray(f, buffer, target.includes);
        } else if (Cstrings::isEqual(buffer, "copts") || Cstrings::isEqual(buffer, "compileopts")) {
          isMalformed |= fscanf(f, " %1[=]", buffer) <= 0;
          isMalformed |= !parseArray(f, buffer, target.compileopts);
        } else if (Cstrings::isEqual(buffer, "lopts") || Cstrings::isEqual(buffer, "linkopts")) {
          isMalformed |= fscanf(f, " %1[=]", buffer) <= 0;
          isMalformed |= !parseArray(f, buffer, target.linkopts);
        } else if (Cstrings::isEqual(buffer, "visibility")) {
          std::vector<std::string> ignore;
          isMalformed |= fscanf(f, " %1[=]", buffer) <= 0;
          isMalformed |= !parseArray(f, buffer, ignore);
        }
        if (fscanf(f, " %1[)]", buffer) > 0) {
          break;
        }
        if (fscanf(f, " %1[,]", buffer) <= 0) {
          Console::error("Missing comma after item %s", buffer);
          isMalformed = true;
        }
        if (fscanf(f, " %1[)]", buffer) > 0) {
          break;
        }
      }
    }
    fclose(f);

    if (isMalformed) {
      Console::error("Invalid BLUCK file %s", fileName.c_str());
      return false;
    }
    if (!targetFound) {
      Console::error("The BLUCK file %s does not contain target %s",
          fileName.c_str(), target.getBluckPath().c_str());
      return false;
    }

    expandLocalDeps(target);
    toStandardForm(target);
    return true;
  }

} // namespace
