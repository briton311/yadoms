#include "stdafx.h"

#include "FileSystemExtension.h"
#include "Windows.h"
#include <boost/filesystem.hpp>

namespace shared
{

   std::string CFileSystemExtension::getModuleFullPath()
   {
      std::vector<char> modulePath(MAX_PATH);

      HMODULE hModule = NULL;
      GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCTSTR)getModulePath, &hModule);
      if (hModule == NULL)
      {
         DWORD error = GetLastError();
         throw shared::exception::CException(std::string("CFileSystemExtension::getModuleFullPath fails with error ")
            + boost::lexical_cast<std::string>(error));
      }

      /* Try to get the executable path with a buffer of MAX_PATH characters. */
      DWORD result = GetModuleFileNameA(hModule, &modulePath[0], static_cast<DWORD>(modulePath.size()));

      /* As long the function returns the buffer size, it is indicating that the buffer */
      /* was too small. Keep enlarging the buffer by a factor of 2 until it fits. */
      while (result == modulePath.size())
      {
         modulePath.resize(modulePath.size() * 2);
         result = GetModuleFileNameA(hModule, &modulePath[0], static_cast<DWORD>(modulePath.size()));
      }

      /* If the function returned 0, something went wrong */
      if (result == 0)
      {
         DWORD error = GetLastError();
         throw shared::exception::CException(std::string("CFileSystemExtension::getModuleFullPath fails with error ")
            + boost::lexical_cast<std::string>(error));
      }

      /* We've got the path, construct a path from it */
      return boost::filesystem::path(std::string(modulePath.begin(), modulePath.begin() + result)).string();
   }

   std::string CFileSystemExtension::getModulePath()
   {
      return boost::filesystem::path(getModuleFullPath()).parent_path().string();
   }

   std::string CFileSystemExtension::getFileRelativeToModulePath(const std::string & file)
   {
      return (boost::filesystem::path(getModuleFullPath()).parent_path() / boost::filesystem::path(file)).string();
   }
} // namespace shared