#include "org/modcpp/bluckbuild/Stamper.h"

#include <iostream>
#include <unistd.h>

#include "org/modcpp/bluckbuild/BluckEnvironment.h"
#include "org/modcpp/bluckbuild/Stamper.h"
#include "org/modcpp/bluckbuild/Target.h"

using org::modcpp::bluckbuild::BluckEnvironment;
using org::modcpp::bluckbuild::Stamper;
using org::modcpp::bluckbuild::Target;

void test_applyStamp() {
  BluckEnvironment environment;
  environment.readConfigFile();

  Target target;
  target.type = Target::Type::CppLibrary;
  target.package = "//cpptests/org/modcpp/bluckbuild";
  target.name = "test";

  target.srcs.push_back("FAKEBLUCK");
  target.srcs.push_back("StamperTest.cpp");

  Stamper stamper(environment);
  printf("\n asdf\n");
  stamper.applyStamp(target);
  printf("\n masdf\n");
}

int main() {
  test_applyStamp(); 
  return 0;
}
