#include "stdafx.h"
#include "Keyword.h"
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
         CKeyword::CKeyword(boost::shared_ptr<dbCommon::IDatabaseRequester> databaseRequester)
            : m_databaseRequester(databaseRequester)
         {
         }

         CKeyword::~CKeyword()
         {
         }

         void CKeyword::addKeyword(const dbCommon::entities::CKeyword& newKeyword)
         {
            auto qSelect = m_databaseRequester->newQuery();

            qSelect.SelectCount().
                   From(dbCommon::CKeywordTable::getTableName()).
                   Where(dbCommon::CKeywordTable::getDeviceIdColumnName(), CQUERY_OP_EQUAL, newKeyword.DeviceId()).
                   And(dbCommon::CKeywordTable::getNameColumnName(), CQUERY_OP_EQUAL, newKeyword.Name());

            if (m_databaseRequester->queryCount(qSelect) == 0)
            {
               //create the database entry with needed fields
               auto qInsert = m_databaseRequester->newQuery();
               qInsert.InsertInto(dbCommon::CKeywordTable::getTableName(), dbCommon::CKeywordTable::getDeviceIdColumnName(), dbCommon::CKeywordTable::getCapacityNameColumnName(), dbCommon::CKeywordTable::getAccessModeColumnName(), dbCommon::CKeywordTable::getNameColumnName(), dbCommon::CKeywordTable::getTypeColumnName(), dbCommon::CKeywordTable::getMeasureColumnName()).
                      Values(newKeyword.DeviceId(), newKeyword.CapacityName(), newKeyword.AccessMode(), newKeyword.Name(), newKeyword.Type(), newKeyword.Measure());

               if (m_databaseRequester->queryStatement(qInsert) <= 0)
                  throw shared::exception::CEmptyResult("Fail to insert keyword into table");

               //update fields
               auto friendlyName = newKeyword.Name();
               if (newKeyword.FriendlyName.isDefined())
                  friendlyName = newKeyword.FriendlyName();

               auto update = m_databaseRequester->newQuery();
               update.Update(dbCommon::CKeywordTable::getTableName()).Set(dbCommon::CKeywordTable::getFriendlyNameColumnName(), friendlyName).
                     Where(dbCommon::CKeywordTable::getDeviceIdColumnName(), CQUERY_OP_EQUAL, newKeyword.DeviceId()).
                     And(dbCommon::CKeywordTable::getNameColumnName(), CQUERY_OP_EQUAL, newKeyword.Name());

               if (m_databaseRequester->queryStatement(update) <= 0)
                  throw shared::exception::CEmptyResult("Fail to update FriendlyName field");


               if (newKeyword.Details.isDefined())
               {
                  auto updateDetails = m_databaseRequester->newQuery();
                  updateDetails.Update(dbCommon::CKeywordTable::getTableName()).Set(dbCommon::CKeywordTable::getDetailsColumnName(), newKeyword.Details()).
                               Where(dbCommon::CKeywordTable::getDeviceIdColumnName(), CQUERY_OP_EQUAL, newKeyword.DeviceId()).
                               And(dbCommon::CKeywordTable::getNameColumnName(), CQUERY_OP_EQUAL, newKeyword.Name());

                  if (m_databaseRequester->queryStatement(updateDetails) <= 0)
                     throw shared::exception::CEmptyResult("Fail to update Details field");
               }

               if (newKeyword.Units.isDefined())
               {
                  auto updateUnits = m_databaseRequester->newQuery();
                  updateUnits.Update(dbCommon::CKeywordTable::getTableName()).Set(dbCommon::CKeywordTable::getUnitsColumnName(), newKeyword.Units()).
                             Where(dbCommon::CKeywordTable::getDeviceIdColumnName(), CQUERY_OP_EQUAL, newKeyword.DeviceId()).
                             And(dbCommon::CKeywordTable::getNameColumnName(), CQUERY_OP_EQUAL, newKeyword.Name());

                  if (m_databaseRequester->queryStatement(updateUnits) <= 0)
                     throw shared::exception::CEmptyResult("Fail to update Units field");
               }

               if (newKeyword.TypeInfo.isDefined())
               {
                  auto updateValues = m_databaseRequester->newQuery();
                  updateValues.Update(dbCommon::CKeywordTable::getTableName()).Set(dbCommon::CKeywordTable::getTypeInfoColumnName(), newKeyword.TypeInfo()).
                              Where(dbCommon::CKeywordTable::getDeviceIdColumnName(), CQUERY_OP_EQUAL, newKeyword.DeviceId()).
                              And(dbCommon::CKeywordTable::getNameColumnName(), CQUERY_OP_EQUAL, newKeyword.Name());

                  if (m_databaseRequester->queryStatement(updateValues) <= 0)
                     throw shared::exception::CEmptyResult("Fail to update Values field");
               }
            }
            else
            {
               //already exist, do nothing
            }
         }

         boost::shared_ptr<dbCommon::entities::CKeyword> CKeyword::getKeyword(int deviceId, const std::string& keyword) const
         {
            dbCommon::adapters::CKeywordAdapter adapter;

            auto qSelect = m_databaseRequester->newQuery();

            qSelect.Select().
                   From(dbCommon::CKeywordTable::getTableName()).
                   Where(dbCommon::CKeywordTable::getDeviceIdColumnName(), CQUERY_OP_EQUAL, deviceId).
                   And(dbCommon::CKeywordTable::getNameColumnName(), CQUERY_OP_EQUAL, keyword);

            m_databaseRequester->queryEntities(&adapter, qSelect);
            if (adapter.getResults().empty())
               throw shared::exception::CEmptyResult((boost::format("Keyword name %1% for device %2% not found in database") % keyword % deviceId).str());

            return adapter.getResults().at(0);
         }

         boost::shared_ptr<dbCommon::entities::CKeyword> CKeyword::getKeyword(int keywordId) const
         {
            dbCommon::adapters::CKeywordAdapter adapter;

            auto qSelect = m_databaseRequester->newQuery();
            qSelect.Select().
                   From(dbCommon::CKeywordTable::getTableName()).
                   Where(dbCommon::CKeywordTable::getIdColumnName(), CQUERY_OP_EQUAL, keywordId);

            m_databaseRequester->queryEntities(&adapter, qSelect);
            if (adapter.getResults().empty())
               throw shared::exception::CEmptyResult((boost::format("Keyword id %1% not found in database") % keywordId).str());

            return adapter.getResults().at(0);
         }

         std::vector<boost::shared_ptr<dbCommon::entities::CKeyword> > CKeyword::getKeywordIdFromFriendlyName(int deviceId, const std::string& friendlyName) const
         {
            auto qSelect = m_databaseRequester->newQuery();
            qSelect.Select().
                   From(dbCommon::CKeywordTable::getTableName()).
                   Where(dbCommon::CKeywordTable::getDeviceIdColumnName(), CQUERY_OP_EQUAL, deviceId).
                   And(dbCommon::CKeywordTable::getFriendlyNameColumnName(), CQUERY_OP_EQUAL, friendlyName);

            dbCommon::adapters::CKeywordAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, qSelect);
            return adapter.getResults();
         }

         std::vector<boost::shared_ptr<dbCommon::entities::CKeyword> > CKeyword::getKeywords(int deviceId) const
         {
            dbCommon::adapters::CKeywordAdapter adapter;
            auto qSelect = m_databaseRequester->newQuery();
            qSelect.Select().
                   From(dbCommon::CKeywordTable::getTableName()).
                   Where(dbCommon::CKeywordTable::getDeviceIdColumnName(), CQUERY_OP_EQUAL, deviceId);
            m_databaseRequester->queryEntities(&adapter, qSelect);
            return adapter.getResults();
         }

         std::vector<boost::shared_ptr<dbCommon::entities::CKeyword> > CKeyword::getKeywordsMatchingCapacity(const std::string& capacity) const
         {
            dbCommon::adapters::CKeywordAdapter adapter;
            auto qSelect = m_databaseRequester->newQuery();
            qSelect.Select().
                   From(dbCommon::CKeywordTable::getTableName()).
                   Where(dbCommon::CKeywordTable::getCapacityNameColumnName(), CQUERY_OP_EQUAL, capacity);
            m_databaseRequester->queryEntities(&adapter, qSelect);
            return adapter.getResults();
         }

         std::vector<boost::shared_ptr<dbCommon::entities::CKeyword> > CKeyword::getAllKeywords() const
         {
            dbCommon::adapters::CKeywordAdapter adapter;
            auto qSelect = m_databaseRequester->newQuery();
            qSelect.Select().From(dbCommon::CKeywordTable::getTableName());
            m_databaseRequester->queryEntities(&adapter, qSelect);
            return adapter.getResults();
         }

         std::vector<boost::shared_ptr<dbCommon::entities::CKeyword> > CKeyword::getDeviceKeywordsWithCapacity(int deviceId, const std::string& capacityName, const shared::plugin::yPluginApi::EKeywordAccessMode& accessMode) const
         {
            dbCommon::adapters::CKeywordAdapter adapter;
            auto qSelect = m_databaseRequester->newQuery();
            qSelect.Select().
                   From(dbCommon::CKeywordTable::getTableName()).
                   Where(dbCommon::CKeywordTable::getDeviceIdColumnName(), CQUERY_OP_EQUAL, deviceId).
                   And(dbCommon::CKeywordTable::getCapacityNameColumnName(), CQUERY_OP_EQUAL, capacityName).
                   And(dbCommon::CKeywordTable::getAccessModeColumnName(), CQUERY_OP_EQUAL, accessMode);

            m_databaseRequester->queryEntities(&adapter, qSelect);
            return adapter.getResults();
         }

         void CKeyword::updateKeywordBlacklistState(int keywordId, const bool blacklist)
         {
            auto keywordToUpdate = getKeyword(keywordId);
            if (!keywordToUpdate)
               throw shared::exception::CEmptyResult("Can not find keyword");

            //insert in db
            auto qUpdate = m_databaseRequester->newQuery();
            qUpdate.Update(dbCommon::CKeywordTable::getTableName()).
               Set(dbCommon::CKeywordTable::getBlacklistColumnName(), blacklist).
               Where(dbCommon::CKeywordTable::getIdColumnName(), CQUERY_OP_EQUAL, keywordId);

            if (m_databaseRequester->queryStatement(qUpdate) <= 0)
               throw shared::exception::CEmptyResult("Fail to update keyword blacklist state");
         }


         void CKeyword::removeKeyword(const int keywordId)
         {
            //delete keyword
            auto qDelete = m_databaseRequester->newQuery();
            qDelete.DeleteFrom(dbCommon::CKeywordTable::getTableName()).
                   Where(dbCommon::CKeywordTable::getIdColumnName(), CQUERY_OP_EQUAL, keywordId);
            if (m_databaseRequester->queryStatement(qDelete) <= 0)
               throw shared::exception::CEmptyResult("No lines affected");
         }

         void CKeyword::updateKeywordFriendlyName(int keywordId, const std::string& newFriendlyName)
         {
            //get a good name
            if (newFriendlyName != std::string())
            {
               auto keywordToUpdate = getKeyword(keywordId);
               if (!keywordToUpdate)
                  throw shared::exception::CEmptyResult("Can not find keyword");
               
               //insert in db
               auto qUpdate = m_databaseRequester->newQuery();
               qUpdate.Update(dbCommon::CKeywordTable::getTableName()).
                      Set(dbCommon::CKeywordTable::getFriendlyNameColumnName(), newFriendlyName).
                      Where(dbCommon::CKeywordTable::getIdColumnName(), CQUERY_OP_EQUAL, keywordId);

               if (m_databaseRequester->queryStatement(qUpdate) <= 0)
                  throw shared::exception::CEmptyResult("Fail to update keyword friendlyName");
            }
         }
      } //namespace requesters
   } //namespace common
} //namespace database 


