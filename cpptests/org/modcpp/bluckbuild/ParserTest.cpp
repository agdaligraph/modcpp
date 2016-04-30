#include "org/modcpp/bluckbuild/Parser.h"

#include <cassert>

#include "org/modcpp/bluckbuild/Target.h"

using org::modcpp::bluckbuild::Parser;
using org::modcpp::bluckbuild::Target;

void test_fakeBluck() {
  Parser parser("FAKEBLUCK");
  Target target;

  target.name = "StringTest";
  parser.populateTarget(target);

  target.name = "StringBuilderTest";
  parser.populateTarget(target);

  target.name = "StringBuilderBenchmark";
  parser.populateTarget(target);

  target.name = "EncodingTest";
  parser.populateTarget(target);

  target.name = "StringTest";
  parser.populateTarget(target);

  target.name = "StringBenchmark";
  parser.populateTarget(target);

  target.name = "bluck";
  parser.populateTarget(target);

  target.name = "Parser";
  parser.populateTarget(target);
}

void test_expandLocalDeps() {
  Target target;
  target.name = "service";
  target.package = "//cpp/org/selenium/service";
  target.deps = {":localDep1", ":localDep2", "//cpp/org/selenium/service2:entities", ":localDep3"};
  
  Parser::expandLocalDeps(target);

  assert(target.deps[0] == "//cpp/org/selenium/service:localDep1");
  assert(target.deps[1] == "//cpp/org/selenium/service:localDep2");
  assert(target.deps[2] == "//cpp/org/selenium/service2:entities");
  assert(target.deps[3] == "//cpp/org/selenium/service:localDep3");
}

int main() {
  test_fakeBluck();
  test_expandLocalDeps();
  return 0;
}
