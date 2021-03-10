%language "c++"
%skeleton "lalr1.cc"


%param {Driver* driver}
%locations

%define api.value.type variant
%define parse.error custom

%code requires
{
namespace yy { class Driver; };

#include <string>
}

%code
{
#include "../driver/driver.hh"

namespace yy
{
parser::token_type yylex(parser::semantic_type* yylval, parser::location_type* yylloc, Driver* driver);
};
}

%token  

SEMICOLON      ";"
EDGE           "--"
VOLT           "V"
COMMA          ","
NEW_LINE       "\\n"
;

%token <int>   INT
%token <float> DOUBLE

%nterm <int>   junction
%nterm <float> resistor
%nterm <float> voltage

%%

program:      line                            { /* program starting */};
	    |

line:         expr                            {};
            | expr NEW_LINE                   {};
            | NEW_LINE                        {};

expr:        junc EDGE junction COMMA
             resistr SEMICOLON voltage       { driver->insert($1, $3, $5, $7); };

junction:    INT                              { $$ = $1; };

resistor:    INT                              { $$ = $1; };
           | DOUBLE                           { $$ = $1; };

voltage:     INT VOLT                         { $$ = $1; };
           | DOUBLE VOLT                      { $$ = $1; };
           |                                  { $$ = 0.0; };

%%

namespace yy
{

  void parser::error (const parser::location_type& location, const std::string& string)
  {
      std::cerr << string << " in (line.column): "<< location << std::endl;
  }

  parser::token_type yylex(parser::semantic_type* yylval, parser::location_type* yylloc, Driver* driver)
  {
       return driver->yylex(yylval, yylloc);
  }

  void parser::report_syntax_error(parser::context const& ctx) const
  {
      driver->report_syntax_error(ctx);
  }

}
