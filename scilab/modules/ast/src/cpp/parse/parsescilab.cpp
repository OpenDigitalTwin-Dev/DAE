/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
 // -*- C++ -*-
/*
 *  Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2010 - DIGITEO - Bruno JOFRET
 *  Copyright (C) 2012 - 2016 - Scilab Enterprises
 *  Copyright (C) 2023 - Dassault Systemes - Bruno JOFRET
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */
#define YYERROR_VERBOSE 1

#define YYDEBUG 1

#define YYLTYPE Location

/*
** This build the tree in verbose mode
** for instance adding CommentExp
** where nothing is needed.
*/
//#define BUILD_DEBUG_AST

#include <string>
#include <sstream>
#include <list>
#include "all.hxx"
#include "parse.hxx"
#include "parser_private.hxx"
#include "location.hxx"
#include "symbol.hxx"
#include "charEncoding.h"
#include "sci_malloc.h"

// #define DEBUG_RULES
#ifdef DEBUG_RULES
    #include <iomanip>
#endif

static void print_rules(const std::string& _parent, const std::string& _rules)
{
#ifdef DEBUG_RULES
    static std::list<std::pair<std::string, std::string> > rules;
    // add a space to perform a find as whole word of _parent in _rules
    rules.emplace_front(_parent+" ", _rules+" ");

    if(_parent == "program")
    {
        std::list<std::pair<std::string, std::string> > last;
        int spaces = 5; // 5 is the size of "|_./ "

        std::cout <<  "--- RULES ---" << std::endl;
        std::cout <<  "|_./ " << _parent << " : " << _rules << std::endl;

        last.emplace_back(rules.front());
        rules.pop_front();
        for(auto r : rules)
        {
            size_t pos = last.back().second.find(r.first);
            while(pos == std::string::npos)
            {
                spaces -= 2;
                last.pop_back();
                if(last.empty())
                {
                    break;
                }
                pos = last.back().second.find(r.first);
            }

            if(last.empty() == false)
            {
                last.back().second.erase(pos, r.first.length());
            }

            spaces += 2;
            last.emplace_back(r);

            std::setfill(" ");
            std::cout << std::setw(spaces) << "|_./ " << r.first << ": " << r.second << std::endl;
        }

        rules.clear();
    }
#endif
}

static void print_rules(const std::string& _parent, const double _value)
{
#ifdef DEBUG_RULES
    std::stringstream ostr;
    ostr << _value;
    print_rules(_parent, ostr.str());
#endif
}

#define StopOnError()                                           \
    {                                                           \
        if(ParserSingleInstance::stopOnFirstError())            \
        {                                                       \
            ParserSingleInstance::setExitStatus(Parser::ParserStatus::Failed);       \
        }                                                       \
    }

#define SetTree(PTR)                                                \
    {                                                               \
        if(ParserSingleInstance::getExitStatus() == Parser::Failed) \
        {                                                           \
            delete PTR;                                             \
            ParserSingleInstance::setTree(nullptr);                 \
        }                                                           \
        else                                                        \
        {                                                           \
            ParserSingleInstance::setTree(PTR);                     \
        }                                                           \
    }




# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    DOTS = 258,                    /* "line break"  */
    EOL = 259,                     /* "end of line"  */
    SPACES = 260,                  /* "spaces"  */
    BOOLTRUE = 261,                /* "%t or %T"  */
    BOOLFALSE = 262,               /* "%f or %F"  */
    QUOTE = 263,                   /* "'"  */
    NOT = 264,                     /* "~ or @"  */
    ARROW = 265,                   /* "->"  */
    SHARP = 266,                   /* "#"  */
    DOLLAR = 267,                  /* "$"  */
    COMMA = 268,                   /* ","  */
    COLON = 269,                   /* ":"  */
    SEMI = 270,                    /* ";"  */
    LPAREN = 271,                  /* "("  */
    RPAREN = 272,                  /* ")"  */
    LBRACK = 273,                  /* "["  */
    RBRACK = 274,                  /* "]"  */
    LBRACE = 275,                  /* "{"  */
    RBRACE = 276,                  /* "}"  */
    DOT = 277,                     /* "."  */
    DOTQUOTE = 278,                /* ".'"  */
    PLUS = 279,                    /* "+"  */
    MINUS = 280,                   /* "-"  */
    TIMES = 281,                   /* "*"  */
    DOTTIMES = 282,                /* ".*"  */
    KRONTIMES = 283,               /* ".*."  */
    CONTROLTIMES = 284,            /* "*."  */
    RDIVIDE = 285,                 /* "/"  */
    DOTRDIVIDE = 286,              /* "./"  */
    CONTROLRDIVIDE = 287,          /* "/."  */
    KRONRDIVIDE = 288,             /* "./."  */
    LDIVIDE = 289,                 /* "\\"  */
    DOTLDIVIDE = 290,              /* ".\\"  */
    CONTROLLDIVIDE = 291,          /* "\\."  */
    KRONLDIVIDE = 292,             /* ".\\."  */
    POWER = 293,                   /* "** or ^"  */
    DOTPOWER = 294,                /* ".^"  */
    EQ = 295,                      /* "=="  */
    NE = 296,                      /* "<> or ~="  */
    LT = 297,                      /* "<"  */
    LE = 298,                      /* "<="  */
    GT = 299,                      /* ">"  */
    GE = 300,                      /* ">="  */
    AND = 301,                     /* "&"  */
    ANDAND = 302,                  /* "&&"  */
    OR = 303,                      /* "|"  */
    OROR = 304,                    /* "||"  */
    ASSIGN = 305,                  /* "="  */
    ARGUMENTS = 306,               /* "arguments"  */
    IF = 307,                      /* "if"  */
    THEN = 308,                    /* "then"  */
    ELSE = 309,                    /* "else"  */
    ELSEIF = 310,                  /* "elseif"  */
    END = 311,                     /* "end"  */
    SELECT = 312,                  /* "select"  */
    SWITCH = 313,                  /* "switch"  */
    CASE = 314,                    /* "case"  */
    OTHERWISE = 315,               /* "otherwise"  */
    FUNCTION = 316,                /* "function"  */
    ENDFUNCTION = 317,             /* "endfunction"  */
    FOR = 318,                     /* "for"  */
    WHILE = 319,                   /* "while"  */
    DO = 320,                      /* "do"  */
    BREAK = 321,                   /* "break"  */
    CONTINUE = 322,                /* "continue"  */
    TRY = 323,                     /* "try"  */
    CATCH = 324,                   /* "catch"  */
    RETURN = 325,                  /* "return"  */
    FLEX_ERROR = 326,              /* FLEX_ERROR  */
    STR = 327,                     /* "string"  */
    ID = 328,                      /* "identifier"  */
    VARINT = 329,                  /* "integer"  */
    VARFLOAT = 330,                /* "float"  */
    COMPLEXNUM = 331,              /* "complex number"  */
    NUM = 332,                     /* "number"  */
    PATH = 333,                    /* "path"  */
    COMMENT = 334,                 /* "line comment"  */
    BLOCKCOMMENT = 335,            /* "block comment"  */
    TOPLEVEL = 336,                /* TOPLEVEL  */
    HIGHLEVEL = 337,               /* HIGHLEVEL  */
    UPLEVEL = 338,                 /* UPLEVEL  */
    LISTABLE = 339,                /* LISTABLE  */
    CONTROLBREAK = 340,            /* CONTROLBREAK  */
    UMINUS = 341,                  /* UMINUS  */
    UPLUS = 342,                   /* UPLUS  */
    FUNCTIONCALL = 343             /* FUNCTIONCALL  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define DOTS 258
#define EOL 259
#define SPACES 260
#define BOOLTRUE 261
#define BOOLFALSE 262
#define QUOTE 263
#define NOT 264
#define ARROW 265
#define SHARP 266
#define DOLLAR 267
#define COMMA 268
#define COLON 269
#define SEMI 270
#define LPAREN 271
#define RPAREN 272
#define LBRACK 273
#define RBRACK 274
#define LBRACE 275
#define RBRACE 276
#define DOT 277
#define DOTQUOTE 278
#define PLUS 279
#define MINUS 280
#define TIMES 281
#define DOTTIMES 282
#define KRONTIMES 283
#define CONTROLTIMES 284
#define RDIVIDE 285
#define DOTRDIVIDE 286
#define CONTROLRDIVIDE 287
#define KRONRDIVIDE 288
#define LDIVIDE 289
#define DOTLDIVIDE 290
#define CONTROLLDIVIDE 291
#define KRONLDIVIDE 292
#define POWER 293
#define DOTPOWER 294
#define EQ 295
#define NE 296
#define LT 297
#define LE 298
#define GT 299
#define GE 300
#define AND 301
#define ANDAND 302
#define OR 303
#define OROR 304
#define ASSIGN 305
#define ARGUMENTS 306
#define IF 307
#define THEN 308
#define ELSE 309
#define ELSEIF 310
#define END 311
#define SELECT 312
#define SWITCH 313
#define CASE 314
#define OTHERWISE 315
#define FUNCTION 316
#define ENDFUNCTION 317
#define FOR 318
#define WHILE 319
#define DO 320
#define BREAK 321
#define CONTINUE 322
#define TRY 323
#define CATCH 324
#define RETURN 325
#define FLEX_ERROR 326
#define STR 327
#define ID 328
#define VARINT 329
#define VARFLOAT 330
#define COMPLEXNUM 331
#define NUM 332
#define PATH 333
#define COMMENT 334
#define BLOCKCOMMENT 335
#define TOPLEVEL 336
#define HIGHLEVEL 337
#define UPLEVEL 338
#define LISTABLE 339
#define CONTROLBREAK 340
#define UMINUS 341
#define UPLUS 342
#define FUNCTIONCALL 343

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{

  /* Tokens. */
    double                      number;
    std::wstring*               str;
    std::wstring*               path;
    std::wstring*               comment;

    LineBreakStr*               mute;

    ast::exps_t*                t_list_var;
    ast::exps_t*                t_list_exp;
    ast::Exp*                   t_exp;

    ast::SeqExp*                t_seq_exp;
    ast::ReturnExp*             t_return_exp;

    ast::ArgumentsExp*          t_arguments_exp;
    ast::IfExp*                 t_if_exp;
    ast::WhileExp*              t_while_exp;
    ast::ForExp*                t_for_exp;
    ast::TryCatchExp*           t_try_exp;
    ast::SelectExp*             t_select_exp;
    ast::CaseExp*               t_case_exp;
    ast::exps_t*                t_list_case;

    ast::CallExp*               t_call_exp;

    ast::MathExp*               t_math_exp;

    ast::OpExp*                 t_op_exp;
    ast::OpExp::Oper            t_op_exp_oper;
    ast::LogicalOpExp::Oper     t_lop_exp_oper;

    ast::AssignExp*             t_assign_exp;

    ast::StringExp*             t_string_exp;

    ast::ListExp*               t_implicit_list;

    ast::MatrixExp*             t_matrix_exp;
    ast::MatrixLineExp*         t_matrixline_exp;
    ast::exps_t*                t_list_mline;

    ast::CellExp*               t_cell_exp;

    ast::CellCallExp*           t_cell_call_exp;

    ast::FunctionDec*           t_function_dec;
    ast::ArgumentDec*           t_argument_dec;

    ast::ArrayListExp*          t_arraylist_exp;
    ast::AssignListExp*         t_assignlist_exp;
    ast::ArrayListVar*          t_arraylist_var;

    ast::SimpleVar*             t_simple_var;


};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;

int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_DOTS = 3,                       /* "line break"  */
  YYSYMBOL_EOL = 4,                        /* "end of line"  */
  YYSYMBOL_SPACES = 5,                     /* "spaces"  */
  YYSYMBOL_BOOLTRUE = 6,                   /* "%t or %T"  */
  YYSYMBOL_BOOLFALSE = 7,                  /* "%f or %F"  */
  YYSYMBOL_QUOTE = 8,                      /* "'"  */
  YYSYMBOL_NOT = 9,                        /* "~ or @"  */
  YYSYMBOL_ARROW = 10,                     /* "->"  */
  YYSYMBOL_SHARP = 11,                     /* "#"  */
  YYSYMBOL_DOLLAR = 12,                    /* "$"  */
  YYSYMBOL_COMMA = 13,                     /* ","  */
  YYSYMBOL_COLON = 14,                     /* ":"  */
  YYSYMBOL_SEMI = 15,                      /* ";"  */
  YYSYMBOL_LPAREN = 16,                    /* "("  */
  YYSYMBOL_RPAREN = 17,                    /* ")"  */
  YYSYMBOL_LBRACK = 18,                    /* "["  */
  YYSYMBOL_RBRACK = 19,                    /* "]"  */
  YYSYMBOL_LBRACE = 20,                    /* "{"  */
  YYSYMBOL_RBRACE = 21,                    /* "}"  */
  YYSYMBOL_DOT = 22,                       /* "."  */
  YYSYMBOL_DOTQUOTE = 23,                  /* ".'"  */
  YYSYMBOL_PLUS = 24,                      /* "+"  */
  YYSYMBOL_MINUS = 25,                     /* "-"  */
  YYSYMBOL_TIMES = 26,                     /* "*"  */
  YYSYMBOL_DOTTIMES = 27,                  /* ".*"  */
  YYSYMBOL_KRONTIMES = 28,                 /* ".*."  */
  YYSYMBOL_CONTROLTIMES = 29,              /* "*."  */
  YYSYMBOL_RDIVIDE = 30,                   /* "/"  */
  YYSYMBOL_DOTRDIVIDE = 31,                /* "./"  */
  YYSYMBOL_CONTROLRDIVIDE = 32,            /* "/."  */
  YYSYMBOL_KRONRDIVIDE = 33,               /* "./."  */
  YYSYMBOL_LDIVIDE = 34,                   /* "\\"  */
  YYSYMBOL_DOTLDIVIDE = 35,                /* ".\\"  */
  YYSYMBOL_CONTROLLDIVIDE = 36,            /* "\\."  */
  YYSYMBOL_KRONLDIVIDE = 37,               /* ".\\."  */
  YYSYMBOL_POWER = 38,                     /* "** or ^"  */
  YYSYMBOL_DOTPOWER = 39,                  /* ".^"  */
  YYSYMBOL_EQ = 40,                        /* "=="  */
  YYSYMBOL_NE = 41,                        /* "<> or ~="  */
  YYSYMBOL_LT = 42,                        /* "<"  */
  YYSYMBOL_LE = 43,                        /* "<="  */
  YYSYMBOL_GT = 44,                        /* ">"  */
  YYSYMBOL_GE = 45,                        /* ">="  */
  YYSYMBOL_AND = 46,                       /* "&"  */
  YYSYMBOL_ANDAND = 47,                    /* "&&"  */
  YYSYMBOL_OR = 48,                        /* "|"  */
  YYSYMBOL_OROR = 49,                      /* "||"  */
  YYSYMBOL_ASSIGN = 50,                    /* "="  */
  YYSYMBOL_ARGUMENTS = 51,                 /* "arguments"  */
  YYSYMBOL_IF = 52,                        /* "if"  */
  YYSYMBOL_THEN = 53,                      /* "then"  */
  YYSYMBOL_ELSE = 54,                      /* "else"  */
  YYSYMBOL_ELSEIF = 55,                    /* "elseif"  */
  YYSYMBOL_END = 56,                       /* "end"  */
  YYSYMBOL_SELECT = 57,                    /* "select"  */
  YYSYMBOL_SWITCH = 58,                    /* "switch"  */
  YYSYMBOL_CASE = 59,                      /* "case"  */
  YYSYMBOL_OTHERWISE = 60,                 /* "otherwise"  */
  YYSYMBOL_FUNCTION = 61,                  /* "function"  */
  YYSYMBOL_ENDFUNCTION = 62,               /* "endfunction"  */
  YYSYMBOL_FOR = 63,                       /* "for"  */
  YYSYMBOL_WHILE = 64,                     /* "while"  */
  YYSYMBOL_DO = 65,                        /* "do"  */
  YYSYMBOL_BREAK = 66,                     /* "break"  */
  YYSYMBOL_CONTINUE = 67,                  /* "continue"  */
  YYSYMBOL_TRY = 68,                       /* "try"  */
  YYSYMBOL_CATCH = 69,                     /* "catch"  */
  YYSYMBOL_RETURN = 70,                    /* "return"  */
  YYSYMBOL_FLEX_ERROR = 71,                /* FLEX_ERROR  */
  YYSYMBOL_STR = 72,                       /* "string"  */
  YYSYMBOL_ID = 73,                        /* "identifier"  */
  YYSYMBOL_VARINT = 74,                    /* "integer"  */
  YYSYMBOL_VARFLOAT = 75,                  /* "float"  */
  YYSYMBOL_COMPLEXNUM = 76,                /* "complex number"  */
  YYSYMBOL_NUM = 77,                       /* "number"  */
  YYSYMBOL_PATH = 78,                      /* "path"  */
  YYSYMBOL_COMMENT = 79,                   /* "line comment"  */
  YYSYMBOL_BLOCKCOMMENT = 80,              /* "block comment"  */
  YYSYMBOL_TOPLEVEL = 81,                  /* TOPLEVEL  */
  YYSYMBOL_HIGHLEVEL = 82,                 /* HIGHLEVEL  */
  YYSYMBOL_UPLEVEL = 83,                   /* UPLEVEL  */
  YYSYMBOL_LISTABLE = 84,                  /* LISTABLE  */
  YYSYMBOL_CONTROLBREAK = 85,              /* CONTROLBREAK  */
  YYSYMBOL_UMINUS = 86,                    /* UMINUS  */
  YYSYMBOL_UPLUS = 87,                     /* UPLUS  */
  YYSYMBOL_FUNCTIONCALL = 88,              /* FUNCTIONCALL  */
  YYSYMBOL_YYACCEPT = 89,                  /* $accept  */
  YYSYMBOL_program = 90,                   /* program  */
  YYSYMBOL_expressions = 91,               /* expressions  */
  YYSYMBOL_recursiveExpression = 92,       /* recursiveExpression  */
  YYSYMBOL_expressionLineBreak = 93,       /* expressionLineBreak  */
  YYSYMBOL_expression = 94,                /* expression  */
  YYSYMBOL_implicitFunctionCall = 95,      /* implicitFunctionCall  */
  YYSYMBOL_implicitCallable = 96,          /* implicitCallable  */
  YYSYMBOL_functionCall = 97,              /* functionCall  */
  YYSYMBOL_simpleFunctionCall = 98,        /* simpleFunctionCall  */
  YYSYMBOL_functionArgs = 99,              /* functionArgs  */
  YYSYMBOL_functionDeclaration = 100,      /* functionDeclaration  */
  YYSYMBOL_lambdaFunctionDeclaration = 101, /* lambdaFunctionDeclaration  */
  YYSYMBOL_endfunction = 102,              /* endfunction  */
  YYSYMBOL_functionDeclarationReturns = 103, /* functionDeclarationReturns  */
  YYSYMBOL_functionDeclarationArguments = 104, /* functionDeclarationArguments  */
  YYSYMBOL_idList = 105,                   /* idList  */
  YYSYMBOL_functionDeclarationBreak = 106, /* functionDeclarationBreak  */
  YYSYMBOL_functionBody = 107,             /* functionBody  */
  YYSYMBOL_condition = 108,                /* condition  */
  YYSYMBOL_comparison = 109,               /* comparison  */
  YYSYMBOL_rightComparable = 110,          /* rightComparable  */
  YYSYMBOL_operation = 111,                /* operation  */
  YYSYMBOL_rightOperand = 112,             /* rightOperand  */
  YYSYMBOL_listableBegin = 113,            /* listableBegin  */
  YYSYMBOL_listableEnd = 114,              /* listableEnd  */
  YYSYMBOL_variable = 115,                 /* variable  */
  YYSYMBOL_variableFields = 116,           /* variableFields  */
  YYSYMBOL_cell = 117,                     /* cell  */
  YYSYMBOL_matrix = 118,                   /* matrix  */
  YYSYMBOL_matrixOrCellLines = 119,        /* matrixOrCellLines  */
  YYSYMBOL_matrixOrCellLineBreak = 120,    /* matrixOrCellLineBreak  */
  YYSYMBOL_matrixOrCellLine = 121,         /* matrixOrCellLine  */
  YYSYMBOL_matrixOrCellColumns = 122,      /* matrixOrCellColumns  */
  YYSYMBOL_matrixOrCellColumnsBreak = 123, /* matrixOrCellColumnsBreak  */
  YYSYMBOL_variableDeclaration = 124,      /* variableDeclaration  */
  YYSYMBOL_assignable = 125,               /* assignable  */
  YYSYMBOL_multipleResults = 126,          /* multipleResults  */
  YYSYMBOL_argumentsControl = 127,         /* argumentsControl  */
  YYSYMBOL_argumentsDeclarations = 128,    /* argumentsDeclarations  */
  YYSYMBOL_argumentDeclaration = 129,      /* argumentDeclaration  */
  YYSYMBOL_argumentName = 130,             /* argumentName  */
  YYSYMBOL_argumentDimension = 131,        /* argumentDimension  */
  YYSYMBOL_argumentValidators = 132,       /* argumentValidators  */
  YYSYMBOL_argumentDefaultValue = 133,     /* argumentDefaultValue  */
  YYSYMBOL_ifControl = 134,                /* ifControl  */
  YYSYMBOL_thenBody = 135,                 /* thenBody  */
  YYSYMBOL_elseBody = 136,                 /* elseBody  */
  YYSYMBOL_ifConditionBreak = 137,         /* ifConditionBreak  */
  YYSYMBOL_then = 138,                     /* then  */
  YYSYMBOL_else = 139,                     /* else  */
  YYSYMBOL_elseIfControl = 140,            /* elseIfControl  */
  YYSYMBOL_selectControl = 141,            /* selectControl  */
  YYSYMBOL_select = 142,                   /* select  */
  YYSYMBOL_defaultCase = 143,              /* defaultCase  */
  YYSYMBOL_selectable = 144,               /* selectable  */
  YYSYMBOL_selectConditionBreak = 145,     /* selectConditionBreak  */
  YYSYMBOL_casesControl = 146,             /* casesControl  */
  YYSYMBOL_caseBody = 147,                 /* caseBody  */
  YYSYMBOL_caseControlBreak = 148,         /* caseControlBreak  */
  YYSYMBOL_forControl = 149,               /* forControl  */
  YYSYMBOL_forIterator = 150,              /* forIterator  */
  YYSYMBOL_forConditionBreak = 151,        /* forConditionBreak  */
  YYSYMBOL_forBody = 152,                  /* forBody  */
  YYSYMBOL_whileControl = 153,             /* whileControl  */
  YYSYMBOL_whileBody = 154,                /* whileBody  */
  YYSYMBOL_whileConditionBreak = 155,      /* whileConditionBreak  */
  YYSYMBOL_tryControl = 156,               /* tryControl  */
  YYSYMBOL_catchBody = 157,                /* catchBody  */
  YYSYMBOL_returnControl = 158,            /* returnControl  */
  YYSYMBOL_comments = 159,                 /* comments  */
  YYSYMBOL_lineEnd = 160,                  /* lineEnd  */
  YYSYMBOL_keywords = 161                  /* keywords  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  112
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   4082

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  89
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  73
/* YYNRULES -- Number of rules.  */
#define YYNRULES  417
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  609

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   343


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   423,   423,   424,   425,   434,   449,   453,   459,   466,
     473,   488,   500,   508,   517,   538,   539,   540,   541,   542,
     543,   551,   552,   553,   554,   555,   556,   557,   558,   559,
     560,   561,   562,   563,   564,   565,   566,   580,   586,   602,
     603,   609,   615,   621,   622,   623,   624,   625,   632,   640,
     642,   653,   654,   655,   656,   679,   680,   681,   682,   683,
     684,   685,   686,   687,   688,   689,   690,   691,   692,   708,
     720,   729,   739,   768,   773,   778,   783,   794,   795,   803,
     811,   812,   813,   821,   827,   840,   841,   842,   843,   844,
     852,   858,   873,   874,   882,   889,   904,   905,   906,   908,
     909,   910,   912,   913,   914,   916,   917,   918,   920,   921,
     922,   924,   925,   926,   928,   929,   930,   932,   933,   934,
     936,   937,   938,   940,   941,   942,   950,   957,   964,   965,
     966,   967,   968,   969,   970,   971,   972,   973,   974,   975,
     976,   977,   978,   979,   988,   989,   991,   992,   994,   995,
     996,   997,   998,   999,  1000,  1001,  1003,  1004,  1005,  1006,
    1007,  1008,  1009,  1010,  1012,  1013,  1014,  1015,  1016,  1017,
    1018,  1019,  1027,  1028,  1036,  1037,  1038,  1046,  1047,  1048,
    1049,  1050,  1056,  1057,  1058,  1063,  1068,  1069,  1070,  1071,
    1072,  1073,  1074,  1075,  1076,  1077,  1078,  1079,  1080,  1081,
    1082,  1083,  1084,  1085,  1086,  1094,  1099,  1104,  1110,  1116,
    1122,  1134,  1135,  1136,  1141,  1146,  1152,  1158,  1159,  1168,
    1169,  1170,  1171,  1172,  1173,  1174,  1175,  1183,  1184,  1194,
    1195,  1196,  1197,  1205,  1206,  1214,  1215,  1216,  1217,  1218,
    1219,  1220,  1221,  1222,  1230,  1231,  1232,  1233,  1241,  1242,
    1243,  1244,  1246,  1247,  1249,  1250,  1259,  1260,  1261,  1262,
    1263,  1264,  1265,  1266,  1267,  1274,  1282,  1283,  1296,  1301,
    1306,  1312,  1323,  1332,  1347,  1348,  1355,  1356,  1363,  1364,
    1371,  1372,  1373,  1381,  1382,  1393,  1401,  1407,  1422,  1428,
    1445,  1446,  1447,  1448,  1449,  1457,  1458,  1459,  1460,  1461,
    1462,  1470,  1471,  1472,  1473,  1474,  1475,  1483,  1489,  1503,
    1519,  1520,  1531,  1532,  1551,  1552,  1560,  1561,  1562,  1563,
    1564,  1565,  1566,  1574,  1575,  1583,  1584,  1585,  1586,  1587,
    1595,  1596,  1597,  1598,  1599,  1600,  1604,  1610,  1625,  1626,
    1627,  1628,  1629,  1630,  1631,  1632,  1633,  1634,  1635,  1636,
    1644,  1645,  1653,  1654,  1663,  1664,  1665,  1666,  1667,  1668,
    1669,  1670,  1674,  1680,  1695,  1703,  1709,  1724,  1725,  1726,
    1727,  1728,  1729,  1730,  1731,  1732,  1733,  1734,  1735,  1736,
    1737,  1738,  1739,  1740,  1741,  1749,  1750,  1765,  1771,  1777,
    1783,  1789,  1797,  1812,  1813,  1814,  1821,  1822,  1830,  1831,
    1839,  1840,  1841,  1842,  1843,  1844,  1845,  1846,  1847,  1848,
    1849,  1850,  1851,  1852,  1853,  1854,  1855,  1856
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "\"line break\"",
  "\"end of line\"", "\"spaces\"", "\"%t or %T\"", "\"%f or %F\"", "\"'\"",
  "\"~ or @\"", "\"->\"", "\"#\"", "\"$\"", "\",\"", "\":\"", "\";\"",
  "\"(\"", "\")\"", "\"[\"", "\"]\"", "\"{\"", "\"}\"", "\".\"", "\".'\"",
  "\"+\"", "\"-\"", "\"*\"", "\".*\"", "\".*.\"", "\"*.\"", "\"/\"",
  "\"./\"", "\"/.\"", "\"./.\"", "\"\\\\\"", "\".\\\\\"", "\"\\\\.\"",
  "\".\\\\.\"", "\"** or ^\"", "\".^\"", "\"==\"", "\"<> or ~=\"", "\"<\"",
  "\"<=\"", "\">\"", "\">=\"", "\"&\"", "\"&&\"", "\"|\"", "\"||\"",
  "\"=\"", "\"arguments\"", "\"if\"", "\"then\"", "\"else\"", "\"elseif\"",
  "\"end\"", "\"select\"", "\"switch\"", "\"case\"", "\"otherwise\"",
  "\"function\"", "\"endfunction\"", "\"for\"", "\"while\"", "\"do\"",
  "\"break\"", "\"continue\"", "\"try\"", "\"catch\"", "\"return\"",
  "FLEX_ERROR", "\"string\"", "\"identifier\"", "\"integer\"", "\"float\"",
  "\"complex number\"", "\"number\"", "\"path\"", "\"line comment\"",
  "\"block comment\"", "TOPLEVEL", "HIGHLEVEL", "UPLEVEL", "LISTABLE",
  "CONTROLBREAK", "UMINUS", "UPLUS", "FUNCTIONCALL", "$accept", "program",
  "expressions", "recursiveExpression", "expressionLineBreak",
  "expression", "implicitFunctionCall", "implicitCallable", "functionCall",
  "simpleFunctionCall", "functionArgs", "functionDeclaration",
  "lambdaFunctionDeclaration", "endfunction", "functionDeclarationReturns",
  "functionDeclarationArguments", "idList", "functionDeclarationBreak",
  "functionBody", "condition", "comparison", "rightComparable",
  "operation", "rightOperand", "listableBegin", "listableEnd", "variable",
  "variableFields", "cell", "matrix", "matrixOrCellLines",
  "matrixOrCellLineBreak", "matrixOrCellLine", "matrixOrCellColumns",
  "matrixOrCellColumnsBreak", "variableDeclaration", "assignable",
  "multipleResults", "argumentsControl", "argumentsDeclarations",
  "argumentDeclaration", "argumentName", "argumentDimension",
  "argumentValidators", "argumentDefaultValue", "ifControl", "thenBody",
  "elseBody", "ifConditionBreak", "then", "else", "elseIfControl",
  "selectControl", "select", "defaultCase", "selectable",
  "selectConditionBreak", "casesControl", "caseBody", "caseControlBreak",
  "forControl", "forIterator", "forConditionBreak", "forBody",
  "whileControl", "whileBody", "whileConditionBreak", "tryControl",
  "catchBody", "returnControl", "comments", "lineEnd", "keywords", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-499)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-393)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    1324,  -499,  -499,  -499,  -499,  3332,    98,  -499,  -499,  -499,
    3332,   455,  1194,  3332,  3332,   139,  3332,  -499,  -499,    29,
       4,  3332,  -499,  -499,  1488,  3332,  -499,   125,  -499,  -499,
    -499,  -499,  -499,    49,  -499,  1240,  1407,    17,    79,  3494,
    -499,  -499,  -499,  -499,  -499,  3621,  -499,  -499,  -499,   100,
    -499,  -499,  -499,  -499,  3332,  -499,  -499,  -499,  -499,   455,
     175,   171,   227,    14,   138,  3537,  3579,   234,  2432,  -499,
    -499,  3663,  3705,  2506,  -499,   332,  2526,  -499,  2546,   422,
     261,   351,   261,   351,   -40,  3663,   131,  3705,    45,    18,
      95,   120,    13,  1748,  2172,  2172,  -499,   113,  3663,  3705,
    -499,  -499,  -499,  2660,  2732,  -499,  -499,  -499,  -499,  -499,
    -499,   199,  -499,    33,  -499,  -499,  -499,  -499,   200,   210,
     199,  -499,  3332,  2752,  3987,  -499,  3332,  3332,  3332,  3332,
    3332,  3332,  3332,  3332,  3332,  3332,  3332,  3332,  3332,  3332,
    3332,  3332,  2884,  2956,  2976,  2996,  3068,  3088,  3108,  3180,
    3200,  3220,  2772,  -499,  -499,   208,  -499,  -499,  2844,  1570,
    -499,  3332,  3332,  -499,  -499,  -499,  2864,  3663,  3705,    54,
    1014,  2752,  4009,  2844,  1647,  -499,  -499,   242,   143,  3332,
    -499,  3332,  -499,  3332,  -499,  -499,  2620,  2194,  -499,  -499,
    2270,  -499,  -499,  -499,  -499,   195,  -499,  3663,  3705,   212,
     104,  -499,  2640,  2292,  -499,  2314,  -499,  -499,   231,   276,
      24,    10,   265,  -499,   289,   294,   213,   251,  1671,   257,
     282,   299,   254,    86,   292,  3332,  -499,   328,   350,   216,
     220,   361,  1960,  -499,  -499,  -499,  -499,  1517,  3292,  -499,
    -499,   114,  3494,   248,  3621,  -499,  -499,    61,   256,   200,
     210,   210,  1330,  3967,  -499,   258,  -499,  -499,  -499,  -499,
    -499,  -499,  -499,  -499,  -499,  -499,  -499,  -499,  -499,  -499,
    -499,  -499,  -499,  -499,   305,   312,   287,  3999,   287,  3999,
     261,   351,   261,   351,   261,   351,   261,   351,   261,   351,
     261,   351,   261,   351,   261,   351,   261,   351,   261,   351,
     261,   351,   261,   351,   261,   351,   261,   351,  -499,  3903,
    3935,  -499,  3903,  3935,  -499,  3903,  3935,  -499,  3903,  3935,
    -499,  3903,  3935,  -499,  3903,  3935,  -499,  3827,  3865,  -499,
    3827,  3865,  -499,  3747,  3787,  -499,  3747,  3787,  -499,  3663,
    3705,  -499,  3332,   273,    11,   142,   318,   320,   261,   351,
     261,   351,  -499,  3663,  3705,  -499,  -499,   362,   367,   226,
     -24,  -499,   279,  -499,  -499,   317,   175,  -499,  -499,   302,
    -499,   360,  1777,  3663,  3705,  3663,  3705,  3663,  3705,  -499,
    2390,  -499,  -499,  -499,  -499,  -499,  -499,  -499,  3663,  3705,
     212,  -499,  2412,  -499,  -499,   307,  -499,  -499,   379,    10,
    -499,   382,  -499,  2844,    34,  -499,  -499,  -499,   383,  -499,
     358,   319,   341,    98,   391,   392,  1854,  -499,  3332,  3663,
    3705,   123,  -499,  -499,  -499,   394,   396,  -499,   397,   406,
    -499,  -499,   363,   364,  -499,  3494,  3621,  -499,  3312,  -499,
    -499,  -499,   210,   366,  1330,  3967,   368,   175,   400,  -499,
    -499,   -24,  3332,   417,   304,   -18,  -499,  -499,  -499,  1883,
    1989,  -499,   405,  -499,  -499,  -499,  -499,  -499,  -499,   336,
    2844,   403,   374,  -499,   253,  3332,  -499,  2066,   369,    98,
     357,    86,  -499,  -499,   -11,   415,  -499,   432,   435,   437,
    2095,  -499,  -499,  -499,  -499,  -499,  -499,  -499,  3494,  3621,
    -499,   325,  3406,  3452,  -499,  -499,  3332,   259,  -499,  2066,
    3332,   441,  1989,   431,   433,  -499,  -499,   173,   374,  3332,
    -499,  -499,   445,   447,   131,  -499,   398,  -499,    86,    98,
    1854,  -499,  -499,  -499,   123,  -499,  -499,  -499,  -499,   401,
    -499,  2066,  -499,   448,   449,   274,  1594,  1594,  3406,  3452,
    -499,   454,   459,   409,  3406,  3452,  -499,   451,  -499,  -499,
    -499,  -499,  3663,  3705,  -499,  -499,  1671,  -499,  1854,    86,
     -11,  2095,  -499,   413,  -499,  -499,  -499,   466,   468,  -499,
    -499,  -499,  1594,  1594,  -499,  -499,  -499,  1594,  1594,  -499,
     154,   -11,  1854,  -499,   420,  -499,  -499,  -499,  -499,  -499,
    -499,  -499,  2066,  -499,  -499,   -11,  -499,  -499,  -499
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int16 yydefact[] =
{
       0,    36,    17,   197,   198,     0,    82,   196,    16,    15,
       0,     0,     0,     0,     0,     0,     0,   314,   315,     0,
       0,     0,    32,    33,     0,   393,   195,   190,   191,   193,
     194,   192,    35,     0,     2,     0,     0,     9,    31,    22,
      49,    21,   186,   201,   189,    30,   188,   187,    23,     0,
     262,    24,    25,    26,     0,    27,    28,    29,    34,     0,
     190,   178,   177,     0,     0,     0,     0,     0,     0,   226,
     243,   242,   241,     0,   228,     0,     0,   218,     0,     0,
     131,   130,   129,   128,     0,    92,   300,    93,     0,    82,
       0,     0,     0,     0,     0,     0,   387,     0,   395,   394,
      45,    46,    44,     0,     0,    43,    39,    40,    42,    41,
      48,    38,     1,     7,    20,    19,    18,     3,    10,    14,
      37,   142,     0,     0,     0,   143,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    95,   127,   176,   185,   140,     0,     0,
     141,     0,     0,    94,   126,   184,     0,   324,   323,     0,
       0,     0,     0,     0,     0,    81,    84,     0,     0,     0,
      50,     0,   199,     0,   200,   225,     0,     0,   219,   227,
       0,   230,   247,   246,   229,   223,   239,   238,   237,   233,
       0,   217,     0,     0,   211,     0,   215,   267,   274,     0,
       0,     0,   277,   294,   292,   290,   295,   299,     0,     0,
       0,    79,     0,     0,     0,     0,   376,   367,   368,   372,
     369,     0,     0,   388,   390,   389,   386,     0,    59,    57,
      53,   190,    56,     0,    55,    58,    54,     0,     0,     8,
      11,    13,   173,   172,   204,     0,   400,   401,   402,   403,
     404,   405,   406,   408,   407,   409,   410,   411,   412,   413,
     414,   415,   416,   417,   182,   183,   145,   144,   147,   146,
     149,   148,   151,   150,   153,   152,   155,   154,   157,   156,
     159,   158,   163,   162,   161,   160,   165,   164,   167,   166,
     171,   170,   169,   168,   135,   134,   139,   138,   110,   109,
     108,   113,   112,   111,   119,   118,   117,   125,   124,   123,
     116,   115,   114,   122,   121,   120,    98,    97,    96,   101,
     100,    99,   104,   103,   102,   107,   106,   105,   253,   251,
     250,   255,     0,     0,     0,   179,   181,   180,   133,   132,
     137,   136,   252,   249,   248,   254,   325,   328,   329,     0,
       0,   223,     0,   182,   183,     0,   179,   181,   180,     0,
      80,     0,     0,   208,   209,   210,   207,   206,   205,   220,
       0,   224,   221,   231,   232,   245,   244,   240,   236,   235,
     234,   212,     0,   216,   213,     0,   271,   266,     0,     0,
     398,     0,   270,     0,   279,   293,   291,   298,   296,   286,
       0,     0,     0,    82,    88,    86,     0,    85,     0,   352,
     353,   361,   377,   378,   382,   373,   374,   379,   370,   371,
     375,   365,     0,     0,    62,    61,    60,    63,    64,    51,
      52,    47,    12,   203,   175,   174,   202,     0,     0,   326,
     327,     0,     0,     0,     0,     0,   203,   202,    83,     0,
       0,    90,     0,   222,   214,   275,   269,   268,   399,     0,
       0,   279,   282,   297,   301,     0,   283,     0,     0,    82,
       0,     0,    89,    87,     0,     0,   354,   357,   355,   359,
       0,   383,   384,   380,   381,   364,   385,    67,    66,    65,
      68,     0,   349,   349,   396,   310,     0,   317,   316,     0,
       0,     0,     0,     0,     0,    73,   276,     0,   282,     0,
     272,   304,   302,   303,   300,   288,     0,   285,     0,    82,
       0,    78,    77,    72,   361,   358,   356,   360,   362,     0,
     312,     0,   341,   339,   340,   338,     0,     0,   349,   349,
     320,   318,   319,     0,   349,   349,   397,     0,    74,    75,
     278,   273,   281,   280,   305,   306,     0,   284,     0,     0,
       0,     0,   350,     0,   343,   344,   342,   345,   347,   336,
     331,   330,     0,     0,   321,   322,   311,     0,     0,    76,
     307,     0,     0,    69,     0,   313,   346,   348,   335,   334,
     333,   332,     0,   309,    71,     0,   351,   308,    70
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -499,  -499,     0,  -499,   -30,   443,  -499,   444,   769,  -499,
    -101,  -499,  -499,  -431,  -499,   -85,   393,  -469,  -407,   -20,
    -499,   -43,  -499,   316,  -499,   438,   992,  -499,  -499,  -499,
      30,   285,   -60,     3,  -499,   -98,  -499,  -499,  -499,  -499,
     277,  -499,  -499,    15,   -29,  -499,   -78,  -498,   275,   -32,
    -400,   -97,  -499,  -499,     1,  -499,   144,    56,  -376,  -382,
    -499,    87,   -26,   -58,  -499,  -499,  -499,  -499,   278,    -3,
    -499,  -203,  -109
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,    33,   461,    35,    36,    37,    38,   111,    39,    40,
     243,    41,    42,   533,   220,    64,   177,   416,   462,    86,
      43,   153,    44,   154,   155,   156,    45,    67,    46,    47,
      73,   199,    74,    75,   200,    48,    49,    50,    51,   210,
     211,   212,   404,   472,   520,    52,   410,   526,   217,   218,
     508,   478,    53,    54,   509,   169,   360,   454,   580,   546,
      55,   421,   490,   539,    56,   432,   232,    57,    97,    58,
     455,   417,   275
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      34,    92,   163,   247,   223,   245,   245,   119,   402,   484,
     477,   553,   530,   189,   400,    79,   207,   226,   189,   163,
      90,     2,   255,   163,    96,   245,   227,   344,   228,   163,
       8,   175,     9,   208,    63,   452,   117,     2,   163,   209,
     163,   510,    78,   573,   163,   531,     8,    88,     9,   112,
     347,   532,   513,   514,   470,   453,   163,   343,   356,   568,
     245,   511,   170,   364,   219,   368,   229,   357,   222,   358,
     362,   187,   365,   245,   438,   245,   190,    91,   230,   203,
     397,   205,   440,   250,   447,   100,   101,   176,   251,   401,
     400,   102,   231,   233,   234,   235,   118,   208,   186,   414,
     592,   415,    89,   398,   607,   557,   202,   471,   191,   385,
       3,     4,   249,     5,    63,     6,     7,   386,   176,   194,
      10,   547,    59,   570,    12,   163,   189,   486,    13,    14,
     103,   100,   101,   359,   104,   213,   487,   102,   488,   593,
     437,   103,   189,    84,   214,   104,   215,   371,   178,   341,
     166,   105,   106,   107,   108,   163,   109,   110,   103,   372,
     604,   591,   104,   355,  -261,   401,   582,   583,   224,   236,
     225,   581,   587,   588,   608,  -261,    26,    60,    28,    29,
      30,    31,   237,   387,   216,   605,   438,   171,   489,   380,
     602,   103,  -256,   172,   560,   104,   467,   105,   106,   107,
     108,   163,   109,   110,     2,   392,   598,   599,   474,   475,
     163,   600,   601,     8,   114,     9,   383,   213,   409,   442,
     424,   248,   342,   115,   427,   116,   214,   384,   215,   425,
     356,   426,   431,   428,   163,   429,   163,    96,   163,   357,
     163,   358,   163,   173,   163,  -265,   163,   183,   163,   174,
     163,   184,   163,   395,   163,   369,   163,   521,   163,   370,
     163,   438,   163,   550,   163,   439,   522,   163,   523,   121,
     163,   438,   551,   163,   552,   443,   163,   171,   576,   163,
     396,   403,   163,   172,   125,   163,   438,   577,   163,   578,
     446,   163,   438,   405,   163,   121,   456,   163,   406,   140,
     141,   412,   469,   171,   408,   245,   163,   411,   163,   172,
     125,   163,   369,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   413,   481,   441,
     438,   163,   422,   163,   457,   163,   191,   192,     3,     4,
     500,     5,   418,     6,     7,   193,   163,   194,    10,   438,
      59,   195,    12,   516,   423,  -259,    13,    14,   474,   157,
     505,   164,  -260,   506,   507,   430,   449,   173,  -258,   517,
    -257,   450,   245,   174,   160,   458,   459,   163,   164,   474,
     465,   540,   164,   466,   506,   507,   468,   473,   164,   161,
     162,   480,   479,   163,   528,   482,   483,   164,   491,   164,
     492,   493,   163,   164,    26,    60,    28,    29,    30,    31,
     494,   196,   474,   475,   476,   164,  -264,   180,  -263,   495,
     496,   504,   515,   470,   519,   527,   191,   192,     3,     4,
     529,     5,   534,     6,     7,   193,   535,   194,    10,   536,
      59,   537,    12,   206,   569,   556,    13,    14,   558,   564,
     559,   565,   574,   575,   567,   524,   163,   572,   584,    68,
     163,     3,     4,   585,     5,   586,     6,     7,   589,   595,
     596,    10,   597,    59,    69,    12,   606,   525,   113,    13,
      14,   221,   120,   165,   164,   390,   518,   399,   590,   561,
     538,   407,   566,   603,    26,    60,    28,    29,    30,    31,
     165,   196,   541,   451,   165,   485,   163,   501,   571,   525,
     165,     0,   163,   594,   164,   433,     0,     0,     0,   165,
     163,   165,     0,     0,     0,   165,     0,    26,    60,    28,
      29,    30,    31,     0,    70,     0,     0,   165,     0,     0,
       0,   525,     0,     0,     0,     0,   579,   579,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     164,     0,     0,     0,     0,     0,   409,     0,     0,   164,
       0,   538,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   579,   579,     0,     0,     0,   579,   579,     0,
       0,     0,     0,   164,     0,   164,     0,   164,     0,   164,
       0,   164,   525,   164,     0,   164,   165,   164,     0,   164,
       0,   164,     0,   164,     0,   164,     0,   164,     0,   164,
       0,   164,     0,   164,     0,     0,   164,     0,     0,   164,
       0,     0,   164,     0,     0,   164,   165,     0,   164,     0,
       0,   164,     0,     0,   164,     0,     0,   164,     0,     0,
     164,     0,     0,   164,     0,     0,   164,     0,     0,     0,
       0,     0,     0,     0,     0,   164,     0,   164,     0,     0,
     164,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   165,     0,     0,     0,     0,     0,     0,     0,
     164,   165,   164,     0,   164,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   164,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   165,     0,   165,     0,   165,
       0,   165,     0,   165,     0,   165,     0,   165,     0,   165,
       0,   165,     0,   165,     0,   165,   164,   165,     0,   165,
       0,   165,     0,   165,     0,   165,     0,     0,   165,     0,
       0,   165,   164,     0,   165,     0,     0,   165,     0,     0,
     165,   164,     0,   165,     0,     0,   165,     0,     0,   165,
       0,     0,   165,     0,    61,   165,     0,     0,   165,    65,
      71,    71,    80,    82,     0,    85,     0,   165,     0,   165,
      85,     0,   165,     0,    98,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   165,     0,   165,   164,   165,     0,     0,   164,
       0,     0,     0,   167,     0,     0,     0,   165,    71,     0,
       0,     0,     0,     0,     0,     0,     0,    71,     0,     0,
       0,     0,    71,     0,   197,    71,     0,    71,   197,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   165,     0,
       0,     0,     0,     0,     0,   164,     0,     0,     0,     0,
       0,   164,   242,   242,   165,     0,     0,     0,     0,   164,
       0,     0,     0,   165,     0,     0,     0,     0,     0,     0,
       0,   252,   242,     0,     0,   276,   278,   280,   282,   284,
     286,   288,   290,   292,   294,   296,   298,   300,   302,   304,
     306,   309,   312,   315,   318,   321,   324,   327,   330,   333,
     336,   339,     0,     0,     0,     0,     0,   242,   346,     0,
     348,   350,     0,     0,     0,   353,     0,   165,     0,   197,
     242,   165,   242,   367,     0,     0,     0,     0,   373,     0,
     375,     0,   377,     0,     0,    71,   197,     0,     0,   197,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   388,
       0,    71,   197,     0,   197,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   165,     0,     0,
       0,     0,     0,   165,   419,     0,     0,    62,     0,     0,
       0,   165,    66,    72,    72,    81,    83,   435,    87,     0,
       0,     0,     0,    87,     0,     0,     0,    99,   191,   192,
       3,     4,     0,     5,     0,     6,     7,   193,     0,   194,
      10,     0,    59,   361,    12,     0,     0,     0,    13,    14,
       0,     0,     0,     0,     0,     0,   168,     0,     0,     0,
       0,    72,     0,     0,     0,     0,     0,     0,     0,     0,
      72,     0,     0,     0,     0,    72,     0,   198,    72,     0,
      72,   198,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    26,    60,    28,    29,
      30,    31,     0,   196,     0,   244,   244,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   444,     0,   448,   253,   244,     0,     0,   277,   279,
     281,   283,   285,   287,   289,   291,   293,   295,   297,   299,
     301,   303,   305,   307,   310,   313,   316,   319,   322,   325,
     328,   331,   334,   337,   340,     0,     0,     0,     0,   197,
     244,     0,     0,   349,   351,     0,     0,     0,   354,     0,
       0,   197,   198,   244,     0,   244,     0,     0,     0,     0,
       0,   374,   242,   376,     0,   378,     0,     0,    72,   198,
       0,     0,   198,     0,     0,     0,     0,   419,     0,     0,
       0,     0,   389,     0,    72,   198,     0,   198,    76,     0,
       3,     4,     0,     5,     0,     6,     7,   498,     0,     0,
      10,     0,    59,     0,    12,    77,     0,   420,    13,    14,
       0,   502,     0,     0,     0,     0,     0,     0,     0,     0,
     436,     0,     0,     0,     0,     0,     0,     0,     0,   242,
      -6,     1,     0,     0,    85,     0,     3,     4,     0,     5,
       0,     6,     7,     0,     0,     0,    10,    -6,    11,     0,
      12,     0,     0,     0,    13,    14,    26,    60,    28,    29,
      30,    31,     0,    70,     0,   548,     0,     0,     0,   554,
       0,     0,     0,     0,     0,     0,     0,     0,   562,     0,
       0,    15,    16,     0,    -6,    -6,    -6,    17,    18,    -6,
      -6,    19,    -6,    20,    21,     0,    22,    23,    24,    -6,
      25,     0,    26,    27,    28,    29,    30,    31,     0,    32,
       0,     0,     0,     0,    -5,     1,     0,     0,     2,     0,
       3,     4,     0,     5,   445,     6,     7,     8,   121,     9,
      10,     0,    11,     0,    12,     0,   171,     0,    13,    14,
       0,     0,   172,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
       0,     0,   198,     0,     0,    15,    16,     0,     0,     0,
       0,    17,    18,     0,   198,    19,     0,    20,    21,     0,
      22,    23,    24,     0,    25,   244,    26,    27,    28,    29,
      30,    31,     0,    32,     0,     0,     0,    -4,     1,     0,
     420,   114,     0,     3,     4,     0,     5,     0,     6,     7,
     115,     0,   116,    10,     0,    11,     0,    12,     0,     0,
     499,    13,    14,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   503,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    15,    16,
       0,     0,   244,     0,    17,    18,     0,    87,    19,     0,
      20,    21,     0,    22,    23,    24,     0,    25,     0,    26,
      27,    28,    29,    30,    31,     0,    32,     0,     0,     1,
       0,     0,    93,     0,     3,     4,     0,     5,   549,     6,
       7,    94,   555,    95,    10,     0,    11,     0,    12,     0,
       0,   563,    13,    14,     0,     0,     0,     0,     1,     0,
       0,    93,     0,     3,     4,     0,     5,     0,     6,     7,
      94,     0,    95,    10,     0,    11,     0,    12,     0,    15,
      16,    13,    14,     0,  -392,    17,    18,     0,     0,    19,
       0,    20,    21,     0,    22,    23,    24,  -392,    25,     0,
      26,    27,    28,    29,    30,    31,     0,    32,    15,    16,
       0,     0,     0,  -392,    17,    18,     0,     0,    19,     0,
      20,    21,     0,    22,    23,    24,   344,    25,     0,    26,
      27,    28,    29,    30,    31,     1,    32,     0,     0,     0,
       3,     4,     0,     5,     0,     6,     7,     0,     0,     0,
      10,     0,    11,     0,    12,     0,     0,     0,    13,    14,
       0,     0,   256,   257,   258,   259,   260,   261,   262,   263,
     264,   265,   266,   267,   268,   269,   270,     0,   271,   272,
     273,     0,     0,   345,     0,    15,    16,     0,  -337,     0,
    -337,    17,    18,  -337,  -337,    19,     0,    20,    21,     0,
      22,    23,    24,   344,    25,     0,    26,    27,    28,    29,
      30,    31,     1,    32,     0,     0,     0,     3,     4,     0,
       5,     0,     6,     7,     0,     0,     0,    10,     0,    11,
       0,    12,     0,     0,     0,    13,    14,     0,     0,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,   271,   272,   273,     0,     0,
     366,     0,    15,    16,     0,  -287,  -287,  -287,    17,    18,
       0,     0,    19,     0,    20,    21,     0,    22,    23,    24,
       0,    25,     0,    26,    27,    28,    29,    30,    31,     1,
      32,     0,     0,     0,     3,     4,     0,     5,     0,     6,
       7,     0,     0,     0,    10,     0,    11,     0,    12,     0,
       0,     0,    13,    14,     0,     0,     0,     0,     1,     0,
       0,   460,     0,     3,     4,     0,     5,     0,     6,     7,
       0,     0,     0,    10,   -91,    11,     0,    12,     0,    15,
      16,    13,    14,     0,  -391,    17,    18,     0,     0,    19,
       0,    20,    21,     0,    22,    23,    24,  -391,    25,     0,
      26,    27,    28,    29,    30,    31,     0,    32,    15,    16,
       0,     0,     0,     0,    17,    18,     0,     0,    19,     0,
      20,    21,     0,    22,    23,    24,     0,    25,     0,    26,
      27,    28,    29,    30,    31,     1,    32,     0,     0,     0,
       3,     4,     0,     5,     0,     6,     7,     0,     0,     0,
      10,     0,    11,     0,    12,     0,     0,     0,    13,    14,
       0,     0,     0,     0,     1,     0,     0,   512,     0,     3,
       4,     0,     5,     0,     6,     7,     0,     0,     0,    10,
     -91,    11,     0,    12,     0,    15,    16,    13,    14,     0,
     -91,    17,    18,     0,     0,    19,   -91,    20,    21,     0,
      22,    23,    24,     0,    25,     0,    26,    27,    28,    29,
      30,    31,     0,    32,    15,    16,     0,     0,     0,     0,
      17,    18,     0,     0,    19,     0,    20,    21,     0,    22,
      23,    24,     0,    25,     0,    26,    27,    28,    29,    30,
      31,     1,    32,     0,     0,     0,     3,     4,     0,     5,
       0,     6,     7,     0,     0,     0,    10,     0,    11,     0,
      12,     0,     0,     0,    13,    14,     0,     0,     0,     0,
       1,     0,     0,     0,     0,     3,     4,     0,     5,     0,
       6,     7,     0,     0,     0,    10,   -91,    11,     0,    12,
       0,    15,    16,    13,    14,     0,  -366,    17,    18,     0,
       0,    19,     0,    20,    21,     0,    22,    23,    24,     0,
      25,     0,    26,    27,    28,    29,    30,    31,     0,    32,
      15,    16,     0,     0,     0,     0,    17,    18,     0,     0,
      19,     0,    20,    21,     0,    22,    23,    24,     0,    25,
       0,    26,    27,    28,    29,    30,    31,     1,    32,     0,
       0,     0,     3,     4,     0,     5,     0,     6,     7,     0,
       0,     0,    10,     0,    11,     0,    12,     0,     0,     0,
      13,    14,     0,     0,     0,     0,     1,     0,     0,     0,
       0,     3,     4,     0,     5,     0,     6,     7,     0,     0,
       0,    10,     0,    11,     0,    12,     0,    15,    16,    13,
      14,     0,  -289,    17,    18,     0,     0,    19,     0,    20,
      21,     0,    22,    23,    24,     0,    25,     0,    26,    27,
      28,    29,    30,    31,     0,    32,    15,    16,     0,     0,
       0,  -363,    17,    18,     0,     0,    19,     0,    20,    21,
       0,    22,    23,    24,     0,    25,     0,    26,    27,    28,
      29,    30,    31,     1,    32,     0,     0,     0,     3,     4,
       0,     5,     0,     6,     7,     0,     0,     0,    10,     0,
      11,     0,    12,     0,     0,     0,    13,    14,   191,   192,
       3,     4,     0,     5,     0,     6,     7,   193,     0,   194,
      10,     0,    59,   381,    12,     0,     0,     0,    13,    14,
       0,     0,     0,    15,    16,     0,     0,     0,     0,    17,
      18,     0,     0,    19,     0,    20,    21,     0,    22,    23,
      24,     0,    25,     0,    26,    27,    28,    29,    30,    31,
       0,    32,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    26,    60,    28,    29,
      30,    31,     0,   196,   191,   192,     3,     4,     0,     5,
       0,     6,     7,   193,     0,   194,    10,     0,    59,   382,
      12,     0,     0,     0,    13,    14,   191,   192,     3,     4,
       0,     5,     0,     6,     7,   193,     0,   194,    10,     0,
      59,     0,    12,   393,     0,     0,    13,    14,   191,   192,
       3,     4,     0,     5,     0,     6,     7,   193,     0,   194,
      10,     0,    59,     0,    12,   394,     0,     0,    13,    14,
       0,     0,    26,    60,    28,    29,    30,    31,     0,   196,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    26,    60,    28,    29,    30,    31,
       0,   196,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    26,    60,    28,    29,
      30,    31,     0,   196,   191,   192,     3,     4,     0,     5,
       0,     6,     7,   193,     0,   194,    10,     0,    59,   463,
      12,     0,     0,     0,    13,    14,   191,   192,     3,     4,
       0,     5,     0,     6,     7,   193,     0,   194,    10,     0,
      59,     0,    12,   464,     0,     0,    13,    14,     3,     4,
       0,     5,     0,     6,     7,     0,     0,     0,    10,     0,
      59,   185,    12,     0,     0,     0,    13,    14,     0,     0,
       0,     0,    26,    60,    28,    29,    30,    31,     0,   196,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    26,    60,    28,    29,    30,    31,
       0,   196,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    26,    60,    28,    29,    30,    31,
       0,    70,     3,     4,     0,     5,     0,     6,     7,     0,
       0,     0,    10,     0,    59,   188,    12,     0,     0,     0,
      13,    14,     3,     4,     0,     5,     0,     6,     7,     0,
       0,     0,    10,     0,    59,     0,    12,   201,     0,     0,
      13,    14,     3,     4,     0,     5,     0,     6,     7,     0,
       0,     0,    10,     0,    59,     0,    12,   204,     0,     0,
      13,    14,     0,     0,     0,     0,     0,     0,    26,    60,
      28,    29,    30,    31,     0,    70,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    26,    60,
      28,    29,    30,    31,     0,    70,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    26,    60,
      28,    29,    30,    31,     0,    70,     3,     4,     0,     5,
       0,     6,     7,     0,     0,     0,    10,     0,    59,   379,
      12,     0,     0,     0,    13,    14,     3,     4,     0,     5,
       0,     6,     7,     0,     0,     0,    10,     0,    59,     0,
      12,   391,     0,     0,    13,    14,     3,     4,     0,     5,
       0,     6,     7,   238,   239,     0,    10,   240,    11,     0,
      12,     0,     0,     0,    13,    14,     0,     0,     0,     0,
       0,     0,    26,    60,    28,    29,    30,    31,     0,    70,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    26,    60,    28,    29,    30,    31,     0,    70,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    26,   241,    28,    29,    30,    31,     3,     4,
       0,     5,     0,     6,     7,   238,   239,     0,    10,     0,
      11,     0,    12,   246,     0,     0,    13,    14,     3,     4,
       0,     5,     0,     6,     7,   238,   239,     0,    10,   254,
      11,     0,    12,     0,     0,     0,    13,    14,     3,     4,
       0,     5,     0,     6,     7,     0,   338,     0,    10,     0,
      59,     0,    12,     0,     0,     0,    13,    14,     0,     0,
       0,     0,     0,     0,    26,   241,    28,    29,    30,    31,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    26,   241,    28,    29,    30,    31,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    25,     0,    26,    60,    28,    29,    30,    31,
       3,     4,     0,     5,     0,     6,     7,   238,   239,     0,
      10,     0,    11,     0,    12,     0,     0,     0,    13,    14,
       3,     4,     0,     5,     0,     6,     7,     0,   352,     0,
      10,     0,    59,     0,    12,     0,     0,     0,    13,    14,
       3,     4,     0,     5,     0,     6,     7,     0,   308,     0,
      10,     0,    59,     0,    12,     0,     0,     0,    13,    14,
       0,     0,     0,     0,     0,     0,    26,   241,    28,    29,
      30,    31,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    25,     0,    26,    60,    28,    29,
      30,    31,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    26,    60,    28,    29,
      30,    31,     3,     4,     0,     5,     0,     6,     7,     0,
     311,     0,    10,     0,    59,     0,    12,     0,     0,     0,
      13,    14,     3,     4,     0,     5,     0,     6,     7,     0,
     314,     0,    10,     0,    59,     0,    12,     0,     0,     0,
      13,    14,     3,     4,     0,     5,     0,     6,     7,     0,
     317,     0,    10,     0,    59,     0,    12,     0,     0,     0,
      13,    14,     0,     0,     0,     0,     0,     0,    26,    60,
      28,    29,    30,    31,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    26,    60,
      28,    29,    30,    31,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    26,    60,
      28,    29,    30,    31,     3,     4,     0,     5,     0,     6,
       7,     0,   320,     0,    10,     0,    59,     0,    12,     0,
       0,     0,    13,    14,     3,     4,     0,     5,     0,     6,
       7,     0,   323,     0,    10,     0,    59,     0,    12,     0,
       0,     0,    13,    14,     3,     4,     0,     5,     0,     6,
       7,     0,   326,     0,    10,     0,    59,     0,    12,     0,
       0,     0,    13,    14,     0,     0,     0,     0,     0,     0,
      26,    60,    28,    29,    30,    31,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      26,    60,    28,    29,    30,    31,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      26,    60,    28,    29,    30,    31,     3,     4,     0,     5,
       0,     6,     7,     0,   329,     0,    10,     0,    59,     0,
      12,     0,     0,     0,    13,    14,     3,     4,     0,     5,
       0,     6,     7,     0,   332,     0,    10,     0,    59,     0,
      12,     0,     0,     0,    13,    14,     3,     4,     0,     5,
       0,     6,     7,     0,   335,     0,    10,     0,    59,     0,
      12,     0,     0,     0,    13,    14,     0,     0,     0,     0,
       0,     0,    26,    60,    28,    29,    30,    31,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    26,    60,    28,    29,    30,    31,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    26,    60,    28,    29,    30,    31,     3,     4,
       0,     5,     0,     6,     7,     0,   434,     0,    10,     0,
      11,     0,    12,     0,     0,     0,    13,    14,     3,     4,
       0,     5,     0,     6,     7,     0,   497,     0,    10,     0,
      11,     0,    12,     0,     0,     0,    13,    14,     3,     4,
       0,     5,     0,     6,     7,     0,     0,     0,    10,     0,
      59,     0,    12,     0,     0,     0,    13,    14,     0,     0,
       0,     0,     0,     0,    26,   241,    28,    29,    30,    31,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    26,   241,    28,    29,    30,    31,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    26,    60,    28,    29,    30,    31,
     542,     0,     0,     0,   121,     0,     0,     0,     0,   543,
     122,   544,   171,     0,     0,     0,     0,     0,   172,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   542,     0,     0,   545,
     157,     0,     0,     0,     0,   543,   122,   544,   173,     0,
       0,     0,     0,     0,   174,   160,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     161,   162,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   121,     0,     0,   545,     0,     0,   122,     0,
     123,     0,     0,     0,     0,     0,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   121,     0,     0,     0,     0,
     179,   122,     0,   171,   180,     0,     0,     0,     0,   172,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   157,     0,     0,
       0,     0,   181,   122,     0,   173,   182,     0,     0,     0,
       0,   174,   160,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   161,   162,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   157,
       0,     0,     0,     0,     0,   122,     0,   158,     0,     0,
       0,     0,     0,   159,   160,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   161,
     162,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   121,     0,     0,     0,     0,     0,   122,     0,   171,
       0,     0,     0,     0,     0,   172,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   157,     0,     0,     0,     0,     0,   122,
       0,   173,     0,     0,     0,     0,     0,   174,   160,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   161,   162,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   121,     0,     0,     0,     0,
       0,   122,     0,   171,     0,     0,     0,     0,     0,   172,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   157,     0,     0,     0,     0,
       0,   122,     0,   173,     0,     0,     0,     0,     0,   174,
     160,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   161,   162,   142,   143,   144,
     145,   146,   147,   148,   149,   121,     0,     0,     0,     0,
       0,   122,     0,   171,     0,     0,     0,     0,     0,   172,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   157,     0,     0,     0,     0,     0,   122,
       0,   173,     0,     0,     0,     0,     0,   174,   160,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   161,   162,   142,   143,   144,   145,   146,
     147,   121,     0,     0,     0,     0,     0,   122,     0,   171,
       0,     0,     0,     0,     0,   172,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   157,     0,     0,     0,     0,     0,   122,
       0,   173,     0,     0,     0,     0,     0,   174,   160,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   161,   162,   157,     0,     0,     0,     0,
       0,     0,     0,   173,     0,     0,     0,     0,     0,   174,
     160,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   161,   162,   157,     0,     0,
       0,     0,     0,     0,     0,   173,     0,     0,     0,     0,
       0,   174,   160,     0,     0,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   161,   162,   256,
     257,   258,   259,   260,   261,   262,   263,   264,   265,   266,
     267,   268,   269,   270,     0,   271,   272,   273,     0,     0,
     274,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,     0,   271,   272,   273,
       0,     0,   363
};

static const yytype_int16 yycheck[] =
{
       0,    21,    45,   104,    89,   103,   104,    37,   211,   416,
     410,   509,   481,    73,     4,    12,    56,     4,    78,    62,
      16,     4,   123,    66,    24,   123,    13,    16,    15,    72,
      13,    17,    15,    73,    16,    59,    36,     4,    81,    79,
      83,    59,    12,   541,    87,    56,    13,    18,    15,     0,
     159,    62,   459,   460,    20,    79,    99,   158,     4,   528,
     158,    79,    59,   172,    19,   174,    53,    13,    50,    15,
     171,    68,   173,   171,    13,   173,    73,    73,    65,    76,
      56,    78,    21,   113,    73,     6,     7,    73,   118,    79,
       4,    12,    79,    93,    94,    95,    79,    73,    68,    13,
     569,    15,    73,    79,   602,   512,    76,    73,     4,     5,
       6,     7,    79,     9,    16,    11,    12,    13,    73,    15,
      16,   503,    18,   530,    20,   168,   186,     4,    24,    25,
      16,     6,     7,    79,    20,     4,    13,    12,    15,   570,
     238,    16,   202,     4,    13,    20,    15,     4,    10,   152,
      50,    72,    73,    74,    75,   198,    77,    78,    16,    16,
     591,   568,    20,   166,    50,    79,   548,   549,    73,    56,
      50,   547,   554,   555,   605,    50,    72,    73,    74,    75,
      76,    77,    69,    79,    53,   592,    13,    16,    65,   186,
     590,    16,    50,    22,    21,    20,   399,    72,    73,    74,
      75,   244,    77,    78,     4,   202,   582,   583,    54,    55,
     253,   587,   588,    13,     4,    15,     4,     4,   218,   249,
       4,    22,    14,    13,     4,    15,    13,    15,    15,    13,
       4,    15,   232,    13,   277,    15,   279,   237,   281,    13,
     283,    15,   285,    16,   287,    50,   289,    13,   291,    22,
     293,    17,   295,    22,   297,    13,   299,     4,   301,    17,
     303,    13,   305,     4,   307,    17,    13,   310,    15,     8,
     313,    13,    13,   316,    15,    17,   319,    16,     4,   322,
       4,    16,   325,    22,    23,   328,    13,    13,   331,    15,
      17,   334,    13,     4,   337,     8,    17,   340,     4,    38,
      39,    19,   403,    16,    53,   403,   349,    50,   351,    22,
      23,   354,    13,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    73,   413,    73,
      13,   374,     4,   376,    17,   378,     4,     5,     6,     7,
     438,     9,    50,    11,    12,    13,   389,    15,    16,    13,
      18,    19,    20,    17,     4,    50,    24,    25,    54,     8,
      56,    45,    50,    59,    60,     4,     4,    16,    50,   470,
      50,     4,   470,    22,    23,    73,    16,   420,    62,    54,
      73,    56,    66,     4,    59,    60,     4,     4,    72,    38,
      39,    50,    73,   436,   479,     4,     4,    81,     4,    83,
       4,     4,   445,    87,    72,    73,    74,    75,    76,    77,
       4,    79,    54,    55,    56,    99,    50,    17,    50,    56,
      56,     4,    17,    20,    50,    56,     4,     5,     6,     7,
      73,     9,    17,    11,    12,    13,     4,    15,    16,     4,
      18,     4,    20,    21,   529,     4,    24,    25,    17,     4,
      17,     4,     4,     4,    56,   475,   499,    56,     4,     4,
     503,     6,     7,     4,     9,    56,    11,    12,    17,    56,
       4,    16,     4,    18,    19,    20,    56,   477,    35,    24,
      25,    88,    38,    45,   168,   200,   471,   210,   566,   518,
     490,   216,   524,   590,    72,    73,    74,    75,    76,    77,
      62,    79,   501,   359,    66,   418,   549,   451,   534,   509,
      72,    -1,   555,   571,   198,   237,    -1,    -1,    -1,    81,
     563,    83,    -1,    -1,    -1,    87,    -1,    72,    73,    74,
      75,    76,    77,    -1,    79,    -1,    -1,    99,    -1,    -1,
      -1,   541,    -1,    -1,    -1,    -1,   546,   547,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     244,    -1,    -1,    -1,    -1,    -1,   566,    -1,    -1,   253,
      -1,   571,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   582,   583,    -1,    -1,    -1,   587,   588,    -1,
      -1,    -1,    -1,   277,    -1,   279,    -1,   281,    -1,   283,
      -1,   285,   602,   287,    -1,   289,   168,   291,    -1,   293,
      -1,   295,    -1,   297,    -1,   299,    -1,   301,    -1,   303,
      -1,   305,    -1,   307,    -1,    -1,   310,    -1,    -1,   313,
      -1,    -1,   316,    -1,    -1,   319,   198,    -1,   322,    -1,
      -1,   325,    -1,    -1,   328,    -1,    -1,   331,    -1,    -1,
     334,    -1,    -1,   337,    -1,    -1,   340,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   349,    -1,   351,    -1,    -1,
     354,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   244,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     374,   253,   376,    -1,   378,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   389,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   277,    -1,   279,    -1,   281,
      -1,   283,    -1,   285,    -1,   287,    -1,   289,    -1,   291,
      -1,   293,    -1,   295,    -1,   297,   420,   299,    -1,   301,
      -1,   303,    -1,   305,    -1,   307,    -1,    -1,   310,    -1,
      -1,   313,   436,    -1,   316,    -1,    -1,   319,    -1,    -1,
     322,   445,    -1,   325,    -1,    -1,   328,    -1,    -1,   331,
      -1,    -1,   334,    -1,     5,   337,    -1,    -1,   340,    10,
      11,    12,    13,    14,    -1,    16,    -1,   349,    -1,   351,
      21,    -1,   354,    -1,    25,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   374,    -1,   376,   499,   378,    -1,    -1,   503,
      -1,    -1,    -1,    54,    -1,    -1,    -1,   389,    59,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    68,    -1,    -1,
      -1,    -1,    73,    -1,    75,    76,    -1,    78,    79,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   420,    -1,
      -1,    -1,    -1,    -1,    -1,   549,    -1,    -1,    -1,    -1,
      -1,   555,   103,   104,   436,    -1,    -1,    -1,    -1,   563,
      -1,    -1,    -1,   445,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,   123,    -1,    -1,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,    -1,    -1,    -1,    -1,    -1,   158,   159,    -1,
     161,   162,    -1,    -1,    -1,   166,    -1,   499,    -1,   170,
     171,   503,   173,   174,    -1,    -1,    -1,    -1,   179,    -1,
     181,    -1,   183,    -1,    -1,   186,   187,    -1,    -1,   190,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   200,
      -1,   202,   203,    -1,   205,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   549,    -1,    -1,
      -1,    -1,    -1,   555,   225,    -1,    -1,     5,    -1,    -1,
      -1,   563,    10,    11,    12,    13,    14,   238,    16,    -1,
      -1,    -1,    -1,    21,    -1,    -1,    -1,    25,     4,     5,
       6,     7,    -1,     9,    -1,    11,    12,    13,    -1,    15,
      16,    -1,    18,    19,    20,    -1,    -1,    -1,    24,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    54,    -1,    -1,    -1,
      -1,    59,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      68,    -1,    -1,    -1,    -1,    73,    -1,    75,    76,    -1,
      78,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    72,    73,    74,    75,
      76,    77,    -1,    79,    -1,   103,   104,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   342,    -1,   344,   122,   123,    -1,    -1,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,    -1,    -1,    -1,    -1,   380,
     158,    -1,    -1,   161,   162,    -1,    -1,    -1,   166,    -1,
      -1,   392,   170,   171,    -1,   173,    -1,    -1,    -1,    -1,
      -1,   179,   403,   181,    -1,   183,    -1,    -1,   186,   187,
      -1,    -1,   190,    -1,    -1,    -1,    -1,   418,    -1,    -1,
      -1,    -1,   200,    -1,   202,   203,    -1,   205,     4,    -1,
       6,     7,    -1,     9,    -1,    11,    12,   438,    -1,    -1,
      16,    -1,    18,    -1,    20,    21,    -1,   225,    24,    25,
      -1,   452,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     238,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   470,
       0,     1,    -1,    -1,   475,    -1,     6,     7,    -1,     9,
      -1,    11,    12,    -1,    -1,    -1,    16,    17,    18,    -1,
      20,    -1,    -1,    -1,    24,    25,    72,    73,    74,    75,
      76,    77,    -1,    79,    -1,   506,    -1,    -1,    -1,   510,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   519,    -1,
      -1,    51,    52,    -1,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    -1,    66,    67,    68,    69,
      70,    -1,    72,    73,    74,    75,    76,    77,    -1,    79,
      -1,    -1,    -1,    -1,     0,     1,    -1,    -1,     4,    -1,
       6,     7,    -1,     9,   342,    11,    12,    13,     8,    15,
      16,    -1,    18,    -1,    20,    -1,    16,    -1,    24,    25,
      -1,    -1,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    37,    38,    39,
      -1,    -1,   380,    -1,    -1,    51,    52,    -1,    -1,    -1,
      -1,    57,    58,    -1,   392,    61,    -1,    63,    64,    -1,
      66,    67,    68,    -1,    70,   403,    72,    73,    74,    75,
      76,    77,    -1,    79,    -1,    -1,    -1,     0,     1,    -1,
     418,     4,    -1,     6,     7,    -1,     9,    -1,    11,    12,
      13,    -1,    15,    16,    -1,    18,    -1,    20,    -1,    -1,
     438,    24,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   452,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    51,    52,
      -1,    -1,   470,    -1,    57,    58,    -1,   475,    61,    -1,
      63,    64,    -1,    66,    67,    68,    -1,    70,    -1,    72,
      73,    74,    75,    76,    77,    -1,    79,    -1,    -1,     1,
      -1,    -1,     4,    -1,     6,     7,    -1,     9,   506,    11,
      12,    13,   510,    15,    16,    -1,    18,    -1,    20,    -1,
      -1,   519,    24,    25,    -1,    -1,    -1,    -1,     1,    -1,
      -1,     4,    -1,     6,     7,    -1,     9,    -1,    11,    12,
      13,    -1,    15,    16,    -1,    18,    -1,    20,    -1,    51,
      52,    24,    25,    -1,    56,    57,    58,    -1,    -1,    61,
      -1,    63,    64,    -1,    66,    67,    68,    69,    70,    -1,
      72,    73,    74,    75,    76,    77,    -1,    79,    51,    52,
      -1,    -1,    -1,    56,    57,    58,    -1,    -1,    61,    -1,
      63,    64,    -1,    66,    67,    68,    16,    70,    -1,    72,
      73,    74,    75,    76,    77,     1,    79,    -1,    -1,    -1,
       6,     7,    -1,     9,    -1,    11,    12,    -1,    -1,    -1,
      16,    -1,    18,    -1,    20,    -1,    -1,    -1,    24,    25,
      -1,    -1,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    -1,    68,    69,
      70,    -1,    -1,    73,    -1,    51,    52,    -1,    54,    -1,
      56,    57,    58,    59,    60,    61,    -1,    63,    64,    -1,
      66,    67,    68,    16,    70,    -1,    72,    73,    74,    75,
      76,    77,     1,    79,    -1,    -1,    -1,     6,     7,    -1,
       9,    -1,    11,    12,    -1,    -1,    -1,    16,    -1,    18,
      -1,    20,    -1,    -1,    -1,    24,    25,    -1,    -1,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    -1,    68,    69,    70,    -1,    -1,
      73,    -1,    51,    52,    -1,    54,    55,    56,    57,    58,
      -1,    -1,    61,    -1,    63,    64,    -1,    66,    67,    68,
      -1,    70,    -1,    72,    73,    74,    75,    76,    77,     1,
      79,    -1,    -1,    -1,     6,     7,    -1,     9,    -1,    11,
      12,    -1,    -1,    -1,    16,    -1,    18,    -1,    20,    -1,
      -1,    -1,    24,    25,    -1,    -1,    -1,    -1,     1,    -1,
      -1,     4,    -1,     6,     7,    -1,     9,    -1,    11,    12,
      -1,    -1,    -1,    16,    17,    18,    -1,    20,    -1,    51,
      52,    24,    25,    -1,    56,    57,    58,    -1,    -1,    61,
      -1,    63,    64,    -1,    66,    67,    68,    69,    70,    -1,
      72,    73,    74,    75,    76,    77,    -1,    79,    51,    52,
      -1,    -1,    -1,    -1,    57,    58,    -1,    -1,    61,    -1,
      63,    64,    -1,    66,    67,    68,    -1,    70,    -1,    72,
      73,    74,    75,    76,    77,     1,    79,    -1,    -1,    -1,
       6,     7,    -1,     9,    -1,    11,    12,    -1,    -1,    -1,
      16,    -1,    18,    -1,    20,    -1,    -1,    -1,    24,    25,
      -1,    -1,    -1,    -1,     1,    -1,    -1,     4,    -1,     6,
       7,    -1,     9,    -1,    11,    12,    -1,    -1,    -1,    16,
      17,    18,    -1,    20,    -1,    51,    52,    24,    25,    -1,
      56,    57,    58,    -1,    -1,    61,    62,    63,    64,    -1,
      66,    67,    68,    -1,    70,    -1,    72,    73,    74,    75,
      76,    77,    -1,    79,    51,    52,    -1,    -1,    -1,    -1,
      57,    58,    -1,    -1,    61,    -1,    63,    64,    -1,    66,
      67,    68,    -1,    70,    -1,    72,    73,    74,    75,    76,
      77,     1,    79,    -1,    -1,    -1,     6,     7,    -1,     9,
      -1,    11,    12,    -1,    -1,    -1,    16,    -1,    18,    -1,
      20,    -1,    -1,    -1,    24,    25,    -1,    -1,    -1,    -1,
       1,    -1,    -1,    -1,    -1,     6,     7,    -1,     9,    -1,
      11,    12,    -1,    -1,    -1,    16,    17,    18,    -1,    20,
      -1,    51,    52,    24,    25,    -1,    56,    57,    58,    -1,
      -1,    61,    -1,    63,    64,    -1,    66,    67,    68,    -1,
      70,    -1,    72,    73,    74,    75,    76,    77,    -1,    79,
      51,    52,    -1,    -1,    -1,    -1,    57,    58,    -1,    -1,
      61,    -1,    63,    64,    -1,    66,    67,    68,    -1,    70,
      -1,    72,    73,    74,    75,    76,    77,     1,    79,    -1,
      -1,    -1,     6,     7,    -1,     9,    -1,    11,    12,    -1,
      -1,    -1,    16,    -1,    18,    -1,    20,    -1,    -1,    -1,
      24,    25,    -1,    -1,    -1,    -1,     1,    -1,    -1,    -1,
      -1,     6,     7,    -1,     9,    -1,    11,    12,    -1,    -1,
      -1,    16,    -1,    18,    -1,    20,    -1,    51,    52,    24,
      25,    -1,    56,    57,    58,    -1,    -1,    61,    -1,    63,
      64,    -1,    66,    67,    68,    -1,    70,    -1,    72,    73,
      74,    75,    76,    77,    -1,    79,    51,    52,    -1,    -1,
      -1,    56,    57,    58,    -1,    -1,    61,    -1,    63,    64,
      -1,    66,    67,    68,    -1,    70,    -1,    72,    73,    74,
      75,    76,    77,     1,    79,    -1,    -1,    -1,     6,     7,
      -1,     9,    -1,    11,    12,    -1,    -1,    -1,    16,    -1,
      18,    -1,    20,    -1,    -1,    -1,    24,    25,     4,     5,
       6,     7,    -1,     9,    -1,    11,    12,    13,    -1,    15,
      16,    -1,    18,    19,    20,    -1,    -1,    -1,    24,    25,
      -1,    -1,    -1,    51,    52,    -1,    -1,    -1,    -1,    57,
      58,    -1,    -1,    61,    -1,    63,    64,    -1,    66,    67,
      68,    -1,    70,    -1,    72,    73,    74,    75,    76,    77,
      -1,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    72,    73,    74,    75,
      76,    77,    -1,    79,     4,     5,     6,     7,    -1,     9,
      -1,    11,    12,    13,    -1,    15,    16,    -1,    18,    19,
      20,    -1,    -1,    -1,    24,    25,     4,     5,     6,     7,
      -1,     9,    -1,    11,    12,    13,    -1,    15,    16,    -1,
      18,    -1,    20,    21,    -1,    -1,    24,    25,     4,     5,
       6,     7,    -1,     9,    -1,    11,    12,    13,    -1,    15,
      16,    -1,    18,    -1,    20,    21,    -1,    -1,    24,    25,
      -1,    -1,    72,    73,    74,    75,    76,    77,    -1,    79,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    72,    73,    74,    75,    76,    77,
      -1,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    72,    73,    74,    75,
      76,    77,    -1,    79,     4,     5,     6,     7,    -1,     9,
      -1,    11,    12,    13,    -1,    15,    16,    -1,    18,    19,
      20,    -1,    -1,    -1,    24,    25,     4,     5,     6,     7,
      -1,     9,    -1,    11,    12,    13,    -1,    15,    16,    -1,
      18,    -1,    20,    21,    -1,    -1,    24,    25,     6,     7,
      -1,     9,    -1,    11,    12,    -1,    -1,    -1,    16,    -1,
      18,    19,    20,    -1,    -1,    -1,    24,    25,    -1,    -1,
      -1,    -1,    72,    73,    74,    75,    76,    77,    -1,    79,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    72,    73,    74,    75,    76,    77,
      -1,    79,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    72,    73,    74,    75,    76,    77,
      -1,    79,     6,     7,    -1,     9,    -1,    11,    12,    -1,
      -1,    -1,    16,    -1,    18,    19,    20,    -1,    -1,    -1,
      24,    25,     6,     7,    -1,     9,    -1,    11,    12,    -1,
      -1,    -1,    16,    -1,    18,    -1,    20,    21,    -1,    -1,
      24,    25,     6,     7,    -1,     9,    -1,    11,    12,    -1,
      -1,    -1,    16,    -1,    18,    -1,    20,    21,    -1,    -1,
      24,    25,    -1,    -1,    -1,    -1,    -1,    -1,    72,    73,
      74,    75,    76,    77,    -1,    79,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,    73,
      74,    75,    76,    77,    -1,    79,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,    73,
      74,    75,    76,    77,    -1,    79,     6,     7,    -1,     9,
      -1,    11,    12,    -1,    -1,    -1,    16,    -1,    18,    19,
      20,    -1,    -1,    -1,    24,    25,     6,     7,    -1,     9,
      -1,    11,    12,    -1,    -1,    -1,    16,    -1,    18,    -1,
      20,    21,    -1,    -1,    24,    25,     6,     7,    -1,     9,
      -1,    11,    12,    13,    14,    -1,    16,    17,    18,    -1,
      20,    -1,    -1,    -1,    24,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    72,    73,    74,    75,    76,    77,    -1,    79,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    72,    73,    74,    75,    76,    77,    -1,    79,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    72,    73,    74,    75,    76,    77,     6,     7,
      -1,     9,    -1,    11,    12,    13,    14,    -1,    16,    -1,
      18,    -1,    20,    21,    -1,    -1,    24,    25,     6,     7,
      -1,     9,    -1,    11,    12,    13,    14,    -1,    16,    17,
      18,    -1,    20,    -1,    -1,    -1,    24,    25,     6,     7,
      -1,     9,    -1,    11,    12,    -1,    14,    -1,    16,    -1,
      18,    -1,    20,    -1,    -1,    -1,    24,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    72,    73,    74,    75,    76,    77,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    72,    73,    74,    75,    76,    77,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    70,    -1,    72,    73,    74,    75,    76,    77,
       6,     7,    -1,     9,    -1,    11,    12,    13,    14,    -1,
      16,    -1,    18,    -1,    20,    -1,    -1,    -1,    24,    25,
       6,     7,    -1,     9,    -1,    11,    12,    -1,    14,    -1,
      16,    -1,    18,    -1,    20,    -1,    -1,    -1,    24,    25,
       6,     7,    -1,     9,    -1,    11,    12,    -1,    14,    -1,
      16,    -1,    18,    -1,    20,    -1,    -1,    -1,    24,    25,
      -1,    -1,    -1,    -1,    -1,    -1,    72,    73,    74,    75,
      76,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    70,    -1,    72,    73,    74,    75,
      76,    77,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    72,    73,    74,    75,
      76,    77,     6,     7,    -1,     9,    -1,    11,    12,    -1,
      14,    -1,    16,    -1,    18,    -1,    20,    -1,    -1,    -1,
      24,    25,     6,     7,    -1,     9,    -1,    11,    12,    -1,
      14,    -1,    16,    -1,    18,    -1,    20,    -1,    -1,    -1,
      24,    25,     6,     7,    -1,     9,    -1,    11,    12,    -1,
      14,    -1,    16,    -1,    18,    -1,    20,    -1,    -1,    -1,
      24,    25,    -1,    -1,    -1,    -1,    -1,    -1,    72,    73,
      74,    75,    76,    77,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,    73,
      74,    75,    76,    77,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    72,    73,
      74,    75,    76,    77,     6,     7,    -1,     9,    -1,    11,
      12,    -1,    14,    -1,    16,    -1,    18,    -1,    20,    -1,
      -1,    -1,    24,    25,     6,     7,    -1,     9,    -1,    11,
      12,    -1,    14,    -1,    16,    -1,    18,    -1,    20,    -1,
      -1,    -1,    24,    25,     6,     7,    -1,     9,    -1,    11,
      12,    -1,    14,    -1,    16,    -1,    18,    -1,    20,    -1,
      -1,    -1,    24,    25,    -1,    -1,    -1,    -1,    -1,    -1,
      72,    73,    74,    75,    76,    77,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      72,    73,    74,    75,    76,    77,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      72,    73,    74,    75,    76,    77,     6,     7,    -1,     9,
      -1,    11,    12,    -1,    14,    -1,    16,    -1,    18,    -1,
      20,    -1,    -1,    -1,    24,    25,     6,     7,    -1,     9,
      -1,    11,    12,    -1,    14,    -1,    16,    -1,    18,    -1,
      20,    -1,    -1,    -1,    24,    25,     6,     7,    -1,     9,
      -1,    11,    12,    -1,    14,    -1,    16,    -1,    18,    -1,
      20,    -1,    -1,    -1,    24,    25,    -1,    -1,    -1,    -1,
      -1,    -1,    72,    73,    74,    75,    76,    77,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    72,    73,    74,    75,    76,    77,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    72,    73,    74,    75,    76,    77,     6,     7,
      -1,     9,    -1,    11,    12,    -1,    14,    -1,    16,    -1,
      18,    -1,    20,    -1,    -1,    -1,    24,    25,     6,     7,
      -1,     9,    -1,    11,    12,    -1,    14,    -1,    16,    -1,
      18,    -1,    20,    -1,    -1,    -1,    24,    25,     6,     7,
      -1,     9,    -1,    11,    12,    -1,    -1,    -1,    16,    -1,
      18,    -1,    20,    -1,    -1,    -1,    24,    25,    -1,    -1,
      -1,    -1,    -1,    -1,    72,    73,    74,    75,    76,    77,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    72,    73,    74,    75,    76,    77,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    72,    73,    74,    75,    76,    77,
       4,    -1,    -1,    -1,     8,    -1,    -1,    -1,    -1,    13,
      14,    15,    16,    -1,    -1,    -1,    -1,    -1,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,     4,    -1,    -1,    53,
       8,    -1,    -1,    -1,    -1,    13,    14,    15,    16,    -1,
      -1,    -1,    -1,    -1,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,     8,    -1,    -1,    53,    -1,    -1,    14,    -1,
      16,    -1,    -1,    -1,    -1,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,     8,    -1,    -1,    -1,    -1,
      13,    14,    -1,    16,    17,    -1,    -1,    -1,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    49,     8,    -1,    -1,
      -1,    -1,    13,    14,    -1,    16,    17,    -1,    -1,    -1,
      -1,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,     8,
      -1,    -1,    -1,    -1,    -1,    14,    -1,    16,    -1,    -1,
      -1,    -1,    -1,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    42,    43,    44,    45,    46,    47,    48,
      49,     8,    -1,    -1,    -1,    -1,    -1,    14,    -1,    16,
      -1,    -1,    -1,    -1,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,    40,    41,    42,    43,    44,    45,    46,
      47,    48,    49,     8,    -1,    -1,    -1,    -1,    -1,    14,
      -1,    16,    -1,    -1,    -1,    -1,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,     8,    -1,    -1,    -1,    -1,
      -1,    14,    -1,    16,    -1,    -1,    -1,    -1,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,     8,    -1,    -1,    -1,    -1,
      -1,    14,    -1,    16,    -1,    -1,    -1,    -1,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,    46,    47,     8,    -1,    -1,    -1,    -1,
      -1,    14,    -1,    16,    -1,    -1,    -1,    -1,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,     8,    -1,    -1,    -1,    -1,    -1,    14,
      -1,    16,    -1,    -1,    -1,    -1,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,     8,    -1,    -1,    -1,    -1,    -1,    14,    -1,    16,
      -1,    -1,    -1,    -1,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,    38,    39,     8,    -1,    -1,    -1,    -1,    -1,    14,
      -1,    16,    -1,    -1,    -1,    -1,    -1,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,     8,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    16,    -1,    -1,    -1,    -1,    -1,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,    39,     8,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    16,    -1,    -1,    -1,    -1,
      -1,    22,    23,    -1,    -1,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    -1,    68,    69,    70,    -1,    -1,
      73,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    65,    66,    -1,    68,    69,    70,
      -1,    -1,    73
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     4,     6,     7,     9,    11,    12,    13,    15,
      16,    18,    20,    24,    25,    51,    52,    57,    58,    61,
      63,    64,    66,    67,    68,    70,    72,    73,    74,    75,
      76,    77,    79,    90,    91,    92,    93,    94,    95,    97,
      98,   100,   101,   109,   111,   115,   117,   118,   124,   125,
     126,   127,   134,   141,   142,   149,   153,   156,   158,    18,
      73,    97,   115,    16,   104,    97,   115,   116,     4,    19,
      79,    97,   115,   119,   121,   122,     4,    21,   119,   122,
      97,   115,    97,   115,     4,    97,   108,   115,    18,    73,
      16,    73,   108,     4,    13,    15,    91,   157,    97,   115,
       6,     7,    12,    16,    20,    72,    73,    74,    75,    77,
      78,    96,     0,    94,     4,    13,    15,    91,    79,    93,
      96,     8,    14,    16,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,   110,   112,   113,   114,     8,    16,    22,
      23,    38,    39,   110,   112,   114,    50,    97,   115,   144,
     122,    16,    22,    16,    22,    17,    73,   105,    10,    13,
      17,    13,    17,    13,    17,    19,   119,   122,    19,   121,
     122,     4,     5,    13,    15,    19,    79,    97,   115,   120,
     123,    21,   119,   122,    21,   122,    21,    56,    73,    79,
     128,   129,   130,     4,    13,    15,    53,   137,   138,    19,
     103,   105,    50,   104,    73,    50,     4,    13,    15,    53,
      65,    79,   155,    91,    91,    91,    56,    69,    13,    14,
      17,    73,    97,    99,   115,   124,    21,    99,    22,    79,
      93,    93,    97,   115,    17,    99,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    68,    69,    70,    73,   161,    97,   115,    97,   115,
      97,   115,    97,   115,    97,   115,    97,   115,    97,   115,
      97,   115,    97,   115,    97,   115,    97,   115,    97,   115,
      97,   115,    97,   115,    97,   115,    97,   115,    14,    97,
     115,    14,    97,   115,    14,    97,   115,    14,    97,   115,
      14,    97,   115,    14,    97,   115,    14,    97,   115,    14,
      97,   115,    14,    97,   115,    14,    97,   115,    14,    97,
     115,   158,    14,    99,    16,    73,    97,   161,    97,   115,
      97,   115,    14,    97,   115,   158,     4,    13,    15,    79,
     145,    19,    99,    73,   161,    99,    73,    97,   161,    13,
      17,     4,    16,    97,   115,    97,   115,    97,   115,    19,
     122,    19,    19,     4,    15,     5,    13,    79,    97,   115,
     120,    21,   122,    21,    21,    22,     4,    56,    79,   129,
       4,    79,   160,    16,   131,     4,     4,   137,    53,    91,
     135,    50,    19,    73,    13,    15,   106,   160,    50,    97,
     115,   150,     4,     4,     4,    13,    15,     4,    13,    15,
       4,    91,   154,   157,    14,    97,   115,   124,    13,    17,
      21,    73,    93,    17,    97,   115,    17,    73,    97,     4,
       4,   145,    59,    79,   146,   159,    17,    17,    73,    16,
       4,    91,   107,    19,    21,    73,     4,   160,     4,    99,
      20,    73,   132,     4,    54,    55,    56,   139,   140,    73,
      50,   104,     4,     4,   107,   150,     4,    13,    15,    65,
     151,     4,     4,     4,     4,    56,    56,    14,    97,   115,
     124,   146,    97,   115,     4,    56,    59,    60,   139,   143,
      59,    79,     4,   107,   107,    17,    17,    99,   132,    50,
     133,     4,    13,    15,   108,    91,   136,    56,   104,    73,
     106,    56,    62,   102,    17,     4,     4,     4,    91,   152,
      56,   143,     4,    13,    15,    53,   148,   148,    97,   115,
       4,    13,    15,   136,    97,   115,     4,   107,    17,    17,
      21,   133,    97,   115,     4,     4,   138,    56,   106,   104,
     107,   151,    56,   136,     4,     4,     4,    13,    15,    91,
     147,   147,   148,   148,     4,     4,    56,   148,   148,    17,
     135,   107,   106,   102,   152,    56,     4,     4,   147,   147,
     147,   147,   139,   140,   102,   107,    56,   136,   102
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,    89,    90,    90,    90,    90,    91,    91,    91,    91,
      91,    92,    92,    92,    92,    93,    93,    93,    93,    93,
      93,    94,    94,    94,    94,    94,    94,    94,    94,    94,
      94,    94,    94,    94,    94,    94,    94,    95,    95,    96,
      96,    96,    96,    96,    96,    96,    96,    96,    96,    97,
      97,    98,    98,    98,    98,    99,    99,    99,    99,    99,
      99,    99,    99,    99,    99,    99,    99,    99,    99,   100,
     100,   100,   100,   101,   101,   101,   101,   102,   102,   103,
     104,   104,   104,   105,   105,   106,   106,   106,   106,   106,
     107,   107,   108,   108,   109,   109,   110,   110,   110,   110,
     110,   110,   110,   110,   110,   110,   110,   110,   110,   110,
     110,   110,   110,   110,   110,   110,   110,   110,   110,   110,
     110,   110,   110,   110,   110,   110,   111,   111,   111,   111,
     111,   111,   111,   111,   111,   111,   111,   111,   111,   111,
     111,   111,   111,   111,   112,   112,   112,   112,   112,   112,
     112,   112,   112,   112,   112,   112,   112,   112,   112,   112,
     112,   112,   112,   112,   112,   112,   112,   112,   112,   112,
     112,   112,   113,   113,   114,   114,   114,   115,   115,   115,
     115,   115,   115,   115,   115,   115,   115,   115,   115,   115,
     115,   115,   115,   115,   115,   115,   115,   115,   115,   115,
     115,   115,   115,   115,   115,   116,   116,   116,   116,   116,
     116,   117,   117,   117,   117,   117,   117,   117,   117,   118,
     118,   118,   118,   118,   118,   118,   118,   119,   119,   120,
     120,   120,   120,   121,   121,   122,   122,   122,   122,   122,
     122,   122,   122,   122,   123,   123,   123,   123,   124,   124,
     124,   124,   124,   124,   124,   124,   125,   125,   125,   125,
     125,   125,   125,   125,   125,   126,   127,   127,   128,   128,
     128,   128,   129,   129,   130,   130,   131,   131,   132,   132,
     133,   133,   133,   134,   134,   134,   135,   135,   136,   136,
     137,   137,   137,   137,   137,   138,   138,   138,   138,   138,
     138,   139,   139,   139,   139,   139,   139,   140,   140,   140,
     141,   141,   141,   141,   142,   142,   143,   143,   143,   143,
     143,   143,   143,   144,   144,   145,   145,   145,   145,   145,
     146,   146,   146,   146,   146,   146,   147,   147,   148,   148,
     148,   148,   148,   148,   148,   148,   148,   148,   148,   148,
     149,   149,   150,   150,   151,   151,   151,   151,   151,   151,
     151,   151,   152,   152,   153,   154,   154,   155,   155,   155,
     155,   155,   155,   155,   155,   155,   155,   155,   155,   155,
     155,   155,   155,   155,   155,   156,   156,   157,   157,   157,
     157,   157,   157,   158,   158,   158,   159,   159,   160,   160,
     161,   161,   161,   161,   161,   161,   161,   161,   161,   161,
     161,   161,   161,   161,   161,   161,   161,   161
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     1,     0,     1,     2,     3,     1,
       2,     3,     4,     3,     2,     1,     1,     1,     2,     2,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     1,     1,
       3,     4,     4,     3,     3,     1,     1,     1,     1,     1,
       2,     2,     2,     2,     2,     3,     3,     3,     3,     8,
      10,     9,     6,     6,     7,     7,     8,     1,     1,     1,
       3,     2,     0,     3,     1,     1,     1,     2,     1,     2,
       1,     0,     1,     1,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     3,     3,     3,     3,     3,     3,     3,     3,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     3,     3,     1,     2,     2,     3,
       3,     3,     3,     3,     2,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       3,     1,     4,     4,     3,     3,     3,     3,     3,     3,
       3,     3,     4,     4,     5,     3,     4,     3,     2,     3,
       4,     4,     5,     3,     4,     3,     2,     2,     1,     1,
       1,     2,     2,     2,     3,     3,     3,     2,     2,     2,
       3,     1,     1,     1,     2,     2,     1,     1,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     1,     1,     4,     4,     3,     4,     3,     3,     3,
       2,     2,     4,     5,     1,     3,     3,     0,     3,     0,
       2,     2,     0,     5,     7,     6,     1,     0,     1,     0,
       1,     2,     1,     2,     1,     1,     2,     3,     2,     1,
       0,     1,     2,     2,     2,     3,     3,     4,     6,     5,
       5,     7,     6,     8,     1,     1,     1,     1,     2,     2,
       2,     3,     3,     1,     1,     1,     2,     2,     1,     1,
       4,     4,     5,     5,     5,     5,     1,     0,     1,     1,
       1,     1,     2,     2,     2,     2,     3,     2,     3,     0,
       7,     9,     1,     1,     1,     1,     2,     1,     2,     1,
       2,     0,     1,     0,     5,     1,     0,     1,     1,     1,
       2,     2,     1,     2,     2,     2,     1,     2,     2,     2,
       3,     3,     2,     3,     3,     5,     3,     1,     2,     2,
       2,     1,     0,     1,     2,     2,     2,     3,     1,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  if defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc)  YY_LOCATION_PRINT(File, *(Loc))

#  elif defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
}

#   define YYLOCATION_PRINT  yy_location_print_

    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT(File, Loc)  YYLOCATION_PRINT(File, &(Loc))

#  else

#   define YYLOCATION_PRINT(File, Loc) ((void) 0)
    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT  YYLOCATION_PRINT

#  endif
# endif /* !defined YYLOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YYLOCATION_PRINT (yyo, yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]));
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  switch (yykind)
    {
    case YYSYMBOL_STR: /* "string"  */
            { delete ((*yyvaluep).str); }
        break;

    case YYSYMBOL_ID: /* "identifier"  */
            { delete ((*yyvaluep).str); }
        break;

    case YYSYMBOL_VARINT: /* "integer"  */
            { }
        break;

    case YYSYMBOL_VARFLOAT: /* "float"  */
            { }
        break;

    case YYSYMBOL_COMPLEXNUM: /* "complex number"  */
            { }
        break;

    case YYSYMBOL_NUM: /* "number"  */
            { }
        break;

    case YYSYMBOL_PATH: /* "path"  */
            { delete ((*yyvaluep).path); }
        break;

    case YYSYMBOL_COMMENT: /* "line comment"  */
            { delete ((*yyvaluep).comment); }
        break;

    case YYSYMBOL_BLOCKCOMMENT: /* "block comment"  */
            { delete ((*yyvaluep).comment); }
        break;

    case YYSYMBOL_expressions: /* expressions  */
            { delete ((*yyvaluep).t_seq_exp); }
        break;

    case YYSYMBOL_recursiveExpression: /* recursiveExpression  */
            { for (auto e : *((*yyvaluep).t_list_exp)) delete e; delete ((*yyvaluep).t_list_exp); }
        break;

    case YYSYMBOL_expressionLineBreak: /* expressionLineBreak  */
            { delete ((*yyvaluep).mute); }
        break;

    case YYSYMBOL_expression: /* expression  */
            { delete ((*yyvaluep).t_exp); }
        break;

    case YYSYMBOL_implicitFunctionCall: /* implicitFunctionCall  */
            { delete ((*yyvaluep).t_call_exp); }
        break;

    case YYSYMBOL_implicitCallable: /* implicitCallable  */
            { delete ((*yyvaluep).t_string_exp); }
        break;

    case YYSYMBOL_functionCall: /* functionCall  */
            { delete ((*yyvaluep).t_call_exp); }
        break;

    case YYSYMBOL_simpleFunctionCall: /* simpleFunctionCall  */
            { delete ((*yyvaluep).t_call_exp); }
        break;

    case YYSYMBOL_functionArgs: /* functionArgs  */
            { for (auto e : *((*yyvaluep).t_list_exp)) delete e; delete ((*yyvaluep).t_list_exp); }
        break;

    case YYSYMBOL_functionDeclaration: /* functionDeclaration  */
            { delete ((*yyvaluep).t_function_dec); }
        break;

    case YYSYMBOL_lambdaFunctionDeclaration: /* lambdaFunctionDeclaration  */
            { delete ((*yyvaluep).t_function_dec); }
        break;

    case YYSYMBOL_functionDeclarationReturns: /* functionDeclarationReturns  */
            { for (auto e : *((*yyvaluep).t_list_var)) delete e; delete ((*yyvaluep).t_list_var); }
        break;

    case YYSYMBOL_functionDeclarationArguments: /* functionDeclarationArguments  */
            { for (auto e : *((*yyvaluep).t_list_var)) delete e; delete ((*yyvaluep).t_list_var); }
        break;

    case YYSYMBOL_idList: /* idList  */
            { for (auto e : *((*yyvaluep).t_list_var)) delete e; delete ((*yyvaluep).t_list_var); }
        break;

    case YYSYMBOL_functionBody: /* functionBody  */
            { delete ((*yyvaluep).t_seq_exp); }
        break;

    case YYSYMBOL_condition: /* condition  */
            { delete ((*yyvaluep).t_exp); }
        break;

    case YYSYMBOL_comparison: /* comparison  */
            { delete ((*yyvaluep).t_op_exp); }
        break;

    case YYSYMBOL_rightComparable: /* rightComparable  */
            { delete ((*yyvaluep).t_op_exp); }
        break;

    case YYSYMBOL_operation: /* operation  */
            { delete ((*yyvaluep).t_exp); }
        break;

    case YYSYMBOL_rightOperand: /* rightOperand  */
            { delete ((*yyvaluep).t_op_exp); }
        break;

    case YYSYMBOL_listableBegin: /* listableBegin  */
            { delete ((*yyvaluep).t_exp); }
        break;

    case YYSYMBOL_listableEnd: /* listableEnd  */
            { delete ((*yyvaluep).t_implicit_list); }
        break;

    case YYSYMBOL_variable: /* variable  */
            { delete ((*yyvaluep).t_exp); }
        break;

    case YYSYMBOL_variableFields: /* variableFields  */
            { for (auto e : *((*yyvaluep).t_list_exp)) delete e; delete ((*yyvaluep).t_list_exp); }
        break;

    case YYSYMBOL_cell: /* cell  */
            { delete ((*yyvaluep).t_cell_exp); }
        break;

    case YYSYMBOL_matrix: /* matrix  */
            { delete ((*yyvaluep).t_matrix_exp); }
        break;

    case YYSYMBOL_matrixOrCellLines: /* matrixOrCellLines  */
            { for (auto e : *((*yyvaluep).t_list_mline)) delete e; delete ((*yyvaluep).t_list_mline); }
        break;

    case YYSYMBOL_matrixOrCellLine: /* matrixOrCellLine  */
            { delete ((*yyvaluep).t_matrixline_exp); }
        break;

    case YYSYMBOL_matrixOrCellColumns: /* matrixOrCellColumns  */
            { for (auto e : *((*yyvaluep).t_list_exp)) delete e; delete ((*yyvaluep).t_list_exp); }
        break;

    case YYSYMBOL_variableDeclaration: /* variableDeclaration  */
            { delete ((*yyvaluep).t_assign_exp); }
        break;

    case YYSYMBOL_assignable: /* assignable  */
            { delete ((*yyvaluep).t_exp); }
        break;

    case YYSYMBOL_multipleResults: /* multipleResults  */
            { delete ((*yyvaluep).t_assignlist_exp); }
        break;

    case YYSYMBOL_argumentsControl: /* argumentsControl  */
            { delete ((*yyvaluep).t_arguments_exp); }
        break;

    case YYSYMBOL_argumentsDeclarations: /* argumentsDeclarations  */
            { delete ((*yyvaluep).t_arguments_exp); }
        break;

    case YYSYMBOL_argumentDeclaration: /* argumentDeclaration  */
            { delete ((*yyvaluep).t_argument_dec); }
        break;

    case YYSYMBOL_argumentName: /* argumentName  */
            { delete ((*yyvaluep).t_exp); }
        break;

    case YYSYMBOL_argumentDimension: /* argumentDimension  */
            { delete ((*yyvaluep).t_exp); }
        break;

    case YYSYMBOL_argumentValidators: /* argumentValidators  */
            { delete ((*yyvaluep).t_exp); }
        break;

    case YYSYMBOL_argumentDefaultValue: /* argumentDefaultValue  */
            { delete ((*yyvaluep).t_exp); }
        break;

    case YYSYMBOL_ifControl: /* ifControl  */
            { delete ((*yyvaluep).t_if_exp); }
        break;

    case YYSYMBOL_thenBody: /* thenBody  */
            { delete ((*yyvaluep).t_seq_exp); }
        break;

    case YYSYMBOL_elseBody: /* elseBody  */
            { delete ((*yyvaluep).t_seq_exp); }
        break;

    case YYSYMBOL_elseIfControl: /* elseIfControl  */
            { delete ((*yyvaluep).t_seq_exp); }
        break;

    case YYSYMBOL_selectControl: /* selectControl  */
            { delete ((*yyvaluep).t_select_exp); }
        break;

    case YYSYMBOL_selectable: /* selectable  */
            { delete ((*yyvaluep).t_exp); }
        break;

    case YYSYMBOL_casesControl: /* casesControl  */
            { for (auto e : *((*yyvaluep).t_list_case)) delete e; delete ((*yyvaluep).t_list_case); }
        break;

    case YYSYMBOL_caseBody: /* caseBody  */
            { delete ((*yyvaluep).t_seq_exp); }
        break;

    case YYSYMBOL_forControl: /* forControl  */
            { delete ((*yyvaluep).t_for_exp); }
        break;

    case YYSYMBOL_forIterator: /* forIterator  */
            { delete ((*yyvaluep).t_exp); }
        break;

    case YYSYMBOL_forBody: /* forBody  */
            { delete ((*yyvaluep).t_seq_exp); }
        break;

    case YYSYMBOL_whileControl: /* whileControl  */
            { delete ((*yyvaluep).t_while_exp); }
        break;

    case YYSYMBOL_whileBody: /* whileBody  */
            { delete ((*yyvaluep).t_seq_exp); }
        break;

    case YYSYMBOL_tryControl: /* tryControl  */
            { delete ((*yyvaluep).t_try_exp); }
        break;

    case YYSYMBOL_catchBody: /* catchBody  */
            { delete ((*yyvaluep).t_seq_exp); }
        break;

    case YYSYMBOL_returnControl: /* returnControl  */
            { delete ((*yyvaluep).t_return_exp); }
        break;

    case YYSYMBOL_keywords: /* keywords  */
            { delete ((*yyvaluep).t_simple_var); }
        break;

      default:
        break;
    }
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  yylsp[0] = yylloc;
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      yyerror_range[1] = yylloc;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: expressions  */
                                { SetTree((yyvsp[0].t_seq_exp)); print_rules("program", "expressions");}
    break;

  case 3: /* program: expressionLineBreak expressions  */
                                  { SetTree((yyvsp[0].t_seq_exp)); delete (yyvsp[-1].mute); print_rules("program", "expressionLineBreak expressions");}
    break;

  case 4: /* program: expressionLineBreak  */
                                {
                                    print_rules("program", "expressionLineBreak");
                                    ast::exps_t* tmp = new ast::exps_t;
                                    #ifdef BUILD_DEBUG_AST
                                        tmp->push_back(new ast::CommentExp((yyloc), new std::wstring(L"Empty body")));
                                    #endif
                                    SetTree(new ast::SeqExp((yyloc), *tmp));
                                    delete (yyvsp[0].mute);
                                }
    break;

  case 5: /* program: %empty  */
                                {
                                    print_rules("program", "Epsilon");
                                    ast::exps_t* tmp = new ast::exps_t;
                                    #ifdef BUILD_DEBUG_AST
                                        tmp->push_back(new ast::CommentExp((yyloc), new std::wstring(L"Empty body")));
                                    #endif
                                    SetTree(new ast::SeqExp((yyloc), *tmp));
                                }
    break;

  case 6: /* expressions: recursiveExpression  */
                                                {
                                                  print_rules("expressions", "recursiveExpression");
                                                  (yyval.t_seq_exp) = new ast::SeqExp((yyloc), *(yyvsp[0].t_list_exp));
                                                }
    break;

  case 7: /* expressions: recursiveExpression expression  */
                                                {
                                                  print_rules("expressions", "recursiveExpression expression");
                                                  (yyvsp[0].t_exp)->setVerbose(true);
                                                  (yyvsp[-1].t_list_exp)->push_back((yyvsp[0].t_exp));
                                                  (yyval.t_seq_exp) = new ast::SeqExp((yyloc), *(yyvsp[-1].t_list_exp));
                                                }
    break;

  case 8: /* expressions: recursiveExpression expression "line comment"  */
                                                {
                                                  print_rules("expressions", "recursiveExpression expression COMMENT");
                                                  (yyvsp[-1].t_exp)->setVerbose(true);
                                                  (yyvsp[-2].t_list_exp)->push_back((yyvsp[-1].t_exp));
                                                  (yyvsp[-2].t_list_exp)->push_back(new ast::CommentExp((yylsp[0]), (yyvsp[0].comment)));
                                                  (yyval.t_seq_exp) = new ast::SeqExp((yyloc), *(yyvsp[-2].t_list_exp));
                                                }
    break;

  case 9: /* expressions: expression  */
                                                {
                                                  print_rules("expressions", "expression");
                                                  ast::exps_t* tmp = new ast::exps_t;
                                                  (yyvsp[0].t_exp)->setVerbose(true);
                                                  tmp->push_back((yyvsp[0].t_exp));
                                                  (yyval.t_seq_exp) = new ast::SeqExp((yyloc), *tmp);
                                                }
    break;

  case 10: /* expressions: expression "line comment"  */
                                                {
                                                  print_rules("expressions", "expression COMMENT");
                                                  ast::exps_t* tmp = new ast::exps_t;
                                                  (yyvsp[-1].t_exp)->setVerbose(true);
                                                  tmp->push_back((yyvsp[-1].t_exp));
                                                  tmp->push_back(new ast::CommentExp((yylsp[0]), (yyvsp[0].comment)));
                                                  (yyval.t_seq_exp) = new ast::SeqExp((yyloc), *tmp);
                                                }
    break;

  case 11: /* recursiveExpression: recursiveExpression expression expressionLineBreak  */
                                                      {
                              print_rules("recursiveExpression", "recursiveExpression expression expressionLineBreak");
                              (yyvsp[-1].t_exp)->setVerbose((yyvsp[0].mute)->bVerbose);
                              // set the expressionLineBreak last position to the expression
                              if((yyvsp[0].mute)->iNbBreaker)
                              {
                                (yyvsp[-1].t_exp)->getLocation().last_column = (yyvsp[0].mute)->iNbBreaker;
                              }
                              (yyvsp[-2].t_list_exp)->push_back((yyvsp[-1].t_exp));
                              (yyval.t_list_exp) = (yyvsp[-2].t_list_exp);
                              delete (yyvsp[0].mute);
                            }
    break;

  case 12: /* recursiveExpression: recursiveExpression expression "line comment" expressionLineBreak  */
                                                             {
                              print_rules("recursiveExpression", "recursiveExpression expression COMMENT expressionLineBreak");
                              (yyvsp[-2].t_exp)->setVerbose((yyvsp[0].mute)->bVerbose);
                              (yyvsp[-3].t_list_exp)->push_back((yyvsp[-2].t_exp));
                              (yyvsp[-3].t_list_exp)->push_back(new ast::CommentExp((yylsp[-1]), (yyvsp[-1].comment)));
                              (yyval.t_list_exp) = (yyvsp[-3].t_list_exp);
                              delete (yyvsp[0].mute);
                            }
    break;

  case 13: /* recursiveExpression: expression "line comment" expressionLineBreak  */
                                                {
                              print_rules("recursiveExpression", "expression COMMENT expressionLineBreak");
                              ast::exps_t* tmp = new ast::exps_t;
                              (yyvsp[-2].t_exp)->setVerbose((yyvsp[0].mute)->bVerbose);
                              tmp->push_back((yyvsp[-2].t_exp));
                              tmp->push_back(new ast::CommentExp((yylsp[-1]), (yyvsp[-1].comment)));
                              (yyval.t_list_exp) = tmp;
                              delete (yyvsp[0].mute);
                            }
    break;

  case 14: /* recursiveExpression: expression expressionLineBreak  */
                                            {
                              print_rules("recursiveExpression", "expression expressionLineBreak");
                              ast::exps_t* tmp = new ast::exps_t;
                              // set the expressionLineBreak last position to the expression
                              if((yyvsp[0].mute)->iNbBreaker)
                              {
                                (yyvsp[-1].t_exp)->getLocation().last_column = (yyvsp[0].mute)->iNbBreaker;
                              }
                              (yyvsp[-1].t_exp)->setVerbose((yyvsp[0].mute)->bVerbose);
                              tmp->push_back((yyvsp[-1].t_exp));
                              (yyval.t_list_exp) = tmp;
                              delete (yyvsp[0].mute);
                            }
    break;

  case 15: /* expressionLineBreak: ";"  */
                                { (yyval.mute) = new LineBreakStr(); (yyval.mute)->bVerbose = false; (yyval.mute)->iNbBreaker = (yylsp[0]).last_column; print_rules("expressionLineBreak", "SEMI"); }
    break;

  case 16: /* expressionLineBreak: ","  */
                                { (yyval.mute) = new LineBreakStr(); (yyval.mute)->bVerbose = true;  (yyval.mute)->iNbBreaker = (yylsp[0]).last_column; print_rules("expressionLineBreak", "COMMA"); }
    break;

  case 17: /* expressionLineBreak: "end of line"  */
                                { (yyval.mute) = new LineBreakStr(); (yyval.mute)->bVerbose = true;  (yyval.mute)->iNbBreaker = 0; print_rules("expressionLineBreak", "EOL");}
    break;

  case 18: /* expressionLineBreak: expressionLineBreak ";"  */
                                { (yyval.mute) = (yyvsp[-1].mute); print_rules("expressionLineBreak", "expressionLineBreak SEMI"); }
    break;

  case 19: /* expressionLineBreak: expressionLineBreak ","  */
                                { (yyval.mute) = (yyvsp[-1].mute); print_rules("expressionLineBreak", "expressionLineBreak COMMA"); }
    break;

  case 20: /* expressionLineBreak: expressionLineBreak "end of line"  */
                                { (yyval.mute) = (yyvsp[-1].mute); print_rules("expressionLineBreak", "expressionLineBreak EOL"); }
    break;

  case 21: /* expression: functionDeclaration  */
                                            { (yyval.t_exp) = (yyvsp[0].t_function_dec); print_rules("expression", "functionDeclaration");}
    break;

  case 22: /* expression: functionCall  */
                                            { (yyval.t_exp) = (yyvsp[0].t_call_exp); print_rules("expression", "functionCall");}
    break;

  case 23: /* expression: variableDeclaration  */
                                            { (yyval.t_exp) = (yyvsp[0].t_assign_exp); print_rules("expression", "variableDeclaration");}
    break;

  case 24: /* expression: argumentsControl  */
                                            { (yyval.t_exp) = (yyvsp[0].t_arguments_exp); print_rules("expression", "argumentsControl");}
    break;

  case 25: /* expression: ifControl  */
                                            { (yyval.t_exp) = (yyvsp[0].t_if_exp); print_rules("expression", "ifControl");}
    break;

  case 26: /* expression: selectControl  */
                                            { (yyval.t_exp) = (yyvsp[0].t_select_exp); print_rules("expression", "selectControl");}
    break;

  case 27: /* expression: forControl  */
                                            { (yyval.t_exp) = (yyvsp[0].t_for_exp); print_rules("expression", "forControl");}
    break;

  case 28: /* expression: whileControl  */
                                            { (yyval.t_exp) = (yyvsp[0].t_while_exp); print_rules("expression", "whileControl");}
    break;

  case 29: /* expression: tryControl  */
                                            { (yyval.t_exp) = (yyvsp[0].t_try_exp); print_rules("expression", "tryControl");}
    break;

  case 30: /* expression: variable  */
                                            { (yyval.t_exp) = (yyvsp[0].t_exp); print_rules("expression", "variable");}
    break;

  case 31: /* expression: implicitFunctionCall  */
                                            { (yyval.t_exp) = (yyvsp[0].t_call_exp); print_rules("expression", "implicitFunctionCall");}
    break;

  case 32: /* expression: "break"  */
                                            { (yyval.t_exp) = new ast::BreakExp((yyloc)); print_rules("expression", "BREAK");}
    break;

  case 33: /* expression: "continue"  */
                                            { (yyval.t_exp) = new ast::ContinueExp((yyloc)); print_rules("expression", "CONTINUE");}
    break;

  case 34: /* expression: returnControl  */
                                            { (yyval.t_exp) = (yyvsp[0].t_return_exp); print_rules("expression", "returnControl");}
    break;

  case 35: /* expression: "line comment"  */
                                            { (yyval.t_exp) = new ast::CommentExp((yyloc), (yyvsp[0].comment)); print_rules("expression", "COMMENT");}
    break;

  case 36: /* expression: error  */
                                   {
    print_rules("expression", "error");
    (yyval.t_exp) = new ast::CommentExp((yyloc), new std::wstring(L"@@ ERROR RECOVERY @@"));
    StopOnError();
  }
    break;

  case 37: /* implicitFunctionCall: implicitFunctionCall implicitCallable  */
                                             {
                          print_rules("implicitFunctionCall", "implicitFunctionCall implicitCallable");
                          (yyvsp[-1].t_call_exp)->addArg((yyvsp[0].t_string_exp));
                          (yyvsp[-1].t_call_exp)->setLocation((yyloc));
                          (yyval.t_call_exp) = (yyvsp[-1].t_call_exp);
                        }
    break;

  case 38: /* implicitFunctionCall: "identifier" implicitCallable  */
                                     {
                          print_rules("implicitFunctionCall", "ID implicitCallable");
                          ast::exps_t* tmp = new ast::exps_t;
                          tmp->push_back((yyvsp[0].t_string_exp));
                          (yyval.t_call_exp) = new ast::CallExp((yyloc), *new ast::SimpleVar((yylsp[-1]), symbol::Symbol(*(yyvsp[-1].str))), *tmp);
                          delete (yyvsp[-1].str);
                        }
    break;

  case 39: /* implicitCallable: "identifier"  */
                        { (yyval.t_string_exp) = new ast::StringExp((yyloc), *(yyvsp[0].str)); delete (yyvsp[0].str);print_rules("implicitCallable", "ID");}
    break;

  case 40: /* implicitCallable: "integer"  */
                        {
                              print_rules("implicitCallable", (yyvsp[0].number));
                              std::wstringstream tmp;
                              tmp << (yyvsp[0].number);
                              (yyval.t_string_exp) = new ast::StringExp((yyloc), tmp.str());
                        }
    break;

  case 41: /* implicitCallable: "number"  */
                        {
                              print_rules("implicitCallable", (yyvsp[0].number));
                              std::wstringstream tmp;
                              tmp << (yyvsp[0].number);
                              (yyval.t_string_exp) = new ast::StringExp((yyloc), tmp.str());
                        }
    break;

  case 42: /* implicitCallable: "float"  */
                        {
                              print_rules("implicitCallable", (yyvsp[0].number));
                              std::wstringstream tmp;
                              tmp << (yyvsp[0].number);
                              (yyval.t_string_exp) = new ast::StringExp((yyloc), tmp.str());
                        }
    break;

  case 43: /* implicitCallable: "string"  */
                        { (yyval.t_string_exp) = new ast::StringExp((yyloc), *(yyvsp[0].str)); delete (yyvsp[0].str);print_rules("implicitCallable", "STR");}
    break;

  case 44: /* implicitCallable: "$"  */
                        { (yyval.t_string_exp) = new ast::StringExp((yyloc), std::wstring(L"$")); print_rules("implicitCallable", "DOLLAR");}
    break;

  case 45: /* implicitCallable: "%t or %T"  */
                        { (yyval.t_string_exp) = new ast::StringExp((yyloc), std::wstring(L"%t")); print_rules("implicitCallable", "BOOLTRUE");}
    break;

  case 46: /* implicitCallable: "%f or %F"  */
                        { (yyval.t_string_exp) = new ast::StringExp((yyloc), std::wstring(L"%f")); print_rules("implicitCallable", "BOOLFALSE");}
    break;

  case 47: /* implicitCallable: implicitCallable "." "identifier"  */
                            {
                              print_rules("implicitCallable", "implicitCallable DOT ID");
                              std::wstringstream tmp;
                              tmp << (yyvsp[-2].t_string_exp)->getValue() << "." << *(yyvsp[0].str);
                              (yyval.t_string_exp) = new ast::StringExp((yyloc), tmp.str());
                              delete (yyvsp[0].str);
                        }
    break;

  case 48: /* implicitCallable: "path"  */
                        { (yyval.t_string_exp) = new ast::StringExp((yyloc), *(yyvsp[0].path)); delete (yyvsp[0].path);print_rules("implicitCallable", "PATH");}
    break;

  case 49: /* functionCall: simpleFunctionCall  */
                                { (yyval.t_call_exp) = (yyvsp[0].t_call_exp); print_rules("functionCall", "simpleFunctionCall");}
    break;

  case 50: /* functionCall: "(" functionCall ")"  */
                                { (yyval.t_call_exp) = (yyvsp[-1].t_call_exp); print_rules("functionCall", "LPAREN functionCall RPAREN");}
    break;

  case 51: /* simpleFunctionCall: "identifier" "(" functionArgs ")"  */
                                    { (yyval.t_call_exp) = new ast::CallExp((yyloc), *new ast::SimpleVar((yylsp[-3]), symbol::Symbol(*(yyvsp[-3].str))), *(yyvsp[-1].t_list_exp)); delete (yyvsp[-3].str);print_rules("simpleFunctionCall", "ID LPAREN functionArgs RPAREN");}
    break;

  case 52: /* simpleFunctionCall: "identifier" "{" functionArgs "}"  */
                                    { (yyval.t_call_exp) = new ast::CellCallExp((yyloc), *new ast::SimpleVar((yylsp[-3]), symbol::Symbol(*(yyvsp[-3].str))), *(yyvsp[-1].t_list_exp)); delete (yyvsp[-3].str);print_rules("simpleFunctionCall", "ID LBRACE functionArgs RBRACE");}
    break;

  case 53: /* simpleFunctionCall: "identifier" "(" ")"  */
                                    { (yyval.t_call_exp) = new ast::CallExp((yyloc), *new ast::SimpleVar((yylsp[-2]), symbol::Symbol(*(yyvsp[-2].str))), *new ast::exps_t); delete (yyvsp[-2].str);print_rules("simpleFunctionCall", "ID LPAREN RPAREN");}
    break;

  case 54: /* simpleFunctionCall: "identifier" "{" "}"  */
                                    { (yyval.t_call_exp) = new ast::CellCallExp((yyloc), *new ast::SimpleVar((yylsp[-2]), symbol::Symbol(*(yyvsp[-2].str))), *new ast::exps_t); delete (yyvsp[-2].str);print_rules("simpleFunctionCall", "ID LBRACE RBRACE");}
    break;

  case 55: /* functionArgs: variable  */
                                            {(yyval.t_list_exp) = new ast::exps_t;(yyval.t_list_exp)->push_back((yyvsp[0].t_exp));print_rules("functionArgs", "variable");}
    break;

  case 56: /* functionArgs: functionCall  */
                                            {(yyval.t_list_exp) = new ast::exps_t;(yyval.t_list_exp)->push_back((yyvsp[0].t_call_exp));print_rules("functionArgs", "functionCall");}
    break;

  case 57: /* functionArgs: ":"  */
                                            {(yyval.t_list_exp) = new ast::exps_t;(yyval.t_list_exp)->push_back(new ast::ColonVar((yylsp[0])));print_rules("functionArgs", "COLON");}
    break;

  case 58: /* functionArgs: variableDeclaration  */
                                            {(yyval.t_list_exp) = new ast::exps_t;(yyval.t_list_exp)->push_back((yyvsp[0].t_assign_exp));print_rules("functionArgs", "variableDeclaration");}
    break;

  case 59: /* functionArgs: ","  */
                                            {(yyval.t_list_exp) = new ast::exps_t;(yyval.t_list_exp)->push_back(new ast::NilExp((yylsp[0])));(yyval.t_list_exp)->push_back(new ast::NilExp((yylsp[0])));print_rules("functionArgs", "COMMA");}
    break;

  case 60: /* functionArgs: "," variable  */
                                            {(yyval.t_list_exp) = new ast::exps_t;(yyval.t_list_exp)->push_back(new ast::NilExp((yylsp[-1])));(yyval.t_list_exp)->push_back((yyvsp[0].t_exp));print_rules("functionArgs", "COMMA variable");}
    break;

  case 61: /* functionArgs: "," functionCall  */
                                            {(yyval.t_list_exp) = new ast::exps_t;(yyval.t_list_exp)->push_back(new ast::NilExp((yylsp[-1])));(yyval.t_list_exp)->push_back((yyvsp[0].t_call_exp));print_rules("functionArgs", "COMMA functionCall");}
    break;

  case 62: /* functionArgs: "," ":"  */
                                            {(yyval.t_list_exp) = new ast::exps_t;(yyval.t_list_exp)->push_back(new ast::NilExp((yylsp[-1])));(yyval.t_list_exp)->push_back(new ast::ColonVar((yylsp[0])));print_rules("functionArgs", "COMMA COLON");}
    break;

  case 63: /* functionArgs: "," variableDeclaration  */
                                            {(yyval.t_list_exp) = new ast::exps_t;(yyval.t_list_exp)->push_back(new ast::NilExp((yylsp[-1])));(yyval.t_list_exp)->push_back((yyvsp[0].t_assign_exp));print_rules("functionArgs", "COMMA variableDeclaration");}
    break;

  case 64: /* functionArgs: functionArgs ","  */
                                            {(yyvsp[-1].t_list_exp)->push_back(new ast::NilExp((yylsp[0])));(yyval.t_list_exp) = (yyvsp[-1].t_list_exp);print_rules("functionArgs", "functionArgs COMMA");}
    break;

  case 65: /* functionArgs: functionArgs "," variable  */
                                            {(yyvsp[-2].t_list_exp)->push_back((yyvsp[0].t_exp));(yyval.t_list_exp) = (yyvsp[-2].t_list_exp);print_rules("functionArgs", "functionArgs COMMA variable");}
    break;

  case 66: /* functionArgs: functionArgs "," functionCall  */
                                            {(yyvsp[-2].t_list_exp)->push_back((yyvsp[0].t_call_exp));(yyval.t_list_exp) = (yyvsp[-2].t_list_exp);print_rules("functionArgs", "functionArgs COMMA functionCall");}
    break;

  case 67: /* functionArgs: functionArgs "," ":"  */
                                            {(yyvsp[-2].t_list_exp)->push_back(new ast::ColonVar((yylsp[-2])));(yyval.t_list_exp) = (yyvsp[-2].t_list_exp);print_rules("functionArgs", "functionArgs COMMA COLON");}
    break;

  case 68: /* functionArgs: functionArgs "," variableDeclaration  */
                                            {(yyvsp[-2].t_list_exp)->push_back((yyvsp[0].t_assign_exp));(yyval.t_list_exp) = (yyvsp[-2].t_list_exp);print_rules("functionArgs", "functionArgs COMMA variableDeclaration");}
    break;

  case 69: /* functionDeclaration: "function" "identifier" "=" "identifier" functionDeclarationArguments functionDeclarationBreak functionBody endfunction  */
                                                                                                     {
                  print_rules("functionDeclaration", "FUNCTION ID ASSIGN ID functionDeclarationArguments functionDeclarationBreak functionBody endfunction");
                  ast::exps_t* tmp = new ast::exps_t;
                  tmp->push_back(new ast::SimpleVar((yylsp[-6]), symbol::Symbol(*(yyvsp[-6].str))));
                  (yyval.t_function_dec) = new ast::FunctionDec((yyloc),
                                symbol::Symbol(*(yyvsp[-4].str)),
                                *new ast::ArrayListVar((yylsp[-3]), *(yyvsp[-3].t_list_var)),
                                *new ast::ArrayListVar((yylsp[-6]), *tmp),
                                *(yyvsp[-1].t_seq_exp));
                  delete (yyvsp[-6].str);
                  delete (yyvsp[-4].str);
                }
    break;

  case 70: /* functionDeclaration: "function" "[" functionDeclarationReturns "]" "=" "identifier" functionDeclarationArguments functionDeclarationBreak functionBody endfunction  */
                                                                                                                                             {
                  print_rules("functionDeclaration", "FUNCTION LBRACK functionDeclarationReturns RBRACK ASSIGN ID functionDeclarationArguments functionDeclarationBreak functionBody endfunction");
                  (yyval.t_function_dec) = new ast::FunctionDec((yyloc),
                                symbol::Symbol(*(yyvsp[-4].str)),
                                *new ast::ArrayListVar((yylsp[-3]), *(yyvsp[-3].t_list_var)),
                                *new ast::ArrayListVar((yylsp[-7]) ,*(yyvsp[-7].t_list_var)),
                                *(yyvsp[-1].t_seq_exp));
                  delete (yyvsp[-4].str);
                }
    break;

  case 71: /* functionDeclaration: "function" "[" "]" "=" "identifier" functionDeclarationArguments functionDeclarationBreak functionBody endfunction  */
                                                                                                                  {
                  print_rules("functionDeclaration", "FUNCTION LBRACK RBRACK ASSIGN ID functionDeclarationArguments functionDeclarationBreak functionBody endfunction");
                  ast::exps_t* tmp = new ast::exps_t;
                  (yyval.t_function_dec) = new ast::FunctionDec((yyloc),
                                symbol::Symbol(*(yyvsp[-4].str)),
                                *new ast::ArrayListVar((yylsp[-3]), *(yyvsp[-3].t_list_var)),
                                *new ast::ArrayListVar((yylsp[-7]), *tmp),
                                *(yyvsp[-1].t_seq_exp));
                  delete (yyvsp[-4].str);
                }
    break;

  case 72: /* functionDeclaration: "function" "identifier" functionDeclarationArguments functionDeclarationBreak functionBody endfunction  */
                                                                                             {
                  print_rules("functionDeclaration", "FUNCTION ID functionDeclarationArguments functionDeclarationBreak functionBody endfunction");
                  ast::exps_t* tmp = new ast::exps_t;
                  (yyval.t_function_dec) = new ast::FunctionDec((yyloc),
                                symbol::Symbol(*(yyvsp[-4].str)),
                                *new ast::ArrayListVar((yylsp[-3]), *(yyvsp[-3].t_list_var)),
                                *new ast::ArrayListVar((yyloc), *tmp),
                                *(yyvsp[-1].t_seq_exp));
                  delete (yyvsp[-4].str);
                }
    break;

  case 73: /* lambdaFunctionDeclaration: "#" functionDeclarationArguments "->" "(" functionBody ")"  */
                                                                    {
                        print_rules("lambdaFunctionDeclaration", "SHARP functionDeclarationArguments ARROW LPAREN functionBody RPAREN");
                        (yyvsp[-1].t_seq_exp)->setVerbose(true);
                        (yyval.t_function_dec) = new ast::FunctionDec((yyloc), *new ast::ArrayListVar((yylsp[-4]), *(yyvsp[-4].t_list_var)), *(yyvsp[-1].t_seq_exp));
                        }
    break;

  case 74: /* lambdaFunctionDeclaration: "#" functionDeclarationArguments "->" "end of line" "(" functionBody ")"  */
                                                                          {
                        print_rules("lambdaFunctionDeclaration", "SHARP functionDeclarationArguments ARROW LPAREN functionBody RPAREN");
                        (yyvsp[-1].t_seq_exp)->setVerbose(true);
                        (yyval.t_function_dec) = new ast::FunctionDec((yyloc), *new ast::ArrayListVar((yylsp[-5]), *(yyvsp[-5].t_list_var)), *(yyvsp[-1].t_seq_exp));
                        }
    break;

  case 75: /* lambdaFunctionDeclaration: "#" functionDeclarationArguments "->" "(" "end of line" functionBody ")"  */
                                                                          {
                        print_rules("lambdaFunctionDeclaration", "SHARP functionDeclarationArguments ARROW LPAREN EOL functionBody RPAREN");
                        (yyvsp[-1].t_seq_exp)->setVerbose(true);
                        (yyval.t_function_dec) = new ast::FunctionDec((yyloc), *new ast::ArrayListVar((yylsp[-5]), *(yyvsp[-5].t_list_var)), *(yyvsp[-1].t_seq_exp));
                        }
    break;

  case 76: /* lambdaFunctionDeclaration: "#" functionDeclarationArguments "->" "end of line" "(" "end of line" functionBody ")"  */
                                                                              {
                        print_rules("lambdaFunctionDeclaration", "SHARP functionDeclarationArguments ARROW EOL LPAREN EOL functionBody RPAREN");
                        (yyvsp[-1].t_seq_exp)->setVerbose(true);
                        (yyval.t_function_dec) = new ast::FunctionDec((yyloc), *new ast::ArrayListVar((yylsp[-6]), *(yyvsp[-6].t_list_var)), *(yyvsp[-1].t_seq_exp));
                        }
    break;

  case 79: /* functionDeclarationReturns: idList  */
        { (yyval.t_list_var) = (yyvsp[0].t_list_var); print_rules("functionDeclarationReturns", "idList");}
    break;

  case 80: /* functionDeclarationArguments: "(" idList ")"  */
                            { (yyval.t_list_var) = (yyvsp[-1].t_list_var); print_rules("functionDeclarationArguments", "LPAREN idList RPAREN");}
    break;

  case 81: /* functionDeclarationArguments: "(" ")"  */
                            { (yyval.t_list_var) = new ast::exps_t;    print_rules("functionDeclarationArguments", "LPAREN RPAREN");}
    break;

  case 82: /* functionDeclarationArguments: %empty  */
                            { (yyval.t_list_var) = new ast::exps_t;    print_rules("functionDeclarationArguments", "Epsilon");}
    break;

  case 83: /* idList: idList "," "identifier"  */
                {
                    print_rules("idList", "idList COMMA ID");
                    (yyvsp[-2].t_list_var)->push_back(new ast::SimpleVar((yylsp[0]), symbol::Symbol(*(yyvsp[0].str))));
                    delete (yyvsp[0].str);
                    (yyval.t_list_var) = (yyvsp[-2].t_list_var);
                }
    break;

  case 84: /* idList: "identifier"  */
                {
                    print_rules("idList", "ID");
                    (yyval.t_list_var) = new ast::exps_t;
                    (yyval.t_list_var)->push_back(new ast::SimpleVar((yyloc), symbol::Symbol(*(yyvsp[0].str))));
                    delete (yyvsp[0].str);
                }
    break;

  case 85: /* functionDeclarationBreak: lineEnd  */
                { /* !! Do Nothing !! */ print_rules("functionDeclarationBreak", "lineEnd");}
    break;

  case 86: /* functionDeclarationBreak: ";"  */
                { /* !! Do Nothing !! */ print_rules("functionDeclarationBreak", "SEMI");}
    break;

  case 87: /* functionDeclarationBreak: ";" "end of line"  */
                { /* !! Do Nothing !! */ print_rules("functionDeclarationBreak", "SEMI EOL");}
    break;

  case 88: /* functionDeclarationBreak: ","  */
                { /* !! Do Nothing !! */ print_rules("functionDeclarationBreak", "COMMA");}
    break;

  case 89: /* functionDeclarationBreak: "," "end of line"  */
                { /* !! Do Nothing !! */ print_rules("functionDeclarationBreak", "COMMA EOL");}
    break;

  case 90: /* functionBody: expressions  */
                    {
                        print_rules("functionBody", "expressions");
                        (yyvsp[0].t_seq_exp)->getLocation().last_line = (yyvsp[0].t_seq_exp)->getExps().back()->getLocation().last_line;
                        (yyvsp[0].t_seq_exp)->getLocation().last_column = (yyvsp[0].t_seq_exp)->getExps().back()->getLocation().last_column;
                        (yyval.t_seq_exp) = (yyvsp[0].t_seq_exp);
                    }
    break;

  case 91: /* functionBody: %empty  */
                    {
                        print_rules("functionBody", "Epsilon");
                        ast::exps_t* tmp = new ast::exps_t;
                        #ifdef BUILD_DEBUG_AST
                            tmp->push_back(new ast::CommentExp((yyloc), new std::wstring(L"Empty function body")));
                        #endif
                        (yyval.t_seq_exp) = new ast::SeqExp((yyloc), *tmp);
                    }
    break;

  case 92: /* condition: functionCall  */
                                    { (yyval.t_exp) = (yyvsp[0].t_call_exp); print_rules("condition", "functionCall");}
    break;

  case 93: /* condition: variable  */
                                    { (yyval.t_exp) = (yyvsp[0].t_exp); print_rules("condition", "variable");}
    break;

  case 94: /* comparison: variable rightComparable  */
                                {
                      print_rules("comparison", "variable rightComparable");
                      delete &((yyvsp[0].t_op_exp)->getLeft());
                      (yyvsp[0].t_op_exp)->setLeft(*(yyvsp[-1].t_exp));
                      (yyvsp[0].t_op_exp)->setLocation((yyloc));
                      (yyval.t_op_exp) = (yyvsp[0].t_op_exp);
                    }
    break;

  case 95: /* comparison: functionCall rightComparable  */
                                      {
                      print_rules("comparison", "functionCall rightComparable");
                      delete &((yyvsp[0].t_op_exp)->getLeft());
                      (yyvsp[0].t_op_exp)->setLeft(*(yyvsp[-1].t_call_exp));
                      (yyvsp[0].t_op_exp)->setLocation((yyloc));
                      (yyval.t_op_exp) = (yyvsp[0].t_op_exp);
                    }
    break;

  case 96: /* rightComparable: "&" variable  */
                        { (yyval.t_op_exp) = new ast::LogicalOpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::LogicalOpExp::logicalAnd, *(yyvsp[0].t_exp)); print_rules("rightComparable", "AND variable");}
    break;

  case 97: /* rightComparable: "&" functionCall  */
                        { (yyval.t_op_exp) = new ast::LogicalOpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::LogicalOpExp::logicalAnd, *(yyvsp[0].t_call_exp)); print_rules("rightComparable", "AND functionCall");}
    break;

  case 98: /* rightComparable: "&" ":"  */
                        { (yyval.t_op_exp) = new ast::LogicalOpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::LogicalOpExp::logicalAnd, * new ast::ColonVar((yyloc))); print_rules("rightComparable", "AND COLON");}
    break;

  case 99: /* rightComparable: "&&" variable  */
                        { (yyval.t_op_exp) = new ast::LogicalOpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::LogicalOpExp::logicalShortCutAnd, *(yyvsp[0].t_exp)); print_rules("rightComparable", "ANDAND variable");}
    break;

  case 100: /* rightComparable: "&&" functionCall  */
                        { (yyval.t_op_exp) = new ast::LogicalOpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::LogicalOpExp::logicalShortCutAnd, *(yyvsp[0].t_call_exp)); print_rules("rightComparable", "ANDAND functionCall");}
    break;

  case 101: /* rightComparable: "&&" ":"  */
                        { (yyval.t_op_exp) = new ast::LogicalOpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::LogicalOpExp::logicalShortCutAnd, * new ast::ColonVar((yyloc))); print_rules("rightComparable", "ANDAND COLON");}
    break;

  case 102: /* rightComparable: "|" variable  */
                        { (yyval.t_op_exp) = new ast::LogicalOpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::LogicalOpExp::logicalOr, *(yyvsp[0].t_exp)); print_rules("rightComparable", "OR variable");}
    break;

  case 103: /* rightComparable: "|" functionCall  */
                        { (yyval.t_op_exp) = new ast::LogicalOpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::LogicalOpExp::logicalOr, *(yyvsp[0].t_call_exp)); print_rules("rightComparable", "OR functionCall");}
    break;

  case 104: /* rightComparable: "|" ":"  */
                        { (yyval.t_op_exp) = new ast::LogicalOpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::LogicalOpExp::logicalOr, * new ast::ColonVar((yyloc))); print_rules("rightComparable", "OR COLON");}
    break;

  case 105: /* rightComparable: "||" variable  */
                        { (yyval.t_op_exp) = new ast::LogicalOpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::LogicalOpExp::logicalShortCutOr, *(yyvsp[0].t_exp)); print_rules("rightComparable", "OROR variable");}
    break;

  case 106: /* rightComparable: "||" functionCall  */
                        { (yyval.t_op_exp) = new ast::LogicalOpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::LogicalOpExp::logicalShortCutOr, *(yyvsp[0].t_call_exp)); print_rules("rightComparable", "OROR functionCall");}
    break;

  case 107: /* rightComparable: "||" ":"  */
                        { (yyval.t_op_exp) = new ast::LogicalOpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::LogicalOpExp::logicalShortCutOr, * new ast::ColonVar((yyloc))); print_rules("rightComparable", "OROR COLON");}
    break;

  case 108: /* rightComparable: "==" variable  */
                        { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::eq, *(yyvsp[0].t_exp)); print_rules("rightComparable", "EQ variable");}
    break;

  case 109: /* rightComparable: "==" functionCall  */
                        { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::eq, *(yyvsp[0].t_call_exp)); print_rules("rightComparable", "EQ functionCall");}
    break;

  case 110: /* rightComparable: "==" ":"  */
                        { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::eq, * new ast::ColonVar((yyloc))); print_rules("rightComparable", "EQ COLON");}
    break;

  case 111: /* rightComparable: "<> or ~=" variable  */
                        { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::ne, *(yyvsp[0].t_exp)); print_rules("rightComparable", "NE variable");}
    break;

  case 112: /* rightComparable: "<> or ~=" functionCall  */
                        { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::ne, *(yyvsp[0].t_call_exp)); print_rules("rightComparable", "NE functionCall");}
    break;

  case 113: /* rightComparable: "<> or ~=" ":"  */
                        { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::ne, * new ast::ColonVar((yyloc))); print_rules("rightComparable", "NE COLON");}
    break;

  case 114: /* rightComparable: ">" variable  */
                        { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::gt, *(yyvsp[0].t_exp)); print_rules("rightComparable", "GT variable");}
    break;

  case 115: /* rightComparable: ">" functionCall  */
                        { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::gt, *(yyvsp[0].t_call_exp)); print_rules("rightComparable", "GT functionCall");}
    break;

  case 116: /* rightComparable: ">" ":"  */
                        { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::gt, * new ast::ColonVar((yyloc))); print_rules("rightComparable", "GT COLON");}
    break;

  case 117: /* rightComparable: "<" variable  */
                        { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::lt, *(yyvsp[0].t_exp)); print_rules("rightComparable", "LT variable");}
    break;

  case 118: /* rightComparable: "<" functionCall  */
                        { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::lt, *(yyvsp[0].t_call_exp)); print_rules("rightComparable", "LT functionCall");}
    break;

  case 119: /* rightComparable: "<" ":"  */
                        { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::lt, * new ast::ColonVar((yyloc))); print_rules("rightComparable", "LT COLON");}
    break;

  case 120: /* rightComparable: ">=" variable  */
                        { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::ge, *(yyvsp[0].t_exp)); print_rules("rightComparable", "GE variable");}
    break;

  case 121: /* rightComparable: ">=" functionCall  */
                        { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::ge, *(yyvsp[0].t_call_exp)); print_rules("rightComparable", "GE functionCall");}
    break;

  case 122: /* rightComparable: ">=" ":"  */
                        { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::ge, * new ast::ColonVar((yyloc))); print_rules("rightComparable", "GE COLON");}
    break;

  case 123: /* rightComparable: "<=" variable  */
                        { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::le, *(yyvsp[0].t_exp)); print_rules("rightComparable", "LE variable");}
    break;

  case 124: /* rightComparable: "<=" functionCall  */
                        { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::le, *(yyvsp[0].t_call_exp)); print_rules("rightComparable", "LE functionCall");}
    break;

  case 125: /* rightComparable: "<=" ":"  */
                        { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::le, * new ast::ColonVar((yyloc))); print_rules("rightComparable", "LE COLON");}
    break;

  case 126: /* operation: variable rightOperand  */
                                 {
                      print_rules("operation", "rightOperand");
                      delete &((yyvsp[0].t_op_exp)->getLeft());
                      (yyvsp[0].t_op_exp)->setLeft(*(yyvsp[-1].t_exp));
                      (yyvsp[0].t_op_exp)->setLocation((yyloc));
                      (yyval.t_exp) = (yyvsp[0].t_op_exp);
                    }
    break;

  case 127: /* operation: functionCall rightOperand  */
                                   {
                      print_rules("operation", "functionCall rightOperand");
                      delete &((yyvsp[0].t_op_exp)->getLeft());
                      (yyvsp[0].t_op_exp)->setLeft(*(yyvsp[-1].t_call_exp));
                      (yyvsp[0].t_op_exp)->setLocation((yyloc));
                      (yyval.t_exp) = (yyvsp[0].t_op_exp);
                    }
    break;

  case 128: /* operation: "-" variable  */
                                        { if ((yyvsp[0].t_exp)->isDoubleExp()) { (yyval.t_exp) = (yyvsp[0].t_exp)->getAs<ast::DoubleExp>()->neg();  (yyvsp[0].t_exp)->setLocation((yyloc));} else { (yyval.t_exp) = new ast::OpExp((yyloc), *new ast::DoubleExp((yyloc), 0.0), ast::OpExp::unaryMinus, *(yyvsp[0].t_exp)); } print_rules("operation", "MINUS variable");}
    break;

  case 129: /* operation: "-" functionCall  */
                                        { (yyval.t_exp) = new ast::OpExp((yyloc), *new ast::DoubleExp((yyloc), 0.0), ast::OpExp::unaryMinus, *(yyvsp[0].t_call_exp)); print_rules("operation", "MINUS functionCall");}
    break;

  case 130: /* operation: "+" variable  */
                                        { if ((yyvsp[0].t_exp)->isDoubleExp()) { (yyval.t_exp) = (yyvsp[0].t_exp);} else { (yyval.t_exp) = new ast::OpExp((yyloc), *new ast::DoubleExp((yyloc), 0.0), ast::OpExp::unaryPlus, *(yyvsp[0].t_exp)); } print_rules("operation", "PLUS variable");}
    break;

  case 131: /* operation: "+" functionCall  */
                                        { (yyval.t_exp) = new ast::OpExp((yyloc), *new ast::DoubleExp((yyloc), 0.0), ast::OpExp::unaryPlus, *(yyvsp[0].t_call_exp)); print_rules("operation", "PLUS functionCall");}
    break;

  case 132: /* operation: variable "** or ^" variable  */
                                        { (yyval.t_exp) = new ast::OpExp((yyloc), *(yyvsp[-2].t_exp), ast::OpExp::power, *(yyvsp[0].t_exp)); print_rules("operation", "variable POWER variable");}
    break;

  case 133: /* operation: variable "** or ^" functionCall  */
                                        { (yyval.t_exp) = new ast::OpExp((yyloc), *(yyvsp[-2].t_exp), ast::OpExp::power, *(yyvsp[0].t_call_exp)); print_rules("operation", "variable POWER functionCall");}
    break;

  case 134: /* operation: functionCall "** or ^" variable  */
                                        { (yyval.t_exp) = new ast::OpExp((yyloc), *(yyvsp[-2].t_call_exp), ast::OpExp::power, *(yyvsp[0].t_exp)); print_rules("operation", "functionCall POWER variable");}
    break;

  case 135: /* operation: functionCall "** or ^" functionCall  */
                                        { (yyval.t_exp) = new ast::OpExp((yyloc), *(yyvsp[-2].t_call_exp), ast::OpExp::power, *(yyvsp[0].t_call_exp)); print_rules("operation", "functionCall POWER functionCall");}
    break;

  case 136: /* operation: variable ".^" variable  */
                                        { (yyval.t_exp) = new ast::OpExp((yyloc), *(yyvsp[-2].t_exp), ast::OpExp::dotpower, *(yyvsp[0].t_exp)); print_rules("operation", "variable DOTPOWER variable");}
    break;

  case 137: /* operation: variable ".^" functionCall  */
                                        { (yyval.t_exp) = new ast::OpExp((yyloc), *(yyvsp[-2].t_exp), ast::OpExp::dotpower, *(yyvsp[0].t_call_exp)); print_rules("operation", "variable DOTPOWER functionCall");}
    break;

  case 138: /* operation: functionCall ".^" variable  */
                                        { (yyval.t_exp) = new ast::OpExp((yyloc), *(yyvsp[-2].t_call_exp), ast::OpExp::dotpower, *(yyvsp[0].t_exp)); print_rules("operation", "functionCall DOTPOWER variable");}
    break;

  case 139: /* operation: functionCall ".^" functionCall  */
                                        { (yyval.t_exp) = new ast::OpExp((yyloc), *(yyvsp[-2].t_call_exp), ast::OpExp::dotpower, *(yyvsp[0].t_call_exp)); print_rules("operation", "functionCall DOTPOWER functionCall");}
    break;

  case 140: /* operation: variable "'"  */
                                        { (yyval.t_exp) = new ast::TransposeExp((yyloc), *(yyvsp[-1].t_exp), ast::TransposeExp::_Conjugate_); print_rules("operation", "variable QUOTE");}
    break;

  case 141: /* operation: variable ".'"  */
                                        { (yyval.t_exp) = new ast::TransposeExp((yyloc), *(yyvsp[-1].t_exp), ast::TransposeExp::_NonConjugate_); print_rules("operation", "variable DOTQUOTE");}
    break;

  case 142: /* operation: functionCall "'"  */
                                        { (yyval.t_exp) = new ast::TransposeExp((yyloc), *(yyvsp[-1].t_call_exp), ast::TransposeExp::_Conjugate_); print_rules("operation", "functionCall QUOTE");}
    break;

  case 143: /* operation: functionCall ".'"  */
                                        { (yyval.t_exp) = new ast::TransposeExp((yyloc), *(yyvsp[-1].t_call_exp), ast::TransposeExp::_NonConjugate_); print_rules("operation", "functionCall DOTQUOTE");}
    break;

  case 144: /* rightOperand: "+" variable  */
                                { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::plus, *(yyvsp[0].t_exp)); print_rules("rightOperand", "PLUS variable");}
    break;

  case 145: /* rightOperand: "+" functionCall  */
                                { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::plus, *(yyvsp[0].t_call_exp)); print_rules("rightOperand", "PLUS functionCall");}
    break;

  case 146: /* rightOperand: "-" variable  */
                                { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::minus, *(yyvsp[0].t_exp)); print_rules("rightOperand", "MINUS variable");}
    break;

  case 147: /* rightOperand: "-" functionCall  */
                                { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::minus, *(yyvsp[0].t_call_exp)); print_rules("rightOperand", "MINUS functionCall");}
    break;

  case 148: /* rightOperand: "*" variable  */
                                { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::times, *(yyvsp[0].t_exp)); print_rules("rightOperand", "TIMES variable");}
    break;

  case 149: /* rightOperand: "*" functionCall  */
                                { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::times, *(yyvsp[0].t_call_exp)); print_rules("rightOperand", "TIMES functionCall");}
    break;

  case 150: /* rightOperand: ".*" variable  */
                                { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::dottimes, *(yyvsp[0].t_exp)); print_rules("rightOperand", "DOTTIMES variable");}
    break;

  case 151: /* rightOperand: ".*" functionCall  */
                                { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::dottimes, *(yyvsp[0].t_call_exp)); print_rules("rightOperand", "DOTTIMES functionCall");}
    break;

  case 152: /* rightOperand: ".*." variable  */
                                { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::krontimes, *(yyvsp[0].t_exp)); print_rules("rightOperand", "KRONTIMES variable");}
    break;

  case 153: /* rightOperand: ".*." functionCall  */
                                { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::krontimes, *(yyvsp[0].t_call_exp)); print_rules("rightOperand", "KRONTIMES functionCall");}
    break;

  case 154: /* rightOperand: "*." variable  */
                                { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::controltimes, *(yyvsp[0].t_exp)); print_rules("rightOperand", "CONTROLTIMES variable");}
    break;

  case 155: /* rightOperand: "*." functionCall  */
                                { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::controltimes, *(yyvsp[0].t_call_exp)); print_rules("rightOperand", "CONTROLTIMES functionCall    ");}
    break;

  case 156: /* rightOperand: "/" variable  */
                                { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::rdivide, *(yyvsp[0].t_exp)); print_rules("rightOperand", "RDIVIDE variable");}
    break;

  case 157: /* rightOperand: "/" functionCall  */
                                { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::rdivide, *(yyvsp[0].t_call_exp)); print_rules("rightOperand", "RDIVIDE functionCall");}
    break;

  case 158: /* rightOperand: "./" variable  */
                                { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::dotrdivide, *(yyvsp[0].t_exp)); print_rules("rightOperand", "DOTRDIVIDE variable");}
    break;

  case 159: /* rightOperand: "./" functionCall  */
                                { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::dotrdivide, *(yyvsp[0].t_call_exp)); print_rules("rightOperand", "DOTRDIVIDE functionCall");}
    break;

  case 160: /* rightOperand: "./." variable  */
                                { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::kronrdivide, *(yyvsp[0].t_exp)); print_rules("rightOperand", "KRONRDIVIDE variable");}
    break;

  case 161: /* rightOperand: "./." functionCall  */
                                { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::kronrdivide, *(yyvsp[0].t_call_exp)); print_rules("rightOperand", "KRONRDIVIDE functionCall");}
    break;

  case 162: /* rightOperand: "/." variable  */
                                { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::controlrdivide, *(yyvsp[0].t_exp)); print_rules("rightOperand", "CONTROLRDIVIDE variable");}
    break;

  case 163: /* rightOperand: "/." functionCall  */
                                { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::controlrdivide, *(yyvsp[0].t_call_exp)); print_rules("rightOperand", "CONTROLRDIVIDE functionCall");}
    break;

  case 164: /* rightOperand: "\\" variable  */
                                { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::ldivide, *(yyvsp[0].t_exp)); print_rules("rightOperand", "LDIVIDE variable");}
    break;

  case 165: /* rightOperand: "\\" functionCall  */
                                { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::ldivide, *(yyvsp[0].t_call_exp)); print_rules("rightOperand", "LDIVIDE functionCall");}
    break;

  case 166: /* rightOperand: ".\\" variable  */
                                { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::dotldivide, *(yyvsp[0].t_exp)); print_rules("rightOperand", "DOTLDIVIDE variable");}
    break;

  case 167: /* rightOperand: ".\\" functionCall  */
                                { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::dotldivide, *(yyvsp[0].t_call_exp)); print_rules("rightOperand", "DOTLDIVIDE functionCall");}
    break;

  case 168: /* rightOperand: ".\\." variable  */
                                { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::kronldivide, *(yyvsp[0].t_exp)); print_rules("rightOperand", "KRONLDIVIDE variable");}
    break;

  case 169: /* rightOperand: ".\\." functionCall  */
                                { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::kronldivide, *(yyvsp[0].t_call_exp)); print_rules("rightOperand", "KRONLDIVIDE functionCall");}
    break;

  case 170: /* rightOperand: "\\." variable  */
                                { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::controlldivide, *(yyvsp[0].t_exp)); print_rules("rightOperand", "CONTROLLDIVIDE variable");}
    break;

  case 171: /* rightOperand: "\\." functionCall  */
                                { (yyval.t_op_exp) = new ast::OpExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), ast::OpExp::controlldivide, *(yyvsp[0].t_call_exp)); print_rules("rightOperand", "CONTROLLDIVIDE functionCall");}
    break;

  case 172: /* listableBegin: ":" variable  */
                        { (yyval.t_exp) = (yyvsp[0].t_exp); print_rules("listableBegin", "COLON variable");}
    break;

  case 173: /* listableBegin: ":" functionCall  */
                        { (yyval.t_exp) = (yyvsp[0].t_call_exp); print_rules("listableBegin", "COLON functionCall");}
    break;

  case 174: /* listableEnd: listableBegin ":" variable  */
                                    { (yyval.t_implicit_list) = new ast::ListExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), *(yyvsp[-2].t_exp), *(yyvsp[0].t_exp), true); print_rules("listableEnd", "listableBegin COLON variable");}
    break;

  case 175: /* listableEnd: listableBegin ":" functionCall  */
                                    { (yyval.t_implicit_list) = new ast::ListExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), *(yyvsp[-2].t_exp), *(yyvsp[0].t_call_exp), true); print_rules("listableEnd", "listableBegin COLON functionCall");}
    break;

  case 176: /* listableEnd: listableBegin  */
                                    { (yyval.t_implicit_list) = new ast::ListExp((yyloc), *new ast::CommentExp((yyloc), new std::wstring(L"Should not stay in that state")), *new ast::DoubleExp((yyloc), 1.0), *(yyvsp[0].t_exp)); print_rules("listableEnd", "listableBegin ");}
    break;

  case 177: /* variable: "~ or @" variable  */
                                            { (yyval.t_exp) = new ast::NotExp((yyloc), *(yyvsp[0].t_exp)); print_rules("variable", "NOT variable");}
    break;

  case 178: /* variable: "~ or @" functionCall  */
                                            { (yyval.t_exp) = new ast::NotExp((yyloc), *(yyvsp[0].t_call_exp)); print_rules("variable", "NOT functionCall");}
    break;

  case 179: /* variable: variable "." "identifier"  */
                                            { (yyval.t_exp) = new ast::FieldExp((yyloc), *(yyvsp[-2].t_exp), *new ast::SimpleVar((yyloc), symbol::Symbol(*(yyvsp[0].str)))); delete (yyvsp[0].str);print_rules("variable", "variable DOT ID");}
    break;

  case 180: /* variable: variable "." keywords  */
                                            { (yyval.t_exp) = new ast::FieldExp((yyloc), *(yyvsp[-2].t_exp), *(yyvsp[0].t_simple_var)); print_rules("variable", "variable DOT keywords");}
    break;

  case 181: /* variable: variable "." functionCall  */
                                            {
                              print_rules("variable", "variable DOT functionCall");
                              (yyvsp[0].t_call_exp)->setName(new ast::FieldExp((yyloc), *(yyvsp[-2].t_exp), (yyvsp[0].t_call_exp)->getName()));
                              (yyvsp[0].t_call_exp)->setLocation((yyloc));
                              (yyval.t_exp) = (yyvsp[0].t_call_exp);
}
    break;

  case 182: /* variable: functionCall "." "identifier"  */
                                            { (yyval.t_exp) = new ast::FieldExp((yyloc), *(yyvsp[-2].t_call_exp), *new ast::SimpleVar((yyloc), symbol::Symbol(*(yyvsp[0].str)))); delete (yyvsp[0].str); print_rules("variable", "functionCall DOT ID");}
    break;

  case 183: /* variable: functionCall "." keywords  */
                                            { (yyval.t_exp) = new ast::FieldExp((yyloc), *(yyvsp[-2].t_call_exp), *(yyvsp[0].t_simple_var)); print_rules("variable", "functionCall DOT keywords");}
    break;

  case 184: /* variable: variable listableEnd  */
                                            {
    print_rules("variable", "variable listableEnd");
    (yyval.t_exp) = new ast::ListExp((yyloc), *(yyvsp[-1].t_exp), *((yyvsp[0].t_implicit_list)->getStep().clone()), *((yyvsp[0].t_implicit_list)->getEnd().clone()), (yyvsp[0].t_implicit_list)->hasExplicitStep());
    delete((yyvsp[0].t_implicit_list));
}
    break;

  case 185: /* variable: functionCall listableEnd  */
                                                   {
    print_rules("variable", "functionCall listableEnd");
    (yyval.t_exp) = new ast::ListExp((yyloc), *(yyvsp[-1].t_call_exp), *((yyvsp[0].t_implicit_list)->getStep().clone()), *((yyvsp[0].t_implicit_list)->getEnd().clone()), (yyvsp[0].t_implicit_list)->hasExplicitStep());
    delete((yyvsp[0].t_implicit_list));
}
    break;

  case 186: /* variable: lambdaFunctionDeclaration  */
                                            { (yyval.t_exp) = (yyvsp[0].t_function_dec); print_rules("variable", "lambdaFunctionDeclaration"); }
    break;

  case 187: /* variable: matrix  */
                                            { (yyval.t_exp) = (yyvsp[0].t_matrix_exp); print_rules("variable", "matrix");}
    break;

  case 188: /* variable: cell  */
                                            { (yyval.t_exp) = (yyvsp[0].t_cell_exp); print_rules("variable", "cell");}
    break;

  case 189: /* variable: operation  */
                                            { (yyval.t_exp) = (yyvsp[0].t_exp); print_rules("variable", "operation");}
    break;

  case 190: /* variable: "identifier"  */
                                            { (yyval.t_exp) = new ast::SimpleVar((yyloc), symbol::Symbol(*(yyvsp[0].str))); delete (yyvsp[0].str);print_rules("variable", "ID");}
    break;

  case 191: /* variable: "integer"  */
                                            { (yyval.t_exp) = new ast::DoubleExp((yyloc), (yyvsp[0].number)); print_rules("variable", (yyvsp[0].number));}
    break;

  case 192: /* variable: "number"  */
                                            { (yyval.t_exp) = new ast::DoubleExp((yyloc), (yyvsp[0].number)); print_rules("variable", (yyvsp[0].number));}
    break;

  case 193: /* variable: "float"  */
                                            { (yyval.t_exp) = new ast::DoubleExp((yyloc), (yyvsp[0].number)); print_rules("variable", (yyvsp[0].number));}
    break;

  case 194: /* variable: "complex number"  */
                                            { (yyval.t_exp) = (new ast::DoubleExp((yyloc), (yyvsp[0].number)))->imag(); print_rules("variable", (yyvsp[0].number));}
    break;

  case 195: /* variable: "string"  */
                                            { (yyval.t_exp) = new ast::StringExp((yyloc), *(yyvsp[0].str)); delete (yyvsp[0].str);print_rules("variable", "STR");}
    break;

  case 196: /* variable: "$"  */
                                            { (yyval.t_exp) = new ast::DollarVar((yyloc)); print_rules("variable", "DOLLAR");}
    break;

  case 197: /* variable: "%t or %T"  */
                                            { (yyval.t_exp) = new ast::BoolExp((yyloc), true); print_rules("variable", "BOOLTRUE");}
    break;

  case 198: /* variable: "%f or %F"  */
                                            { (yyval.t_exp) = new ast::BoolExp((yyloc), false); print_rules("variable", "BOOLFALSE");}
    break;

  case 199: /* variable: "(" variable ")"  */
                                            { (yyval.t_exp) = (yyvsp[-1].t_exp); print_rules("variable", "LPAREN variable RPAREN");}
    break;

  case 200: /* variable: "(" variableFields ")"  */
                                            { (yyval.t_exp) = new ast::ArrayListExp((yyloc), *(yyvsp[-1].t_list_exp)); print_rules("variable", "LPAREN variableFields RPAREN");}
    break;

  case 201: /* variable: comparison  */
                                            { (yyval.t_exp) = (yyvsp[0].t_op_exp); print_rules("variable", "comparison");}
    break;

  case 202: /* variable: variable "(" functionArgs ")"  */
                                            { (yyval.t_exp) = new ast::CallExp((yyloc), *(yyvsp[-3].t_exp), *(yyvsp[-1].t_list_exp)); print_rules("variable", "variable LPAREN functionArgs RPAREN");}
    break;

  case 203: /* variable: functionCall "(" functionArgs ")"  */
                                            { (yyval.t_exp) = new ast::CallExp((yyloc), *(yyvsp[-3].t_call_exp), *(yyvsp[-1].t_list_exp)); print_rules("variable", "functionCall LPAREN functionArgs RPAREN");}
    break;

  case 204: /* variable: functionCall "(" ")"  */
                                            { (yyval.t_exp) = new ast::CallExp((yyloc), *(yyvsp[-2].t_call_exp), *new ast::exps_t); print_rules("variable", "functionCall LPAREN RPAREN");}
    break;

  case 205: /* variableFields: variableFields "," variable  */
                                     {
                    print_rules("variableFields", "variableFields COMMA variable");
                      (yyvsp[-2].t_list_exp)->push_back((yyvsp[0].t_exp));
                      (yyval.t_list_exp) = (yyvsp[-2].t_list_exp);
                    }
    break;

  case 206: /* variableFields: variableFields "," functionCall  */
                                       {
                    print_rules("variableFields", "variableFields COMMA functionCall");
                      (yyvsp[-2].t_list_exp)->push_back((yyvsp[0].t_call_exp));
                      (yyval.t_list_exp) = (yyvsp[-2].t_list_exp);
                    }
    break;

  case 207: /* variableFields: variable "," variable  */
                                 {
                      print_rules("variableFields", "variable COMMA variable");
                      (yyval.t_list_exp) = new ast::exps_t;
                      (yyval.t_list_exp)->push_back((yyvsp[-2].t_exp));
                      (yyval.t_list_exp)->push_back((yyvsp[0].t_exp));
                    }
    break;

  case 208: /* variableFields: functionCall "," functionCall  */
                                     {
                      print_rules("variableFields", "functionCall COMMA functionCall");
                      (yyval.t_list_exp) = new ast::exps_t;
                      (yyval.t_list_exp)->push_back((yyvsp[-2].t_call_exp));
                      (yyval.t_list_exp)->push_back((yyvsp[0].t_call_exp));
                    }
    break;

  case 209: /* variableFields: functionCall "," variable  */
                                     {
                      print_rules("variableFields", "functionCall COMMA variable");
                      (yyval.t_list_exp) = new ast::exps_t;
                      (yyval.t_list_exp)->push_back((yyvsp[-2].t_call_exp));
                      (yyval.t_list_exp)->push_back((yyvsp[0].t_exp));
                    }
    break;

  case 210: /* variableFields: variable "," functionCall  */
                                     {
                      print_rules("variableFields", "variable COMMA functionCall");
                      (yyval.t_list_exp) = new ast::exps_t;
                      (yyval.t_list_exp)->push_back((yyvsp[-2].t_exp));
                      (yyval.t_list_exp)->push_back((yyvsp[0].t_call_exp));
}
    break;

  case 211: /* cell: "{" matrixOrCellLines "}"  */
                                                            { (yyval.t_cell_exp) = new ast::CellExp((yyloc), *(yyvsp[-1].t_list_mline)); print_rules("cell", "LBRACE matrixOrCellLines RBRACE");}
    break;

  case 212: /* cell: "{" "end of line" matrixOrCellLines "}"  */
                                                            { (yyval.t_cell_exp) = new ast::CellExp((yyloc), *(yyvsp[-1].t_list_mline)); print_rules("cell", "variable COMMA functionCall");}
    break;

  case 213: /* cell: "{" matrixOrCellLines matrixOrCellColumns "}"  */
                                                            {
                                  print_rules("cell", "LBRACE matrixOrCellLines matrixOrCellColumns RBRACE");
                                  (yyvsp[-2].t_list_mline)->push_back(new ast::MatrixLineExp((yylsp[-1]), *(yyvsp[-1].t_list_exp)));
                                  (yyval.t_cell_exp) = new ast::CellExp((yyloc), *(yyvsp[-2].t_list_mline));
                                }
    break;

  case 214: /* cell: "{" "end of line" matrixOrCellLines matrixOrCellColumns "}"  */
                                                            {
                                  print_rules("cell", "LBRACE EOL matrixOrCellLines matrixOrCellColumns RBRACE");
                                  (yyvsp[-2].t_list_mline)->push_back(new ast::MatrixLineExp((yylsp[-1]), *(yyvsp[-1].t_list_exp)));
                                  (yyval.t_cell_exp) = new ast::CellExp((yyloc), *(yyvsp[-2].t_list_mline));
                                }
    break;

  case 215: /* cell: "{" matrixOrCellColumns "}"  */
                                                            {
                                  print_rules("cell", "LBRACE matrixOrCellColumns RBRACE");
                                  ast::exps_t* tmp = new ast::exps_t;
                                  tmp->push_back(new ast::MatrixLineExp((yylsp[-1]), *(yyvsp[-1].t_list_exp)));
                                  (yyval.t_cell_exp) = new ast::CellExp((yyloc), *tmp);
                                }
    break;

  case 216: /* cell: "{" "end of line" matrixOrCellColumns "}"  */
                                                            {
                                  print_rules("cell", "LBRACE EOL matrixOrCellColumns RBRACE");
                                  ast::exps_t* tmp = new ast::exps_t;
                                  tmp->push_back(new ast::MatrixLineExp((yylsp[-1]), *(yyvsp[-1].t_list_exp)));
                                  (yyval.t_cell_exp) = new ast::CellExp((yyloc), *tmp);
                                }
    break;

  case 217: /* cell: "{" "end of line" "}"  */
                                { ast::exps_t* tmp = new ast::exps_t;(yyval.t_cell_exp) = new ast::CellExp((yyloc), *tmp); print_rules("cell", "LBRACE EOL RBRACE");}
    break;

  case 218: /* cell: "{" "}"  */
                                { ast::exps_t* tmp = new ast::exps_t;(yyval.t_cell_exp) = new ast::CellExp((yyloc), *tmp); print_rules("cell", "LBRACE RBRACE");}
    break;

  case 219: /* matrix: "[" matrixOrCellLines "]"  */
                                                                {(yyval.t_matrix_exp) = new ast::MatrixExp((yyloc), *(yyvsp[-1].t_list_mline)); print_rules("matrix", "LBRACK matrixOrCellLines RBRACK");}
    break;

  case 220: /* matrix: "[" "end of line" matrixOrCellLines "]"  */
                                                                {(yyval.t_matrix_exp) = new ast::MatrixExp((yyloc), *(yyvsp[-1].t_list_mline)); print_rules("matrix", "LBRACK EOL matrixOrCellLines RBRACK");}
    break;

  case 221: /* matrix: "[" matrixOrCellLines matrixOrCellColumns "]"  */
                                                                {(yyvsp[-2].t_list_mline)->push_back(new ast::MatrixLineExp((yylsp[-1]), *(yyvsp[-1].t_list_exp)));(yyval.t_matrix_exp) = new ast::MatrixExp((yyloc), *(yyvsp[-2].t_list_mline));print_rules("matrix", "LBRACK matrixOrCellLines matrixOrCellColumns RBRACK");}
    break;

  case 222: /* matrix: "[" "end of line" matrixOrCellLines matrixOrCellColumns "]"  */
                                                                {(yyvsp[-2].t_list_mline)->push_back(new ast::MatrixLineExp((yylsp[-1]), *(yyvsp[-1].t_list_exp)));(yyval.t_matrix_exp) = new ast::MatrixExp((yyloc), *(yyvsp[-2].t_list_mline));print_rules("matrix", "BRACK EOL matrixOrCellLines matrixOrCellColumns RBRACK");}
    break;

  case 223: /* matrix: "[" matrixOrCellColumns "]"  */
                                                                {ast::exps_t* tmp = new ast::exps_t;tmp->push_back(new ast::MatrixLineExp((yylsp[-1]), *(yyvsp[-1].t_list_exp)));(yyval.t_matrix_exp) = new ast::MatrixExp((yyloc), *tmp);print_rules("matrix", "LBRACK matrixOrCellColumns RBRACK");}
    break;

  case 224: /* matrix: "[" "end of line" matrixOrCellColumns "]"  */
                                                                {ast::exps_t* tmp = new ast::exps_t;tmp->push_back(new ast::MatrixLineExp((yylsp[-1]), *(yyvsp[-1].t_list_exp)));(yyval.t_matrix_exp) = new ast::MatrixExp((yyloc), *tmp);print_rules("matrix", "LBRACK EOL matrixOrCellColumns RBRACK");}
    break;

  case 225: /* matrix: "[" "end of line" "]"  */
                                                                {ast::exps_t* tmp = new ast::exps_t;(yyval.t_matrix_exp) = new ast::MatrixExp((yyloc), *tmp); print_rules("matrix", "LBRACK EOL RBRACK");}
    break;

  case 226: /* matrix: "[" "]"  */
                                                                {ast::exps_t* tmp = new ast::exps_t;(yyval.t_matrix_exp) = new ast::MatrixExp((yyloc), *tmp); print_rules("matrix", "LBRACK RBRACK");}
    break;

  case 227: /* matrixOrCellLines: matrixOrCellLines matrixOrCellLine  */
                                    {(yyvsp[-1].t_list_mline)->push_back((yyvsp[0].t_matrixline_exp));(yyval.t_list_mline) = (yyvsp[-1].t_list_mline);print_rules("matrixOrCellLines", "matrixOrCellLines matrixOrCellLine");}
    break;

  case 228: /* matrixOrCellLines: matrixOrCellLine  */
                                    {(yyval.t_list_mline) = new ast::exps_t;(yyval.t_list_mline)->push_back((yyvsp[0].t_matrixline_exp));print_rules("matrixOrCellLines", "matrixOrCellLine");}
    break;

  case 229: /* matrixOrCellLineBreak: ";"  */
                                { /* !! Do Nothing !! */ print_rules("matrixOrCellLineBreak", "SEMI");}
    break;

  case 230: /* matrixOrCellLineBreak: "end of line"  */
                                { /* !! Do Nothing !! */ print_rules("matrixOrCellLineBreak", "EOL");}
    break;

  case 231: /* matrixOrCellLineBreak: matrixOrCellLineBreak "end of line"  */
                                { /* !! Do Nothing !! */ print_rules("matrixOrCellLineBreak", "matrixOrCellLineBreak EOL");}
    break;

  case 232: /* matrixOrCellLineBreak: matrixOrCellLineBreak ";"  */
                                { /* !! Do Nothing !! */ print_rules("matrixOrCellLineBreak", "matrixOrCellLineBreak SEMI");}
    break;

  case 233: /* matrixOrCellLine: matrixOrCellColumns matrixOrCellLineBreak  */
                                                                        { (yyval.t_matrixline_exp) = new ast::MatrixLineExp((yyloc), *(yyvsp[-1].t_list_exp)); print_rules("matrixOrCellLine", "matrixOrCellColumns matrixOrCellLineBreak ");}
    break;

  case 234: /* matrixOrCellLine: matrixOrCellColumns matrixOrCellColumnsBreak matrixOrCellLineBreak  */
                                                                        { (yyval.t_matrixline_exp) = new ast::MatrixLineExp((yyloc), *(yyvsp[-2].t_list_exp)); print_rules("matrixOrCellLine", "matrixOrCellColumns matrixOrCellColumnsBreak matrixOrCellLineBreak");}
    break;

  case 235: /* matrixOrCellColumns: matrixOrCellColumns matrixOrCellColumnsBreak variable  */
                                                                            {(yyvsp[-2].t_list_exp)->push_back((yyvsp[0].t_exp));(yyval.t_list_exp) = (yyvsp[-2].t_list_exp);print_rules("matrixOrCellColumns", "matrixOrCellColumns matrixOrCellColumnsBreak variable");}
    break;

  case 236: /* matrixOrCellColumns: matrixOrCellColumns matrixOrCellColumnsBreak functionCall  */
                                                                            {(yyvsp[-2].t_list_exp)->push_back((yyvsp[0].t_call_exp));(yyval.t_list_exp) = (yyvsp[-2].t_list_exp);print_rules("matrixOrCellColumns", "matrixOrCellColumns matrixOrCellColumnsBreak functionCall");}
    break;

  case 237: /* matrixOrCellColumns: matrixOrCellColumns variable  */
                                                                            {(yyvsp[-1].t_list_exp)->push_back((yyvsp[0].t_exp));(yyval.t_list_exp) = (yyvsp[-1].t_list_exp);print_rules("matrixOrCellColumns", "matrixOrCellColumns variable");}
    break;

  case 238: /* matrixOrCellColumns: matrixOrCellColumns functionCall  */
                                                                            {(yyvsp[-1].t_list_exp)->push_back((yyvsp[0].t_call_exp));(yyval.t_list_exp) = (yyvsp[-1].t_list_exp);print_rules("matrixOrCellColumns", "matrixOrCellColumns functionCall");}
    break;

  case 239: /* matrixOrCellColumns: matrixOrCellColumns "line comment"  */
                                                                            {(yyvsp[-1].t_list_exp)->push_back(new ast::CommentExp((yylsp[0]), (yyvsp[0].comment)));(yyval.t_list_exp) = (yyvsp[-1].t_list_exp);print_rules("matrixOrCellColumns", "matrixOrCellColumns COMMENT");}
    break;

  case 240: /* matrixOrCellColumns: matrixOrCellColumns matrixOrCellColumnsBreak "line comment"  */
                                                                            {(yyvsp[-2].t_list_exp)->push_back(new ast::CommentExp((yylsp[0]), (yyvsp[0].comment)));(yyval.t_list_exp) = (yyvsp[-2].t_list_exp);print_rules("matrixOrCellColumns", "matrixOrCellColumns matrixOrCellColumnsBreak COMMENT");}
    break;

  case 241: /* matrixOrCellColumns: variable  */
                                                                            {(yyval.t_list_exp) = new ast::exps_t;(yyval.t_list_exp)->push_back((yyvsp[0].t_exp));print_rules("matrixOrCellColumns", "variable");}
    break;

  case 242: /* matrixOrCellColumns: functionCall  */
                                                                            {(yyval.t_list_exp) = new ast::exps_t;(yyval.t_list_exp)->push_back((yyvsp[0].t_call_exp));print_rules("matrixOrCellColumns", "functionCall");}
    break;

  case 243: /* matrixOrCellColumns: "line comment"  */
                                                                            {(yyval.t_list_exp) = new ast::exps_t;(yyval.t_list_exp)->push_back(new ast::CommentExp((yyloc), (yyvsp[0].comment)));print_rules("matrixOrCellColumns", "COMMENT");}
    break;

  case 244: /* matrixOrCellColumnsBreak: matrixOrCellColumnsBreak ","  */
                                    { /* !! Do Nothing !! */ print_rules("matrixOrCellColumnsBreak", "matrixOrCellColumnsBreak COMMA");}
    break;

  case 245: /* matrixOrCellColumnsBreak: matrixOrCellColumnsBreak "spaces"  */
                                    { /* !! Do Nothing !! */ print_rules("matrixOrCellColumnsBreak", "matrixOrCellColumnsBreak SPACES");}
    break;

  case 246: /* matrixOrCellColumnsBreak: ","  */
                                    { /* !! Do Nothing !! */ print_rules("matrixOrCellColumnsBreak", "COMMA");}
    break;

  case 247: /* matrixOrCellColumnsBreak: "spaces"  */
                                    { /* !! Do Nothing !! */ print_rules("matrixOrCellColumnsBreak", "SPACES");}
    break;

  case 248: /* variableDeclaration: assignable "=" variable  */
                                                        { (yyval.t_assign_exp) = new ast::AssignExp((yyloc), *(yyvsp[-2].t_exp), *(yyvsp[0].t_exp)); print_rules("variableDeclaration", "assignable ASSIGN variable");}
    break;

  case 249: /* variableDeclaration: assignable "=" functionCall  */
                                                        { (yyval.t_assign_exp) = new ast::AssignExp((yyloc), *(yyvsp[-2].t_exp), *(yyvsp[0].t_call_exp)); print_rules("variableDeclaration", "assignable ASSIGN functionCall");}
    break;

  case 250: /* variableDeclaration: functionCall "=" variable  */
                                                        { (yyval.t_assign_exp) = new ast::AssignExp((yyloc), *(yyvsp[-2].t_call_exp), *(yyvsp[0].t_exp)); print_rules("variableDeclaration", "functionCall ASSIGN variable");}
    break;

  case 251: /* variableDeclaration: functionCall "=" functionCall  */
                                                        { (yyval.t_assign_exp) = new ast::AssignExp((yyloc), *(yyvsp[-2].t_call_exp), *(yyvsp[0].t_call_exp)); print_rules("variableDeclaration", "functionCall ASSIGN functionCall");}
    break;

  case 252: /* variableDeclaration: assignable "=" ":"  */
                                                        { (yyval.t_assign_exp) = new ast::AssignExp((yyloc), *(yyvsp[-2].t_exp), *new ast::ColonVar((yylsp[0]))); print_rules("variableDeclaration", "assignable ASSIGN COLON");}
    break;

  case 253: /* variableDeclaration: functionCall "=" ":"  */
                                                        { (yyval.t_assign_exp) = new ast::AssignExp((yyloc), *(yyvsp[-2].t_call_exp), *new ast::ColonVar((yylsp[0]))); print_rules("variableDeclaration", "functionCall ASSIGN COLON");}
    break;

  case 254: /* variableDeclaration: assignable "=" returnControl  */
                                                        { (yyval.t_assign_exp) = new ast::AssignExp((yyloc), *(yyvsp[-2].t_exp), *(yyvsp[0].t_return_exp)); print_rules("variableDeclaration", "assignable ASSIGN returnControl");}
    break;

  case 255: /* variableDeclaration: functionCall "=" returnControl  */
                                                        { (yyval.t_assign_exp) = new ast::AssignExp((yyloc), *(yyvsp[-2].t_call_exp), *(yyvsp[0].t_return_exp)); print_rules("variableDeclaration", "functionCall ASSIGN returnControl");}
    break;

  case 256: /* assignable: variable "." "identifier"  */
                                                { (yyval.t_exp) = new ast::FieldExp((yyloc), *(yyvsp[-2].t_exp), *new ast::SimpleVar((yyloc), symbol::Symbol(*(yyvsp[0].str)))); delete (yyvsp[0].str);print_rules("assignable", "variable DOT ID");}
    break;

  case 257: /* assignable: variable "." keywords  */
                                                { (yyval.t_exp) = new ast::FieldExp((yyloc), *(yyvsp[-2].t_exp), *(yyvsp[0].t_simple_var)); print_rules("assignable", "variable DOT keywords");}
    break;

  case 258: /* assignable: variable "." functionCall  */
                                                { (yyvsp[0].t_call_exp)->setName(new ast::FieldExp((yyloc), *(yyvsp[-2].t_exp), (yyvsp[0].t_call_exp)->getName()));(yyvsp[0].t_call_exp)->setLocation((yyloc));(yyval.t_exp) = (yyvsp[0].t_call_exp);print_rules("assignable", "variable DOT functionCall");}
    break;

  case 259: /* assignable: functionCall "." "identifier"  */
                                                { (yyval.t_exp) = new ast::FieldExp((yyloc), *(yyvsp[-2].t_call_exp), *new ast::SimpleVar((yyloc), symbol::Symbol(*(yyvsp[0].str)))); delete (yyvsp[0].str); print_rules("assignable", "functionCall DOT ID");}
    break;

  case 260: /* assignable: functionCall "." keywords  */
                                                { (yyval.t_exp) = new ast::FieldExp((yyloc), *(yyvsp[-2].t_call_exp), *(yyvsp[0].t_simple_var)); print_rules("assignable", "functionCall DOT keywords");}
    break;

  case 261: /* assignable: "identifier"  */
                                                { (yyval.t_exp) = new ast::SimpleVar((yyloc), symbol::Symbol(*(yyvsp[0].str))); delete (yyvsp[0].str);print_rules("assignable", "ID");}
    break;

  case 262: /* assignable: multipleResults  */
                                                { (yyval.t_exp) = (yyvsp[0].t_assignlist_exp); print_rules("assignable", "multipleResults");}
    break;

  case 263: /* assignable: variable "(" functionArgs ")"  */
                                                { (yyval.t_exp) = new ast::CallExp((yyloc), *(yyvsp[-3].t_exp), *(yyvsp[-1].t_list_exp)); print_rules("assignable", "ariable LPAREN functionArgs RPAREN");}
    break;

  case 264: /* assignable: functionCall "(" functionArgs ")"  */
                                                { (yyval.t_exp) = new ast::CallExp((yyloc), *(yyvsp[-3].t_call_exp), *(yyvsp[-1].t_list_exp)); print_rules("assignable", "functionCall LPAREN functionArgs RPAREN");}
    break;

  case 265: /* multipleResults: "[" matrixOrCellColumns "]"  */
                                    { (yyval.t_assignlist_exp) = new ast::AssignListExp((yyloc), *(yyvsp[-1].t_list_exp)); print_rules("multipleResults", "LBRACK matrixOrCellColumns RBRACK");}
    break;

  case 266: /* argumentsControl: "arguments" "end of line" argumentsDeclarations "end"  */
                                                  { (yyval.t_arguments_exp) = (yyvsp[-1].t_arguments_exp); print_rules("argumentsControl", "ARGUMENTS EOL argumentsDeclarations END");}
    break;

  case 267: /* argumentsControl: "arguments" "end of line" "end"  */
                                  {
    print_rules("argumentsControl", "ARGUMENTS EOL argumentsDeclarations END");
    ast::exps_t* tmp = new ast::exps_t;
    #ifdef BUILD_DEBUG_AST
    tmp->push_back(new ast::CommentExp((yyloc), new std::wstring(L"Empty arguments")));
    #endif
    (yyval.t_arguments_exp) = new ast::ArgumentsExp((yyloc), *tmp);
}
    break;

  case 268: /* argumentsDeclarations: argumentsDeclarations argumentDeclaration lineEnd  */
                                                        {
        (yyval.t_arguments_exp)->getExps().push_back((yyvsp[-1].t_argument_dec));
        (yyval.t_arguments_exp) = (yyvsp[-2].t_arguments_exp);
        print_rules("argumentsDeclarations", "argumentsDeclarations EOL argumentDeclaration EOL");
    }
    break;

  case 269: /* argumentsDeclarations: argumentsDeclarations "line comment" "end of line"  */
                                                        {
        (yyval.t_arguments_exp)->getExps().push_back(new ast::CommentExp((yylsp[-1]), (yyvsp[-1].comment)));
        (yyval.t_arguments_exp) = (yyvsp[-2].t_arguments_exp);
        print_rules("argumentsDeclarations", "argumentsDeclarations EOL argumentDeclaration EOL");
    }
    break;

  case 270: /* argumentsDeclarations: argumentDeclaration lineEnd  */
                                                        {
        ast::exps_t* tmp = new ast::exps_t;
        tmp->push_back((yyvsp[-1].t_argument_dec));
        (yyval.t_arguments_exp) = new ast::ArgumentsExp((yyloc), *tmp);
        print_rules("argumentsDeclarations", "argumentDeclaration EOL");
    }
    break;

  case 271: /* argumentsDeclarations: "line comment" "end of line"  */
                                                        {
        ast::exps_t* tmp = new ast::exps_t;
        tmp->push_back(new ast::CommentExp((yylsp[-1]), (yyvsp[-1].comment)));
        (yyval.t_arguments_exp) = new ast::ArgumentsExp((yyloc), *tmp);
    }
    break;

  case 272: /* argumentDeclaration: argumentName argumentDimension argumentValidators argumentDefaultValue  */
                                                                                 {
    (yyval.t_argument_dec) = new ast::ArgumentDec((yyloc),
                                *(yyvsp[-3].t_exp),
                                *(yyvsp[-2].t_exp),
                                *new ast::NilExp((yyloc)),
                                *(yyvsp[-1].t_exp),
                                *(yyvsp[0].t_exp));
                                print_rules("argumentDeclaration", "ID LPAREN RPAREN ID");
}
    break;

  case 273: /* argumentDeclaration: argumentName argumentDimension "identifier" argumentValidators argumentDefaultValue  */
                                                                                  {
    (yyval.t_argument_dec) = new ast::ArgumentDec((yyloc),
                                *(yyvsp[-4].t_exp),
                                *(yyvsp[-3].t_exp),
                                *new ast::SimpleVar((yylsp[-2]), symbol::Symbol(*(yyvsp[-2].str))),
                                *(yyvsp[-1].t_exp),
                                *(yyvsp[0].t_exp));
                                print_rules("argumentDeclaration", "ID LPAREN RPAREN ID");
}
    break;

  case 274: /* argumentName: "identifier"  */
            { (yyval.t_exp) = new ast::SimpleVar((yyloc), symbol::Symbol(*(yyvsp[0].str))); print_rules("argumentName", "ID");}
    break;

  case 275: /* argumentName: "identifier" "." "identifier"  */
            { (yyval.t_exp) = new ast::FieldExp((yyloc), *new ast::SimpleVar((yylsp[-2]), symbol::Symbol(*(yyvsp[-2].str))), *new ast::SimpleVar((yylsp[0]), symbol::Symbol(*(yyvsp[0].str)))); print_rules("argumentName", "ID DOT ID");}
    break;

  case 276: /* argumentDimension: "(" functionArgs ")"  */
                                        { (yyval.t_exp) = new ast::ArrayListVar((yyloc), *(yyvsp[-1].t_list_exp)); }
    break;

  case 277: /* argumentDimension: %empty  */
                                        { (yyval.t_exp) = new ast::NilExp((yyloc)); }
    break;

  case 278: /* argumentValidators: "{" functionArgs "}"  */
                                        { (yyval.t_exp) = new ast::ArrayListVar((yyloc), *(yyvsp[-1].t_list_exp)); }
    break;

  case 279: /* argumentValidators: %empty  */
                                        { (yyval.t_exp) = new ast::NilExp((yyloc)); }
    break;

  case 280: /* argumentDefaultValue: "=" variable  */
                                        { (yyval.t_exp) = (yyvsp[0].t_exp); }
    break;

  case 281: /* argumentDefaultValue: "=" functionCall  */
                                        { (yyval.t_exp) = (yyvsp[0].t_call_exp); }
    break;

  case 282: /* argumentDefaultValue: %empty  */
                                        { (yyval.t_exp) = new ast::NilExp((yyloc)); }
    break;

  case 283: /* ifControl: "if" condition then thenBody "end"  */
                                                        { (yyval.t_if_exp) = new ast::IfExp((yyloc), *(yyvsp[-3].t_exp), *(yyvsp[-1].t_seq_exp)); print_rules("ifControl", "IF condition then thenBody END");}
    break;

  case 284: /* ifControl: "if" condition then thenBody else elseBody "end"  */
                                                        {
    if ((yyvsp[-1].t_seq_exp) != NULL)
    {
        (yyval.t_if_exp) = new ast::IfExp((yyloc), *(yyvsp[-5].t_exp), *(yyvsp[-3].t_seq_exp), *(yyvsp[-1].t_seq_exp));
    }
    else
    {
       (yyval.t_if_exp) = new ast::IfExp((yyloc), *(yyvsp[-5].t_exp), *(yyvsp[-3].t_seq_exp));
    }
    print_rules("ifControl", "IF condition then thenBody else elseBody END");
    }
    break;

  case 285: /* ifControl: "if" condition then thenBody elseIfControl "end"  */
                                                        { (yyval.t_if_exp) = new ast::IfExp((yyloc), *(yyvsp[-4].t_exp), *(yyvsp[-2].t_seq_exp), *(yyvsp[-1].t_seq_exp)); print_rules("ifControl", "IF condition then thenBody elseIfControl END");}
    break;

  case 286: /* thenBody: expressions  */
                {
            print_rules("thenBody", "expressions");
            (yyvsp[0].t_seq_exp)->getLocation().last_line = (yyvsp[0].t_seq_exp)->getExps().back()->getLocation().last_line;
            (yyvsp[0].t_seq_exp)->getLocation().last_column = (yyvsp[0].t_seq_exp)->getExps().back()->getLocation().last_column;
            (yyval.t_seq_exp) = (yyvsp[0].t_seq_exp);
                }
    break;

  case 287: /* thenBody: %empty  */
                {
    print_rules("thenBody", "Epsilon");
    ast::exps_t* tmp = new ast::exps_t;
    #ifdef BUILD_DEBUG_AST
    tmp->push_back(new ast::CommentExp((yyloc), new std::wstring(L"Empty then body")));
    #endif
    (yyval.t_seq_exp) = new ast::SeqExp((yyloc), *tmp);
                }
    break;

  case 288: /* elseBody: expressions  */
                    {
                        print_rules("elseBody", "expressions");
                        (yyvsp[0].t_seq_exp)->getLocation().last_line = (yyvsp[0].t_seq_exp)->getExps().back()->getLocation().last_line;
                        (yyvsp[0].t_seq_exp)->getLocation().last_column = (yyvsp[0].t_seq_exp)->getExps().back()->getLocation().last_column;
                        (yyval.t_seq_exp) = (yyvsp[0].t_seq_exp);
                    }
    break;

  case 289: /* elseBody: %empty  */
                    {
                        #ifdef BUILD_DEBUG_AST
                            ast::exps_t* tmp = new ast::exps_t;
                            tmp->push_back(new ast::CommentExp((yyloc), new std::wstring(L"Empty else body")));
                            (yyval.t_seq_exp) = new ast::SeqExp((yyloc), *tmp);
                        #else
                            (yyval.t_seq_exp) = NULL;
                        #endif
                        print_rules("elseBody", "Epsilon");
                    }
    break;

  case 290: /* ifConditionBreak: ";"  */
                { /* !! Do Nothing !! */ print_rules("ifConditionBreak", "SEMI");}
    break;

  case 291: /* ifConditionBreak: ";" "end of line"  */
                { /* !! Do Nothing !! */ print_rules("ifConditionBreak", "SEMI EOL");}
    break;

  case 292: /* ifConditionBreak: ","  */
                { /* !! Do Nothing !! */ print_rules("ifConditionBreak", "COMMA");}
    break;

  case 293: /* ifConditionBreak: "," "end of line"  */
                { /* !! Do Nothing !! */ print_rules("ifConditionBreak", "COMMA EOL");}
    break;

  case 294: /* ifConditionBreak: "end of line"  */
                { /* !! Do Nothing !! */ print_rules("ifConditionBreak", "EOL");}
    break;

  case 295: /* then: "then"  */
                                { /* !! Do Nothing !! */ print_rules("then", "THEN");}
    break;

  case 296: /* then: ifConditionBreak "then"  */
                                { /* !! Do Nothing !! */ print_rules("then", "ifConditionBreak THEN");}
    break;

  case 297: /* then: ifConditionBreak "then" "end of line"  */
                                { /* !! Do Nothing !! */ print_rules("then", "ifConditionBreak THEN EOL");}
    break;

  case 298: /* then: "then" ifConditionBreak  */
                                { /* !! Do Nothing !! */ print_rules("then", "THEN ifConditionBreak");}
    break;

  case 299: /* then: ifConditionBreak  */
                                { /* !! Do Nothing !! */ print_rules("then", "ifConditionBreak");}
    break;

  case 300: /* then: %empty  */
                                { /* !! Do Nothing !! */ print_rules("then", "Epsilon");}
    break;

  case 301: /* else: "else"  */
                    { /* !! Do Nothing !! */ print_rules("else", "ELSE");}
    break;

  case 302: /* else: "else" ","  */
                    { /* !! Do Nothing !! */ print_rules("else", "ELSE COMMA");}
    break;

  case 303: /* else: "else" ";"  */
                    { /* !! Do Nothing !! */ print_rules("else", "ELSE SEMI");}
    break;

  case 304: /* else: "else" "end of line"  */
                    { /* !! Do Nothing !! */ print_rules("else", "ELSE EOL");}
    break;

  case 305: /* else: "else" "," "end of line"  */
                    { /* !! Do Nothing !! */ print_rules("else", "ELSE COMMA EOL");}
    break;

  case 306: /* else: "else" ";" "end of line"  */
                    { /* !! Do Nothing !! */ print_rules("else", "ELSE SEMI EOL");}
    break;

  case 307: /* elseIfControl: "elseif" condition then thenBody  */
                                    {
                                        print_rules("elseIfControl", "ELSEIF condition then thenBody");
                                        ast::exps_t* tmp = new ast::exps_t;
                                        tmp->push_back(new ast::IfExp((yyloc), *(yyvsp[-2].t_exp), *(yyvsp[0].t_seq_exp)));
                                        (yyval.t_seq_exp) = new ast::SeqExp((yyloc), *tmp);
                                    }
    break;

  case 308: /* elseIfControl: "elseif" condition then thenBody else elseBody  */
                                                              {
                                        print_rules("elseIfControl", "ELSEIF condition then thenBody else elseBody");
                                        ast::exps_t* tmp = new ast::exps_t;
                                        if( (yyvsp[0].t_seq_exp) == NULL)
                                        {
                                            tmp->push_back(new ast::IfExp((yyloc), *(yyvsp[-4].t_exp), *(yyvsp[-2].t_seq_exp)));
                                        }
                                        else
                                        {
                                            tmp->push_back(new ast::IfExp((yyloc), *(yyvsp[-4].t_exp), *(yyvsp[-2].t_seq_exp), *(yyvsp[0].t_seq_exp)));
                                        }
                                        (yyval.t_seq_exp) = new ast::SeqExp((yyloc), *tmp);

                                    }
    break;

  case 309: /* elseIfControl: "elseif" condition then thenBody elseIfControl  */
                                                              {
                                        print_rules("elseIfControl", "ELSEIF condition then thenBody elseIfControl");
                                        ast::exps_t* tmp = new ast::exps_t;
                                        tmp->push_back(new ast::IfExp((yyloc), *(yyvsp[-3].t_exp), *(yyvsp[-1].t_seq_exp), *(yyvsp[0].t_seq_exp)));
                                        (yyval.t_seq_exp) = new ast::SeqExp((yyloc), *tmp);
                                    }
    break;

  case 310: /* selectControl: select selectable selectConditionBreak casesControl "end"  */
                                                                                { (yyval.t_select_exp) = new ast::SelectExp((yyloc), *(yyvsp[-3].t_exp), *(yyvsp[-1].t_list_case)); print_rules("selectControl", "select selectable selectConditionBreak casesControl END");}
    break;

  case 311: /* selectControl: select selectable selectConditionBreak casesControl defaultCase elseBody "end"  */
                                                                                {
                                        if((yyvsp[-1].t_seq_exp) == NULL)
                                        {
                                            (yyval.t_select_exp) = new ast::SelectExp((yyloc), *(yyvsp[-5].t_exp), *(yyvsp[-3].t_list_case));
                                        }
                                        else
                                        {
                                            (yyval.t_select_exp) = new ast::SelectExp((yyloc), *(yyvsp[-5].t_exp), *(yyvsp[-3].t_list_case), *(yyvsp[-1].t_seq_exp));
                                        }
                                        print_rules("selectControl", "select selectable selectConditionBreak casesControl defaultCase elseBody END");
                                    }
    break;

  case 312: /* selectControl: select selectable "line comment" selectConditionBreak casesControl "end"  */
                                                                                { (yyval.t_select_exp) = new ast::SelectExp((yyloc), *(yyvsp[-4].t_exp), *(yyvsp[-1].t_list_case)); delete (yyvsp[-3].comment);print_rules("selectControl", "select selectable COMMENT selectConditionBreak casesControl END");}
    break;

  case 313: /* selectControl: select selectable "line comment" selectConditionBreak casesControl defaultCase elseBody "end"  */
                                                                                          {
                                        if((yyvsp[-1].t_seq_exp) == NULL)
                                        {
                                            (yyval.t_select_exp) = new ast::SelectExp((yyloc), *(yyvsp[-6].t_exp), *(yyvsp[-3].t_list_case));
                                        }
                                        else
                                        {
                                            (yyval.t_select_exp) = new ast::SelectExp((yyloc), *(yyvsp[-6].t_exp), *(yyvsp[-3].t_list_case), *(yyvsp[-1].t_seq_exp));
                                        }
                                        delete (yyvsp[-5].comment);
                                        print_rules("selectControl", "select selectable COMMENT selectConditionBreak casesControl defaultCase elseBody END");
                                    }
    break;

  case 314: /* select: "select"  */
            { /* !! Do Nothing !! */ print_rules("select", "SELECT");}
    break;

  case 315: /* select: "switch"  */
            { /* !! Do Nothing !! */ print_rules("select", "SWITCH");}
    break;

  case 316: /* defaultCase: else  */
                        { /* !! Do Nothing !! */ print_rules("defaultCase", "else");}
    break;

  case 317: /* defaultCase: "otherwise"  */
                        { /* !! Do Nothing !! */ print_rules("defaultCase", "OTHERWISE");}
    break;

  case 318: /* defaultCase: "otherwise" ","  */
                        { /* !! Do Nothing !! */ print_rules("defaultCase", "OTHERWISE COMMA");}
    break;

  case 319: /* defaultCase: "otherwise" ";"  */
                        { /* !! Do Nothing !! */ print_rules("defaultCase", "OTHERWISE SEMI");}
    break;

  case 320: /* defaultCase: "otherwise" "end of line"  */
                        { /* !! Do Nothing !! */ print_rules("defaultCase", "OTHERWISE EOL");}
    break;

  case 321: /* defaultCase: "otherwise" "," "end of line"  */
                        { /* !! Do Nothing !! */ print_rules("defaultCase", "OTHERWISE COMMA EOL");}
    break;

  case 322: /* defaultCase: "otherwise" ";" "end of line"  */
                        { /* !! Do Nothing !! */ print_rules("defaultCase", "OTHERWISE SEMI EOL");}
    break;

  case 323: /* selectable: variable  */
                { (yyval.t_exp) = (yyvsp[0].t_exp); print_rules("selectable", "variable");}
    break;

  case 324: /* selectable: functionCall  */
                { (yyval.t_exp) = (yyvsp[0].t_call_exp); print_rules("selectable", "functionCall");}
    break;

  case 325: /* selectConditionBreak: "end of line"  */
                { /* !! Do Nothing !! */ print_rules("selectConditionBreak", "EOL");}
    break;

  case 326: /* selectConditionBreak: "," "end of line"  */
                { /* !! Do Nothing !! */ print_rules("selectConditionBreak", "COMMA EOL");}
    break;

  case 327: /* selectConditionBreak: ";" "end of line"  */
                { /* !! Do Nothing !! */ print_rules("selectConditionBreak", "SEMI EOL");}
    break;

  case 328: /* selectConditionBreak: ","  */
                { /* !! Do Nothing !! */ print_rules("selectConditionBreak", "COMMA");}
    break;

  case 329: /* selectConditionBreak: ";"  */
                { /* !! Do Nothing !! */ print_rules("selectConditionBreak", "SEMI");}
    break;

  case 330: /* casesControl: "case" variable caseControlBreak caseBody  */
                                                            {(yyval.t_list_case) = new ast::exps_t;(yyval.t_list_case)->push_back(new ast::CaseExp((yyloc), *(yyvsp[-2].t_exp), *(yyvsp[0].t_seq_exp)));print_rules("casesControl", "CASE variable caseControlBreak caseBody");}
    break;

  case 331: /* casesControl: "case" functionCall caseControlBreak caseBody  */
                                                            {(yyval.t_list_case) = new ast::exps_t;(yyval.t_list_case)->push_back(new ast::CaseExp((yyloc), *(yyvsp[-2].t_call_exp), *(yyvsp[0].t_seq_exp)));print_rules("casesControl", "CASE functionCall caseControlBreak caseBody");}
    break;

  case 332: /* casesControl: comments "case" variable caseControlBreak caseBody  */
                                                            {(yyval.t_list_case) = new ast::exps_t;(yyval.t_list_case)->push_back(new ast::CaseExp((yyloc), *(yyvsp[-2].t_exp), *(yyvsp[0].t_seq_exp)));print_rules("casesControl", "comments CASE variable caseControlBreak caseBody");}
    break;

  case 333: /* casesControl: comments "case" functionCall caseControlBreak caseBody  */
                                                            {(yyval.t_list_case) = new ast::exps_t;(yyval.t_list_case)->push_back(new ast::CaseExp((yyloc), *(yyvsp[-2].t_call_exp), *(yyvsp[0].t_seq_exp)));print_rules("casesControl", "comments CASE functionCall caseControlBreak caseBody");}
    break;

  case 334: /* casesControl: casesControl "case" variable caseControlBreak caseBody  */
                                                            {(yyvsp[-4].t_list_case)->push_back(new ast::CaseExp((yyloc), *(yyvsp[-2].t_exp), *(yyvsp[0].t_seq_exp)));(yyval.t_list_case) = (yyvsp[-4].t_list_case);print_rules("casesControl", "casesControl CASE variable caseControlBreak caseBody");}
    break;

  case 335: /* casesControl: casesControl "case" functionCall caseControlBreak caseBody  */
                                                            {(yyvsp[-4].t_list_case)->push_back(new ast::CaseExp((yyloc), *(yyvsp[-2].t_call_exp), *(yyvsp[0].t_seq_exp)));(yyval.t_list_case) = (yyvsp[-4].t_list_case);print_rules("casesControl", "casesControl CASE functionCall caseControlBreak caseBody");}
    break;

  case 336: /* caseBody: expressions  */
                {
                    print_rules("caseBody", "expressions");
                    (yyvsp[0].t_seq_exp)->getLocation().last_line = (yyvsp[0].t_seq_exp)->getExps().back()->getLocation().last_line;
                    (yyvsp[0].t_seq_exp)->getLocation().last_column = (yyvsp[0].t_seq_exp)->getExps().back()->getLocation().last_column;
                    (yyval.t_seq_exp) = (yyvsp[0].t_seq_exp);
                }
    break;

  case 337: /* caseBody: %empty  */
                {
                    print_rules("caseBody", "Epsilon");
                    ast::exps_t* tmp = new ast::exps_t;
                    #ifdef BUILD_DEBUG_AST
                        tmp->push_back(new ast::CommentExp((yyloc), new std::wstring(L"Empty case body")));
                    #endif
                    (yyval.t_seq_exp) = new ast::SeqExp((yyloc), *tmp);
                }
    break;

  case 338: /* caseControlBreak: "then"  */
                                    { /* !! Do Nothing !! */ print_rules("caseControlBreak", "THEN");}
    break;

  case 339: /* caseControlBreak: ","  */
                                    { /* !! Do Nothing !! */ print_rules("caseControlBreak", "COMMA");}
    break;

  case 340: /* caseControlBreak: ";"  */
                                    { /* !! Do Nothing !! */ print_rules("caseControlBreak", "SEMI");}
    break;

  case 341: /* caseControlBreak: "end of line"  */
                                    { /* !! Do Nothing !! */ print_rules("caseControlBreak", "EOL");}
    break;

  case 342: /* caseControlBreak: "then" "end of line"  */
                                    { /* !! Do Nothing !! */ print_rules("caseControlBreak", "THEN EOL");}
    break;

  case 343: /* caseControlBreak: "," "end of line"  */
                                    { /* !! Do Nothing !! */ print_rules("caseControlBreak", "COMMA EOL");}
    break;

  case 344: /* caseControlBreak: ";" "end of line"  */
                                    { /* !! Do Nothing !! */ print_rules("caseControlBreak", "SEMI EOL");}
    break;

  case 345: /* caseControlBreak: "then" ","  */
                                    { /* !! Do Nothing !! */ print_rules("caseControlBreak", "THEN COMMA");}
    break;

  case 346: /* caseControlBreak: "then" "," "end of line"  */
                                    { /* !! Do Nothing !! */ print_rules("caseControlBreak", "THEN COMMA EOL");}
    break;

  case 347: /* caseControlBreak: "then" ";"  */
                                    { /* !! Do Nothing !! */ print_rules("caseControlBreak", "THEN SEMI");}
    break;

  case 348: /* caseControlBreak: "then" ";" "end of line"  */
                                    { /* !! Do Nothing !! */ print_rules("caseControlBreak", "THEN SEMI EOL");}
    break;

  case 349: /* caseControlBreak: %empty  */
                                    { /* !! Do Nothing !! */ print_rules("caseControlBreak", "Epsilon");}
    break;

  case 350: /* forControl: "for" "identifier" "=" forIterator forConditionBreak forBody "end"  */
                                                                        { (yyval.t_for_exp) = new ast::ForExp((yyloc), *new ast::VarDec((yylsp[-4]), symbol::Symbol(*(yyvsp[-5].str)), *(yyvsp[-3].t_exp)), *(yyvsp[-1].t_seq_exp)); delete (yyvsp[-5].str);print_rules("forControl", "FOR ID ASSIGN forIterator forConditionBreak forBody END    ");}
    break;

  case 351: /* forControl: "for" "(" "identifier" "=" forIterator ")" forConditionBreak forBody "end"  */
                                                                        { (yyval.t_for_exp) = new ast::ForExp((yyloc), *new ast::VarDec((yylsp[-5]), symbol::Symbol(*(yyvsp[-6].str)), *(yyvsp[-4].t_exp)), *(yyvsp[-1].t_seq_exp)); delete (yyvsp[-6].str);print_rules("forControl", "FOR LPAREN ID ASSIGN forIterator RPAREN forConditionBreak forBody END");}
    break;

  case 352: /* forIterator: functionCall  */
                                { (yyval.t_exp) = (yyvsp[0].t_call_exp); print_rules("forIterator", "functionCall");}
    break;

  case 353: /* forIterator: variable  */
                                { (yyval.t_exp) = (yyvsp[0].t_exp); print_rules("forIterator", "variable");}
    break;

  case 354: /* forConditionBreak: "end of line"  */
                    { /* !! Do Nothing !! */ print_rules("forConditionBreak", "EOL");}
    break;

  case 355: /* forConditionBreak: ";"  */
                    { /* !! Do Nothing !! */ print_rules("forConditionBreak", "SEMI");}
    break;

  case 356: /* forConditionBreak: ";" "end of line"  */
                    { /* !! Do Nothing !! */ print_rules("forConditionBreak", "SEMI EOL");}
    break;

  case 357: /* forConditionBreak: ","  */
                    { /* !! Do Nothing !! */ print_rules("forConditionBreak", "COMMA");}
    break;

  case 358: /* forConditionBreak: "," "end of line"  */
                    { /* !! Do Nothing !! */ print_rules("forConditionBreak", "COMMA EOL");}
    break;

  case 359: /* forConditionBreak: "do"  */
                    { /* !! Do Nothing !! */ print_rules("forConditionBreak", "DO");}
    break;

  case 360: /* forConditionBreak: "do" "end of line"  */
                    { /* !! Do Nothing !! */ print_rules("forConditionBreak", "DO EOL");}
    break;

  case 361: /* forConditionBreak: %empty  */
                    { /* !! Do Nothing !! */ print_rules("forConditionBreak", "Epsilon");}
    break;

  case 362: /* forBody: expressions  */
                {
                    print_rules("forBody", "expressions");
                    (yyvsp[0].t_seq_exp)->getLocation().last_line = (yyvsp[0].t_seq_exp)->getExps().back()->getLocation().last_line;
                    (yyvsp[0].t_seq_exp)->getLocation().last_column = (yyvsp[0].t_seq_exp)->getExps().back()->getLocation().last_column;
                    (yyval.t_seq_exp) = (yyvsp[0].t_seq_exp);
                }
    break;

  case 363: /* forBody: %empty  */
                {
                    print_rules("forBody", "Epsilon");
                    ast::exps_t* tmp = new ast::exps_t;
                    #ifdef BUILD_DEBUG_AST
                        tmp->push_back(new ast::CommentExp((yyloc), new std::wstring(L"Empty for body")));
                    #endif
                    (yyval.t_seq_exp) = new ast::SeqExp((yyloc), *tmp);
                }
    break;

  case 364: /* whileControl: "while" condition whileConditionBreak whileBody "end"  */
                                                    { (yyval.t_while_exp) = new ast::WhileExp((yyloc), *(yyvsp[-3].t_exp), *(yyvsp[-1].t_seq_exp)); print_rules("whileControl", "WHILE condition whileConditionBreak whileBody END");}
    break;

  case 365: /* whileBody: expressions  */
                    {
                        print_rules("whileBody", "expressions");
                        (yyvsp[0].t_seq_exp)->getLocation().last_line = (yyvsp[0].t_seq_exp)->getExps().back()->getLocation().last_line;
                        (yyvsp[0].t_seq_exp)->getLocation().last_column = (yyvsp[0].t_seq_exp)->getExps().back()->getLocation().last_column;
                        (yyval.t_seq_exp) = (yyvsp[0].t_seq_exp);
                    }
    break;

  case 366: /* whileBody: %empty  */
                    {
                        print_rules("whileBody", "Epsilon");
                        ast::exps_t* tmp = new ast::exps_t;
                        #ifdef BUILD_DEBUG_AST
                            tmp->push_back(new ast::CommentExp((yyloc), new std::wstring(L"Empty while body")));
                        #endif
                        (yyval.t_seq_exp) = new ast::SeqExp((yyloc), *tmp);
                    }
    break;

  case 367: /* whileConditionBreak: ","  */
                        { /* !! Do Nothing !! */ print_rules("whileConditionBreak", "COMMA");}
    break;

  case 368: /* whileConditionBreak: ";"  */
                        { /* !! Do Nothing !! */ print_rules("whileConditionBreak", "SEMI");}
    break;

  case 369: /* whileConditionBreak: "do"  */
                        { /* !! Do Nothing !! */ print_rules("whileConditionBreak", "DO");}
    break;

  case 370: /* whileConditionBreak: "do" ","  */
                        { /* !! Do Nothing !! */ print_rules("whileConditionBreak", "DO COMMA");}
    break;

  case 371: /* whileConditionBreak: "do" ";"  */
                        { /* !! Do Nothing !! */ print_rules("whileConditionBreak", "DO SEMI");}
    break;

  case 372: /* whileConditionBreak: "then"  */
                        { /* !! Do Nothing !! */ print_rules("whileConditionBreak", "THEN");}
    break;

  case 373: /* whileConditionBreak: "then" ","  */
                        { /* !! Do Nothing !! */ print_rules("whileConditionBreak", "THEN COMMA");}
    break;

  case 374: /* whileConditionBreak: "then" ";"  */
                        { /* !! Do Nothing !! */ print_rules("whileConditionBreak", "THEN SEMI");}
    break;

  case 375: /* whileConditionBreak: "line comment" "end of line"  */
                        { delete (yyvsp[-1].comment); print_rules("whileConditionBreak", "COMMENT EOL");}
    break;

  case 376: /* whileConditionBreak: "end of line"  */
                        { /* !! Do Nothing !! */ print_rules("whileConditionBreak", "EOL");}
    break;

  case 377: /* whileConditionBreak: "," "end of line"  */
                        { /* !! Do Nothing !! */ print_rules("whileConditionBreak", "COMMA EOL");}
    break;

  case 378: /* whileConditionBreak: ";" "end of line"  */
                        { /* !! Do Nothing !! */ print_rules("whileConditionBreak", "SEMI EOL");}
    break;

  case 379: /* whileConditionBreak: "do" "end of line"  */
                        { /* !! Do Nothing !! */ print_rules("whileConditionBreak", "SEMI EOL");}
    break;

  case 380: /* whileConditionBreak: "do" "," "end of line"  */
                        { /* !! Do Nothing !! */ print_rules("whileConditionBreak", "DO COMMA EOL");}
    break;

  case 381: /* whileConditionBreak: "do" ";" "end of line"  */
                        { /* !! Do Nothing !! */ print_rules("whileConditionBreak", "DO SEMI EOL");}
    break;

  case 382: /* whileConditionBreak: "then" "end of line"  */
                        { /* !! Do Nothing !! */ print_rules("whileConditionBreak", "THEN EOL");}
    break;

  case 383: /* whileConditionBreak: "then" "," "end of line"  */
                        { /* !! Do Nothing !! */ print_rules("whileConditionBreak", "THEN COMMA EOL");}
    break;

  case 384: /* whileConditionBreak: "then" ";" "end of line"  */
                        { /* !! Do Nothing !! */ print_rules("whileConditionBreak", "THEN SEMI EOL");}
    break;

  case 385: /* tryControl: "try" catchBody "catch" catchBody "end"  */
                                    { (yyval.t_try_exp) =new ast::TryCatchExp((yyloc), *(yyvsp[-3].t_seq_exp), *(yyvsp[-1].t_seq_exp)); print_rules("tryControl", "TRY catchBody CATCH catchBody END");}
    break;

  case 386: /* tryControl: "try" catchBody "end"  */
                                    {
                                        print_rules("tryControl", "TRY catchBody END");
                                        ast::exps_t* tmp = new ast::exps_t;
                                        #ifdef BUILD_DEBUG_AST
                                            tmp->push_back(new ast::CommentExp((yyloc), new std::wstring(L"Empty catch body")));
                                        #endif
                                        (yyval.t_try_exp) = new ast::TryCatchExp((yyloc), *(yyvsp[-1].t_seq_exp), *new ast::SeqExp((yyloc), *tmp));
                                    }
    break;

  case 387: /* catchBody: expressions  */
                    {
                        print_rules("catchBody", "expressions");
                        (yyvsp[0].t_seq_exp)->getLocation().last_line = (yyvsp[0].t_seq_exp)->getExps().back()->getLocation().last_line;
                        (yyvsp[0].t_seq_exp)->getLocation().last_column = (yyvsp[0].t_seq_exp)->getExps().back()->getLocation().last_column;
                        (yyval.t_seq_exp) = (yyvsp[0].t_seq_exp);
                    }
    break;

  case 388: /* catchBody: "end of line" expressions  */
                    {
                        print_rules("catchBody", "EOL expressions");
                        (yyvsp[0].t_seq_exp)->getLocation().last_line = (yyvsp[0].t_seq_exp)->getExps().back()->getLocation().last_line;
                        (yyvsp[0].t_seq_exp)->getLocation().last_column = (yyvsp[0].t_seq_exp)->getExps().back()->getLocation().last_column;
                        (yyval.t_seq_exp) = (yyvsp[0].t_seq_exp);
                    }
    break;

  case 389: /* catchBody: ";" expressions  */
                    {
                        print_rules("catchBody", "SEMI expressions");
                        (yyvsp[0].t_seq_exp)->getLocation().last_line = (yyvsp[0].t_seq_exp)->getExps().back()->getLocation().last_line;
                        (yyvsp[0].t_seq_exp)->getLocation().last_column = (yyvsp[0].t_seq_exp)->getExps().back()->getLocation().last_column;
                        (yyval.t_seq_exp) = (yyvsp[0].t_seq_exp);
                    }
    break;

  case 390: /* catchBody: "," expressions  */
                    {
                        print_rules("catchBody", "COMMA expressions");
                        (yyvsp[0].t_seq_exp)->getLocation().last_line = (yyvsp[0].t_seq_exp)->getExps().back()->getLocation().last_line;
                        (yyvsp[0].t_seq_exp)->getLocation().last_column = (yyvsp[0].t_seq_exp)->getExps().back()->getLocation().last_column;
                        (yyval.t_seq_exp) = (yyvsp[0].t_seq_exp);
                    }
    break;

  case 391: /* catchBody: "end of line"  */
                    {
                        print_rules("catchBody", "EOL");
                        ast::exps_t* tmp = new ast::exps_t;
                        #ifdef BUILD_DEBUG_AST
                            tmp->push_back(new ast::CommentExp((yyloc), new std::wstring(L"Empty catch body")));
                        #endif
                        (yyval.t_seq_exp) = new ast::SeqExp((yyloc), *tmp);
                    }
    break;

  case 392: /* catchBody: %empty  */
                    {
                        print_rules("catchBody", "Epsilon");
                        ast::exps_t* tmp = new ast::exps_t;
                        #ifdef BUILD_DEBUG_AST
                            tmp->push_back(new ast::CommentExp((yyloc), new std::wstring(L"Empty catch body")));
                        #endif
                        (yyval.t_seq_exp) = new ast::SeqExp((yyloc), *tmp);
                    }
    break;

  case 393: /* returnControl: "return"  */
                        { (yyval.t_return_exp) = new ast::ReturnExp((yyloc)); print_rules("returnControl", "RETURN");}
    break;

  case 394: /* returnControl: "return" variable  */
                        { (yyval.t_return_exp) = new ast::ReturnExp((yyloc), (yyvsp[0].t_exp)); print_rules("returnControl", "RETURN variable");}
    break;

  case 395: /* returnControl: "return" functionCall  */
                        { (yyval.t_return_exp) = new ast::ReturnExp((yyloc), (yyvsp[0].t_call_exp)); print_rules("returnControl", "RETURN functionCall");}
    break;

  case 396: /* comments: "line comment" "end of line"  */
                        { delete (yyvsp[-1].comment); print_rules("comments", "COMMENT EOL");}
    break;

  case 397: /* comments: comments "line comment" "end of line"  */
                        { delete (yyvsp[-1].comment); print_rules("comments", "comments COMMENT EOL");}
    break;

  case 398: /* lineEnd: "end of line"  */
                { print_rules("lineEnd", "EOL");}
    break;

  case 399: /* lineEnd: "line comment" "end of line"  */
                { delete (yyvsp[-1].comment); print_rules("lineEnd", "COMMENT EOL");}
    break;

  case 400: /* keywords: "if"  */
                { (yyval.t_simple_var) = new ast::SimpleVar((yyloc), symbol::Symbol(L"if"));           print_rules("keywords", "IF");}
    break;

  case 401: /* keywords: "then"  */
                { (yyval.t_simple_var) = new ast::SimpleVar((yyloc), symbol::Symbol(L"then"));         print_rules("keywords", "THEN");}
    break;

  case 402: /* keywords: "else"  */
                { (yyval.t_simple_var) = new ast::SimpleVar((yyloc), symbol::Symbol(L"else"));         print_rules("keywords", "ELSE");}
    break;

  case 403: /* keywords: "elseif"  */
                { (yyval.t_simple_var) = new ast::SimpleVar((yyloc), symbol::Symbol(L"elseif"));       print_rules("keywords", "ELSEIF");}
    break;

  case 404: /* keywords: "end"  */
                { (yyval.t_simple_var) = new ast::SimpleVar((yyloc), symbol::Symbol(L"end"));          print_rules("keywords", "END");}
    break;

  case 405: /* keywords: "select"  */
                { (yyval.t_simple_var) = new ast::SimpleVar((yyloc), symbol::Symbol(L"select"));       print_rules("keywords", "SELECT");}
    break;

  case 406: /* keywords: "switch"  */
                { (yyval.t_simple_var) = new ast::SimpleVar((yyloc), symbol::Symbol(L"switch"));       print_rules("keywords", "SWITCH");}
    break;

  case 407: /* keywords: "otherwise"  */
                { (yyval.t_simple_var) = new ast::SimpleVar((yyloc), symbol::Symbol(L"otherwise"));    print_rules("keywords", "OTHERWISE");}
    break;

  case 408: /* keywords: "case"  */
                { (yyval.t_simple_var) = new ast::SimpleVar((yyloc), symbol::Symbol(L"case"));         print_rules("keywords", "CASE");}
    break;

  case 409: /* keywords: "function"  */
                { (yyval.t_simple_var) = new ast::SimpleVar((yyloc), symbol::Symbol(L"function"));     print_rules("keywords", "FUNCTION");}
    break;

  case 410: /* keywords: "endfunction"  */
                { (yyval.t_simple_var) = new ast::SimpleVar((yyloc), symbol::Symbol(L"endfunction"));  print_rules("keywords", "ENDFUNCTION");}
    break;

  case 411: /* keywords: "for"  */
                { (yyval.t_simple_var) = new ast::SimpleVar((yyloc), symbol::Symbol(L"for"));          print_rules("keywords", "FOR");}
    break;

  case 412: /* keywords: "while"  */
                { (yyval.t_simple_var) = new ast::SimpleVar((yyloc), symbol::Symbol(L"while"));        print_rules("keywords", "WHILE");}
    break;

  case 413: /* keywords: "do"  */
                { (yyval.t_simple_var) = new ast::SimpleVar((yyloc), symbol::Symbol(L"do"));           print_rules("keywords", "DO");}
    break;

  case 414: /* keywords: "break"  */
                { (yyval.t_simple_var) = new ast::SimpleVar((yyloc), symbol::Symbol(L"break"));        print_rules("keywords", "BREAK");}
    break;

  case 415: /* keywords: "try"  */
                { (yyval.t_simple_var) = new ast::SimpleVar((yyloc), symbol::Symbol(L"try"));          print_rules("keywords", "TRY");}
    break;

  case 416: /* keywords: "catch"  */
                { (yyval.t_simple_var) = new ast::SimpleVar((yyloc), symbol::Symbol(L"catch"));        print_rules("keywords", "CATCH");}
    break;

  case 417: /* keywords: "return"  */
                { (yyval.t_simple_var) = new ast::SimpleVar((yyloc), symbol::Symbol(L"return"));       print_rules("keywords", "RETURN");}
    break;



      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  yyerror_range[1] = yylloc;
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}



bool endsWith(const std::string & str, const std::string & end)
{
    if (end.size() > str.size())
    {
    return false;
    }

    return std::equal(end.rbegin(), end.rend(), str.rbegin());
}

void yyerror(std::string msg) {
    if ((!endsWith(msg, "FLEX_ERROR") && !ParserSingleInstance::isStrictMode())
       || ParserSingleInstance::getExitStatus() == Parser::Succeded)
    {
        wchar_t* pstMsg = to_wide_string(msg.c_str());
        ParserSingleInstance::PrintError(pstMsg);
        ParserSingleInstance::setExitStatus(Parser::Failed);
    delete ParserSingleInstance::getTree();
        FREE(pstMsg);
    }
}

