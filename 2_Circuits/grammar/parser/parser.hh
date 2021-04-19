#ifndef CIRC_PARSER_HH
#define CIRC_PARSER_HH

#include <cctype>
#include <iostream>

#ifndef yyFlexLexer
#include <FlexLexer.h>
#endif /* yyFlexLexer */

#include "circuits.hh"
#include "compiler.tab.hh"

enum RES_OF_PARSE
{
  SUCESS_PARSE,
  ERROR_IN_PARSE
};
class OurFlexLexer : public yyFlexLexer
{
private:
  yy::location cur_location_{};
  int last_num_of_line;

public:
  OurFlexLexer() = default;

  OurFlexLexer(const OurFlexLexer &flx) = delete;
  OurFlexLexer &operator=(const OurFlexLexer &) = delete;

  yy::location get_cur_location();
  int get_last_line_();

  bool is_empty_line(const char *str);

  void upd_cur_loc();

  int yylex() override;

  ~OurFlexLexer() = default;
};

#endif // CIRC_PARSER_HH