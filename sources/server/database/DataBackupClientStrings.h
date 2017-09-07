#pragma once

#include <dbCommon/IDataBackupClientStrings.h>

namespace database
{
      //--------------------------------------------------------------
      /// \Brief		   Database provider
      //--------------------------------------------------------------
      class CDataBackupClientStrings : public dbCommon::IDataBackupClientStrings
      {
      public:
         //--------------------------------------------------------------
         /// \Brief		   Constructor
         //--------------------------------------------------------------
         CDataBackupClientStrings();

         //--------------------------------------------------------------
         /// \Brief		   Destructor
         //--------------------------------------------------------------
         virtual ~CDataBackupClientStrings();

         // dbCommon::IDataBackupClientStrings implementation
         std::string getBackupInProgress() const override;
         std::string getBackupSuccess() const override;
         std::string getBackupFailed() const override;
         // [END] dbCommon::IDataBackupClientStrings implementation
      };
} //namespace database


