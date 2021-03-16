%option c++

%option noyywrap nounput noinput batch debug
%option yylineno

%option yyclass="OurFlexLexer"

%{

#include "../FB_BLD/compiler.tab.hh"
#include "../grammar/parser/parser.hh"

#define YY_USER_ACTION upd_cur_loc();

%}

WS                   [\t\v ]+

DIGIT                [0-9]
DIGIT1               [1-9]

UN_OPER              [\+\-]

INT                  {DIGIT}+
DOUBLE               {DIGIT}+\.{DIGIT}+

NEW_LINE             [\n]+

%%

{WS}                              /* skipping blanks and tabs */

";"                               return yy::parser::token_type::SEMICOLON;
"--"                              return yy::parser::token_type::EDGE;
"V"                               return yy::parser::token_type::VOLT;
","                               return yy::parser::token_type::COMMA;

{UN_OPER}?{INT}                   return yy::parser::token_type::INT;
{UN_OPER}?{DOUBLE}                return yy::parser::token_type::DOUBLE;

{NEW_LINE}                        return yy::parser::token_type::NEW_LINE;

.                                 return yy::parser::token_type::ERR;

%%
