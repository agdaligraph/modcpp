#ifndef org_modcpp_bluckbuild_Target
#define org_modcpp_bluckbuild_Target

#include <vector>
#include <string>

namespace org::modcpp::bluckbuild {

  class Target {
   public:
    enum Type {
      CppBinary = 0,
      CppLibrary,
      CppTest,
      CppOther,
      JavaBinary = 4,
      JavaLibrary,
      JavaTest,
      JavaOther,
      External = 11,
    };

    enum Artifact {
      Binary,
      Library,
      Test,
    };

    enum Language {
      Cpp,
      Java,
    };

    Type type;
    std::string package;
    std::string name;
    std::vector<std::string> srcs;
    std::vector<std::string> deps;
    std::vector<std::string> includes;
    std::vector<std::string> compileopts;
    std::vector<std::string> linkopts;

   public:
    std::string getBluckPath() const { return package + ":" + name; }
    Artifact getArtifact() const { return static_cast<Artifact> (type % 4); }
    Language getLanguage() const { return static_cast<Language> (type / 4); }
    bool isExternal() const { return type == Type::External; }
  };

} // namespace

#endif
