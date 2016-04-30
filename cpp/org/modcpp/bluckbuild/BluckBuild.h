#ifndef org_modcpp_bluckbuild_BluckBuild
#define org_modcpp_bluckbuild_BluckBuild

#include <string>
#include <unordered_set>
#include <vector>

#include "org/modcpp/bluckbuild/BluckEnvironment.h"
#include "org/modcpp/bluckbuild/Target.h"

namespace org::modcpp::bluckbuild {

  class BluckBuild {
   public:
    enum Result {
      Fresh = 1,
      Success = 2,
      Fail = 3,
    };

   public:
    BluckBuild();
    ~BluckBuild() {}

   public:
    Result buildTarget(const std::string &path);
    Result runTarget(const std::string &path);
    Result testTarget(const std::string &path);
    Result cleanTarget(const std::string &path);

   private:
    bool isTargetSourcesIntact(const Target &) const;
    void stampTargetSources(const Target &) const;
    Target readTarget(const std::string &bluckPath) const;
    Result buildTargetSelf(const Target &, bool isTest,
        const std::vector<std::string> &) const;
    Result buildTargetRecursive(const Target &, bool isTest,
        std::vector<std::string> &);

   private:
    std::unordered_set<std::string> parentTargetPaths;
    std::unordered_set<std::string> builtTargetPaths;
    BluckEnvironment environment;
  };

} // namespace

#endif
