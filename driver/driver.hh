
#ifndef CIRC_DRIVER_HH
#define CIRC_DRIVER_HH

#include <fstream>
#include <iostream>

#include <unordered_map>
#include <vector>

#include "../parser/parser.hh"

#ifndef yyFlexLexer
#include <FlexLexer.h>
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

  std::unordered_map<size_t, size_t> juncs;
  size_t counter{0};

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

  int get_juncs()
  {
    return max_junc_;
  }

  ~Driver();
};

} // namespace yy

// namespace yy

#endif // CIRC_DRIVER_HH
