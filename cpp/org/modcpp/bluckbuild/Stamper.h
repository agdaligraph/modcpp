#ifndef org_modcpp_bluckbuild_Stamper
#define org_modcpp_bluckbuild_Stamper

#include "org/modcpp/string/String.h"

namespace org::modcpp::bluckbuild {

  using org::modcpp::string::String;
  class BluckEnvironment;
  class Target;

  class Stamper {
   public:
     Stamper(const BluckEnvironment &environment) : environment(environment) {}

   public:
     bool isStampFresh(const Target &) const;
     void applyStamp(const Target &) const;
     String getStamp(const Target &) const;
     bool checkTestedMark(const Target &) const;
     void markTested(const Target &) const;

   private:
     const BluckEnvironment &environment;
  };

}

#endif
