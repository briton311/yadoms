#include "stdafx.h"
#include "EventLogger.h"
#include <shared/currentTime/Provider.h>
#include <shared/exception/EmptyResult.hpp>
#include "dbCommon/adapters/SingleValueAdapter.hpp"
#include "dbCommon/adapters/DatabaseAdapters.h"
#include "dbCommon/DatabaseTables.h"
#include "dbCommon/Query.h"


namespace database
{
   namespace common
   {
      namespace requesters
      {
         CEventLogger::CEventLogger(boost::shared_ptr<dbCommon::IDatabaseRequester> databaseRequester)
            : m_databaseRequester(databaseRequester)
         {
         }

         CEventLogger::~CEventLogger()
         {
         }

         // IEventLoggerRequester implementation

         int CEventLogger::addEvent(const dbCommon::entities::ESystemEventCode& eventCode, const std::string& who, const std::string& what)
         {
            auto insertDate = shared::currentTime::Provider().now();

            auto qInsert = m_databaseRequester->newQuery();
            qInsert.InsertInto(dbCommon::CEventLoggerTable::getTableName(), dbCommon::CEventLoggerTable::getCodeColumnName(), dbCommon::CEventLoggerTable::getWhoColumnName(), dbCommon::CEventLoggerTable::getWhatColumnName(), dbCommon::CEventLoggerTable::getDateColumnName()).
                   Values(eventCode, who, what, insertDate);

            if (m_databaseRequester->queryStatement(qInsert) <= 0)
               throw shared::exception::CEmptyResult("No lines affected");

            auto qSelect = m_databaseRequester->newQuery();
            qSelect.Select(dbCommon::CEventLoggerTable::getIdColumnName()).
                   From(dbCommon::CEventLoggerTable::getTableName()).
                   Where(dbCommon::CEventLoggerTable::getCodeColumnName(), CQUERY_OP_EQUAL, eventCode).
                   And(dbCommon::CEventLoggerTable::getDateColumnName(), CQUERY_OP_EQUAL, insertDate).
                   OrderBy(dbCommon::CEventLoggerTable::getIdColumnName(), dbCommon::CQuery::kDesc);

            dbCommon::adapters::CSingleValueAdapter<int> adapter;
            m_databaseRequester->queryEntities(&adapter, qSelect);
            if (adapter.getResults().size() >= 1)
               return adapter.getResults()[0];

            throw shared::exception::CEmptyResult("Cannot retrieve inserted ");
         }


         std::vector<boost::shared_ptr<dbCommon::entities::CEventLogger>> CEventLogger::getEvents()
         {
            auto qSelect = m_databaseRequester->newQuery();
            qSelect.Select().
                   From(dbCommon::CEventLoggerTable::getTableName()).
                   OrderBy(dbCommon::CEventLoggerTable::getIdColumnName(), dbCommon::CQuery::kDesc);

            dbCommon::adapters::CEventLoggerAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, qSelect);
            return adapter.getResults();
         }

         boost::shared_ptr<dbCommon::entities::CEventLogger> CEventLogger::getEvent(const int eventId)
         {
            auto qSelect = m_databaseRequester->newQuery();
            qSelect.Select().
                   From(dbCommon::CEventLoggerTable::getTableName()).
                   Where(dbCommon::CEventLoggerTable::getIdColumnName(), CQUERY_OP_EQUAL, eventId).
                   Limit(1);

            dbCommon::adapters::CEventLoggerAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, qSelect);
            if (adapter.getResults().empty())
               return boost::shared_ptr<dbCommon::entities::CEventLogger>();
            return adapter.getResults()[0];
         }

         boost::shared_ptr<dbCommon::entities::CEventLogger> CEventLogger::getLastEvent()
         {
            auto qSelect = m_databaseRequester->newQuery();
            qSelect.Select().
                   From(dbCommon::CEventLoggerTable::getTableName()).
                   OrderBy(dbCommon::CEventLoggerTable::getIdColumnName(), dbCommon::CQuery::kDesc).
                   Limit(1);

            dbCommon::adapters::CEventLoggerAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, qSelect);

            auto list = adapter.getResults();
            if (list.empty())
               return boost::shared_ptr<dbCommon::entities::CEventLogger>();
            return adapter.getResults()[0];
         }

         std::vector<boost::shared_ptr<dbCommon::entities::CEventLogger>> CEventLogger::getEventsFrom(const int eventId)
         {
            auto qSelect = m_databaseRequester->newQuery();
            qSelect.Select().
                   From(dbCommon::CEventLoggerTable::getTableName()).
                   Where(dbCommon::CEventLoggerTable::getIdColumnName(), CQUERY_OP_SUP, eventId).
                   OrderBy(dbCommon::CEventLoggerTable::getIdColumnName(), dbCommon::CQuery::kDesc, dbCommon::CEventLoggerTable::getDateColumnName(), dbCommon::CQuery::kDesc);

            dbCommon::adapters::CEventLoggerAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, qSelect);
            return adapter.getResults();
         }

         std::vector<boost::shared_ptr<dbCommon::entities::CEventLogger>> CEventLogger::getEventsRange(const int offset, const int count)
         {
            auto qSelect = m_databaseRequester->newQuery();
            qSelect.Select().
                   From(dbCommon::CEventLoggerTable::getTableName()).
                   OrderBy(dbCommon::CEventLoggerTable::getIdColumnName(), dbCommon::CQuery::kDesc, dbCommon::CEventLoggerTable::getDateColumnName(), dbCommon::CQuery::kDesc).
                   Limit(count, offset);

            dbCommon::adapters::CEventLoggerAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, qSelect);
            return adapter.getResults();
         }

         // [END] IEventLoggerRequester implementation
      } //namespace requesters
   } //namespace common
} //namespace database 


