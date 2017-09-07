#include "stdafx.h"
#include "PluginEventLogger.h"
#include <shared/currentTime/Provider.h>
#include <shared/exception/EmptyResult.hpp>
#include <dbCommon/adapters/SingleValueAdapter.hpp>
#include <dbCommon/adapters/DatabaseAdapters.h>
#include <dbCommon/DatabaseTables.h>
#include <dbCommon/Query.h>


namespace database
{
   namespace common
   {
      namespace requesters
      {
         CPluginEventLogger::CPluginEventLogger(boost::shared_ptr<dbCommon::IDatabaseRequester> databaseRequester)
            : m_databaseRequester(databaseRequester)
         {
         }

         CPluginEventLogger::~CPluginEventLogger()
         {
         }

         // IPluginEventLoggerRequester implementation

         int CPluginEventLogger::addEvent(const std::string& pluginName, const std::string& pluginVersion, const dbCommon::entities::EEventType& eventType, const std::string& message)
         {
            auto insertDate = shared::currentTime::Provider().now();

            auto qInsert = m_databaseRequester->newQuery();
            qInsert.InsertInto(dbCommon::CPluginEventLoggerTable::getTableName(),
                               dbCommon::CPluginEventLoggerTable::getPluginNameColumnName(),
                               dbCommon::CPluginEventLoggerTable::getPluginVersionColumnName(),
                               dbCommon::CPluginEventLoggerTable::getEventTypeColumnName(),
                               dbCommon::CPluginEventLoggerTable::getMessageColumnName(),
                               dbCommon::CPluginEventLoggerTable::getEventDateColumnName()).
                   Values(pluginName,
                          pluginVersion,
                          eventType,
                          message,
                          insertDate);

            if (m_databaseRequester->queryStatement(qInsert) <= 0)
               throw shared::exception::CEmptyResult("No lines affected");

            auto qSelect = m_databaseRequester->newQuery();
            qSelect.Select(dbCommon::CPluginEventLoggerTable::getIdColumnName()).
                   From(dbCommon::CPluginEventLoggerTable::getTableName()).
                   Where(dbCommon::CPluginEventLoggerTable::getPluginNameColumnName(), CQUERY_OP_EQUAL, pluginName).
                   And(dbCommon::CPluginEventLoggerTable::getPluginVersionColumnName(), CQUERY_OP_EQUAL, pluginVersion).
                   And(dbCommon::CPluginEventLoggerTable::getEventTypeColumnName(), CQUERY_OP_EQUAL, eventType).
                   And(dbCommon::CPluginEventLoggerTable::getEventDateColumnName(), CQUERY_OP_EQUAL, insertDate).
                   OrderBy(dbCommon::CPluginEventLoggerTable::getIdColumnName(), dbCommon::CQuery::kDesc);

            dbCommon::adapters::CSingleValueAdapter<int> adapter;
            m_databaseRequester->queryEntities(&adapter, qSelect);
            if (adapter.getResults().size() >= 1)
               return adapter.getResults()[0];

            throw shared::exception::CEmptyResult("Cannot retrieve inserted Plugin");
         }

         int CPluginEventLogger::addEvent(const dbCommon::entities::CPluginEventLogger& pluginLogEntry)
         {
            return addEvent(pluginLogEntry.PluginName(),
                            pluginLogEntry.PluginVersion(),
                            pluginLogEntry.EventType(),
                            pluginLogEntry.Message());
         }

         std::vector<boost::shared_ptr<dbCommon::entities::CPluginEventLogger>> CPluginEventLogger::getPluginEvents(const std::string& pluginName, const std::string& pluginVersion)
         {
            auto qSelect = m_databaseRequester->newQuery();
            qSelect.Select().
                   From(dbCommon::CPluginEventLoggerTable::getTableName()).
                   Where(dbCommon::CPluginEventLoggerTable::getPluginNameColumnName(), CQUERY_OP_EQUAL, pluginName).
                   And(dbCommon::CPluginEventLoggerTable::getPluginVersionColumnName(), CQUERY_OP_EQUAL, pluginVersion).
                   OrderBy(dbCommon::CPluginEventLoggerTable::getEventDateColumnName(), dbCommon::CQuery::kDesc);

            dbCommon::adapters::CPluginEventLoggerAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, qSelect);
            return adapter.getResults();
         }


         std::vector<boost::shared_ptr<dbCommon::entities::CPluginEventLogger>> CPluginEventLogger::getPluginEvents(const std::string& pluginName, const std::string& pluginVersion, const boost::posix_time::ptime& fromDate)
         {
            auto qSelect = m_databaseRequester->newQuery();
            qSelect.Select().
                   From(dbCommon::CPluginEventLoggerTable::getTableName()).
                   Where(dbCommon::CPluginEventLoggerTable::getPluginNameColumnName(), CQUERY_OP_EQUAL, pluginName).
                   And(dbCommon::CPluginEventLoggerTable::getPluginVersionColumnName(), CQUERY_OP_EQUAL, pluginVersion).
                   And(dbCommon::CPluginEventLoggerTable::getEventDateColumnName(), CQUERY_OP_SUP, fromDate).
                   OrderBy(dbCommon::CPluginEventLoggerTable::getEventDateColumnName(), dbCommon::CQuery::kDesc);

            dbCommon::adapters::CPluginEventLoggerAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, qSelect);
            return adapter.getResults();
         }

         // [END] IPluginEventLoggerRequester implementation
      } //namespace requesters
   } //namespace common
} //namespace database 


