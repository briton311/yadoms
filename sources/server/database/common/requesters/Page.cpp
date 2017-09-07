#include "stdafx.h"
#include "Page.h"
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
         CPage::CPage(boost::shared_ptr<dbCommon::IDatabaseRequester> databaseRequester)
            : m_databaseRequester(databaseRequester)
         {
         }

         CPage::~CPage()
         {
         }


         // IPageRequester implementation
         int CPage::addPage(const dbCommon::entities::CPage& page)
         {
            auto qInsert = m_databaseRequester->newQuery();
            if (page.Id.isDefined())
            {
               qInsert.InsertInto(dbCommon::CPageTable::getTableName(), dbCommon::CPageTable::getIdColumnName(), dbCommon::CPageTable::getNameColumnName(), dbCommon::CPageTable::getPageOrderColumnName()).
                      Values(page.Id(), page.Name(), page.PageOrder());
            }
            else
            {
               qInsert.InsertInto(dbCommon::CPageTable::getTableName(), dbCommon::CPageTable::getNameColumnName(), dbCommon::CPageTable::getPageOrderColumnName()).
                      Values(page.Name(), page.PageOrder());
            }

            if (m_databaseRequester->queryStatement(qInsert) <= 0)
               throw shared::exception::CEmptyResult("No lines affected");

            auto qSelect = m_databaseRequester->newQuery();
            qSelect.Select(dbCommon::CPageTable::getIdColumnName()).
                   From(dbCommon::CPageTable::getTableName()).
                   Where(dbCommon::CPageTable::getNameColumnName(), CQUERY_OP_EQUAL, page.Name()).
                   OrderBy(dbCommon::CPageTable::getIdColumnName(), dbCommon::CQuery::kDesc);

            dbCommon::adapters::CSingleValueAdapter<int> adapter;
            m_databaseRequester->queryEntities(&adapter, qSelect);
            if (adapter.getResults().size() >= 1)
               return adapter.getResults()[0];
            throw shared::exception::CEmptyResult("Cannot retrieve inserted Page");
         }


         int CPage::addPage(const std::string& name, const int pageOrder)
         {
            auto qInsert = m_databaseRequester->newQuery();
            qInsert.InsertInto(dbCommon::CPageTable::getTableName(), dbCommon::CPageTable::getNameColumnName(), dbCommon::CPageTable::getPageOrderColumnName()).
                   Values(name, pageOrder);
            if (m_databaseRequester->queryStatement(qInsert) <= 0)
               throw shared::exception::CEmptyResult("No lines affected");

            auto qSelect = m_databaseRequester->newQuery();
            qSelect.Select(dbCommon::CPageTable::getIdColumnName()).
                   From(dbCommon::CPageTable::getTableName()).
                   Where(dbCommon::CPageTable::getNameColumnName(), CQUERY_OP_EQUAL, name).
                   OrderBy(dbCommon::CPageTable::getIdColumnName(), dbCommon::CQuery::kDesc);

            dbCommon::adapters::CSingleValueAdapter<int> adapter;
            m_databaseRequester->queryEntities(&adapter, qSelect);
            if (adapter.getResults().size() >= 1)
               return adapter.getResults()[0];
            throw shared::exception::CEmptyResult("Cannot retrieve inserted Page");
         }


         boost::shared_ptr<dbCommon::entities::CPage> CPage::getPage(int pageId)
         {
            auto qSelect = m_databaseRequester->newQuery();
            qSelect.Select().
                   From(dbCommon::CPageTable::getTableName()).
                   Where(dbCommon::CPageTable::getIdColumnName(), CQUERY_OP_EQUAL, pageId);

            dbCommon::adapters::CPageAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, qSelect);
            if (adapter.getResults().size() >= 1)
               return adapter.getResults()[0];

            std::string sEx = (boost::format("Cannot retrieve Page Id=%1% in database") % pageId).str();
            throw shared::exception::CEmptyResult(sEx);
         }

         std::vector<boost::shared_ptr<dbCommon::entities::CPage>> CPage::getPages()
         {
            auto qSelect = m_databaseRequester->newQuery();
            qSelect.Select().
                   From(dbCommon::CPageTable::getTableName()).
                   OrderBy(dbCommon::CPageTable::getPageOrderColumnName());

            dbCommon::adapters::CPageAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, qSelect);
            return adapter.getResults();
         }

         void CPage::updatePage(int pageId, const std::string& name, const int pageOrder)
         {
            auto qUpdate = m_databaseRequester->newQuery();
            qUpdate.Update(dbCommon::CPageTable::getTableName()).
                   Set(dbCommon::CPageTable::getNameColumnName(), name, dbCommon::CPageTable::getPageOrderColumnName(), pageOrder).
                   Where(dbCommon::CPageTable::getIdColumnName(), CQUERY_OP_EQUAL, pageId);

            if (m_databaseRequester->queryStatement(qUpdate) <= 0)
               throw shared::exception::CEmptyResult("No lines affected");
         }

         void CPage::removePage(int pageId)
         {
            auto qDelete = m_databaseRequester->newQuery();
            qDelete.DeleteFrom(dbCommon::CPageTable::getTableName()).
                   Where(dbCommon::CPageTable::getIdColumnName(), CQUERY_OP_EQUAL, pageId);
            if (m_databaseRequester->queryStatement(qDelete) <= 0)
               throw shared::exception::CEmptyResult("No lines affected");
         }

         void CPage::removeAllPages()
         {
            auto qDelete = m_databaseRequester->newQuery();
            qDelete.DeleteFrom(dbCommon::CPageTable::getTableName());
            m_databaseRequester->queryStatement(qDelete);
         }

         // [END] IPageRequester implementation
      } //namespace requesters
   } //namespace common
} //namespace database 


