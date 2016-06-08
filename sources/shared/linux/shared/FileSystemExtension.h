//
// FileSystemExtension.h
//
// File system extensions
//
#pragma once



namespace shared
{

   //--------------------------------------------------------------
   /// \brief	this class is used to manage platforms peripherals
   //--------------------------------------------------------------
   class CFileSystemExtension
   {
   public:
      //--------------------------------------------------------------
      /// \brief			Get the current module full path (drive + path + filename + ext).
      ///               This code must be executed in the module which the path is required,
      ///               so let it in the header file.
      //--------------------------------------------------------------
      static std::string getModuleFullPath();

      //--------------------------------------------------------------
      /// \brief			Get the current module path  (drive + path, without filename and ext).
      ///               This code must be executed in the module which the path is required,
      ///               so let it in the header file.
      //--------------------------------------------------------------
      static std::string getModulePath();
      
      //--------------------------------------------------------------
      /// \brief			Get the file path relative to current module path
      //--------------------------------------------------------------
      static std::string getFileRelativeToModulePath(const std::string & file);
   };

} // namespace shared