#include "stdafx.h"
#include "Widget.h"
#include <shared/exception/EmptyResult.hpp>
#include <dbCommon/DatabaseTables.h>
#include <dbCommon/Query.h>
#include <dbCommon/DatabaseException.hpp>
#include <dbCommon/adapters/DatabaseAdapters.h>
#include <dbCommon/adapters/SingleValueAdapter.hpp>

namespace database
{
   namespace common
   {
      namespace requesters
      {
         CWidget::CWidget(boost::shared_ptr<dbCommon::IDatabaseRequester> databaseRequester)
            : m_databaseRequester(databaseRequester)
         {
         }

         CWidget::~CWidget()
         {
         }


         // IWidgetRequester implementation
         int CWidget::addWidget(const dbCommon::entities::CWidget& newWidget)
         {
            auto qInsert = m_databaseRequester->newQuery();
            if (newWidget.Id() != 0)
            {
               qInsert.InsertInto(dbCommon::CWidgetTable::getTableName(), dbCommon::CWidgetTable::getIdColumnName(), dbCommon::CWidgetTable::getIdPageColumnName(), dbCommon::CWidgetTable::getTypeColumnName(), dbCommon::CWidgetTable::getSizeXColumnName(), dbCommon::CWidgetTable::getSizeYColumnName(), dbCommon::CWidgetTable::getPositionColumnName(), dbCommon::CWidgetTable::getTitleColumnName(), dbCommon::CWidgetTable::getConfigurationColumnName()).
                      Values(newWidget.Id(), newWidget.IdPage(), newWidget.Type(), newWidget.SizeX(), newWidget.SizeY(), newWidget.Position(), newWidget.Title(), newWidget.Configuration());
            }
            else
            {
               qInsert.InsertInto(dbCommon::CWidgetTable::getTableName(), dbCommon::CWidgetTable::getIdPageColumnName(), dbCommon::CWidgetTable::getTypeColumnName(), dbCommon::CWidgetTable::getSizeXColumnName(), dbCommon::CWidgetTable::getSizeYColumnName(), dbCommon::CWidgetTable::getPositionColumnName(), dbCommon::CWidgetTable::getTitleColumnName(), dbCommon::CWidgetTable::getConfigurationColumnName()).
                      Values(newWidget.IdPage(), newWidget.Type(), newWidget.SizeX(), newWidget.SizeY(), newWidget.Position(), newWidget.Title(), newWidget.Configuration());
            }
            if (m_databaseRequester->queryStatement(qInsert) <= 0)
               throw shared::exception::CEmptyResult("No lines affected");

            auto qSelect = m_databaseRequester->newQuery();
            qSelect.Select(dbCommon::CWidgetTable::getIdColumnName()).
                   From(dbCommon::CWidgetTable::getTableName()).
                   Where(dbCommon::CWidgetTable::getIdPageColumnName(), CQUERY_OP_EQUAL, newWidget.IdPage()).
                   And(dbCommon::CWidgetTable::getTypeColumnName(), CQUERY_OP_EQUAL, newWidget.Type()).
                   And(dbCommon::CWidgetTable::getSizeXColumnName(), CQUERY_OP_EQUAL, newWidget.SizeX()).
                   And(dbCommon::CWidgetTable::getSizeYColumnName(), CQUERY_OP_EQUAL, newWidget.SizeY()).
                   And(dbCommon::CWidgetTable::getPositionColumnName(), CQUERY_OP_EQUAL, newWidget.Position()).
                   And(dbCommon::CWidgetTable::getTitleColumnName(), CQUERY_OP_EQUAL, newWidget.Title()).
                   And(dbCommon::CWidgetTable::getConfigurationColumnName(), CQUERY_OP_EQUAL, newWidget.Configuration()).
                   OrderBy(dbCommon::CWidgetTable::getIdColumnName(), dbCommon::CQuery::kDesc);

            dbCommon::adapters::CSingleValueAdapter<int> adapter;
            m_databaseRequester->queryEntities(&adapter, qSelect);
            if (adapter.getResults().size() >= 1)
               return adapter.getResults()[0];
            throw shared::exception::CEmptyResult("Cannot retrieve inserted Widget");
         }

         boost::shared_ptr<dbCommon::entities::CWidget> CWidget::getWidget(int widgetId)
         {
            auto qSelect = m_databaseRequester->newQuery();
            qSelect.Select().
                   From(dbCommon::CWidgetTable::getTableName()).
                   Where(dbCommon::CWidgetTable::getIdColumnName(), CQUERY_OP_EQUAL, widgetId);

            dbCommon::adapters::CWidgetAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, qSelect);
            if (adapter.getResults().size() >= 1)
               return adapter.getResults()[0];

            auto sEx = (boost::format("Cannot retrieve Widget Id=%1% in database") % widgetId).str();
            throw shared::exception::CEmptyResult(sEx);
         }

         std::vector<boost::shared_ptr<dbCommon::entities::CWidget>> CWidget::getWidgets()
         {
            auto qSelect = m_databaseRequester->newQuery();
            qSelect.Select().
                   From(dbCommon::CWidgetTable::getTableName()).
                   OrderBy(dbCommon::CWidgetTable::getPositionColumnName());

            dbCommon::adapters::CWidgetAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, qSelect);
            return adapter.getResults();
         }

         std::vector<boost::shared_ptr<dbCommon::entities::CWidget>> CWidget::getWidgetsForPage(int pageId)
         {
            auto qSelect = m_databaseRequester->newQuery();
            qSelect.Select().
                   From(dbCommon::CWidgetTable::getTableName()).
                   Where(dbCommon::CWidgetTable::getIdPageColumnName(), CQUERY_OP_EQUAL, pageId).
                   OrderBy(dbCommon::CWidgetTable::getPositionColumnName());

            dbCommon::adapters::CWidgetAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, qSelect);
            return adapter.getResults();
         }

         void CWidget::updateWidgetConfiguration(int widgetId, const std::string& newConfiguration)
         {
            auto qUpdate = m_databaseRequester->newQuery();
            qUpdate.Update(dbCommon::CPluginTable::getTableName()).
                   Set(dbCommon::CWidgetTable::getConfigurationColumnName(), newConfiguration).
                   Where(dbCommon::CWidgetTable::getIdColumnName(), CQUERY_OP_EQUAL, widgetId);

            if (m_databaseRequester->queryStatement(qUpdate) <= 0)
               throw shared::exception::CEmptyResult("No lines affected");
         }

         void CWidget::updateWidgetSize(int widgetId, int sizeX, int sizeY)
         {
            auto qUpdate = m_databaseRequester->newQuery();
            qUpdate.Update(dbCommon::CPluginTable::getTableName()).
                   Set(dbCommon::CWidgetTable::getSizeXColumnName(), sizeX, dbCommon::CWidgetTable::getSizeYColumnName(), sizeY).
                   Where(dbCommon::CWidgetTable::getIdColumnName(), CQUERY_OP_EQUAL, widgetId);

            if (m_databaseRequester->queryStatement(qUpdate) <= 0)
               throw shared::exception::CEmptyResult("No lines affected");
         }

         void CWidget::updateWidgetPosition(int widgetId, int position) const
         {
            auto qUpdate = m_databaseRequester->newQuery();
            qUpdate.Update(dbCommon::CWidgetTable::getTableName()).
                   Set(dbCommon::CWidgetTable::getPositionColumnName(), position).
                   Where(dbCommon::CWidgetTable::getIdColumnName(), CQUERY_OP_EQUAL, widgetId);

            if (m_databaseRequester->queryStatement(qUpdate) <= 0)
               throw shared::exception::CEmptyResult("No lines affected");
         }

         void CWidget::updateWidget(const dbCommon::entities::CWidget& widgetToUpdate, bool createIfNotExists)
         {
            auto qUpdate = m_databaseRequester->newQuery();

            //in all cases the Id must be filled
            if (!widgetToUpdate.Id.isDefined())
               throw dbCommon::CDatabaseException("Need an id to update");

            //search for the widget
            auto qSelect = m_databaseRequester->newQuery();
            qSelect.Select().
                   From(dbCommon::CWidgetTable::getTableName()).
                   Where(dbCommon::CWidgetTable::getIdColumnName(), CQUERY_OP_EQUAL, widgetToUpdate.Id());

            dbCommon::adapters::CWidgetAdapter adapter;
            m_databaseRequester->queryEntities(&adapter, qSelect);
            if (adapter.getResults().size() == 0)
            {
               //do not exist, just add it
               addWidget(widgetToUpdate);
            }
            else
            {
               //already exists, just update fields


               //update type
               if (widgetToUpdate.Type.isDefined())
               {
                  qUpdate.Clear().Update(dbCommon::CWidgetTable::getTableName()).
                         Set(dbCommon::CWidgetTable::getTypeColumnName(), widgetToUpdate.Type()).
                         Where(dbCommon::CWidgetTable::getIdColumnName(), CQUERY_OP_EQUAL, widgetToUpdate.Id());

                  if (m_databaseRequester->queryStatement(qUpdate) <= 0)
                     throw dbCommon::CDatabaseException("Failed to update name");
               }

               //update IdPage
               if (widgetToUpdate.IdPage.isDefined())
               {
                  qUpdate.Clear().Update(dbCommon::CWidgetTable::getTableName()).
                         Set(dbCommon::CWidgetTable::getIdPageColumnName(), widgetToUpdate.IdPage()).
                         Where(dbCommon::CWidgetTable::getIdColumnName(), CQUERY_OP_EQUAL, widgetToUpdate.Id());

                  if (m_databaseRequester->queryStatement(qUpdate) <= 0)
                     throw dbCommon::CDatabaseException("Failed to update id page");
               }

               //update configuration
               if (widgetToUpdate.Configuration.isDefined())
               {
                  qUpdate.Clear().Update(dbCommon::CWidgetTable::getTableName()).
                         Set(dbCommon::CWidgetTable::getConfigurationColumnName(), widgetToUpdate.Configuration()).
                         Where(dbCommon::CWidgetTable::getIdColumnName(), CQUERY_OP_EQUAL, widgetToUpdate.Id());

                  if (m_databaseRequester->queryStatement(qUpdate) <= 0)
                     throw dbCommon::CDatabaseException("Failed to update configuration");
               }

               //update Position
               if (widgetToUpdate.Position.isDefined())
               {
                  qUpdate.Clear().Update(dbCommon::CWidgetTable::getTableName()).
                         Set(dbCommon::CWidgetTable::getPositionColumnName(), widgetToUpdate.Position()).
                         Where(dbCommon::CWidgetTable::getIdColumnName(), CQUERY_OP_EQUAL, widgetToUpdate.Id());

                  if (m_databaseRequester->queryStatement(qUpdate) <= 0)
                     throw dbCommon::CDatabaseException("Failed to update Position");
               }

               //update Size X
               if (widgetToUpdate.SizeX.isDefined())
               {
                  qUpdate.Clear().Update(dbCommon::CWidgetTable::getTableName()).
                         Set(dbCommon::CWidgetTable::getSizeXColumnName(), widgetToUpdate.SizeX()).
                         Where(dbCommon::CWidgetTable::getIdColumnName(), CQUERY_OP_EQUAL, widgetToUpdate.Id());

                  if (m_databaseRequester->queryStatement(qUpdate) <= 0)
                     throw dbCommon::CDatabaseException("Failed to update Size X");
               }

               //update Size Y
               if (widgetToUpdate.SizeY.isDefined())
               {
                  qUpdate.Clear().Update(dbCommon::CWidgetTable::getTableName()).
                         Set(dbCommon::CWidgetTable::getSizeYColumnName(), widgetToUpdate.SizeY()).
                         Where(dbCommon::CWidgetTable::getIdColumnName(), CQUERY_OP_EQUAL, widgetToUpdate.Id());

                  if (m_databaseRequester->queryStatement(qUpdate) <= 0)
                     throw dbCommon::CDatabaseException("Failed to update Size Y");
               }

               //update Title
               if (widgetToUpdate.Title.isDefined())
               {
                  qUpdate.Clear().Update(dbCommon::CWidgetTable::getTableName()).
                         Set(dbCommon::CWidgetTable::getTitleColumnName(), widgetToUpdate.Title()).
                         Where(dbCommon::CWidgetTable::getIdColumnName(), CQUERY_OP_EQUAL, widgetToUpdate.Id());

                  if (m_databaseRequester->queryStatement(qUpdate) <= 0)
                     throw dbCommon::CDatabaseException("Failed to update Title");
               }
            }
         }


         void CWidget::removeWidget(int widgetId)
         {
            auto qDelete = m_databaseRequester->newQuery();
            qDelete.DeleteFrom(dbCommon::CWidgetTable::getTableName()).
                   Where(dbCommon::CWidgetTable::getIdColumnName(), CQUERY_OP_EQUAL, widgetId);
            if (m_databaseRequester->queryStatement(qDelete) <= 0)
               throw shared::exception::CEmptyResult("No lines affected");
         }

         void CWidget::removeWidgetsInPage(int pageId)
         {
            auto qSelect = m_databaseRequester->newQuery();
            qSelect.Select().
                   From(dbCommon::CWidgetTable::getTableName()).
                   Where(dbCommon::CWidgetTable::getIdPageColumnName(), CQUERY_OP_EQUAL, pageId);

            auto qDelete = m_databaseRequester->newQuery();
            qDelete.DeleteFrom(dbCommon::CWidgetTable::getTableName()).
                   Where(dbCommon::CWidgetTable::getIdPageColumnName(), CQUERY_OP_EQUAL, pageId);

            m_databaseRequester->queryStatement(qDelete);
         }

         void CWidget::removeAllWidgets()
         {
            auto qDelete = m_databaseRequester->newQuery();
            qDelete.DeleteFrom(dbCommon::CWidgetTable::getTableName());
            m_databaseRequester->queryStatement(qDelete);
         }

         // [END] IWidgetRequester implementation
      } //namespace requesters
   } //namespace common
} //namespace database 


