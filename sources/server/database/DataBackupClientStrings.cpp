#include "stdafx.h"
#include "DataBackupClientStrings.h"
#include "i18n/ClientStrings.h"

namespace database
{
   CDataBackupClientStrings::CDataBackupClientStrings()
   {
   }

   CDataBackupClientStrings::~CDataBackupClientStrings()
   {
   }

   std::string CDataBackupClientStrings::getBackupInProgress() const
   {
      return i18n::CClientStrings::DatabaseBackupInProgress;
   }

   std::string CDataBackupClientStrings::getBackupSuccess() const
   {
      return i18n::CClientStrings::DatabaseBackupSuccess;
   }

   std::string CDataBackupClientStrings::getBackupFailed() const
   {
      return i18n::CClientStrings::DatabaseBackupFail;
   }
} //namespace database


