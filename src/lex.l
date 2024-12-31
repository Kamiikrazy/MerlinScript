%option noyywrap

%{
#include <stdio.h>
#include <ctype.h>

typedef enum {
    Num, Iden, Fn, Pub, Loc, If, Else, Orif, Return, Try, While, For, Pass, Break, Do,
    String, Int, Float, Bool, Null, True, False, Module, Or, And, Not, Eq, Ne, Eqeq, Lt, Gt, Le, Ge,
    Add, Sub, Mul, Div, Mod, Inc, Dec, Muleq, Diveq,
    Dot, Comma, Semcol, Col, Dbcol, Bsl, Dbbsl, Dbfsl, Hash, Opb, Clb, Opclb, Clclb,
    Huh, EOL,
} Token;

Token tokenValue;
%}

%%
"if"            { tokenValue = If; printf("TOKEN: If\n"); return tokenValue; }
"otherwise"     { tokenValue = Else; printf("TOKEN: Else\n"); return tokenValue; }
"orif"          { tokenValue = Orif; printf("TOKEN: Orif\n"); return tokenValue; }
"return"        { tokenValue = Return; printf("TOKEN: Return\n"); return tokenValue; }
"True"          { tokenValue = True; printf("TOKEN: True\n"); return tokenValue; }
"False"         { tokenValue = False; printf("TOKEN: False\n"); return tokenValue; }
"int"           { tokenValue = Int; printf("TOKEN: Int\n"); return tokenValue; }
"float"         { tokenValue = Float; printf("TOKEN: Float\n"); return tokenValue; }
"string"        { tokenValue = String; printf("TOKEN: String\n"); return tokenValue; }
"null"          { tokenValue = Null; printf("TOKEN: Null\n"); return tokenValue; }
"bool"          { tokenValue = Bool; printf("TOKEN: Bool\n"); return tokenValue; }
"try"           { tokenValue = Try; printf("TOKEN: Try\n"); return tokenValue; }
"while"         { tokenValue = While; printf("TOKEN: While\n"); return tokenValue; }
"do"            { tokenValue = Do; printf("TOKEN: Do\n"); return tokenValue; }
"pass"          { tokenValue = Pass; printf("TOKEN: Pass\n"); return tokenValue; }
"for"           { tokenValue = For; printf("TOKEN: For\n"); return tokenValue; }
"public"        { tokenValue = Pub; printf("TOKEN: Public\n"); return tokenValue; }
"private"       { tokenValue = Loc; printf("TOKEN: Private\n"); return tokenValue; }
"function"      { tokenValue = Fn; printf("TOKEN: Function\n"); return tokenValue; }
"module"        { tokenValue = Mod; printf("TOKEN: Module\n"); return tokenValue; }

[0-9]+     { tokenValue = Num; printf("TOKEN: Num(%s)\n", yytext); return tokenValue; }

"+"        { tokenValue = Add; printf("TOKEN: Add(+)\n"); return tokenValue; }
"-"        { tokenValue = Sub; printf("TOKEN: Sub(-)\n"); return tokenValue; }
"*"        { tokenValue = Mul; printf("TOKEN: Mul(*)\n"); return tokenValue; }
"/"        { tokenValue = Div; printf("TOKEN: Div(/)\n"); return tokenValue; }
"%"        { tokenValue = Mod; printf("TOKEN: Mod(%)\n"); return tokenValue; }
"="        { tokenValue = Eq; printf("TOKEN: Eq(%)\n"); return tokenValue; }
"=="       { tokenValue = Eqeq; printf("TOKEN: Eqeq(==)\n"); return tokenValue; }
"!="       { tokenValue = Ne; printf("TOKEN: Ne(!=)\n"); return tokenValue; }
"<"        { tokenValue = Lt; printf("TOKEN: Lt(<)\n"); return tokenValue; }
">"        { tokenValue = Gt; printf("TOKEN: Gt(>)\n"); return tokenValue; }
"<="       { tokenValue = Le; printf("TOKEN: Le(<=)\n"); return tokenValue; }
">="       { tokenValue = Ge; printf("TOKEN: Ge(>=)\n"); return tokenValue; }
"++"       { tokenValue = Inc; printf("TOKEN: Inc(++)\n"); return tokenValue; }
"--"       { tokenValue = Dec; printf("TOKEN: Dec(--)\n"); return tokenValue; }

"."        { tokenValue = Dot; printf("TOKEN: Dot(.)\n"); return tokenValue; }
","        { tokenValue = Comma; printf("TOKEN: Comma(,)\n"); return tokenValue; }
";"        { tokenValue = Semcol; printf("TOKEN: Semcol(;)\n"); return tokenValue; }
":"        { tokenValue = Col; printf("TOKEN: Col(:)\n"); return tokenValue; }
"::"       { tokenValue = Dbcol; printf("TOKEN: Dbcol(::)\n"); return tokenValue; }
"("        { tokenValue = Opb; printf("TOKEN: Opb(()\n"); return tokenValue; }
")"        { tokenValue = Clb; printf("TOKEN: Clb())\n"); return tokenValue; }
"{"        { tokenValue = Opclb; printf("TOKEN: Opclb({)\n"); return tokenValue; }
"}"        { tokenValue = Clclb; printf("TOKEN: Clclb(})\n"); return tokenValue; }

[_a-zA-Z]+ { tokenValue = Iden; printf("TOKEN: Identifier\n"); return tokenValue; }

[ \t\n]+   { /* Ignore */ }

.          { tokenValue = Huh; printf("Unrecognised character.\n"); return tokenValue; }
%%

int main(int argc, char **argv) {
    printf("Input: \n");
    while (yylex() != 0);
    return 0;
}