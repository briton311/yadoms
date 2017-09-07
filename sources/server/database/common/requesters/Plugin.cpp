#include "stdafx.h"
#include "Plugin.h"
#include <shared/exception/EmptyResult.hpp>
#include <dbCommon/adapters/SingleValueAdapter.hpp>
#include <dbCommon/adapters/DatabaseAdapters.h>
#include <dbCommon/DatabaseTables.h>
#include <dbCommon/Query.h>
#include <dbCommon/DatabaseException.hpp>

namespace database
{
   namespace common
   {
      namespace requesters
      {
         CPlugin::CPlugin(boost::shared_ptr<dbCommon::IDatabaseRequester> databaseRequester)
            : m_databaseRequester(databaseRequester)
         {
         }

         CPlugin::~CPlugin()
         {
         }

         // IPluginRequester implementation
         int CPlugin::addInstance(const dbCommon::entities::CPlugin& newPlugin)
         {
            auto qInsert = m_databaseRequester->newQuery();

            qInsert.InsertInto(dbCommon::CPluginTable::getTableName(), dbCommon::CPluginTable::getDisplayNameColumnName(), dbCommon::CPluginTable::getTypeColumnName(), dbCommon::CPluginTable::getConfigurationColumnName(), dbCommon::CPluginTable::getAutoStartColumnName()).
                   Values(newPlugin.DisplayName(),
                          newPlugin.Type(),
                          newPlugin.Configuration(),
                          newPlugin.AutoStart());

            if (m_databaseRequester->queryStatement(qInsert) <= 0)
               throw shared::exception::CEmptyResult("No lines affected");


            auto qSelect = m_databaseRequester->newQuery();
            qSelect.Select(dbCommon::CPluginTable::getIdColumnName()).
                   From(dbCommon::CPluginTable::getTableName()).
                   Where(dbCommon::CPluginTable::getDisplayNameColumnName(), CQUERY_OP_EQUAL, newPlugin.DisplayName()).
                   And(dbCommon::CPluginTable::getTypeColumnName(), CQUERY_OP_EQUAL, newPlugin.Type()).
                   OrderBy(dbCommon::CPluginTable::getIdColumnName(), dbCommon::CQuery::kDesc);

            dbCommon::adapters::CSingleValueAdapter<int> adapter;
            m_databaseRequester->queryEntities(&adapter, qSelect);
            if (adapter.getResults().size() >= 1)
               return adapter.getResults()[0];

            throw shared::exception::CEmptyResult("Cannot retrieve inserted Plugin");
         }

         boost::shared_ptr<dbCommon::entities::CPlugin> CPlugin::getInstance(int pluginId)
         {
            dbCommon::adapters::CPluginAdapter adapter;

            auto qSelect = m_databaseRequester->newQuery();

            qSelect.Select().
                   From(dbCommon::CPluginTable::getTableName()).
                   Where(dbCommon::CPluginTable::getIdColumnName(), CQUERY_OP_EQUAL, pluginId);

            m_databaseRequester->queryEntities(&adapter, qSelect);
            if (adapter.getResults().empty())
            {
               // Plugin not found
               std::string sEx = (boost::format("Plugin Id %1% not found in database") % pluginId).str();
               throw shared::exception::CEmptyResult(sEx);
            }
            return adapter.getResults().at(0);
         }

         boost::shared_ptr<dbCommon::entities::CPlugin> CPlugin::getSystemInstance()
         {
            dbCommon::adapters::CPluginAdapter adapter;

            auto qSelect = m_databaseRequester->newQuery();

            qSelect.Select().
                   From(dbCommon::CPluginTable::getTableName()).
                   Where(dbCommon::CPluginTable::getCategoryColumnName(), CQUERY_OP_EQUAL, dbCommon::entities::EPluginCategory::kSystem);

            m_databaseRequester->queryEntities(&adapter, qSelect);
            if (adapter.getResults().empty())
            {
               // Plugin not found
               throw shared::exception::CEmptyResult("System plugin not found in database");
            }
            return adapter.getResults().at(0);
         }


         std::vector<boost::shared_ptr<dbCommon::entities::CPlugin>> CPlugin::getInstances()
         {
            dbCommon::adapters::CPluginAdapter adapter;

            auto qSelect = m_databaseRequester->newQuery();
            qSelect.Select().From(dbCommon::CPluginTable::getTableName());

            m_databaseRequester->queryEntities(&adapter, qSelect);
            return adapter.getResults();
         }

         void CPlugin::updateInstance(const dbCommon::entities::CPlugin& updatedPluginData)
         {
            auto qUpdate = m_databaseRequester->newQuery();

            if (!updatedPluginData.Id.isDefined())
               throw dbCommon::CDatabaseException("Need an id to update");

            //update name
            if (updatedPluginData.DisplayName.isDefined())
            {
               qUpdate.Clear().Update(dbCommon::CPluginTable::getTableName()).
                      Set(dbCommon::CPluginTable::getDisplayNameColumnName(), updatedPluginData.DisplayName()).
                      Where(dbCommon::CPluginTable::getIdColumnName(), CQUERY_OP_EQUAL, updatedPluginData.Id());

               if (m_databaseRequester->queryStatement(qUpdate) <= 0)
                  throw dbCommon::CDatabaseException("Failed to update name");
            }

            //update configuration
            if (updatedPluginData.Configuration.isDefined())
            {
               qUpdate.Clear().Update(dbCommon::CPluginTable::getTableName()).
                      Set(dbCommon::CPluginTable::getConfigurationColumnName(), updatedPluginData.Configuration()).
                      Where(dbCommon::CPluginTable::getIdColumnName(), CQUERY_OP_EQUAL, updatedPluginData.Id());

               if (m_databaseRequester->queryStatement(qUpdate) <= 0)
                  throw dbCommon::CDatabaseException("Failed to update configuration");
            }

            //update autostart
            if (updatedPluginData.AutoStart.isDefined())
            {
               qUpdate.Clear().Update(dbCommon::CPluginTable::getTableName()).
                      Set(dbCommon::CPluginTable::getAutoStartColumnName(), updatedPluginData.AutoStart()).
                      Where(dbCommon::CPluginTable::getIdColumnName(), CQUERY_OP_EQUAL, updatedPluginData.Id());

               if (m_databaseRequester->queryStatement(qUpdate) <= 0)
                  throw dbCommon::CDatabaseException("Failed to update enabled field");
            }
         }

         void CPlugin::removeInstance(int pluginId)
         {
            auto qUpdate = m_databaseRequester->newQuery();
            qUpdate.DeleteFrom(dbCommon::CPluginTable::getTableName()).
                   Where(dbCommon::CPluginTable::getIdColumnName(), CQUERY_OP_EQUAL, pluginId);

            if (m_databaseRequester->queryStatement(qUpdate) <= 0)
               throw shared::exception::CEmptyResult("No lines affected");
         }

         void CPlugin::disableAutoStartForAllPluginInstances(const std::string& pluginName)
         {
            auto qUpdate = m_databaseRequester->newQuery();
            qUpdate.Update(dbCommon::CPluginTable::getTableName()).
                   Set(dbCommon::CPluginTable::getAutoStartColumnName(), false).
                   Where(dbCommon::CPluginTable::getTypeColumnName(), CQUERY_OP_EQUAL, pluginName);

            m_databaseRequester->queryStatement(qUpdate);
         }

         // [END] IPluginRequester implementation
      } //namespace requesters
   } //namespace common
} //namespace database 


