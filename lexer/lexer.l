%{
#include <cctype>
#include <cstdio>
#include <string>
#define YYEOF 0
int yylex();
void checker();
int main() {
  do {
  } while (yylex() != YYEOF);
}
std::string yyloc = "<stdin>";
char  sol[] = "[StartOfLine] ";
char  ls[] = "[LeadingSpace] ";
char solls[] = "[StartOfLine] [LeadingSpace] ";
char nosp[] = "";
char * loctext;
int yyrow = 1, yycolumn = 1, yycolpre = 1,newrow = 1;
#define YY_USER_ACTION                                                         \
  do {                                                                         \
    yycolumn += yyleng;                                                        \
  } while (0);
%}
%option noyywrap
%%
#.* {
  std::string s(yytext);
  auto l = s.find("\""), r = s.rfind("\"");
  yyloc = s.substr(l + 1, r - l - 1);
  for (int i = 0; i < s.size(); ++i)
    if (std::isdigit(s[i])) {
      for (yyrow = 0; i < s.size() && std::isdigit(s[i]); ++i)
        yyrow = yyrow * 10 + s[i] - '0';
      --yyrow;
      break;
    }
}
\n {
  ++yyrow;
  yycolpre = yycolumn;
  yycolumn = 1;
  newrow = 1;
}
[\f\r\t\v ] {}
\"(\\.|[^"\\])*\" {
  checker();
  std::fprintf(yyout, "string_literal '%s'\t\t%sLoc=<%s:%d:%d>\n",
               yytext,loctext,yyloc.c_str(), yyrow, yycolumn - yyleng);
  return ~YYEOF;
}
int {
  checker();
  std::fprintf(yyout, "int '%s'\t\t%sLoc=<%s:%d:%d>\n", yytext,loctext, yyloc.c_str(),
               yyrow, yycolumn - yyleng);
  return ~YYEOF;
}
float {
  checker();
  std::fprintf(yyout, "float '%s'\t\t%sLoc=<%s:%d:%d>\n", yytext,loctext, yyloc.c_str(),
               yyrow, yycolumn - yyleng);
  return ~YYEOF;
}
long {
  checker();
  std::fprintf(yyout, "long '%s'\t\t%sLoc=<%s:%d:%d>\n", yytext,loctext, yyloc.c_str(),
               yyrow, yycolumn - yyleng);
  return ~YYEOF;
}
return {
  checker();
  std::fprintf(yyout, "return '%s'\t\t%sLoc=<%s:%d:%d>\n", yytext,loctext, yyloc.c_str(),
               yyrow, yycolumn - yyleng);
  return ~YYEOF;
}
\( {
  checker();
  std::fprintf(yyout, "l_paren '%s'\t\t%sLoc=<%s:%d:%d>\n", yytext,loctext, yyloc.c_str(),
               yyrow, yycolumn - yyleng);
  return ~YYEOF;
}
\) {
  checker();
  std::fprintf(yyout, "r_paren '%s'\t\t%sLoc=<%s:%d:%d>\n", yytext,loctext, yyloc.c_str(),
               yyrow, yycolumn - yyleng);
  return ~YYEOF;
}
\{ {
  checker();
  std::fprintf(yyout, "l_brace '%s'\t\t%sLoc=<%s:%d:%d>\n", yytext,loctext, yyloc.c_str(),
               yyrow, yycolumn - yyleng);
  return ~YYEOF;
}
\} {
  checker();
  std::fprintf(yyout, "r_brace '%s'\t\t%sLoc=<%s:%d:%d>\n", yytext,loctext, yyloc.c_str(),
               yyrow, yycolumn - yyleng);
  return ~YYEOF;
}
; {
  checker();
  std::fprintf(yyout, "semi '%s'\t\t%sLoc=<%s:%d:%d>\n", yytext,loctext, yyloc.c_str(),
               yyrow, yycolumn - yyleng);
  return ~YYEOF;
}
[0-9]+(\.[0-9]*)?((e|E)(\+|-)?[0-9]+)?|0(x|X)[0-9a-fA-F]+(\.[0-9a-fA-F]*)?((p|P)(\+|-)?[0-9a-fA-F]+)?|\.[0-9]+((e|E)(\+|-)?[0-9]+)?|0(x|X)(\.[0-9a-fA-F]+)((p|P)(\+|-)?[0-9a-fA-F]+)? {
  checker();
  std::fprintf(yyout, "numeric_constant '%s'\t\t%sLoc=<%s:%d:%d>\n", yytext,loctext,
               yyloc.c_str(), yyrow, yycolumn - yyleng);
  return ~YYEOF;
}
const {
  checker();
  std::fprintf(yyout, "const '%s'\t\t%sLoc=<%s:%d:%d>\n",
               yytext,loctext,yyloc.c_str(), yyrow, yycolumn - yyleng);
  return ~YYEOF;
}
, {
  checker();
  std::fprintf(yyout, "comma '%s'\t\t%sLoc=<%s:%d:%d>\n",
               yytext,loctext,yyloc.c_str(), yyrow, yycolumn - yyleng);
  return ~YYEOF;
}
\[ {
  checker();
  std::fprintf(yyout, "l_square '%s'\t\t%sLoc=<%s:%d:%d>\n",
               yytext,loctext,yyloc.c_str(), yyrow, yycolumn - yyleng);
  return ~YYEOF;
}
\] {
  checker();
  std::fprintf(yyout, "r_square '%s'\t\t%sLoc=<%s:%d:%d>\n",
               yytext,loctext,yyloc.c_str(), yyrow, yycolumn - yyleng);
  return ~YYEOF;
}
= {
  checker();
  std::fprintf(yyout, "equal '%s'\t\t%sLoc=<%s:%d:%d>\n",
               yytext,loctext,yyloc.c_str(), yyrow, yycolumn - yyleng);
  return ~YYEOF;
}
"+=" {
  checker();
  std::fprintf(yyout, "plusequal '%s'\t\t%sLoc=<%s:%d:%d>\n",
               yytext,loctext,yyloc.c_str(), yyrow, yycolumn - yyleng);
  return ~YYEOF;
}
"-=" {
  checker();
  std::fprintf(yyout, "minusequal '%s'\t\t%sLoc=<%s:%d:%d>\n",
               yytext,loctext,yyloc.c_str(), yyrow, yycolumn - yyleng);
  return ~YYEOF;
}
"*=" {
  checker();
  std::fprintf(yyout, "starequal '%s'\t\t%sLoc=<%s:%d:%d>\n",
               yytext,loctext,yyloc.c_str(), yyrow, yycolumn - yyleng);
  return ~YYEOF;
}
"/=" {
  checker();
  std::fprintf(yyout, "slashequal '%s'\t\t%sLoc=<%s:%d:%d>\n",
               yytext,loctext,yyloc.c_str(), yyrow, yycolumn - yyleng);
  return ~YYEOF;
}
"&=" {
  checker();
  std::fprintf(yyout, "ampequal '%s'\t\t%sLoc=<%s:%d:%d>\n",
               yytext,loctext,yyloc.c_str(), yyrow, yycolumn - yyleng);
  return ~YYEOF;
}
"^=" {
  checker();
  std::fprintf(yyout, "caretequal '%s'\t\t%sLoc=<%s:%d:%d>\n",
               yytext,loctext,yyloc.c_str(), yyrow, yycolumn - yyleng);
  return ~YYEOF;
}
"|=" {
  checker();
  std::fprintf(yyout, "pipeequal '%s'\t\t%sLoc=<%s:%d:%d>\n",
               yytext,loctext,yyloc.c_str(), yyrow, yycolumn - yyleng);
  return ~YYEOF;
}
"<<=" {
  checker();
  std::fprintf(yyout, "lesslessequal '%s'\t\t%sLoc=<%s:%d:%d>\n",
               yytext,loctext,yyloc.c_str(), yyrow, yycolumn - yyleng);
  return ~YYEOF;
}
">>=" {
  checker();
  std::fprintf(yyout, "greatergreaterequal '%s'\t\t%sLoc=<%s:%d:%d>\n",
               yytext,loctext,yyloc.c_str(), yyrow, yycolumn - yyleng);
  return ~YYEOF;
}
"%=" {
  checker();
  std::fprintf(yyout, "percentequal '%s'\t\t%sLoc=<%s:%d:%d>\n",
               yytext,loctext,yyloc.c_str(), yyrow, yycolumn - yyleng);
  return ~YYEOF;
}
void {
  checker();
  std::fprintf(yyout, "void '%s'\t\t%sLoc=<%s:%d:%d>\n",
               yytext,loctext,yyloc.c_str(), yyrow, yycolumn - yyleng);
  return ~YYEOF;
}
if {
  checker();
  std::fprintf(yyout, "if '%s'\t\t%sLoc=<%s:%d:%d>\n",
               yytext,loctext,yyloc.c_str(), yyrow, yycolumn - yyleng);
  return ~YYEOF;
}
else {
  checker();
  std::fprintf(yyout, "else '%s'\t\t%sLoc=<%s:%d:%d>\n",
               yytext,loctext,yyloc.c_str(), yyrow, yycolumn - yyleng);
  return ~YYEOF;
}
while {
  checker();
  std::fprintf(yyout, "while '%s'\t\t%sLoc=<%s:%d:%d>\n",
               yytext,loctext,yyloc.c_str(), yyrow, yycolumn - yyleng);
  return ~YYEOF;
}
do {
  checker();
  std::fprintf(yyout, "do '%s'\t\t%sLoc=<%s:%d:%d>\n",
               yytext,loctext,yyloc.c_str(), yyrow, yycolumn - yyleng);
  return ~YYEOF;
}
for {
  checker();
  std::fprintf(yyout, "for '%s'\t\t%sLoc=<%s:%d:%d>\n",
               yytext,loctext,yyloc.c_str(), yyrow, yycolumn - yyleng);
  return ~YYEOF;
}
break {
  checker();
  std::fprintf(yyout, "break '%s'\t\t%sLoc=<%s:%d:%d>\n",
               yytext,loctext,yyloc.c_str(), yyrow, yycolumn - yyleng);
  return ~YYEOF;
}
continue {
  checker();
  std::fprintf(yyout, "continue '%s'\t\t%sLoc=<%s:%d:%d>\n",
               yytext,loctext,yyloc.c_str(), yyrow, yycolumn - yyleng);
  return ~YYEOF;
}
"~" {
  checker();
  std::fprintf(yyout, "tilde '%s'\t\t%sLoc=<%s:%d:%d>\n",
               yytext,loctext,yyloc.c_str(), yyrow, yycolumn - yyleng);
  return ~YYEOF;
}
"+" {
  checker();
  std::fprintf(yyout, "plus '%s'\t\t%sLoc=<%s:%d:%d>\n",
               yytext,loctext,yyloc.c_str(), yyrow, yycolumn - yyleng);
  return ~YYEOF;
}
"-" {
  checker();
  std::fprintf(yyout, "minus '%s'\t\t%sLoc=<%s:%d:%d>\n",
               yytext,loctext,yyloc.c_str(), yyrow, yycolumn - yyleng);
  return ~YYEOF;
}
! {
  checker();
  std::fprintf(yyout, "exclaim '%s'\t\t%sLoc=<%s:%d:%d>\n",
               yytext,loctext,yyloc.c_str(), yyrow, yycolumn - yyleng);
  return ~YYEOF;
}
"*" {
  checker();
  std::fprintf(yyout, "star '%s'\t\t%sLoc=<%s:%d:%d>\n",
               yytext,loctext,yyloc.c_str(), yyrow, yycolumn - yyleng);
  return ~YYEOF;
}
"/" {
  checker();
  std::fprintf(yyout, "slash '%s'\t\t%sLoc=<%s:%d:%d>\n",
               yytext,loctext,yyloc.c_str(), yyrow, yycolumn - yyleng);
  return ~YYEOF;
}
"&" {
  checker();
  std::fprintf(yyout, "amp '%s'\t\t%sLoc=<%s:%d:%d>\n",
               yytext,loctext,yyloc.c_str(), yyrow, yycolumn - yyleng);
  return ~YYEOF;
}
"^" {
  checker();
  std::fprintf(yyout, "caret '%s'\t\t%sLoc=<%s:%d:%d>\n",
               yytext,loctext,yyloc.c_str(), yyrow, yycolumn - yyleng);
  return ~YYEOF;
}
"|" {
  checker();
  std::fprintf(yyout, "pipe '%s'\t\t%sLoc=<%s:%d:%d>\n",
               yytext,loctext,yyloc.c_str(), yyrow, yycolumn - yyleng);
  return ~YYEOF;
}
"<<" {
  checker();
  std::fprintf(yyout, "lessless '%s'\t\t%sLoc=<%s:%d:%d>\n",
               yytext,loctext,yyloc.c_str(), yyrow, yycolumn - yyleng);
  return ~YYEOF;
}
">>" {
  checker();
  std::fprintf(yyout, "greatergreater '%s'\t\t%sLoc=<%s:%d:%d>\n",
               yytext,loctext,yyloc.c_str(), yyrow, yycolumn - yyleng);
  return ~YYEOF;
}
% {
  checker();
  std::fprintf(yyout, "percent '%s'\t\t%sLoc=<%s:%d:%d>\n",
               yytext,loctext,yyloc.c_str(), yyrow, yycolumn - yyleng);
  return ~YYEOF;
}
"<" {
  checker();
  std::fprintf(yyout, "less '%s'\t\t%sLoc=<%s:%d:%d>\n",
               yytext,loctext,yyloc.c_str(), yyrow, yycolumn - yyleng);
  return ~YYEOF;
}
">" {
  checker();
  std::fprintf(yyout, "greater '%s'\t\t%sLoc=<%s:%d:%d>\n",
               yytext,loctext,yyloc.c_str(), yyrow, yycolumn - yyleng);
  return ~YYEOF;
}
"<=" {
  checker();
  std::fprintf(yyout, "lessequal '%s'\t\t%sLoc=<%s:%d:%d>\n",
               yytext,loctext,yyloc.c_str(), yyrow, yycolumn - yyleng);
  return ~YYEOF;
}
">=" {
  checker();
  std::fprintf(yyout, "greaterequal '%s'\t\t%sLoc=<%s:%d:%d>\n",
               yytext,loctext,yyloc.c_str(), yyrow, yycolumn - yyleng);
  return ~YYEOF;
}
"==" {
  checker();
  std::fprintf(yyout, "equalequal '%s'\t\t%sLoc=<%s:%d:%d>\n",
               yytext,loctext,yyloc.c_str(), yyrow, yycolumn - yyleng);
  return ~YYEOF;
}
!= {
  checker();
  std::fprintf(yyout, "exclaimequal '%s'\t\t%sLoc=<%s:%d:%d>\n",
               yytext,loctext,yyloc.c_str(), yyrow, yycolumn - yyleng);
  return ~YYEOF;
}
&& {
  checker();
  std::fprintf(yyout, "ampamp '%s'\t\t%sLoc=<%s:%d:%d>\n",
               yytext,loctext,yyloc.c_str(), yyrow, yycolumn - yyleng);
  return ~YYEOF;
}
"||" {
  checker();
  std::fprintf(yyout, "pipepipe '%s'\t\t%sLoc=<%s:%d:%d>\n",
               yytext,loctext,yyloc.c_str(), yyrow, yycolumn - yyleng);
  return ~YYEOF;
}
"++" {
  checker();
  std::fprintf(yyout, "plusplus '%s'\t\t%sLoc=<%s:%d:%d>\n",
               yytext,loctext,yyloc.c_str(), yyrow, yycolumn - yyleng);
  return ~YYEOF;
}
"--" {
  checker();
  std::fprintf(yyout, "minusminus '%s'\t\t%sLoc=<%s:%d:%d>\n",
               yytext,loctext,yyloc.c_str(), yyrow, yycolumn - yyleng);
  return ~YYEOF;
}
char {
  checker();
  std::fprintf(yyout, "char '%s'\t\t%sLoc=<%s:%d:%d>\n",
               yytext,loctext,yyloc.c_str(), yyrow, yycolumn - yyleng);
  return ~YYEOF;
}
[a-zA-Z_][a-zA-Z_0-9]* {
  checker();
  std::fprintf(yyout, "identifier '%s'\t\t%sLoc=<%s:%d:%d>\n", yytext,loctext,
               yyloc.c_str(), yyrow, yycolumn - yyleng);
  return ~YYEOF;
}
\.\.\. {
  checker();
  std::fprintf(yyout, "ellipsis '%s'\t\t%sLoc=<%s:%d:%d>\n", yytext,loctext,
               yyloc.c_str(), yyrow, yycolumn - yyleng);
  return ~YYEOF;
}
<<EOF>> {
  std::fprintf(yyout, "eof ''\t\tLoc=<%s:%d:%d>\n", yyloc.c_str(), yyrow - 1,
               yycolpre - yyleng);
  return YYEOF;
}
. {
  checker();
  std::fprintf(yyout, "unknown '%s'\t\t%sLoc=<%s:%d:%d>\n", yytext,loctext, yyloc.c_str(),
               yyrow, yycolumn - yyleng);
  return ~YYEOF;
}

%%

void checker()
{ loctext = nosp;
  if(newrow == 1)
  { loctext = sol;
      if(yycolumn - yyleng != 1)
        loctext = solls;
    newrow = 0;
  }
  else
  {
    if(yycolumn - yyleng != yycolpre)
      loctext = ls;
  }
  yycolpre = yycolumn;
}