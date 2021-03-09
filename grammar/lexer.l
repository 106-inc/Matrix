%option c++

%option noyywrap
%option yylineno

%option warn nodefault debug noinput
%option yyclass="OurFlexLexer"

%{

#include "../FB_BLD/compiler.tab.hh"
#include "../parser/parser.hh"

#define YY_USER_ACTION upd_cur_loc();

%}

WS                  [ \t\v]+
COMM                "//".*"\n"

DIGIT                [0-9]
DIGIT1               [1-9]

INT                 {DIGIT1}{DIGIT1}*
DOUBLE              {DIGIT}+\.{DIGIT}+

NEW_LINE            [\n]+

%%

{WS}                /* skipping blanks and tabs */
{COMM}              /* skipping comments to the end of lne */

";"                 return yy::parser::token_type::SEMICOLON;
"--"                return yy::parser::token_type::EDGE;
"V"                 return yy::parser::token_type::VOLT;
","                 return yy::parser::token_type::COMMA;

{INT}               return yy::parser::token_type::INT;
{DOUBLE_DIGIT}      return yy::parser::token_type::DOUBLE;

{NEW_LINE}          return yy::parser::token_type::NEW_LINE;

%%

