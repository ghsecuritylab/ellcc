/*=========================================================================

  Program:   CMake - Cross-Platform Makefile Generator
  Module:    $RCSfile: cmCPackDragNDropGenerator.h,v $
  Language:  C++
  Date:      $Date: 2009-02-05 03:04:18 $
  Version:   $Revision: 1.1.2.2 $

  Copyright (c) 2002 Kitware, Inc. All rights reserved.
  See Copyright.txt or http://www.cmake.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef cmCPackDragNDropGenerator_h
#define cmCPackDragNDropGenerator_h

#include "cmCPackGenerator.h"

/** \class cmCPackDragNDropGenerator
 * \brief A generator for OSX drag-n-drop installs
 */
class cmCPackDragNDropGenerator : public cmCPackGenerator
{
public:
  cmCPackTypeMacro(cmCPackDragNDropGenerator, cmCPackGenerator);

  cmCPackDragNDropGenerator();
  virtual ~cmCPackDragNDropGenerator();

protected:
  virtual int InitializeInternal();
  virtual const char* GetOutputExtension();
  int CompressFiles(const char* outFileName, const char* toplevel,
    const std::vector<std::string>& files);

  bool CopyFile(cmOStringStream& source, cmOStringStream& target);
  bool RunCommand(cmOStringStream& command, std::string* output = 0);

  std::string InstallPrefix;
};

#endif

