#include "stdafx.h"
#include "SQLiteOptions.h"


namespace database
{
   namespace sqlite
   {
      CSQLiteOptions::CSQLiteOptions(boost::shared_ptr<const shared::CDataContainer> options)
         : m_options(options)
      {
      }

      CSQLiteOptions::~CSQLiteOptions()
      {
      }

      std::string CSQLiteOptions::getDatabaseFile() const
      {
         return m_options->getWithDefault<std::string>("databasePath", "yadoms.db3");
      }

      std::string CSQLiteOptions::getDatabaseBackupFile() const
      {
         return m_options->getWithDefault<std::string>("databaseBackupPath", "yadoms_backup.db3");
      }
   } //namespace sqlite
} //namespace database 


