// generic_amb.h
// generic ambiguity resolution; was in cc_tcheck.cc, but gnu.cc
// wants to use it too

#ifndef GENERIC_AMB_H
#define GENERIC_AMB_H

#include "cc_ast.h"         // C++ AST
#include "cc_env.h"         // Env, DisambiguationErrorTrapper
#include "objlist.h"        // ObjList
#include "trace.h"          // TRACE

using namespace sm;
using namespace ellcc;

// when I print a message saying how an ambiguity was resolved,
// I want to describe the selected node; in most cases the name
// of that node's type suffices
template <class NODE>
string ambiguousNodeName(NODE const *n)
{
  return n->kindName();
}

// definitions in cc_tcheck.cc
sm::string ambiguousNodeName(Declarator const *n);
sm::string ambiguousNodeName(Expression const *e);


// after a node has failed to typecheck, I may need to mark it
// as "never been typechecked", so that if it's a shared subtree
// in an ambiguous region we'll attempt to check it in each
// context (since that's what's needed to put errors into the
// environment); in the common case I don't need anything
//
// update: this doesn't work, because the problem may be one
// level down from the top node..
//  template <class NODE>
//  inline void markAsFailed(NODE *n) {}

//  // but for Expressions I need to nullify the 'type' field
//  inline void markAsFailed(Expression *n)
//  {
//    n->type = NULL;
//  }


// Generic ambiguity resolution:  We check all the alternatives, and
// select the one which typechecks without errors.  Complain if the
// number of successful alternatives is not 1.
template <class NODE, class EXTRA>
NODE *resolveAmbiguity(
  // pointer to the first alternative; we find other alternatives
  // by following the 'ambiguity' links
  NODE *ths,

  // typechecking context
  Env &env,

  // actual type name of 'NODE' (superclass type in the AST)
  char const *nodeTypeName,

  // when 'priority' is true, then the alternatives are considered to
  // be listed in order of preference, such that the first one to
  // successfully typecheck is immediately chosen
  bool priority,

  // This extra argument will be passed to NODE->mid_tcheck, and
  // whatever the succeeding tcheck puts back into its 'extra' will
  // be copied into the caller's version.  (Currently, EXTRA is
  // always either 'Declarator::Tcheck' or it is 'int' (and ignored),
  // if that helps for concreteness.)
  EXTRA &callerExtra)
{
  // grab location before checking the alternatives
  SourceLocation loc = env.loc();

  // how many alternatives?
  int numAlts = 1;
  {
    for (NODE *a = ths->ambiguity; a != NULL; a = a->ambiguity) {
      numAlts++;
    }
  }

  TRACE("disamb",
    toString(loc) << ": ambiguous " << nodeTypeName
                  << " with " << numAlts << " alternatives");

  // make an array of lists to hold the errors generated by the
  // various alternatives
  enum { NOMINAL_MAX_ALT = 3 };
  DiagnosticBuffer* altBufferArray[NOMINAL_MAX_ALT];
  DiagnosticBuffer** altBuffers = altBufferArray;
  
  if (numAlts > NOMINAL_MAX_ALT) {
    // there are too many ambiguities to fit into the stack-allocated
    // array, so use an array on the heap instead; this is very
    // unusual, but not impossible (see e.g. in/t0328.cc)
    altBuffers = new DiagnosticBuffer*[numAlts];
  }

  for (int i = 0; i < numAlts; ++i)
      altBuffers[i] = NULL;

  // copy the caller's 'extra' so we can make untainted copies
  // for each iteration
  EXTRA origExtra(callerExtra);

  // check each one
  int altIndex = 0;
  int numOk = 0;
  NODE *lastOk = NULL;
  int lastOkIndex = -1;
  
  // save current error messages so we will only be dealing with
  // those generated by the current alternative
  {
    DisambiguationErrorTrapper trapper(env);

    for (NODE *alt = ths; alt != NULL; alt = alt->ambiguity, altIndex++) {
      int beforeChange = env.getChangeCount();

      TRACE("disamb",
            toString(loc) << ": considering " << ambiguousNodeName(alt));

      env.push();
      // tcheck 'alt'
      EXTRA extra(origExtra);
      try {
        alt->mid_tcheck(env, extra);
      }
      catch (x_assert &x) {
        HANDLER();
        env.mark(DIAG_FROM_DISAMB);
        env.pop();
        throw;
      }

      // move that run's errors into a per-alternative list

      // did that alternative succeed?
      if (!env.hasDisambErrors()) {
        // yes; update our success trackers
        numOk++;
        lastOk = alt;
        lastOkIndex = altIndex;

        // copy that alternative's 'extra' to the caller's
        callerExtra = extra;

        DiagnosticBuffer* buffer = static_cast<DiagnosticBuffer*>(env.diag.Take());
        altBuffers[altIndex] = buffer;
        if (priority) {
          // the alternatives are listed in priority order, so once an
          // alternative succeeds, stop and select it
          break;
        }
      }
      else {
        // if this NODE failed to check, then it had better not
        // have modified the environment
        if (beforeChange != env.getChangeCount()) {
          // 1/02/03: there used to be an assertion here, but
          // this actually happens sometimes, and it's unavoidable.
          // for example,
          //   int foo(smanip<TP> &m);
          // when interpreted as an integer with an initializer has
          // both of its initializers fail to tcheck (unless TP is
          // a variable), but that's only after 'foo' has been added;
          // and the language requires that 'foo' be visible inside
          // its own initializer, so it has to be added first.
          //
          // what I really want here is to make sure this now-corrupt
          // environment can't participate in any eventually successful
          // parse, so I'll just insert one more (non-disambiguating)
          // error, so this entire interpretation line has to be rejected
          // one way or another
          env.report(loc, diag::err_parse_rejected_alternative_modified_environment);
        }

        DiagnosticBuffer* buffer = static_cast<DiagnosticBuffer*>(env.diag.Take());
        altBuffers[altIndex] = buffer;
      }
    }
    
    // env's error messages restored at this point
  }

  if (numOk == 0) {
    // none of the alternatives checked out
    TRACE("disamb",
      toString(loc) << ": ambiguous " << nodeTypeName << ": all bad");
    //breaker();  // nsFastLoadFile.i provokes this many times, but all benign (?)

    // add a note about the ambiguity
    env.report(loc, diag::note_parse_begin_messages_from_ambiguity);
    for (int i=0; i<numAlts; i++) {
      if (i > 0) {
        env.report(loc, diag::note_parse_separator_messages_from_ambiguity);
      }
      env.diag.Give(altBuffers[i]);
      altBuffers[i] = NULL;
      env.pop();
    }
    env.report(loc, diag::note_parse_end_messages_from_ambiguity);
  } else if (numOk == 1) {
    // one alternative succeeds, which is what we want
    TRACE("disamb",
      toString(loc) << ": ambiguous " << nodeTypeName
                    << ": selected " << ambiguousNodeName(lastOk));

    // put back succeeding alternative's errors (non-disambiguating,
    // and warnings); errors associated with other alternatives will
    // be deleted automatically
    env.diag.Give(altBuffers[lastOkIndex]);
    altBuffers[lastOkIndex] = NULL;
    env.pop();
    
    // select 'lastOk'
    ths = lastOk;
  } else {
    // more than one alternative succeeds, not good
    TRACE("disamb",
      toString(loc) << ": ambiguous " << nodeTypeName << ": multiple good!");

    // now complain; mark it 'disambiguating' so that we'll see
    // this show up even in template code
    env.report(loc, diag::err_parse_more_than_one_ambiguous_alternative_succeeds)
        << DIAG_DISAMBIGUATES;
  }

  // 2005-03-27: I moved this down here so that we always resolve
  // the ambiguity, even when there is no basis for choosing.  The
  // failed resolution will still generate an error, but this way
  // subsequent stages will find an unambiguous AST (in/t0459.cc).
  //
  // break the ambiguity link (if any) in 'lastOk', so if someone
  // comes along and tchecks this again we can skip the last part
  // of the ambiguity list
  const_cast<NODE*&>(ths->ambiguity) = NULL;

  // clean up buffers.
  for (int i = 0; i < numAlts; ++i)
      delete altBuffers[i];
  // cleanup the array if necessary
  if (numAlts > NOMINAL_MAX_ALT) {
    delete[] altBuffers;
  }

  return ths;
}

// swap the order of the first two elements of the ambiguity link
// list before passing to 'resolveAmbiguity'
template <class NODE, class EXTRA>
NODE *swap_then_resolveAmbiguity(
  NODE *ths,
  Env &env,
  char const *nodeTypeName,
  bool priority,
  EXTRA &callerExtra)
{                          
  // original configuration: a b rest
  NODE *a = ths;
  NODE *b = ths->ambiguity;

  // new configuration: b a rest
  a->ambiguity = b->ambiguity;
  b->ambiguity = a;

  // run with priority
  return resolveAmbiguity(b, env, nodeTypeName, priority, callerExtra);
}

#endif // GENERIC_AMB_H
