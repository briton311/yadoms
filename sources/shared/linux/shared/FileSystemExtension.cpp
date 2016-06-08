#include "stdafx.h"

#include "FileSystemExtension.h"
#include <boost/filesystem.hpp>
#include <shared/exception/Exception.hpp>
#include <dlfcn.h>

namespace shared
{

      //--------------------------------------------------------------
      /// \brief			Get the current module full path (drive + path + filename + ext).
      ///               This code must be executed in the module which the path is required,
      ///               so let it in the header file.
      //--------------------------------------------------------------
      std::string CFileSystemExtension::getModuleFullPath()
      { 
         Dl_info dl_info; 
         if (dladdr((void *)getModulePath, &dl_info) == 0) 
         { 
            const std::string error(strerror(errno));
            throw shared::exception::CException(std::string("CFileSystemExtension::getModuleFullPath fails with error ") + error);
         } 
         
         /* We've got the path, construct a path from it */
         return boost::filesystem::path(dl_info.dli_fname).string();
      }

      //--------------------------------------------------------------
      /// \brief			Get the current module path.
      ///               This code must be executed in the module which the path is required,
      ///               so let it in the header file.
      //--------------------------------------------------------------
      std::string CFileSystemExtension::getModulePath()
      {
         return boost::filesystem::path(getModuleFullPath()).parent_path().string();                                                                 
      }


} // namespace shared