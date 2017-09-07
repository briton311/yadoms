#pragma once

#include "database/IWidgetRequester.h"
#include <dbCommon/IDatabaseRequester.h>

namespace database
{
   namespace common
   {
      namespace requesters
      {
         //--------------------------------------------------------------
         /// \Brief		   Widget requester for SQLite database
         //--------------------------------------------------------------
         class CWidget: public IWidgetRequester
         {
         public:
            //--------------------------------------------------------------
            /// \Brief		   Constructor
            /// \param [in]	pDatabaseHandler: the database handler
            //--------------------------------------------------------------
            explicit CWidget(boost::shared_ptr<dbCommon::IDatabaseRequester> databaseRequester);

            //--------------------------------------------------------------
            /// \Brief		   Destructor
            //--------------------------------------------------------------
            virtual ~CWidget();

            // IWidgetRequester implementation
            int addWidget(const dbCommon::entities::CWidget& newWidget) override;
            boost::shared_ptr<dbCommon::entities::CWidget> getWidget(int widgetId) override;
            std::vector<boost::shared_ptr<dbCommon::entities::CWidget>> getWidgets() override;
            std::vector<boost::shared_ptr<dbCommon::entities::CWidget>> getWidgetsForPage(int pageId) override;
            void updateWidgetConfiguration(int widgetId, const std::string& newConfiguration) override;
            void updateWidgetSize(int widgetId, int sizeX, int sizeY) override;
            void updateWidget(const dbCommon::entities::CWidget& widget, bool createIfNotExists) override;
            void removeWidget(int widgetId) override;
            void removeWidgetsInPage(int pageId) override;
            void removeAllWidgets() override;
            // [END] IWidgetRequester implementation

         protected:
            void updateWidgetPosition(int widgetId, int position) const;

         private:
            //--------------------------------------------------------------
            /// \Brief		   Reference to IDatabaseRequester
            //--------------------------------------------------------------
            boost::shared_ptr<dbCommon::IDatabaseRequester> m_databaseRequester;
         };
      } //namespace requesters
   } //namespace common
} //namespace database 


