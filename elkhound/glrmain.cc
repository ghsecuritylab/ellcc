// glrmain.cc            see license.txt for copyright and terms of use
// simple 'main' to parse a file; used by some of the example
// grammars (this file is *not* required for general use of
// the Elkhound parser)

#include <iostream>             // cout
#include <stdlib.h>       // exit

#include "trace.h"        // traceAddSys
#include "parssppt.h"     // ParseTreeAndTokens, treeMain
#include "ckheap.h"       // malloc_stats
#include "SourceLocation.h"    // SourceLocation
using ellcc::SourceLocation;
#include ".h"       // SourceManager
#include "cc_lang.h"      // CCLang
#include "parsetables.h"  // ParseTables

using namespace std;

// no bison-parser present, so define it myself
Lexer2Token const *yylval = NULL;

// what a mess..
ParseTables *makeParseTables();
UserActions *makeUserActions(StringTable &table, CCLang &lang);

void doit(int argc, char **argv)
{
  traceAddSys("progress");
  //traceAddSys("parse-tree");

  SourceManager mgr;
  SemanticValue treeTop;
  CCLang lang;
  ParseTreeAndTokens tree(lang, treeTop);
  UserActions *user = makeUserActions(tree.lexer2.idTable, lang);
  ParseTables *tables = makeParseTables();
  tree.userAct = user;
  tree.tables = tables;
  if (!treeMain(tree, argc, argv)) {
    // parse error
    exit(2);
  }

  cout << "final parse result: " << treeTop << endl;

  // global cleanup
  delete user;
  delete tables;
  traceRemoveAll();
}

int main(int argc, char **argv)
{
  doit(argc, argv);

  //malloc_stats();

  return 0;
}
