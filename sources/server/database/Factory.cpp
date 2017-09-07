#include "stdafx.h"
#include "Factory.h"
#include "startupOptions/IStartupOptions.h"
#include "common/DataProvider.h"
#include "sqlite/SQLiteRequester.h"
#include <shared/Log.h>
#include <dbCommon/DatabaseException.hpp>

#ifndef PGSQL_NOT_FOUND
#  include <boost/dll.hpp>
#endif

namespace database
{
   boost::shared_ptr<IDataProvider> CFactory::create(const boost::shared_ptr<startupOptions::IStartupOptions> startupOptions)
   {
      auto databaseEngine = createEngine(startupOptions);
      return boost::make_shared<common::CDataProvider>(databaseEngine);
   }

   boost::shared_ptr<dbCommon::IDatabaseRequester> CFactory::createEngine(const boost::shared_ptr<startupOptions::IStartupOptions> startupOptions)
   {
      switch (startupOptions->getDatabaseEngine())
      {
      case startupOptions::EDatabaseEngine::kSqliteValue:
         return createSQLiteRequester(startupOptions);
#ifndef PGSQL_NOT_FOUND
      case startupOptions::EDatabaseEngine::kPostgresqlValue:
         return createPqsqlRequester(startupOptions);
#endif
      default:
         throw dbCommon::CDatabaseException("Unsupported database engine");
      }
   }

   boost::shared_ptr<dbCommon::IDatabaseRequester> CFactory::createSQLiteRequester(const boost::shared_ptr<startupOptions::IStartupOptions> startupOptions)
   {
      try
      {
         auto databaseRequester = boost::make_shared<sqlite::CSQLiteRequester>();

         databaseRequester->setOptions(buildSQLiteOptions(startupOptions));

         return databaseRequester;
      }
      catch (std::exception& e)
      {
         YADOMS_LOG(error) << "Unable to load lib-pgsql-adapter library, " << e.what();
         throw;
      }
   }

   boost::shared_ptr<const shared::CDataContainer> CFactory::buildSQLiteOptions(const boost::shared_ptr<startupOptions::IStartupOptions> startupOptions)
   {
      auto options = boost::make_shared<shared::CDataContainer>();

      options->set("databasePath", startupOptions->getDatabaseSqliteFile());
      options->set("databaseBackupPath", startupOptions->getDatabaseSqliteBackupFile());

      return options;
   }

   boost::shared_ptr<dbCommon::IDatabaseRequester> CFactory::createPqsqlRequester(const boost::shared_ptr<startupOptions::IStartupOptions> startupOptions)
   {
      try
      {
         auto databaseRequester = boost::dll::import<dbCommon::IDatabaseRequester>("lib-pgsql-adapter",
                                                                                   "PgsqlRequester",
                                                                                   boost::dll::load_mode::append_decorations);

         databaseRequester->setOptions(buildPqsqlOptions(startupOptions));

         return databaseRequester;
      }
      catch (std::exception& e)
      {
         YADOMS_LOG(error) << "Unable to load lib-pgsql-adapter library, " << e.what();
         throw;
      }
   }

   boost::shared_ptr<const shared::CDataContainer> CFactory::buildPqsqlOptions(const boost::shared_ptr<startupOptions::IStartupOptions> startupOptions)
   {
      auto options = boost::make_shared<shared::CDataContainer>();

      options->set("host", startupOptions->getDatabasePostgresqlHost());
      options->set("port", startupOptions->getDatabasePostgresqlPort());
      options->set("dbname", startupOptions->getDatabasePostgresqlDbName());
      options->set("login", startupOptions->getDatabasePostgresqlLogin());
      options->set("password", startupOptions->getDatabasePostgresqlPassword());
      options->set("hostaddr", startupOptions->getDatabasePostgresqlHostAddr());
      options->set("connect-timeout", startupOptions->getDatabasePostgresqlConnectTimeout());
      options->set("client-encoding", startupOptions->getDatabasePostgresqlClientEncoding());
      options->set("options", startupOptions->getDatabasePostgresqlOptions());
      options->set("keep-alives", startupOptions->getDatabasePostgresqlKeepAlives());
      options->set("keep-alives-idle", startupOptions->getDatabasePostgresqlKeepAlivesIdle());
      options->set("keep-alives-interval", startupOptions->getDatabasePostgresqlKeepAlivesInterval());
      options->set("keep-alives-count", startupOptions->getDatabasePostgresqlKeepAlivesCount());
      options->set("ssl-mode", startupOptions->getDatabasePostgresqlSslMode());
      options->set("require-ssl", startupOptions->getDatabasePostgresqlRequireSsl());
      options->set("ssl-compression", startupOptions->getDatabasePostgresqlSslCompression());
      options->set("ssl-cert", startupOptions->getDatabasePostgresqlSslCert());
      options->set("ssl-key", startupOptions->getDatabasePostgresqlSslKey());
      options->set("ssl-root", startupOptions->getDatabasePostgresqlSslRootCert());
      options->set("ssl-revocation", startupOptions->getDatabasePostgresqlSslRevocationList());
      options->set("require-peer", startupOptions->getDatabasePostgresqlRequirePeer());
      options->set("kerberos", startupOptions->getDatabasePostgresqlKerberos());
      options->set("gss-lib", startupOptions->getDatabasePostgresqlGssLib());
      options->set("service", startupOptions->getDatabasePostgresqlService());

      return options;
   }
} //namespace database 


