/*
 * Copyright (C) 2012-2016. TomTom International BV (http://tomtom.com).
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "Configuration.h"
#include "Constants.h"
#include "FstreamInclude.h"
#include <iostream>
#include <stdlib.h>

static inline std::string Trim(const std::string &s) {
    size_t first = s.find_first_not_of(" \t\r\n"), last = s.find_last_not_of(" \t\r\n");
    return s.substr(first, last - first + 1);
}

void ReadSet(std::unordered_set<std::string>& set,
             std::istream& in)
{
  std::string line;
  while (in.good()) {
    std::getline(in, line);
    size_t pos = line.find_first_of("]");
    if (pos != std::string::npos) {
      return;
    }
    set.insert(Trim(line));
  }
}

std::string ReadMultilineString(std::istream& in)
{
  std::string multiline;
  std::string line;
  while (in.good()) {
    std::getline(in, line);
    size_t pos = line.find_first_of("\"\"\"");
    if (pos != std::string::npos) {
      break;
    }
    multiline += line;
    multiline += "\n";
  }
  return multiline;
}

Configuration::Configuration()
: companyName("YourCompany")
, licenseString("")
, regenTag("GENERATED BY CPP-DEPENDENCIES")
, cmakeVersion("3.0")
, versionUsed(CURRENT_VERSION)
, cycleColor("orange")
, publicDepColor("blue")
, privateDepColor("lightblue")
, componentLinkLimit(30)
, componentLocLowerLimit(200)
, componentLocUpperLimit(20000)
, fileLocUpperLimit(2000)
, reuseCustomSections(false)
{
  addLibraryAliases.insert("add_library");
  addExecutableAliases.insert("add_executable");
}

void Configuration::read(std::istream& in)
{
  std::string line;
  while (in.good()) {
    std::getline(in, line);
    while (in.good() && line.back() == '\\') {
      std::string nextLine;
      std::getline(in, nextLine);
      line += nextLine;
    }

    size_t pos = line.find_first_of("#");
    if (pos != std::string::npos)
      line.resize(line.find_first_of("#"));

    pos = line.find(": ");
    if (pos == std::string::npos)
      continue;

    std::string name = line.substr(0, pos);
    std::string value = line.substr(pos+2);
    if (name == "cycleColor") { cycleColor = value; }
    else if (name == "publicDepColor") { publicDepColor = value; }
    else if (name == "versionUsed") { versionUsed = value; }
    else if (name == "privateDepColor") { privateDepColor = value; }
    else if (name == "regenTag") { regenTag = value; }
    else if (name == "cmakeVersion") { cmakeVersion = value; }
    else if (name == "companyName") { companyName = value; }
    else if (name == "componentLinkLimit") { componentLinkLimit = atol(value.c_str()); }
    else if (name == "componentLocLowerLimit") { componentLocLowerLimit = atol(value.c_str()); }
    else if (name == "componentLocUpperLimit") { componentLocUpperLimit = atol(value.c_str()); }
    else if (name == "fileLocUpperLimit") { fileLocUpperLimit = atol(value.c_str()); }
    else if (name == "addLibraryAlias") { ReadSet(addLibraryAliases, in); }
    else if (name == "addExecutableAlias") { ReadSet(addExecutableAliases, in); }
    else if (name == "addIgnores") { ReadSet(addIgnores, in); }
    else if (name == "licenseString") { licenseString = ReadMultilineString(in); }
    else if (name == "reuseCustomSections") { reuseCustomSections = (value == "true"); }
    else if (name == "blacklist") { ReadSet(blacklist, in); }
    else {
      std::cout << "Ignoring unknown tag in configuration file: " << name << "\n";
    }
  }
}


