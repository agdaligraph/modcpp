#ifndef org_modcpp_bluckbuild_PackageExecutor
#define org_modcpp_bluckbuild_PackageExecutor

#include <vector>
#include <string>

#include "org/modcpp/bluckbuild/BluckBuild.h"
#include "org/modcpp/bluckbuild/BluckEnvironment.h"
#include "org/modcpp/bluckbuild/Target.h"

namespace org::modcpp::bluckbuild {

  class PackageExecutor {
   public:
    PackageExecutor(const BluckEnvironment &environment);

   public:
    BluckBuild::Result build(const Target &target) const;

   private:
    const BluckEnvironment &environment;
  };

} // namespace

#endif
