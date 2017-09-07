#pragma once

#include "sqlite3.h"
#include <dbCommon/IDatabaseRequester.h>
#include <dbCommon/DatabaseException.hpp>
#include "SQLiteOptions.h"

namespace libSQLiteAdapter
{
   class CSQLiteRequester : public dbCommon::IDatabaseRequester
   {
   public:
      explicit CSQLiteRequester();
      virtual ~CSQLiteRequester();

      // dbCommon::IDatabaseEngine implementation
      void setOptions(boost::shared_ptr<const shared::CDataContainer> options) override;
      void initialize() override;
      void finalize() override;
      shared::CDataContainer getInformation() override;
      // [END] dbCommon::IDatabaseEngine implementation

      // dbCommon::IDatabaseRequester implementation
      dbCommon::CQuery newQuery() override;
      void queryEntities(dbCommon::adapters::IResultAdapter* pAdapter, const dbCommon::CQuery& querytoExecute) override;
      int queryStatement(const dbCommon::CQuery& querytoExecute, bool throwIfFails = true) override;
      int queryCount(const dbCommon::CQuery& querytoExecute) override;
      QueryRow querySingleLine(const dbCommon::CQuery& querytoExecute) override;
      QueryResults query(const dbCommon::CQuery& querytoExecute) override;
      bool checkTableExists(const dbCommon::CDatabaseTable& tableName) override;
      bool dropTableIfExists(const dbCommon::CDatabaseTable& tableName) override;
      bool createTableIfNotExists(const dbCommon::CDatabaseTable& tableName, const std::string& tableScript) override;
      void createIndex(const dbCommon::CDatabaseTable& tableName, const std::string& indexScript) override;
      bool addTableColumn(const dbCommon::CDatabaseTable& tableName, const std::string& columnDef) override;
      void vacuum() override;
      boost::shared_ptr<dbCommon::ITableCreationScriptProvider> getTableCreationScriptProvider() override;
      bool supportInsertOrUpdateStatement() override;
      // [END] dbCommon::IDatabaseRequester implementation

      // dbCommon::ITransactionalProvider implementation
      bool transactionSupport() override;
      void transactionBegin() override;
      void transactionCommit() override;
      void transactionRollback() override;
      bool transactionIsAlreadyCreated() override;
      // [END] dbCommon::ITransactionalProvider implementation

      // dbCommon::IDataBackup implementation
      bool backupSupported() override;
      void backupData(ProgressFunc reporter, boost::shared_ptr<dbCommon::IDataBackupClientStrings> strings) override;
      boost::filesystem::path lastBackupData() override;
      // [END] dbCommon::IDataBackup implementation

   protected:
      static dbCommon::CDatabaseException::EDatabaseReturnCodes fromSQLiteReturnCode(int rc);

   private:
      //--------------------------------------------------------------
      /// \Brief		Inject C functions in sqlite engine
      //--------------------------------------------------------------
      void registerExtendedFunctions() const;

      //--------------------------------------------------------------
      /// \Brief		database handler
      //--------------------------------------------------------------
      sqlite3* m_pDatabaseHandler;

      //--------------------------------------------------------------
      /// \Brief		true if a transaction is already begin
      //--------------------------------------------------------------
      bool m_bOneTransactionActive;

      //--------------------------------------------------------------
      /// \Brief		In case of some errors, (database locked,...) the query may be retried
      //--------------------------------------------------------------
      static int m_maxTries;

      //--------------------------------------------------------------
      /// \Brief		SQLite specific options
      //--------------------------------------------------------------
      boost::shared_ptr<const CSQLiteOptions> m_options;
   };
} //namespace libSQLiteAdapter 


