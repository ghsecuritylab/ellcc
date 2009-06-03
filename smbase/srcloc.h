// srcloc.h            see license.txt for copyright and terms of use
// source location information, efficiently represented as one word

// The fundamental assumption in this module is that source location
// information is frequently created, stored and passed around, but
// infrequently decoded into human-readable form.  Therefore the
// module uses a single word to store the information, and appeals
// to several index structures when decoding is necessary.
//
// Since decoding, when it happens, also usually has high locality,
// the data structures include caches to make accesses to nearby
// locations fast.
//
// No attempt is made to fold creation of SourceLocations into other
// file-processing activities, such as traditional lexical analysis.
// The complexity of doing that would be substantial, with little gain
// in efficiency, due to the large buffer caches in modern OSes.  The
// main drawback is the inability to work with non-seekable inputs
// (like pipes) because we consume the whole input when its line
// counts are computed.

// This module mostly uses 'char const *' for filenames instead of
// 'rostring' because performance is important here; the lexer is
// often calling into this module with its raw buffer pointers, and I
// don't want any allocation happening then.

#ifndef SRCLOC_H
#define SRCLOC_H

#include <limits.h>   // UCHAR_MAX

#include "str.h"      // string
#include "objlist.h"  // ObjList
#include "array.h"    // ArrayStack, ObjArrayStack

class HashLineMap;    // hashline.h


// This is a source location.  It's interpreted as an integer
// specifying the byte offset within a hypothetical file created by
// concatenating all the sources together.  Its type is 'enum' so I
// can overload functions to accept SourceLocation without confusion.
// I assume the compiler will use a machine word for this (and check
// that assumption in the .cc file).
//
// I would love to be able to annotate this so that the C++ compiler
// would not allow variables of this type to be created
// uninitialized.. that's the one drawback of calling this an 'enum'
// instead of a 'class': I don't get to write a constructor.
enum SourceLocation {
  // entity is defined within the translator's initialization code
  SL_INIT=-1,

  // location is unknown for some reason
  SL_UNKNOWN=0
};


// This class manages all the data associated with creating and
// interpreting SourceLocations.  It's expected to be a singleton in the
// program, though within this module that assumption is confined to
// the 'toString' function at the end.
class SourceLocationManager {
private:     // types
  // a triple that identifies a line boundary in a file (it's
  // implicit which file it is)
  class Marker {
  public:
    // character offset, starting with 0
    int charOffset;

    // line offset, starting with 1
    int lineOffset;

    // offset into the 'lineLengths' array; this is not simply
    // lineOffset-1 because of the possible presence of lines with
    // length longer than UCHAR_MAX-1 chars
    int arrayOffset;

  public:
    Marker() {}      // for creation in arrays
    Marker(int c, int L, int a)
      : charOffset(c), lineOffset(L), arrayOffset(a) {}
    Marker(Marker const &obj)
      : DMEMB(charOffset), DMEMB(lineOffset), DMEMB(arrayOffset) {}
  };

public:      // types

  // Holds basic data about files for use in initializing the
  // SourceLocationManager when de-serializing it from XML.
  class FileData {
    public:
    sm::string name;
    int numChars;
    int numLines;
    ArrayStack<unsigned char> *lineLengths;
    HashLineMap *hashLines;

    FileData()
      : numChars(-1)
      , numLines(-1)
      , lineLengths(NULL)
      , hashLines(NULL)
    {}
    // FIX: recursively delete the members here

    bool complete() {
      // NOTE: hashLines is nullable, so says Scott, so it is not on
      // the list of things that have to be there for the object to be
      // complete
      return !name.empty() && numChars>=0 && numLines>=0 && lineLengths;
    }
  };

  // describes a file we know about
  class File {
  public:    // data
    // file name; we consider two files to be the same if and only
    // if their names are equal, i.e. there is no checking done to
    // see if their names happen to be aliases in the filesystem
    sm::string name;

    // start offset in the SourceLocation space
    SourceLocation startLoc;

    // number of chars in the file (i.e., if you say stat(2), you get
    // this number)
    int numChars;

    // number of lines in the file; for this purpose we say that a
    // file is a sequence of lines *separated* (not terminated) by
    // newline characters; so this value is exactly one greater than
    // the number of '\n' characters in the file
    int numLines;

    // average number of chars per line; this is used for estimating
    // whether the index should be consulted for some lookups (and
    // it's stored instead of computed to save a division)
    int avgCharsPerLine;

    // known #line directives for this file; NULL if none are known
    HashLineMap *hashLines;          // (nullable owner)

  private:   // data
    // an array of line lengths; to handle lines longer than UCHAR_MAX
    // chars, we use runs of UCHAR_MAX chars to (in unary) encode
    // multiples of UCHAR_MAX chars, plus the final short count (in
    // [0,UCHAR_MAX-1]) to give the total length; "line length" does
    // not include newline characters, but it *does* include carriage
    // return characters if the file (as raw bytes) contains them
    unsigned char *lineLengths;      // (owner)

    // # of elements in 'lineLengths'
    int lineLengthsSize;

    // invariant: lineLengthSum() + numLines-1 == numChars

    // this marker and offset can name an arbitrary point in the
    // array, including those that are not at the start of a line; we
    // move this around when searching within the array
    Marker marker;
    int markerCol;      // 1-based column; it's usually 1

    // an index built on top of 'lineLengths' for faster random access
    Marker *index;                   // (owner)

    // # of elements in 'index'
    int indexSize;

    // whether there have been any hashline errors already in this input file.
    bool erroredNumLines;

  private:   // funcs
    File(File&);                     // disallowed
    void resetMarker();
    void advanceMarker();

    // decode and sum the lengths of all lines
    int lineLengthSum() const;

  public:    // funcs
    // this builds both the array and the index
    File(char const *name, SourceLocation startLoc);
    // used when de-serializing from xml
    File(FileData *fileData, SourceLocation aStartLoc);
    ~File();

    // line number to character offset
    int lineToChar(int lineNum);

    // line/col to offset, with truncation if col exceeds line length
    int lineColToChar(int lineNum, int col);

    // char offset to line/col
    void charToLineCol(int offset, int &line, int &col);

    // true if this file contains the specified location
    bool hasLoc(SourceLocation sl) const
      { return toInt(startLoc) <= sl &&
                                  sl <= toInt(startLoc) + numChars; }

    // returns -1 if the range of 'this' is less than sl, 0 if this contains
    // sl (i.e. hasLoc), and +1 if this is greater than sl.
    int cmpLoc(SourceLocation sl) const
    {
      if (toInt(startLoc) > sl) return +1;
      if (toInt(startLoc) + numChars < sl) return -1;
      return 0;
    }

    // call this time each time a #line directive is encountered;
    // same semantics as HashLineMap::addHashLine
    void addHashLine(int ppLine, int origLine, char const *origFname);
    void doneAdding();

    // check internal invariants
    void selfCheck() const;

    // dsw: the xml serialization code needs access to these two
    // fields; the idea is that the method names suggest that people
    // not use them
    unsigned char *serializationOnly_get_lineLengths() {return lineLengths;}
    int serializationOnly_get_lineLengthsSize() {return lineLengthsSize;}
  };

  // this is used for SourceLocations where the file isn't reliably
  // available, yet we'd like to be able to store some location
  // information anyway; the queries below just return the static
  // information stored, and incremental update is impossible
  class StaticLoc {
  public:
    sm::string name;      // file name
    int offset;       // char offset
    int line, col;    // line,col

  public:
    StaticLoc(char const *n, int o, int L, int c)
      : name(n), offset(o), line(L), col(c) {}
    StaticLoc(StaticLoc const &obj)
      : DMEMB(name), DMEMB(offset), DMEMB(line), DMEMB(col) {}
    ~StaticLoc();
  };

public:
  // type of SourceLocationManager::files, so that we don't have to update
  // everything when the type of this changes.
  typedef ObjArrayStack<File> FileList;

private:     // data
  // list of files; implemented using an array for fast binary search lookup
  // (used to be a linked list).
  FileList files;

  // most-recently accessed File; this is a cache
  File *recent;                      // (nullable serf)

  // list of StaticLocs; any SourceLocation less than 0 is interpreted
  // as an index into this list
  ObjList<StaticLoc> statics;

  // next source location to assign
  SourceLocation nextLoc;

  // next static (negative) location
  SourceLocation nextStaticLoc;

public:      // data
  // when true, the SourceLocationManager may go to the file system and
  // open a file in order to find out something about it.  dsw: I want
  // to turn this off when de-serializing XML for example; whatever
  // the SourceLocationManager wants to kno about a file should be in the
  // XML.
  bool mayOpenFiles;

  // number of static locations at which we print a warning message;
  // defaults to 100
  int maxStaticLocs;

  // when true, we automatically consult the #line maps when decoding;
  // defaults to true; NOTE: when this is true, encode and decode are
  // not necessarily inverses of each other
  bool useHashLines;

  // when true, open the original file and scan it so that we can
  // report true character offset counts; otherwise, just use what is
  // in the preprocessed file
  bool useOriginalOffset;

  // count the # of times we had to truncate a char offset because
  // the #line map pointed at a line shorter than the column number
  // we expected to use; this is initially 0; calling code can use
  // this to tell if the offset information across a given call or
  // sequence of calls is perfect or truncated
  static int shortLineCount;

  // whether to tolerate problems with line numbers (from hashlines) being
  // higher than the number of actual lines in files.  true means print at
  // most 1 warning (per file); false means die.
  static bool tolerateHashlineErrors;

private:     // funcs
  // let File know about these functions
  friend class SourceLocationManager::File;

  static SourceLocation toLoc(int L) {
    SourceLocation ret = (SourceLocation)L;

    // in debug mode, we verify that SourceLocation is wide enough
    // to encode this integer
    xassertdb(toInt(ret) == L);

    return ret;
  }
  static int toInt(SourceLocation loc) { return (int)loc; }

  void makeFirstStatics();

  File *findFile(char const *name);
  File *getFile(char const *name);

  File *findFileWithLoc(SourceLocation loc);
  StaticLoc const *getStatic(SourceLocation loc);

public:      // funcs
  SourceLocationManager();
  ~SourceLocationManager();

  // return to state where no files are known
  void reset();

  // origins:
  //   character offsets start at 0
  //   lines start at 1
  //   columns start at 1

  // Note that the legal source locations go from the first byte
  // in the file through to the last+1 byte.  So, if the file has
  // 5 characters, then offsets 0,1,2,3,4,5 are legal.

  // encode from scratch
  SourceLocation encodeOffset(char const *filename, int charOffset);
  SourceLocation encodeBegin(char const *filename)
    { return encodeOffset(filename, 0 /*offset*/); }
  SourceLocation encodeLineCol(char const *filename, int line, int col);

  // some care is required with 'encodeStatic', since each call makes
  // a new location with a new entry in the static array to back it
  // up, so the caller should ensure a given static location is not
  // encoded more than once, if possible
  SourceLocation encodeStatic(StaticLoc const &obj);
  SourceLocation encodeStatic(char const *fname, int offset, int line, int col)
    { return encodeStatic(StaticLoc(fname, offset, line, col)); }
  static bool isStatic(SourceLocation loc) { return toInt(loc) <= 0; }

  // encode incremental; these are the methods we expect are called
  // the most frequently; this interface is supposed to allow an
  // implementation which uses explicit line/col, even though that
  // is not what is used here
  static SourceLocation advCol(SourceLocation base, int colOffset)
    { xassert(!isStatic(base)); return toLoc(toInt(base) + colOffset); }
  static SourceLocation advLine(SourceLocation base)     // from end of line to beginning of next
    { xassert(!isStatic(base)); return toLoc(toInt(base) + 1); }
  static SourceLocation advText(SourceLocation base, char const * /*text*/, int textLen)
    { xassert(!isStatic(base)); return toLoc(toInt(base) + textLen); }

  // decode
  void decodeOffset(SourceLocation loc, char const *&filename, int &charOffset);
  void decodeLineCol(SourceLocation loc, char const *&filename, int &line, int &col);

  // more specialized decode
  char const *getFile(SourceLocation loc) { return getFile(loc, this->useHashLines); }
  char const *getFile(SourceLocation loc, bool localUseHashLines);
  int getOffset(SourceLocation loc);
  int getOffset_nohashline(SourceLocation loc);
  int getLine(SourceLocation loc);
  int getCol(SourceLocation loc);

  // get access to the File itself, for adding #line directives
  File *getInternalFile(char const *fname)
    { return getFile(fname); }

  // render as string in "file:line:col" format
  sm::string getString(SourceLocation loc);

  // versions of the decode routine that either use or do not use the
  // hashline map (when available) depending on an explicit flag,
  // rather than using this->useHashLines
  void decodeOffset_explicitHL(SourceLocation loc, char const *&filename, int &charOffset, bool localUseHashLines);
  void decodeOffset_nohashline(SourceLocation loc, char const *&filename, int &charOffset)
    { decodeOffset_explicitHL(loc, filename, charOffset, false); }
  void decodeLineCol_explicitHL(SourceLocation loc, char const *&filename, int &line, int &col, bool localUseHashLines);
  void decodeLineCol_nohashline(SourceLocation loc, char const *&filename, int &line, int &col)
    { decodeLineCol_explicitHL(loc, filename, line, col, false); }
  sm::string getString_explicitHL(SourceLocation loc, bool localUseHashLines);
  sm::string getString_nohashline(SourceLocation loc)
    { return getString_explicitHL(loc, false); }

  // "line:col" format
  sm::string getLCString(SourceLocation loc);

  // dsw: the xml serialization code needs access to this field; the
  // idea is that the method name suggests that people not use it
  FileList &serializationOnly_get_files() {return files;}
  // for de-serializing from xml a single File and loading it into the SourceLocationManager
  void loadFile(FileData *fileData);
  // has this file been loaded?
  bool isLoaded(char const *name) { return findFile(name); }
};


// singleton pointer, set automatically by the constructor
extern SourceLocationManager *sourceLocManager;

// dsw: So that gdb can find it please DO NOT inline this; also the
// unique public name is intentional: I don't want gdb doing
// overloading and sometimes getting it wrong, which it does
sm::string locToStr(SourceLocation sl);

inline sm::string toString(SourceLocation sl)
  { return locToStr(sl); }

inline stringBuilder& operator<< (stringBuilder &sb, SourceLocation sl)
  { return sb << toString(sl); }

inline sm::string toLCString(SourceLocation sl)
  { return sourceLocManager->getLCString(sl); }


// macro for obtaining a source location that points at the
// point in the source code where this macro is invoked
#define HERE_SOURCELOC \
  (sourceLocManager->encodeStatic(__FILE__, 0, __LINE__, 1))


// it's silly to demand mention of 'SourceLocationManager' just to update
// the locations, esp. since SourceLocation is its own type and therefore
// overloading will avoid any possible collisions
inline SourceLocation advCol(SourceLocation base, int colOffset)
  { return SourceLocationManager::advCol(base, colOffset); }
inline SourceLocation advLine(SourceLocation base)
  { return SourceLocationManager::advLine(base); }
inline SourceLocation advText(SourceLocation base, char const *text, int textLen)
  { return SourceLocationManager::advText(base, text, textLen); }

//  string toXml(SourceLocation index);
//  void fromXml(SourceLocation &out, string str);


#endif // SRCLOC_H
