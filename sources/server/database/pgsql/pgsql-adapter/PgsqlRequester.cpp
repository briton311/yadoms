#include "stdafx.h"

#include "PgsqlRequester.h"
#include "database/common/adapters/GenericAdapter.h"
#include "database/common/adapters/SingleValueAdapter.hpp"
#include "PgsqlSystemTables.h"
#include "database/DatabaseException.hpp"
#include "PgsqlResultHandler.h"
#include "PgsqlQuery.h"
#include "PgsqlTableCreationScriptProvider.h"
#include "PgsqlSqlState.h"
#include <shared/ServiceLocator.h>
#include "startupOptions/IStartupOptions.h"
#include <shared/Log.h>

namespace libPgsqlAdapter
{
   CPgsqlRequester::CPgsqlRequester()
   {
   }

   CPgsqlRequester::~CPgsqlRequester()
   {
   }

   PGconn* CPgsqlRequester::createNewConnection()
   {
      //connect to postgresql engine
      auto result = PQconnectdb(createConnectionString().c_str());

      //Check to see that the backend connection was successfully made
      if (PQstatus(result) != CONNECTION_OK)
      {
         //save the error message
         auto error = getLastErrorMessage(result);

         //clear connection
         PQfinish(result);

         throw database::CDatabaseException("Fail to connect database", error);
      }
      return result;
   }

   PGconn* CPgsqlRequester::getConnection()
   {
      try
      {
         //obtain thread id
         auto threadId = boost::lexical_cast<std::string>(boost::this_thread::get_id());
         unsigned long threadNumber = 0;
         sscanf(threadId.c_str(), "%lx", &threadNumber);

         //if connection do not exists for this thread, then create a new one
         if (m_connectionList.find(threadNumber) == m_connectionList.end())
            m_connectionList[threadNumber] = createNewConnection();

         //return it
         return m_connectionList[threadNumber];
      }
      catch (std::exception& ex)
      {
         YADOMS_LOG(error) << "Fail to get connection :" << ex.what();
         throw;
      }
      catch (...)
      {
         YADOMS_LOG(error) << "Fail to get connection : unknown exception";
         throw;
      }
   }

   void CPgsqlRequester::terminateConnection(PGconn* pConnection)
   {
      if (pConnection != nullptr)
         PQfinish(pConnection);
   }

   std::string CPgsqlRequester::getLastErrorMessage(PGconn* pConnection)
   {
      return std::string(PQerrorMessage(pConnection));
   }


   std::string CPgsqlRequester::createConnectionString(const EConnectionStringMode mode)
   {
      auto startupOptions = shared::CServiceLocator::instance().get<startupOptions::IStartupOptions>();

      std::string result;

      switch (mode)
      {
      case kPing:
         result = (boost::format("host=%1% port=%2% application_name=Yadoms")
            % startupOptions->getDatabasePostgresqlHost()
            % startupOptions->getDatabasePostgresqlPort()).str();
         break;
      case kMasterDb:
         result = (boost::format("host=%1% port=%2% user=%3% password=%4% dbname=postgres application_name=Yadoms")
            % startupOptions->getDatabasePostgresqlHost()
            % startupOptions->getDatabasePostgresqlPort()
            % startupOptions->getDatabasePostgresqlLogin()
            % startupOptions->getDatabasePostgresqlPassword()).str();
         break;
      case kNormal:
      default:
         result = (boost::format("host=%1% port=%2% user=%3% password=%4% dbname=%5% application_name=Yadoms")
            % startupOptions->getDatabasePostgresqlHost()
            % startupOptions->getDatabasePostgresqlPort()
            % startupOptions->getDatabasePostgresqlLogin()
            % startupOptions->getDatabasePostgresqlPassword()
            % startupOptions->getDatabasePostgresqlDbName()).str();
         break;
      }

      if (mode != kPing)
      {
         //append all optional parameters
         connectionStringAppendOptionalParameter(result, startupOptions->getDatabasePostgresqlHostAddr(), "hostaddr");
         connectionStringAppendOptionalParameter(result, startupOptions->getDatabasePostgresqlConnectTimeout(), "connect_timeout");
         connectionStringAppendOptionalParameter(result, startupOptions->getDatabasePostgresqlClientEncoding(), "client_encoding");
         connectionStringAppendOptionalParameter(result, startupOptions->getDatabasePostgresqlOptions(), "options");
         connectionStringAppendOptionalParameter(result, startupOptions->getDatabasePostgresqlKeepAlives(), "keepalives");
         connectionStringAppendOptionalParameter(result, startupOptions->getDatabasePostgresqlKeepAlivesIdle(), "keepalives_idle");
         connectionStringAppendOptionalParameter(result, startupOptions->getDatabasePostgresqlKeepAlivesInterval(), "keepalives_interval");
         connectionStringAppendOptionalParameter(result, startupOptions->getDatabasePostgresqlKeepAlivesCount(), "keepalives_count");
         connectionStringAppendOptionalParameter(result, startupOptions->getDatabasePostgresqlSslMode(), "sslmode");
         connectionStringAppendOptionalParameter(result, startupOptions->getDatabasePostgresqlRequireSsl(), "requiressl");
         connectionStringAppendOptionalParameter(result, startupOptions->getDatabasePostgresqlSslCompression(), "sslcompression");
         connectionStringAppendOptionalParameter(result, startupOptions->getDatabasePostgresqlSslCert(), "sslcert");
         connectionStringAppendOptionalParameter(result, startupOptions->getDatabasePostgresqlSslKey(), "sslkey");
         connectionStringAppendOptionalParameter(result, startupOptions->getDatabasePostgresqlSslRootCert(), "sslrootcert");
         connectionStringAppendOptionalParameter(result, startupOptions->getDatabasePostgresqlSslRevocationList(), "sslcrl");
         connectionStringAppendOptionalParameter(result, startupOptions->getDatabasePostgresqlRequirePeer(), "requirepeer");
         connectionStringAppendOptionalParameter(result, startupOptions->getDatabasePostgresqlKerberos(), "krbsrvname");
         connectionStringAppendOptionalParameter(result, startupOptions->getDatabasePostgresqlGssLib(), "gsslib");
         connectionStringAppendOptionalParameter(result, startupOptions->getDatabasePostgresqlService(), "service");
      }
      return result;
   }

   void CPgsqlRequester::pingServer()
   {
      //fails to connect database
      auto serverStatus = PQping(createConnectionString(kPing).c_str());
      switch (serverStatus)
      {
      case PQPING_REJECT:
         throw database::CDatabaseException("Fail to connect database : REJECT : The server is running but is in a state that disallows connections (startup, shutdown, or crash recovery) ");
      case PQPING_NO_RESPONSE:
         throw database::CDatabaseException("Fail to connect database : NO_RESPONSE : The server could not be contacted. This might indicate that the server is not running, or that there is something wrong with the given connection parameters (for example, wrong port number), or that there is a network connectivity problem (for example, a firewall blocking the connection request) ");
      case PQPING_NO_ATTEMPT:
         throw database::CDatabaseException("Fail to connect database : PQPING_NO_ATTEMPT : No attempt was made to contact the server, because the supplied parameters were obviously incorrect or there was some client-side problem (for example, out of memory) ");
      default:
         //ping success
         break;
      }
   }

   // IDatabaseEngine implementation
   database::common::CQuery CPgsqlRequester::newQuery()
   {
      return CPgsqlQuery();
   }

   void CPgsqlRequester::initialize()
   {
      YADOMS_LOG(information) << "Connect to PostgreSQL database";
      try
      {
         //connect to postgresql engine
         auto pConnection = PQconnectdb(createConnectionString().c_str());

         //Check to see that the backend connection was successfully made 
         if (PQstatus(pConnection) != CONNECTION_OK)
         {
            //save the error message
            auto firstError = getLastErrorMessage(pConnection);

            //clear connection
            PQfinish(pConnection);

            YADOMS_LOG(information) << "Fail to connect the PostgreSQL database.";

            //ping the server (throws if ping failed)
            YADOMS_LOG(information) << "Check server availability";
            pingServer();

            //retry with "postgres" database to be able to list db
            YADOMS_LOG(information) << "PostgreSQL Server is available. Checking database existance";
            pConnection = PQconnectdb(createConnectionString(kMasterDb).c_str());
            if (PQstatus(pConnection) != CONNECTION_OK)
            {
               throw database::CDatabaseException("Fail to connect PostgreSQL database", getLastErrorMessage(pConnection));
            }

            auto startupOptions = shared::CServiceLocator::instance().get<startupOptions::IStartupOptions>();

            //list all databases
            CPgsqlQuery dbList;

            dbList.SelectCount().From(CPgDatabaseTable::getTableName()).Where(CPgDatabaseTable::getDatabaseNameColumnName(), CQUERY_OP_ILIKE, startupOptions->getDatabasePostgresqlDbName());
            auto count = queryCount(dbList, pConnection);
            if (count == 0)
            {
               YADOMS_LOG(information) << "PostgreSQL Database do not exists, try to create it";
               //create database
               auto result = queryStatement(CPgsqlQuery().CreateDatabase(startupOptions->getDatabasePostgresqlDbName()), true, pConnection);
               if (result == 0)
               {
                  YADOMS_LOG(information) << "PostgreSQL Database created";

                  //terminate master connection
                  PQfinish(pConnection);

                  //retry connection to database
                  pConnection = PQconnectdb(createConnectionString().c_str());

                  //Check to see that the backend connection was successfully made 
                  if (PQstatus(pConnection) != CONNECTION_OK)
                     throw database::CDatabaseException("Fail to connect the newly created PostgreSQL database.", getLastErrorMessage(pConnection));

                  //terminate master connection
                  PQfinish(pConnection);
               }
               else
               {
                  throw database::CDatabaseException("Fail to create PostgreSQL database.", getLastErrorMessage(pConnection));
               }
            }
            else
            {
               throw database::CDatabaseException("Database exists, but connection fails. (credentials for database ?)", firstError);
            }
         }
      }
      catch (...)
      {
         throw;
      }
   }

   void CPgsqlRequester::finalize()
   {
      closeAllConnections();
   }

   shared::CDataContainer CPgsqlRequester::getInformation()
   {
      shared::CDataContainer results;
      try
      {
         results.set("type", "PostgreSQL");

         auto pcon = getConnection();


         auto version = PQserverVersion(pcon);
         auto revision = version % 100;
         version /= 100;
         auto minor = version % 100;
         version /= 100;
         auto major = version % 100;

         results.set("version", (boost::format("%1%.%2%.%3%") % major % minor % revision).str());

         results.set("host", PQhost(pcon));
         results.set("port", PQport(pcon));
         results.set("name", PQdb(pcon));
         results.set("user", PQuser(pcon));
         results.set("secured", (PQgetssl(pcon) != nullptr));
      }
      catch (std::exception& ex)
      {
         YADOMS_LOG(error) << "Fail to get PostgreSQL database information : " << ex.what();
      }
      return results;
   }

   void CPgsqlRequester::closeAllConnections()
   {
      //clear transactions
      //do not do anything, let pgsql engine manage it
      m_activeTransactionsList.clear();

      //close all connection
      for (auto i = m_connectionList.begin(); i != m_connectionList.end(); ++i)
         PQfinish(i->second);
      m_connectionList.clear();
   }

   PGresult* CPgsqlRequester::executeQuery(PGconn* pConnection, const std::string& querytoExecute, ExecStatusType expectedResultCode, bool throwIfFails)
   {
      YADOMS_LOG(debug) << "[REQUEST] executeQuery - " << querytoExecute;
      auto res = PQexec(pConnection, querytoExecute.c_str());

      auto resultCode = PQresultStatus(res);
      if (resultCode != expectedResultCode)
      {
         //make a copy of the err message
         std::string errMessage(PQerrorMessage(pConnection));

         //log the message
         YADOMS_LOG(error) << "Query failed : " << "result code =" << resultCode << std::endl << "Query: " << querytoExecute << std::endl << "Error : " << errMessage;

         const CPgsqlSqlState& realError = CPgsqlSqlState::Parse(PQresultErrorField(res, PG_DIAG_SQLSTATE));

         //free memory
         PQclear(res);
         res = nullptr;


         if (resultCode == PGRES_FATAL_ERROR)
         {
            if (realError.GetClass() == ESqlErrorClass::kConnectionException)
            {
               try
               {
                  YADOMS_LOG(information) << "Check if server is still alive...";
                  pingServer();

                  //the server is still alive, just throw exception
                  if (throwIfFails)
                     throw database::CDatabaseException(errMessage);
               }
               catch (database::CDatabaseException& pingex)
               {
                  YADOMS_LOG(information) << "Cannot ping server. ";

                  //we cannot ping PostgreSQL server engine
                  //clear all active connections
                  closeAllConnections();

                  //always throws, because of server disconnection
                  throw database::CDatabaseException(errMessage, pingex.what());
               }
            }
            else
            {
               YADOMS_LOG(information) << "SQL ERROR " << realError.GetDescription() << " [" << realError.GetCode() << "]";
               if (throwIfFails)
                  throw database::CDatabaseException(errMessage);
            }
         }
         else
         {
            if (throwIfFails)
               throw database::CDatabaseException(errMessage);
         }
      }

      return res;
   }

   void CPgsqlRequester::queryEntities(database::common::adapters::IResultAdapter* pAdapter, const database::common::CQuery& querytoExecute)
   {
      queryEntities(pAdapter, querytoExecute, getConnection());
   }

   void CPgsqlRequester::queryEntities(database::common::adapters::IResultAdapter* pAdapter, const database::common::CQuery& querytoExecute, PGconn* pConnection)
   {
      BOOST_ASSERT(pAdapter != NULL);

      if (pAdapter != nullptr)
      {
         PGresult* res = nullptr;
         try
         {
            //execute query
            res = executeQuery(pConnection, querytoExecute.c_str(), PGRES_TUPLES_OK, true);
            auto handler= boost::make_shared<CPgsqlResultHandler>(res);
            if (!pAdapter->adapt(handler))
            {
               YADOMS_LOG(error) << "Fail to adapt values";
            }
         }
         catch (std::exception& ex)
         {
            YADOMS_LOG(error) << "Exception: Fail to execute query : " << ex.what();
         }
         catch (...)
         {
            YADOMS_LOG(error) << "Unknown exception: Fail to execute query";
         }

         //free memory
         if (res != nullptr)
         {
            PQclear(res);
            res = nullptr;
         }
      }
      else
      {
         //throw exception
         throw shared::exception::CNullReference("pAdapter");
      }
   }

   int CPgsqlRequester::queryStatement(const database::common::CQuery& querytoExecute, bool throwIfFails)
   {
      return queryStatement(querytoExecute, throwIfFails, getConnection());
   }

   int CPgsqlRequester::queryStatement(const database::common::CQuery& querytoExecute, bool throwIfFails, PGconn* pConnection)
   {
      BOOST_ASSERT(querytoExecute.GetQueryType() != CPgsqlQuery::kNotYetDefined);
      BOOST_ASSERT(querytoExecute.GetQueryType() != CPgsqlQuery::kSelect);


      //execute query
      auto res = executeQuery(pConnection, querytoExecute.c_str(), PGRES_COMMAND_OK, throwIfFails);
      auto resultCode = PQresultStatus(res);
      if (resultCode != PGRES_COMMAND_OK)
         return -1;

      auto affectedRows = 0;
      std::string affectedRowsString = PQcmdTuples(res);
      if (!affectedRowsString.empty())
         affectedRows = atoi(affectedRowsString.c_str());
      PQclear(res);
      return affectedRows;
   }

   int CPgsqlRequester::queryCount(const database::common::CQuery& querytoExecute)
   {
      return queryCount(querytoExecute, getConnection());
   }

   int CPgsqlRequester::queryCount(const database::common::CQuery& querytoExecute, PGconn* pConnection)
   {
      BOOST_ASSERT(querytoExecute.GetQueryType() != CPgsqlQuery::kNotYetDefined);
      BOOST_ASSERT(querytoExecute.GetQueryType() == CPgsqlQuery::kSelect);

      //execute query
      YADOMS_LOG(debug) << "[REQUEST] queryCount - " << querytoExecute.str();
      PGresult* res = executeQuery(pConnection, querytoExecute.c_str(), PGRES_TUPLES_OK, true);

      CPgsqlResultHandler handler(res);
      handler.next_step();
      return handler.extractValueAsInt(0);
   }


   CPgsqlRequester::QueryRow CPgsqlRequester::querySingleLine(const database::common::CQuery& querytoExecute)
   {
      BOOST_ASSERT(querytoExecute.GetQueryType() != CPgsqlQuery::kNotYetDefined);
      BOOST_ASSERT(querytoExecute.GetQueryType() == CPgsqlQuery::kSelect);

      QueryResults results = query(querytoExecute);
      if (results.size() >= 1)
         return results[0];

      return QueryRow(); //returns empty data
   }


   CPgsqlRequester::QueryResults CPgsqlRequester::query(const database::common::CQuery& querytoExecute)
   {
      BOOST_ASSERT(querytoExecute.GetQueryType() != CPgsqlQuery::kNotYetDefined);
      BOOST_ASSERT(querytoExecute.GetQueryType() == CPgsqlQuery::kSelect);

      database::common::adapters::CGenericAdapter genericAdapter;
      queryEntities(&genericAdapter, querytoExecute);
      return genericAdapter.getResults();
   }


   bool CPgsqlRequester::transactionSupport()
   {
      return true;
   }

   void CPgsqlRequester::transactionBegin()
   {
      transactionBegin(getConnection());
   }

   void CPgsqlRequester::transactionBegin(PGconn* pConnection)
   {
      if (!transactionIsAlreadyCreated(pConnection))
      {
         auto res = executeQuery(pConnection, "BEGIN", PGRES_COMMAND_OK, false);
         if (PQresultStatus(res) == PGRES_COMMAND_OK)
         {
            m_activeTransactionsList[pConnection] = true;
         }
         PQclear(res);
      }
   }

   void CPgsqlRequester::transactionCommit()
   {
      transactionCommit(getConnection());
   }

   void CPgsqlRequester::transactionCommit(PGconn* pConnection)
   {
      if (transactionIsAlreadyCreated(pConnection))
      {
         auto res = executeQuery(pConnection, "COMMIT", PGRES_COMMAND_OK, false);
         if (PQresultStatus(res) == PGRES_COMMAND_OK)
         {
            m_activeTransactionsList[pConnection] = false;
         }
         PQclear(res);
      }
   }

   void CPgsqlRequester::transactionRollback()
   {
      transactionRollback(getConnection());
   }

   void CPgsqlRequester::transactionRollback(PGconn* pConnection)
   {
      if (transactionIsAlreadyCreated(pConnection))
      {
         auto res = executeQuery(pConnection, "ROLLBACK", PGRES_COMMAND_OK, false);
         if (PQresultStatus(res) == PGRES_COMMAND_OK)
         {
            m_activeTransactionsList[pConnection] = false;
         }
         PQclear(res);
      }
   }

   bool CPgsqlRequester::transactionIsAlreadyCreated(PGconn* pConnection)
   {
      auto findIter = m_activeTransactionsList.find(pConnection);
      if (findIter != m_activeTransactionsList.end())
         return findIter->second;
      return false;
   }

   bool CPgsqlRequester::transactionIsAlreadyCreated()
   {
      return transactionIsAlreadyCreated(getConnection());
   }

   bool CPgsqlRequester::checkTableExists(const database::common::CDatabaseTable& tableName)
   {
      //check that table Configuration exists
      CPgsqlQuery sCheckForTableExists;
      sCheckForTableExists.SelectExists(CPgsqlQuery().
         Select().
         From(CPgsqlTablesTable::getTableName()).
         Where(CPgsqlTablesTable::getTableColumnName(), CQUERY_OP_ILIKE, tableName.GetName()));


      database::common::adapters::CSingleValueAdapter<bool> existsAdapter;
      queryEntities(&existsAdapter, sCheckForTableExists);
      if (existsAdapter.getResults().size() >= 1)
         return existsAdapter.getResults()[0];
      return false;
   }


   bool CPgsqlRequester::dropTableIfExists(const database::common::CDatabaseTable& tableName)
   {
      if (checkTableExists(tableName))
      {
         CPgsqlQuery drop;
         queryStatement(drop.DropTable(tableName));
         return !checkTableExists(tableName);
      }
      return true;
   }


   bool CPgsqlRequester::createTableIfNotExists(const database::common::CDatabaseTable& tableName, const std::string& tableScript)
   {
      if (!checkTableExists(tableName))
      {
         queryStatement(CPgsqlQuery::CustomQuery(tableScript, CPgsqlQuery::kCreate));
         return checkTableExists(tableName);
      }
      return true;
   }

   bool CPgsqlRequester::addTableColumn(const database::common::CDatabaseTable& tableName, const std::string& columnDef)
   {
      if (checkTableExists(tableName))
      {
         CPgsqlQuery alter;
         queryStatement(alter.AddTableColumn(tableName, columnDef));
         return true;
      }
      return false;
   }

   void CPgsqlRequester::createIndex(const database::common::CDatabaseTable& tableName, const std::string& indexScript)
   {
      queryStatement(CPgsqlQuery::CustomQuery(indexScript, CPgsqlQuery::kCreate));
   }

   void CPgsqlRequester::vacuum()
   {
      queryStatement(CPgsqlQuery().Vacuum());
   }

   boost::shared_ptr<database::ITableCreationScriptProvider> CPgsqlRequester::getTableCreationScriptProvider()
   {
      return boost::make_shared<CPgsqlTableCreationScriptProvider>();
   }

   bool CPgsqlRequester::backupSupported()
   {
      return false;
   }

   void CPgsqlRequester::backupData(IDataBackup::ProgressFunc reporter)
   {
      throw database::CDatabaseException("Unsupported backup for PostgreSQL");
   }

   boost::filesystem::path CPgsqlRequester::lastBackupData()
   {
      throw database::CDatabaseException("Unsupported backup for PostgreSQL");
   }

   bool CPgsqlRequester::supportInsertOrUpdateStatement()
   {
      return false;
   }
} //namespace libPgsqlAdapter 


