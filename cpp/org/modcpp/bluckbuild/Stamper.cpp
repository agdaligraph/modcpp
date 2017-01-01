#include "org/modcpp/bluckbuild/Stamper.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <string>

#include "org/modcpp/bluckbuild/BluckEnvironment.h"
#include "org/modcpp/bluckbuild/Target.h"
#include "org/modcpp/hashing/Md5Hasher.h"
#include "org/modcpp/logging/Console.h"

namespace org::modcpp::bluckbuild {

  using org::modcpp::hashing::Md5Hasher;
  using org::modcpp::logging::Console;
  using org::modcpp::string::String;
  using std::string;

  static std::size_t getFileSize(int fileDescriptor) {
    struct stat statbuf;
    int result = fstat(fileDescriptor, &statbuf);
    assert(result >= 0);
    return statbuf.st_size;
  }

  static void handleFile(Md5Hasher &hasher, const string fileName) {
    int fileDescriptor = ::open(fileName.c_str(), O_RDONLY);
    if (fileDescriptor < 0) {
      Console::error("Tracked file does not exist: %s", fileName.c_str());
      return;
    }

    std::size_t fileSize = getFileSize(fileDescriptor);

    void *fileBuffer = mmap(0, fileSize, PROT_READ, MAP_SHARED, fileDescriptor, 0);
    hasher.put((unsigned char *) fileBuffer, fileSize);

    ::close(fileDescriptor);
  }

  String Stamper::getStamp(const Target &target) const {
    string packagePath = environment.getBluckRootPath() + target.package.substr(1) + "/";
    Md5Hasher hasher;
    for (auto &src : target.srcs) {
      handleFile(hasher, packagePath + src);
    }
    handleFile(hasher, packagePath + "BLUCK");
    return hasher.hash();
  }

  void Stamper::applyStamp(const Target &target) const {
    if (target.isExternal()) {
      return;
    }
    string packageBinPath = environment.getBinFolderPath() + target.package.substr(1) + "/";
    FILE * f = fopen((packageBinPath + target.name + ".stamp").c_str(), "w+");
    assert(f && "File cannot be opened.");
    String hash = getStamp(target);
    fwrite(hash.begin(), 1, hash.length(), f);
    fclose(f);
  }

  bool Stamper::isStampFresh(const Target &target) const {
    if (target.isExternal()) {
      return false;
    }
    char buffer[64];
    string packageBinPath = environment.getBinFolderPath() + target.package.substr(1) + "/";
    FILE * f = fopen((packageBinPath + target.name + ".stamp").c_str(), "r");
    if (!f) {
      return false;
    }
    if (fscanf(f, "%s", buffer) < 0) {
      return false;
    }
    fclose(f);
    return String(buffer) == getStamp(target);
  }

  bool Stamper::checkTestedMark(const Target &target) const {
    return false;
  }

  void Stamper::markTested(const Target &target) const {
    return;
  }

}
