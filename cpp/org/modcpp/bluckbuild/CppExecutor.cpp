#include "org/modcpp/bluckbuild/CppExecutor.h"

#include <unistd.h>
#include <vector>
#include <string>
#include <cassert>
#include <cstdlib>

#include "org/modcpp/bluckbuild/BluckBuild.h"
#include "org/modcpp/bluckbuild/BluckEnvironment.h"
#include "org/modcpp/bluckbuild/Target.h"
#include "org/modcpp/io/Files.h"
#include "org/modcpp/logging/Console.h"
#include "org/modcpp/sets/Sets.h"

namespace org::modcpp::bluckbuild {
  using org::modcpp::io::Files;
  using org::modcpp::logging::Console;
  using org::modcpp::sets::Sets;
  using std::string;
  using std::vector;

  static bool isHeaderFile(const string& fileName) {
    size_t pos = fileName.rfind(".h");
    return pos != string::npos && pos > fileName.length() - 4;
  }

  static bool isCppFile(const string& fileName) {
    size_t pos = fileName.rfind(".c");
    return pos != string::npos && pos > fileName.length() - 6 && pos < fileName.length() - 2;
  }

  static string stripExtension(const string &fileName) {
    auto extensionStart = fileName.find(".");
    if (extensionStart == string::npos) {
      return fileName;
    } else {
      return fileName.substr(0, extensionStart);
    }
  }

  static string getLibraryNameFromBluckPath(string bluckPath) {
    auto targetSeparator = bluckPath.rfind(":");
    assert(targetSeparator != string::npos);
    bluckPath[targetSeparator] = '/';
    return bluckPath.substr(1) + ".a";
  }

  CppExecutor::CppExecutor(const BluckEnvironment &environment)
      : environment(environment) {}

  BluckBuild::Result CppExecutor::run(const Target &target, const string workingDir) const {
    string binaryPath = environment.getBluckRootPath() + environment.getBinFolderName()
        + target.package.substr(1) + "/" + target.name + ".out";

    if (!workingDir.empty()) {
      // Change to the working directory
      if (chdir(workingDir.c_str())) {
        Console::error("Cannot change to directory %s", workingDir.c_str());
        return BluckBuild::Result::Fail;
      }
    }
    return std::system(binaryPath.c_str()) ? BluckBuild::Result::Fail : BluckBuild::Result::Success;
  }

  BluckBuild::Result CppExecutor::test(const Target &target) const {
    string workingDir = environment.getBluckRootPath() + target.package.substr(1);
    return run(target, workingDir);
  }

  BluckBuild::Result CppExecutor::build(const Target &target, bool isTest,
      const vector<string> &dependencies) const {
    const string targetBinPath = environment.getBinFolderPath() + target.package.substr(1);
    const string targetPackagePath = environment.getBluckRootPath() + target.package.substr(1)
        + "/";

    if (chdir(targetBinPath.c_str())) {
      return BluckBuild::Result::Fail;
    }

    string compilerFlags = string(" ")
        + (isTest ? "-Dtestvisible=public" : "-Dtestvisible=private")
        + " -I" + environment.getBluckRootPath() + "/cpp";
    for (const string& compileopt : target.compileopts) {
      compilerFlags += " " + compileopt;
    }
    for (const string& include : target.includes) {
      compilerFlags += " -I" + targetPackagePath + include;
      if (!Files::ensureDirectory(include.c_str(), targetBinPath.c_str())) {
        Console::error("Cannot create directory %s", include.c_str());
        return BluckBuild::Result::Fail;
      }
    }

    string objectFiles;
    for (const string &src : target.srcs) {
      if (isHeaderFile(src)) {
        continue;
      }
      string objectFile = stripExtension(src) + ".o ";
      objectFiles += objectFile;
      const string compileCommand = (isCppFile(src) ?
          environment.getCppCompilerCommand() : environment.getCCompilerCommand())
          + compilerFlags + " -c " + targetPackagePath + src + " -o " + objectFile;
      int result = environment.isDryRun() ? 0 : std::system(compileCommand.c_str());
      if (result) {
        Console::error("Failed with error code %d", result);
        return BluckBuild::Result::Fail;
      }
    }

    if (target.getArtifact() == Target::Artifact::Library) {
      if (not objectFiles.empty()) {
        Console::info("Creating library %s.a", target.name.c_str());
        string fileName = target.name + ".a ";
        const string packCommand = "rm -f " + fileName + "&& ar qcs " + fileName + objectFiles;
        int result = environment.isDryRun() ? 0 : std::system(packCommand.c_str());
        if (result) {
          Console::error("Failed with error code %d", result);
          return BluckBuild::Result::Fail;
        }
      }
    } else if (Sets::isIn(
        target.getArtifact(), {Target::Artifact::Binary, Target::Artifact::Test})) {
      string dependencyLibs;
      for (const string &dep : dependencies) {
        string libraryPath = environment.getBinFolderPath() + getLibraryNameFromBluckPath(dep);
        if (Files::exists(libraryPath.c_str())) {
          dependencyLibs = libraryPath + " " + dependencyLibs;
        }
      }
      string linkOpts;
      for (const string& linkOpt : target.linkopts) {
        linkOpts += " " + linkOpt;
      }
      const string linkCommand = environment.getCppCompilerCommand() + " -o " + target.name + ".out "
          + objectFiles + dependencyLibs + linkOpts + " " + environment.getCppLinkerFlags();
      Console::info("Linking binary %s.out %s", target.name.c_str(), linkCommand.c_str());
      int result = environment.isDryRun() ? 0 : std::system(linkCommand.c_str());
      if (result) {
        Console::error("Failed with error code %d", result);
        return BluckBuild::Result::Fail;
      }
    } else {
      Console::error("Unknown artifact type");
      return BluckBuild::Result::Fail;
    }

    return BluckBuild::Result::Success;
  }

} // namespace
