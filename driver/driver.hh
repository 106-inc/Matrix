
#ifndef PARACL_DRIVER_HH
#define PARACL_DRIVER_HH

#include <fstream>
#include <iostream>
#include <vector>

#include "../parser/parser.hh"
#include "../circuits.hh"


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


class Driver final
{
private:
  std::string name_of_file_;
  std::ifstream in_file;

  int max_junc_;

  OurFlexLexer *plex_; // maybe this is good name
  std::vector<std::string> lines_of_prog;

public:
  Driver(const char *name_of_file);

  Driver(const Driver &drvr) = delete;

  Driver &operator=(const Driver &) = delete;

  bool parse();

  void insert(size_t junc1, size_t junc2, float rtor, float voltage);

  parser::token_type yylex(parser::semantic_type *yylval, parser::location_type *yylloc);

  void report_syntax_error(const parser::context &ctx);

  void dump();


    ~Driver();
};

} // namespace yy

// namespace yy

#endif // PARACL_DRIVER_HH
