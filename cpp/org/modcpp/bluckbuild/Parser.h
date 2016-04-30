#ifndef org_modcpp_bluckbuild_Parser
#define org_modcpp_bluckbuild_Parser

#include <string>
#include <vector>

#include "org/modcpp/bluckbuild/Target.h"

namespace org::modcpp::bluckbuild {

  class Parser {
   public:
    Parser(std::string fileName);
    ~Parser();

   public:
    bool populateTarget(Target &target) const;

   testvisible:
    static void expandLocalDeps(Target &target);
    static void toStandardForm(Target &target);
    static bool parseLiteral(FILE* file, char buffer[]);
    static bool parseArray(FILE* file, char buffer[], std::vector<std::string> &literals);

   private:
    std::string fileName;
  };

} // namespace

#endif
