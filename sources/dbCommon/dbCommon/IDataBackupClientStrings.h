#pragma once


namespace dbCommon
{
   //---------------------------------
   ///\brief Interface to strings ID to user interface when backuping
   //---------------------------------
   class IDataBackupClientStrings
   {
   public:
      //---------------------------------
      ///\brief Get string ID for each operation
      ///\return Concerned string ID
      //---------------------------------
      virtual std::string getBackupInProgress() const = 0;
      virtual std::string getBackupSuccess() const = 0;
      virtual std::string getBackupFailed() const = 0;

      //--------------------------------------------------------------
      /// \brief       Destructor
      //--------------------------------------------------------------
      virtual ~IDataBackupClientStrings()
      {
      }
   };
} //namespace dbCommon 


