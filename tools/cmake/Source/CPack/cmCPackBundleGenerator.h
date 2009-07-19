/*=========================================================================

  Program:   CMake - Cross-Platform Makefile Generator
  Module:    $RCSfile: cmCPackBundleGenerator.h,v $
  Language:  C++
  Date:      $Date: 2009-02-04 16:44:18 $
  Version:   $Revision: 1.1.2.3 $

  Copyright (c) 2002 Kitware, Inc. All rights reserved.
  See Copyright.txt or http://www.cmake.org/HTML/Copyright.html for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef cmCPackBundleGenerator_h
#define cmCPackBundleGenerator_h

#include "cmCPackDragNDropGenerator.h"

/** \class cmCPackBundleGenerator
 * \brief A generator for OSX bundles
 *
 * Based on Gimp.app
 */
class cmCPackBundleGenerator : public cmCPackDragNDropGenerator
{
public:
  cmCPackTypeMacro(cmCPackBundleGenerator, cmCPackDragNDropGenerator);

  cmCPackBundleGenerator();
  virtual ~cmCPackBundleGenerator();

protected:
  virtual int InitializeInternal();
  virtual const char* GetPackagingInstallPrefix();
  int CompressFiles(const char* outFileName, const char* toplevel,
    const std::vector<std::string>& files);

  std::string InstallPrefix;
};

#endif
