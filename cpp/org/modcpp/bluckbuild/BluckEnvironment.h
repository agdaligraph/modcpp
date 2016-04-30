#ifndef org_modcpp_bluckbuild_BluckEnvironment
#define org_modcpp_bluckbuild_BluckEnvironment

#include <string>

namespace org::modcpp::bluckbuild {

  class BluckEnvironment {
   public:
    BluckEnvironment();

   public:
    void readConfigFile();
    void readConfigFile(const std::string& fileName);

    std::string translatePath(std::string path) const;
    std::string getBluckRootPath() const { return bluckRootPath; }
    std::string getWorkingPath() const { return workingPath; }
    std::string getBinFolderName() const { return binFolderName; }
    std::string getBinFolderPath() const { return bluckRootPath + binFolderName; }
    std::string getCppCompilerCommand() const { return cppCompilerCommand; }
    std::string getCppLinkerFlags() const { return cppLinkerFlags; }
    std::string getCCompilerCommand() const { return cCompilerCommand; }
    bool isDryRun() const { return dryRun; }

   private:
    std::string bluckRootPath;
    std::string workingPath;
    std::string binFolderName;
    std::string cppCompilerCommand;
    std::string cppLinkerFlags;
    std::string cCompilerCommand;
    bool dryRun;
  };

}

#endif
