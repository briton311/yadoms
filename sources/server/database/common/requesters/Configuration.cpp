#include "stdafx.h"
#include "Configuration.h"
#include <shared/currentTime/Provider.h>
#include <shared/exception/EmptyResult.hpp>
#include <dbCommon/adapters/DatabaseAdapters.h>
#include <dbCommon/DatabaseTables.h>
#include <dbCommon/Query.h>


namespace database
{
   namespace common
   {
      namespace requesters
      {
         CConfiguration::CConfiguration(boost::shared_ptr<dbCommon::IDatabaseRequester> databaseRequester)
            : m_databaseRequester(databaseRequester)
         {
         }

         CConfiguration::~CConfiguration()
         {
         }


         // IConfigurationRequester implementation
         void CConfiguration::create(dbCommon::entities::CConfiguration& configurationToCreate)
         {
            auto insertDate = shared::currentTime::Provider().now();
            auto qInsert = m_databaseRequester->newQuery();
            qInsert.InsertInto(dbCommon::CConfigurationTable::getTableName(), dbCommon::CConfigurationTable::getSectionColumnName(), dbCommon::CConfigurationTable::getNameColumnName(), dbCommon::CConfigurationTable::getValueColumnName(), dbCommon::CConfigurationTable::getDescriptionColumnName(), dbCommon::CConfigurationTable::getDefaultValueColumnName(), dbCommon::CConfigurationTable::getLastModificationDateColumnName()).
                   Values(configurationToCreate.Section(), configurationToCreate.Name(), configurationToCreate.Value(), configurationToCreate.Description(), configurationToCreate.DefaultValue(), insertDate);
            if (m_databaseRequester->queryStatement(qInsert) <= 0)
               throw shared::exception::CEmptyResult("No lines affected");
         }

         bool CConfiguration::exists(const std::string& section, const std::string& name)
         {
            auto qSelect = m_databaseRequester->newQuery();
            qSelect.SelectCount().
                   From(dbCommon::CConfigurationTable::getTableName()).
                   Where(dbCommon::CConfigurationTable::getSectionColumnName(), CQUERY_OP_LIKE, section).
                   And(dbCommon::CConfigurationTable::getNameColumnName(), CQUERY_OP_LIKE, name);
            return (m_databaseRequester->queryCount(qSelect) == 1);
         }


         boost::shared_ptr<dbCommon::entities::CConfiguration> CConfiguration::getConfiguration(const std::string& section, const std::string& name)
         {
            auto qSelect = m_databaseRequester->newQuery();
            qSelect.Select().
                   From(dbCommon::CConfigurationTable::getTableName()).
                   Where(dbCommon::CConfigurationTable::getSectionColumnName(), CQUERY_OP_LIKE, section).
                   And(dbCommon::CConfigurationTable::getNameColumnName(), CQUERY_OP_LIKE, name);

            dbCommon::adapters::CConfigurationAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, qSelect);
            if (adapter.getResults().size() >= 1)
               return adapter.getResults()[0];

            auto sEx = (boost::format("Cannot retrieve Configuration Section=%1% and Name=%2% in database") % section % name).str();
            throw shared::exception::CEmptyResult(sEx);
         }

         std::vector<boost::shared_ptr<dbCommon::entities::CConfiguration>> CConfiguration::getConfigurations(const std::string& section)
         {
            auto qSelect = m_databaseRequester->newQuery();
            qSelect.Select().
                   From(dbCommon::CConfigurationTable::getTableName()).
                   Where(dbCommon::CConfigurationTable::getSectionColumnName(), CQUERY_OP_LIKE, section).
                   OrderBy(dbCommon::CConfigurationTable::getNameColumnName());

            dbCommon::adapters::CConfigurationAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, qSelect);
            return adapter.getResults();
         }

         std::vector<boost::shared_ptr<dbCommon::entities::CConfiguration>> CConfiguration::getConfigurations()
         {
            auto qSelect = m_databaseRequester->newQuery();
            qSelect.Select().
                   From(dbCommon::CConfigurationTable::getTableName()).
                   OrderBy(dbCommon::CConfigurationTable::getSectionColumnName(), dbCommon::CQuery::kAsc, dbCommon::CConfigurationTable::getNameColumnName(), dbCommon::CQuery::kAsc);

            dbCommon::adapters::CConfigurationAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, qSelect);
            return adapter.getResults();
         }


         void CConfiguration::updateConfiguration(dbCommon::entities::CConfiguration& configurationToUpdate)
         {
            boost::posix_time::ptime updateDate = shared::currentTime::Provider().now();

            if (exists(configurationToUpdate.Section(), configurationToUpdate.Name()))
            {
               auto qUpdate = m_databaseRequester->newQuery();
               qUpdate.Update(dbCommon::CConfigurationTable::getTableName()).
                      Set(dbCommon::CConfigurationTable::getValueColumnName(), configurationToUpdate.Value(),
                          dbCommon::CConfigurationTable::getLastModificationDateColumnName(), updateDate).
                      Where(dbCommon::CConfigurationTable::getSectionColumnName(), CQUERY_OP_LIKE, configurationToUpdate.Section()).
                      And(dbCommon::CConfigurationTable::getNameColumnName(), CQUERY_OP_LIKE, configurationToUpdate.Name());

               if (m_databaseRequester->queryStatement(qUpdate) <= 0)
                  throw shared::exception::CEmptyResult("No lines affected");
            }
            else
            {
               return create(configurationToUpdate);
            }
         }

         void CConfiguration::removeConfiguration(dbCommon::entities::CConfiguration& configurationToRemove)
         {
            auto qDelete = m_databaseRequester->newQuery();
            qDelete.DeleteFrom(dbCommon::CConfigurationTable::getTableName()).
                   Where(dbCommon::CConfigurationTable::getSectionColumnName(), CQUERY_OP_LIKE, configurationToRemove.Section()).
                   And(dbCommon::CConfigurationTable::getNameColumnName(), CQUERY_OP_LIKE, configurationToRemove.Name());
            if (m_databaseRequester->queryStatement(qDelete) <= 0)
               throw new shared::exception::CEmptyResult("No lines affected");
         }

         // [END] IConfigurationRequester implementation
      } //namespace requesters
   } //namespace common
} //namespace database 


