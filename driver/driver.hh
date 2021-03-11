
#ifndef PARACL_DRIVER_HH
#define PARACL_DRIVER_HH

#include <fstream>
#include <iostream>
#include <vector>

#include "../parser/parser.hh"

#ifndef yyFlexLexer
#include <FlexLexer.h>
#endif /* yyFlexLexer */

#ifndef yyFlexLexer
//#include "../FB_BLD/lex.yy.cc"
#endif /* yyFlexLexer */

enum
{
  NUM_OF_TOKENS = 30
};

namespace yy
{

struct Edge
{
  int junc1, junc2;
  float rtor;
  float voltage;
};

class Driver final
{
private:
  std::string name_of_file_;
  std::ifstream in_file;

  std::vector<Edge> edges_;
  int max_junc_;

  OurFlexLexer *plex_; // maybe this is good name
  std::vector<std::string> lines_of_prog;

public:
  Driver(const char *name_of_file);

  Driver(const Driver &drvr) = delete;

  Driver &operator=(const Driver &) = delete;

  bool parse();

  void insert(int jnction1, int jnction2, float resistor, float voltage);

  parser::token_type yylex(parser::semantic_type *yylval, parser::location_type *yylloc);

  void report_syntax_error(const parser::context &ctx);

  void dump();

  ~Driver();
};

} // namespace yy

// namespace yy

#endif // PARACL_DRIVER_HH
