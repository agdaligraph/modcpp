#ifndef org_modcpp_bluckbuild_CppExecutor
#define org_modcpp_bluckbuild_CppExecutor

#include <vector>
#include <string>

#include "org/modcpp/bluckbuild/BluckBuild.h"
#include "org/modcpp/bluckbuild/BluckEnvironment.h"
#include "org/modcpp/bluckbuild/Target.h"

namespace org::modcpp::bluckbuild {

  class CppExecutor {
   public:
    CppExecutor(const BluckEnvironment &environment);

   public:
    BluckBuild::Result build(const Target &target, bool isTest,
        const std::vector<std::string> &depPaths);
    BluckBuild::Result run(const Target &target, const std::string workingDir = "");
    BluckBuild::Result test(const Target &target);

   private:
    const BluckEnvironment &environment;
  };

} // namespace

#endif
