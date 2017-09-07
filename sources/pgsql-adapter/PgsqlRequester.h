#pragma once

#include "libpq-fe.h"
#include <dbCommon/IDatabaseRequester.h>
#include "PgsqlOptions.h"

namespace libPgsqlAdapter
{
   class CPgsqlRequester : public dbCommon::IDatabaseRequester
   {
   public:
      CPgsqlRequester();
      virtual ~CPgsqlRequester();

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
      bool addTableColumn(const dbCommon::CDatabaseTable& tableName, const std::string& columnDef) override;
      void createIndex(const dbCommon::CDatabaseTable& tableName, const std::string& indexScript) override;
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
      void backupData(IDataBackup::ProgressFunc reporter) override;
      boost::filesystem::path lastBackupData() override;
      // [END] dbCommon::IDataBackup implementation

   private:
      void queryEntities(dbCommon::adapters::IResultAdapter* pAdapter, const dbCommon::CQuery& querytoExecute, PGconn* pConnection);
      int queryStatement(const dbCommon::CQuery& querytoExecute, bool throwIfFails, PGconn* pConnection);
      int queryCount(const dbCommon::CQuery& querytoExecute, PGconn* pConnection);
      void transactionBegin(PGconn* pConnection);
      void transactionCommit(PGconn* pConnection);
      void transactionRollback(PGconn* pConnection);
      bool transactionIsAlreadyCreated(PGconn* pConnection);

      //--------------------------------------------------------------
      /// \Brief		Close all active connections
      //--------------------------------------------------------------
      void closeAllConnections();

      //--------------------------------------------------------------
      /// \Brief		Execute a query
      /// \param [in] pConnection         The connection to use
      /// \param [in] querytoExecute      The query to execute
      /// \param [in] expectedResultCode  The expected result code (might differ depending on the statement type)
      /// \param [in] throwIfFails        If true an exception is thrown on error, else the PGresult is returned
      /// \return    The PGresult data (must be cleared)
      /// \throws    CDatabaseException If failed. In case of disconnection, close any active connection
      //--------------------------------------------------------------
      PGresult* executeQuery(PGconn* pConnection, const std::string& querytoExecute, ExecStatusType expectedResultCode, bool throwIfFails);

      //--------------------------------------------------------------
      /// \Brief		Try to ping PostgreSQL server
      /// \throws    CDatabaseException If ping failed
      //--------------------------------------------------------------
      void pingServer();

      typedef enum
      {
         kNormal, /// full connection string
         kPing, ///ping connection string
         kMasterDb //light connection string, with targetting postgres database
      } EConnectionStringMode;

      //--------------------------------------------------------------
      /// \Brief		         Create the connection string
      /// \param [in]	mode  The connection string mode.
      /// \return             The connection string
      //--------------------------------------------------------------
      std::string createConnectionString(const EConnectionStringMode mode = kNormal);

      //--------------------------------------------------------------
      /// \Brief		         Append (or not) an optional parameter to the connection string
      /// \param [in/out]	   cnx         The connection string to complete.
      /// \param [in]	      value       The optional parameter
      /// \param [in]	      paramName   The parameter name (to add in connection string)
      //--------------------------------------------------------------
      template <class T>
      void connectionStringAppendOptionalParameter(std::string& cnx, Poco::Nullable<T> value, const std::string& paramName)
      {
         if (!value.isNull())
         {
            std::stringstream ss;
            ss << " " << paramName << "=" << value.value();
            cnx += ss.str();
         }
      }

      //--------------------------------------------------------------
      /// \Brief		         Create a new connection (one for each thread; testing one for each request)
      /// \return             A connection pointer
      //--------------------------------------------------------------
      PGconn* createNewConnection();

      //--------------------------------------------------------------
      /// \Brief		         Get the current thread connection (create it if necessary)
      /// \return             A connection pointer
      //--------------------------------------------------------------
      PGconn* getConnection();

      //--------------------------------------------------------------
      /// \Brief		         Terminate a connection (one for each thread; testing one for each request)
      /// \param [in] pConnection   The connection pointer to close
      //--------------------------------------------------------------
      static void terminateConnection(PGconn* pConnection);

      //--------------------------------------------------------------
      /// \Brief		         Obtain the last error message
      /// \param [in] pConnection   The connection pointer to retreive the last error message
      /// \return                   The last error message above the specified connection
      //--------------------------------------------------------------
      static std::string getLastErrorMessage(PGconn* pConnection);


      //--------------------------------------------------------------
      /// \Brief		List all connections (one per thread)
      //--------------------------------------------------------------
      std::map<unsigned long, PGconn*> m_connectionList;

      //--------------------------------------------------------------
      /// \Brief		List all transactions (one per connection)
      //--------------------------------------------------------------
      std::map<PGconn*, bool> m_activeTransactionsList;

      //--------------------------------------------------------------
      /// \Brief		PGSQL specific options
      //--------------------------------------------------------------
      boost::shared_ptr<const CPgsqlOptions> m_options;
   };
} //namespace libPgsqlAdapter 


