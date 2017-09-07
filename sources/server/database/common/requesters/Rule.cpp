#include "stdafx.h"
#include "Rule.h"
#include <dbCommon/adapters/DatabaseAdapters.h>
#include <dbCommon/DatabaseTables.h>
#include <dbCommon/Query.h>
#include <shared/exception/EmptyResult.hpp>
#include <dbCommon/adapters/SingleValueAdapter.hpp>


namespace database
{
   namespace common
   {
      namespace requesters
      {
         CRule::CRule(boost::shared_ptr<dbCommon::IDatabaseRequester> databaseRequester)
            : m_databaseRequester(databaseRequester)
         {
         }

         CRule::~CRule()
         {
         }

         // IRuleRequester implementation

         std::vector<boost::shared_ptr<dbCommon::entities::CRule>> CRule::getRules() const
         {
            auto qSelect = m_databaseRequester->newQuery();
            qSelect.Select().
                   From(dbCommon::CRuleTable::getTableName());

            dbCommon::adapters::CRuleAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, qSelect);
            return adapter.getResults();
         }

         std::vector<boost::shared_ptr<dbCommon::entities::CRule>> CRule::getRules(const std::string& interpreterName) const
         {
            auto qSelect = m_databaseRequester->newQuery();
            qSelect.Select().
                   From(dbCommon::CRuleTable::getTableName()).
                   Where(dbCommon::CRuleTable::getInterpreterColumnName(), CQUERY_OP_EQUAL, interpreterName);

            dbCommon::adapters::CRuleAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, qSelect);
            return adapter.getResults();
         }

         boost::shared_ptr<dbCommon::entities::CRule> CRule::getRule(int ruleId) const
         {
            dbCommon::adapters::CRuleAdapter adapter;

            auto qSelect = m_databaseRequester->newQuery();

            qSelect.Select().
                   From(dbCommon::CRuleTable::getTableName()).
                   Where(dbCommon::CRuleTable::getIdColumnName(), CQUERY_OP_EQUAL, ruleId);

            m_databaseRequester->queryEntities(&adapter, qSelect);
            if (adapter.getResults().empty())
            {
               // Rule not found
               throw shared::exception::CEmptyResult((boost::format("Rule Id %1% not found in database") % ruleId).str());
            }
            return adapter.getResults().at(0);
         }

         int CRule::addRule(boost::shared_ptr<const dbCommon::entities::CRule> ruleData)
         {
            auto qInsert = m_databaseRequester->newQuery();

            qInsert.InsertInto(dbCommon::CRuleTable::getTableName(),
                               dbCommon::CRuleTable::getNameColumnName(),
                               dbCommon::CRuleTable::getDescriptionColumnName(),
                               dbCommon::CRuleTable::getInterpreterColumnName(),
                               dbCommon::CRuleTable::getEditorColumnName(),
                               dbCommon::CRuleTable::getModelColumnName(),
                               dbCommon::CRuleTable::getContentColumnName(),
                               dbCommon::CRuleTable::getConfigurationColumnName(),
                               dbCommon::CRuleTable::getStateColumnName()).
                   Values(ruleData->Name(),
                          ruleData->Description(),
                          ruleData->Interpreter(),
                          ruleData->Editor(),
                          ruleData->Model(),
                          ruleData->Content(),
                          ruleData->Configuration(),
                          ruleData->State.isDefined() ? ruleData->State() : dbCommon::entities::ERuleState::kStopped);

            if (m_databaseRequester->queryStatement(qInsert) <= 0)
               throw shared::exception::CEmptyResult("No lines affected");


            auto qSelect = m_databaseRequester->newQuery();
            qSelect.Select(dbCommon::CRuleTable::getIdColumnName()).
                   From(dbCommon::CRuleTable::getTableName()).
                   Where(dbCommon::CRuleTable::getNameColumnName(), CQUERY_OP_EQUAL, ruleData->Name()).
                   OrderBy(dbCommon::CRuleTable::getIdColumnName(), dbCommon::CQuery::kDesc);

            dbCommon::adapters::CSingleValueAdapter<int> adapter;
            m_databaseRequester->queryEntities(&adapter, qSelect);
            if (adapter.getResults().size() >= 1)
            {
               //search for inserted rule
               auto createdId = adapter.getResults()[0];

               //update all optional flags 
               auto qUpdate = m_databaseRequester->newQuery();

               //update error message
               if (ruleData->ErrorMessage.isDefined())
               {
                  qUpdate.Clear().Update(dbCommon::CRuleTable::getTableName()).
                         Set(dbCommon::CRuleTable::getErrorMessageColumnName(), ruleData->ErrorMessage()).
                         Where(dbCommon::CRuleTable::getIdColumnName(), CQUERY_OP_EQUAL, createdId);

                  if (m_databaseRequester->queryStatement(qUpdate) <= 0)
                     throw dbCommon::CDatabaseException("Failed to update error message field");
               }

               //update start date
               if (ruleData->StartDate.isDefined())
               {
                  qUpdate.Clear().Update(dbCommon::CRuleTable::getTableName()).
                         Set(dbCommon::CRuleTable::getStartDateColumnName(), ruleData->StartDate()).
                         Where(dbCommon::CRuleTable::getIdColumnName(), CQUERY_OP_EQUAL, createdId);

                  if (m_databaseRequester->queryStatement(qUpdate) <= 0)
                     throw dbCommon::CDatabaseException("Failed to update start date field");
               }

               //update stop date
               if (ruleData->StopDate.isDefined())
               {
                  qUpdate.Clear().Update(dbCommon::CRuleTable::getTableName()).
                         Set(dbCommon::CRuleTable::getStopDateColumnName(), ruleData->StopDate()).
                         Where(dbCommon::CRuleTable::getIdColumnName(), CQUERY_OP_EQUAL, createdId);

                  if (m_databaseRequester->queryStatement(qUpdate) <= 0)
                     throw dbCommon::CDatabaseException("Failed to update stop date field");
               }

               return createdId;
            }

            throw shared::exception::CEmptyResult("Cannot retrieve inserted Rule");
         }

         void CRule::updateRule(boost::shared_ptr<const dbCommon::entities::CRule> ruleData)
         {
            auto qUpdate = m_databaseRequester->newQuery();

            if (!ruleData->Id.isDefined())
               throw dbCommon::CDatabaseException("Need an id to update");

            //update name
            if (ruleData->Name.isDefined())
            {
               qUpdate.Clear().Update(dbCommon::CRuleTable::getTableName()).
                      Set(dbCommon::CRuleTable::getNameColumnName(), ruleData->Name()).
                      Where(dbCommon::CRuleTable::getIdColumnName(), CQUERY_OP_EQUAL, ruleData->Id());

               if (m_databaseRequester->queryStatement(qUpdate) <= 0)
                  throw dbCommon::CDatabaseException("Failed to update name");
            }

            //update configuration
            if (ruleData->Description.isDefined())
            {
               qUpdate.Clear().Update(dbCommon::CRuleTable::getTableName()).
                      Set(dbCommon::CRuleTable::getDescriptionColumnName(), ruleData->Description()).
                      Where(dbCommon::CRuleTable::getIdColumnName(), CQUERY_OP_EQUAL, ruleData->Id());

               if (m_databaseRequester->queryStatement(qUpdate) <= 0)
                  throw dbCommon::CDatabaseException("Failed to update description");
            }

            //update interpreter
            if (ruleData->Interpreter.isDefined())
            {
               qUpdate.Clear().Update(dbCommon::CRuleTable::getTableName()).
                      Set(dbCommon::CRuleTable::getInterpreterColumnName(), ruleData->Interpreter()).
                      Where(dbCommon::CRuleTable::getIdColumnName(), CQUERY_OP_EQUAL, ruleData->Id());

               if (m_databaseRequester->queryStatement(qUpdate) <= 0)
                  throw dbCommon::CDatabaseException("Failed to update interpreter");
            }

            //update editor
            if (ruleData->Editor.isDefined())
            {
               qUpdate.Clear().Update(dbCommon::CRuleTable::getTableName()).
                      Set(dbCommon::CRuleTable::getEditorColumnName(), ruleData->Editor()).
                      Where(dbCommon::CRuleTable::getIdColumnName(), CQUERY_OP_EQUAL, ruleData->Id());

               if (m_databaseRequester->queryStatement(qUpdate) <= 0)
                  throw dbCommon::CDatabaseException("Failed to update editor");
            }

            //update content
            if (ruleData->Content.isDefined())
            {
               qUpdate.Clear().Update(dbCommon::CRuleTable::getTableName()).
                      Set(dbCommon::CRuleTable::getContentColumnName(), ruleData->Content()).
                      Where(dbCommon::CRuleTable::getIdColumnName(), CQUERY_OP_EQUAL, ruleData->Id());

               if (m_databaseRequester->queryStatement(qUpdate) <= 0)
                  throw dbCommon::CDatabaseException("Failed to update content field");
            }

            //update configuration
            if (ruleData->Configuration.isDefined())
            {
               qUpdate.Clear().Update(dbCommon::CRuleTable::getTableName()).
                      Set(dbCommon::CRuleTable::getConfigurationColumnName(), ruleData->Configuration()).
                      Where(dbCommon::CRuleTable::getIdColumnName(), CQUERY_OP_EQUAL, ruleData->Id());

               if (m_databaseRequester->queryStatement(qUpdate) <= 0)
                  throw dbCommon::CDatabaseException("Failed to update Configuration field");
            }

            //update autoStart flag
            if (ruleData->AutoStart.isDefined())
            {
               qUpdate.Clear().Update(dbCommon::CRuleTable::getTableName()).
                      Set(dbCommon::CRuleTable::getAutoStartColumnName(), ruleData->AutoStart()).
                      Where(dbCommon::CRuleTable::getIdColumnName(), CQUERY_OP_EQUAL, ruleData->Id());

               if (m_databaseRequester->queryStatement(qUpdate) <= 0)
                  throw dbCommon::CDatabaseException("Failed to update autoStart flag field");
            }

            //update state flag
            if (ruleData->State.isDefined())
            {
               qUpdate.Clear().Update(dbCommon::CRuleTable::getTableName()).
                      Set(dbCommon::CRuleTable::getStateColumnName(), ruleData->State()).
                      Where(dbCommon::CRuleTable::getIdColumnName(), CQUERY_OP_EQUAL, ruleData->Id());

               if (m_databaseRequester->queryStatement(qUpdate) <= 0)
                  throw dbCommon::CDatabaseException("Failed to update state flag field");
            }

            //update error message
            if (ruleData->ErrorMessage.isDefined())
            {
               qUpdate.Clear().Update(dbCommon::CRuleTable::getTableName()).
                      Set(dbCommon::CRuleTable::getErrorMessageColumnName(), ruleData->ErrorMessage()).
                      Where(dbCommon::CRuleTable::getIdColumnName(), CQUERY_OP_EQUAL, ruleData->Id());

               if (m_databaseRequester->queryStatement(qUpdate) <= 0)
                  throw dbCommon::CDatabaseException("Failed to update error message field");
            }

            //update start date
            if (ruleData->StartDate.isDefined())
            {
               qUpdate.Clear().Update(dbCommon::CRuleTable::getTableName()).
                      Set(dbCommon::CRuleTable::getStartDateColumnName(), ruleData->StartDate()).
                      Where(dbCommon::CRuleTable::getIdColumnName(), CQUERY_OP_EQUAL, ruleData->Id());

               if (m_databaseRequester->queryStatement(qUpdate) <= 0)
                  throw dbCommon::CDatabaseException("Failed to update start date field");
            }

            //update stop date
            if (ruleData->StopDate.isDefined())
            {
               qUpdate.Clear().Update(dbCommon::CRuleTable::getTableName()).
                      Set(dbCommon::CRuleTable::getStopDateColumnName(), ruleData->StopDate()).
                      Where(dbCommon::CRuleTable::getIdColumnName(), CQUERY_OP_EQUAL, ruleData->Id());

               if (m_databaseRequester->queryStatement(qUpdate) <= 0)
                  throw dbCommon::CDatabaseException("Failed to update stop date field");
            }
         }

         void CRule::deleteRule(int ruleId)
         {
            auto qUpdate = m_databaseRequester->newQuery();
            qUpdate.DeleteFrom(dbCommon::CRuleTable::getTableName()).
                   Where(dbCommon::CRuleTable::getIdColumnName(), CQUERY_OP_EQUAL, ruleId);

            if (m_databaseRequester->queryStatement(qUpdate) <= 0)
               throw shared::exception::CEmptyResult("No lines affected");
         }

         // [END] IRuleRequester implementation
      } //namespace requesters
   } //namespace common
} //namespace database 


