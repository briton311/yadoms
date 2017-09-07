#pragma once
#include <shared/DataContainer.h>


namespace libSQLiteAdapter
{
   class CSQLiteOptions
   {
   public:
      explicit CSQLiteOptions(boost::shared_ptr<const shared::CDataContainer> options);
      virtual ~CSQLiteOptions();

      std::string getDatabaseFile() const;
      std::string getDatabaseBackupFile() const;

   private:
      const boost::shared_ptr<const shared::CDataContainer> m_options;
   };
} //namespace libSQLiteAdapter 


