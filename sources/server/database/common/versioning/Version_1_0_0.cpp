#include "stdafx.h"
#include "Version_1_0_0.h"
#include <dbCommon/Query.h>
#include <dbCommon/DatabaseTables.h>
#include <shared/versioning/Version.h>
#include "VersionException.h"
#include <shared/Log.h>

namespace database
{
   namespace common
   {
      namespace versioning
      {
         // Modify this version to a greater value, to force update of current version
         const shared::versioning::CVersion CVersion_1_0_0::Version(1, 0, 0);

         CVersion_1_0_0::CVersion_1_0_0()
         {
         }

         CVersion_1_0_0::~CVersion_1_0_0()
         {
         }

         // ISQLiteVersionUpgrade implementation
         void CVersion_1_0_0::checkForUpgrade(const boost::shared_ptr<dbCommon::IDatabaseRequester>& pRequester,
                                              const shared::versioning::CVersion& currentVersion)
         {
            auto bNeedToCreateOrUpgrade = false;

            if (currentVersion >= Version)
            {
               //not for me, version is correct

               //check that all tables exist
               if (!pRequester->checkTableExists(dbCommon::CConfigurationTable::getTableName()) ||
                  !pRequester->checkTableExists(dbCommon::CDeviceTable::getTableName()) ||
                  !pRequester->checkTableExists(dbCommon::CPluginTable::getTableName()) ||
                  !pRequester->checkTableExists(dbCommon::CKeywordTable::getTableName()) ||
                  !pRequester->checkTableExists(dbCommon::CPageTable::getTableName()) ||
                  !pRequester->checkTableExists(dbCommon::CWidgetTable::getTableName()) ||
                  !pRequester->checkTableExists(dbCommon::CPluginEventLoggerTable::getTableName()) ||
                  !pRequester->checkTableExists(dbCommon::CEventLoggerTable::getTableName()) ||
                  !pRequester->checkTableExists(dbCommon::CAcquisitionTable::getTableName()) ||
                  !pRequester->checkTableExists(dbCommon::CAcquisitionSummaryTable::getTableName()) ||
                  !pRequester->checkTableExists(dbCommon::CRuleTable::getTableName()) ||
                  !pRequester->checkTableExists(dbCommon::CRecipientTable::getTableName()) ||
                  !pRequester->checkTableExists(dbCommon::CRecipientFieldTable::getTableName()))
               {
                  //at least one table is missing
                  bNeedToCreateOrUpgrade = true;
               }
            }
            else
            {
               //version is lower to requested
               bNeedToCreateOrUpgrade = true;
            }

            if (bNeedToCreateOrUpgrade)
            {
               CreateDatabase(pRequester);
            }
         }

         void CVersion_1_0_0::updateDatabaseVersion(const boost::shared_ptr<dbCommon::IDatabaseRequester> pRequester,
                                                    const shared::versioning::CVersion& newVersion)
         {
            auto qUpdate = pRequester->newQuery();

            qUpdate.Update(dbCommon::CConfigurationTable::getTableName())
                   .Set(dbCommon::CConfigurationTable::getValueColumnName(), newVersion.toString())
                   .Where(dbCommon::CConfigurationTable::getSectionColumnName(), CQUERY_OP_EQUAL, "Database")
                   .And(dbCommon::CConfigurationTable::getNameColumnName(), CQUERY_OP_EQUAL, "Version");

            pRequester->queryStatement(qUpdate);
         }

         // [END] ISQLiteVersionUpgrade implementation


         //-----------------------------------
         /// \brief     Create the database (when tables are not found)
         ///\param [in] pRequester : database requester object
         ///\throw      CVersionException if create database failed
         //-----------------------------------
         void CVersion_1_0_0::CreateDatabase(const boost::shared_ptr<dbCommon::IDatabaseRequester>& pRequester)
         {
            try
            {
               //create transaction if supported
               if (pRequester->transactionSupport())
                  pRequester->transactionBegin();

               //delete tables
               if (!pRequester->dropTableIfExists(dbCommon::CConfigurationTable::getTableName()))
                  throw CVersionException("Failed to delete Configuration table");
               if (!pRequester->dropTableIfExists(dbCommon::CDeviceTable::getTableName()))
                  throw CVersionException("Failed to delete Device table");
               if (!pRequester->dropTableIfExists(dbCommon::CPluginTable::getTableName()))
                  throw CVersionException("Failed to delete Plugin table");
               if (!pRequester->dropTableIfExists(dbCommon::CKeywordTable::getTableName()))
                  throw CVersionException("Failed to delete Keyword table");
               if (!pRequester->dropTableIfExists(dbCommon::CPageTable::getTableName()))
                  throw CVersionException("Failed to delete Page table");
               if (!pRequester->dropTableIfExists(dbCommon::CWidgetTable::getTableName()))
                  throw CVersionException("Failed to delete Widget table");
               if (!pRequester->dropTableIfExists(dbCommon::CPluginEventLoggerTable::getTableName()))
                  throw CVersionException("Failed to delete PluginEventLogger table");
               if (!pRequester->dropTableIfExists(dbCommon::CEventLoggerTable::getTableName()))
                  throw CVersionException("Failed to delete EventLogger table");
               if (!pRequester->dropTableIfExists(dbCommon::CAcquisitionTable::getTableName()))
                  throw CVersionException("Failed to delete Acquisition table");
               if (!pRequester->dropTableIfExists(dbCommon::CAcquisitionSummaryTable::getTableName()))
                  throw CVersionException("Failed to delete AcquisitionSummary table");
               if (!pRequester->dropTableIfExists(dbCommon::CRuleTable::getTableName()))
                  throw CVersionException("Failed to delete rule table");
               if (!pRequester->dropTableIfExists(dbCommon::CRecipientTable::getTableName()))
                  throw CVersionException("Failed to delete Recipient table");
               if (!pRequester->dropTableIfExists(dbCommon::CRecipientFieldTable::getTableName()))
                  throw CVersionException("Failed to delete RecipientFields table");

               auto scriptProvider = pRequester->getTableCreationScriptProvider();

               //create tables
               if (!pRequester->createTableIfNotExists(dbCommon::CConfigurationTable::getTableName(), scriptProvider->getTableConfiguration()))
                  throw CVersionException("Failed to create Configuration table");
               if (!pRequester->createTableIfNotExists(dbCommon::CDeviceTable::getTableName(), scriptProvider->getTableDevice()))
                  throw CVersionException("Failed to create Device table");
               if (!pRequester->createTableIfNotExists(dbCommon::CPluginTable::getTableName(), scriptProvider->getTablePlugin()))
                  throw CVersionException("Failed to create Plugin table");
               if (!pRequester->createTableIfNotExists(dbCommon::CKeywordTable::getTableName(), scriptProvider->getTableKeyword()))
                  throw CVersionException("Failed to create Keyword table");
               if (!pRequester->createTableIfNotExists(dbCommon::CPageTable::getTableName(), scriptProvider->getTablePage()))
                  throw CVersionException("Failed to create Page table");
               if (!pRequester->createTableIfNotExists(dbCommon::CWidgetTable::getTableName(), scriptProvider->getTableWidget()))
                  throw CVersionException("Failed to create Widget table");
               if (!pRequester->createTableIfNotExists(dbCommon::CPluginEventLoggerTable::getTableName(), scriptProvider->getTablePluginEventLogger()))
                  throw CVersionException("Failed to create PluginEventLogger table");
               if (!pRequester->createTableIfNotExists(dbCommon::CEventLoggerTable::getTableName(), scriptProvider->getTableEventLogger()))
                  throw CVersionException("Failed to create EventLogger table");
               if (!pRequester->createTableIfNotExists(dbCommon::CAcquisitionTable::getTableName(), scriptProvider->getTableAcquisition()))
                  throw CVersionException("Failed to create Acquisition table");
               if (!pRequester->createTableIfNotExists(dbCommon::CAcquisitionSummaryTable::getTableName(), scriptProvider->getTableAcquisitionSummary()))
                  throw CVersionException("Failed to create AcquisitionSummary table");
               if (!pRequester->createTableIfNotExists(dbCommon::CRuleTable::getTableName(), scriptProvider->getTableRule()))
                  throw CVersionException("Failed to create Rule table");
               if (!pRequester->createTableIfNotExists(dbCommon::CRecipientTable::getTableName(), scriptProvider->getTableRecipient()))
                  throw CVersionException("Failed to create Recipient table");
               if (!pRequester->createTableIfNotExists(dbCommon::CRecipientFieldTable::getTableName(), scriptProvider->getTableRecipientField()))
                  throw CVersionException("Failed to create RecipientFields table");

               std::vector<std::string> indexScripts;
               scriptProvider->getTableAcquisitionIndexes(indexScripts);

               //indexes
               for (std::vector<std::string>::const_iterator i = indexScripts.begin(); i != indexScripts.end(); ++i)
               {
                  pRequester->createIndex(dbCommon::CAcquisitionTable::getTableName(), *i);
               }

               //set the database version
               auto qInsert = pRequester->newQuery();
               qInsert.InsertInto(dbCommon::CConfigurationTable::getTableName(), dbCommon::CConfigurationTable::getSectionColumnName(), dbCommon::CConfigurationTable::getNameColumnName(), dbCommon::CConfigurationTable::getValueColumnName(), dbCommon::CConfigurationTable::getDescriptionColumnName()).
                      Values("Database", "Version", Version.toString(), "Database version");
               pRequester->queryStatement(qInsert);

               //set the developer mode
               qInsert.Clear().InsertInto(dbCommon::CConfigurationTable::getTableName(), dbCommon::CConfigurationTable::getSectionColumnName(), dbCommon::CConfigurationTable::getNameColumnName(), dbCommon::CConfigurationTable::getValueColumnName(), dbCommon::CConfigurationTable::getDefaultValueColumnName(), dbCommon::CConfigurationTable::getDescriptionColumnName()).
                      Values("system", "developerMode", "false", "false", "Developer mode");
               pRequester->queryStatement(qInsert);

               //system plugin
               qInsert.Clear().InsertInto(dbCommon::CPluginTable::getTableName(), dbCommon::CPluginTable::getDisplayNameColumnName(), dbCommon::CPluginTable::getTypeColumnName(), dbCommon::CPluginTable::getAutoStartColumnName(), dbCommon::CPluginTable::getCategoryColumnName()).
                      Values("System", "System", true, dbCommon::entities::EPluginCategory::kSystem);
               pRequester->queryStatement(qInsert);

               //commit transaction
               if (pRequester->transactionSupport())
                  pRequester->transactionCommit();

               //compact database
               pRequester->vacuum();
            }
            catch (CVersionException& ex)
            {
               YADOMS_LOG(fatal) << "Failed to upgrade database : " << ex.what();
               YADOMS_LOG(fatal) << "Rollback transaction";
               if (pRequester->transactionSupport())
                  pRequester->transactionRollback();
               throw CVersionException("Failed to create database");
            }
         }
      } //namespace versioning
   } //namespace common
} //namespace database 


