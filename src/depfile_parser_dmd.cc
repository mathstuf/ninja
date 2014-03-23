/* Generated by re2c 0.13.5 */
// Copyright 2011 Google Inc. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "depfile_parser_dmd.h"

#include <set>

static int ParseFilename(char*& in, char* end) {
  // out: current output point (typically same as in, but can fall behind as we
  // de-escape backslashes).
  char* out = in;
  // filename: start of the current parsed filename.
  char* filename = out;
  for (;;) {
    // start: beginning of the current parsed span.
    const char* start = in;
    
    {
      char yych;
      static const unsigned char yybm[] = {
          0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0, 
          0, 128,   0, 128,   0,   0,   0,   0, 
          0,   0, 128, 128, 128, 128, 128, 128, 
        128, 128, 128, 128, 128, 128, 128, 128, 
        128, 128, 128,   0,   0, 128,   0,   0, 
        128, 128, 128, 128, 128, 128, 128, 128, 
        128, 128, 128, 128, 128, 128, 128, 128, 
        128, 128, 128, 128, 128, 128, 128, 128, 
        128, 128, 128, 128,   0,   0,   0, 128, 
          0, 128, 128, 128, 128, 128, 128, 128, 
        128, 128, 128, 128, 128, 128, 128, 128, 
        128, 128, 128, 128, 128, 128, 128, 128, 
        128, 128, 128,   0, 128,   0, 128,   0, 
          0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0, 
          0,   0,   0,   0,   0,   0,   0,   0, 
      };

      yych = *in;
      if (yybm[0+yych] & 128) {
        goto yy4;
      }
      if (yych == ')') goto yy7;
      if (yych == '\\') goto yy3;
yy2:
yy3:
      yych = *++in;
      if (yych <= '\'') {
        if (yych <= 0x00) goto yy2;
        if (yych == '\n') goto yy2;
        goto yy9;
      } else {
        if (yych <= ')') goto yy11;
        if (yych == '\\') goto yy11;
        goto yy9;
      }
yy4:
      ++in;
      yych = *in;
      if (yybm[0+yych] & 128) {
        goto yy4;
      }
      {
      // Got a span of plain text.
      int len = (int)(in - start);
      // Need to shift it over if we're overwriting backslashes.
      if (out < start)
            memmove(out, start, len);
      out += len;
      continue;
    }
yy7:
      ++in;
      {
      break;
    }
yy9:
      ++in;
      {
      // Let backslash before other characters through verbatim.
      *out++ = '\\';
      *out++ = yych;
      continue;
        }
yy11:
      ++in;
      {
      // De-escape backslashed character.
      *out++ = yych;
      continue;
    }
    }

  }

  return out - filename;
}

bool DepfileParserDMD::Parse(string* content, string* err) {
  // in: current parser input point.
  // end: end of input.
  // uniq_ins: uniquify the input list.
  char* in = &(*content)[0];
  char* end = in + content->size();
  std::set<std::string> uniq_ins;
  // From the ldc code at https://github.com/ldc-developers/ldc/blob/master/dmd2/import.c:
  //
  // The grammar of the file is:
  //   ImportDeclaration
  //   ::= BasicImportDeclaration [ " : " ImportBindList ] [ " -> "
  //   ModuleAliasIdentifier ] "\n"
  //
  //   BasicImportDeclaration
  //   ::= ModuleFullyQualifiedName " (" FilePath ") : " Protection
  //   " [ " static" ] : " ModuleFullyQualifiedName " (" FilePath ")"
  //
  //   FilePath
  //   - any string with '(', ')' and '\' escaped with the '\' character

  while (in < end) {
    // Skip to the first open parenthesis.
    in = strchr(in, '(');
    if (!in) {
      return false;
    }
    ++in;
    // Parse the output file.
    char* start = in;
    int len = ParseFilename(in, end);
    // Store it as the output file if it's the first one.
    if (!out_.str_) {
      out_ = StringPiece(start, len);
    }
    // Skip to the next open parenthesis.
    in = strchr(in, '(');
    if (!in) {
      return false;
    }
    ++in;
    // Parse the input file.
    start = in;
    len = ParseFilename(in, end);
    StringPiece dependee(start, len);
    // Skip to the end of the line.
    in = strchr(in, '\n');
    ++in;

    if (!uniq_ins.count(dependee.AsString())) {
      ins_.push_back(dependee);
      uniq_ins.insert(dependee.AsString());
    }
  }

  return true;
}
