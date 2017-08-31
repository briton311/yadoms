#include "stdafx.h"
#include "Factory.h"
#include <shared/ServiceLocator.h>
#include "startupOptions/IStartupOptions.h"
#include "common/DataProvider.h"
#include "sqlite/SQLiteRequester.h"

#ifndef PGSQL_NOT_FOUND
#  include "pgsql/PgsqlRequester.h"
#endif

namespace database
{
   boost::shared_ptr<IDataProvider> CFactory::create(const IPathProvider& pathProvider)
   {
      auto databaseEngine = createEngine(pathProvider);
      return boost::make_shared<common::CDataProvider>(databaseEngine);
   }

   boost::shared_ptr<IDatabaseRequester> CFactory::createEngine(const IPathProvider& pathProvider)
   {
      auto startupOptions = shared::CServiceLocator::instance().get<startupOptions::IStartupOptions>();

      switch (startupOptions->getDatabaseEngine())
      {
      case startupOptions::EDatabaseEngine::kSqliteValue:
         return boost::make_shared<sqlite::CSQLiteRequester>(pathProvider.databaseSqliteFile().string(), pathProvider.databaseSqliteBackupFile().string());
#ifndef PGSQL_NOT_FOUND
      case startupOptions::EDatabaseEngine::kPostgresqlValue:
         return createPqsqlRequester();
#endif
      default:
         throw CDatabaseException("Unsupported database engine");
      }
   }

   boost::shared_ptr<IDatabaseRequester> CFactory::createPqsqlRequester()
   {
      return boost::make_shared<pgsql::CPgsqlRequester>();
   }
} //namespace database 


