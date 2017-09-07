﻿#pragma once

#include "IRestService.h"
#include "database/IDataProvider.h"

namespace web
{
   namespace rest
   {
      namespace service
      {
         class CPage : public IRestService
         {
         public:
            explicit CPage(boost::shared_ptr<database::IDataProvider> dataProvider);
            virtual ~CPage();

            // IRestService implementation
            void configureDispatcher(CRestDispatcher& dispatcher) override;
            // [END] IRestService implementation

            static const std::string& getRestKeyword();
            shared::CDataContainer getOnePage(const std::vector<std::string>& parameters, const std::string& requestContent);
            shared::CDataContainer getAllPages(const std::vector<std::string>& parameters, const std::string& requestContent);
            shared::CDataContainer getPageWidget(const std::vector<std::string>& parameters, const std::string& requestContent);

            shared::CDataContainer addPage(const std::vector<std::string>& parameters, const std::string& requestContent);
            shared::CDataContainer updatePage(const std::vector<std::string>& parameters, const std::string& requestContent);
            shared::CDataContainer updateAllPages(const std::vector<std::string>& parameters, const std::string& requestContent);
            shared::CDataContainer deletePage(const std::vector<std::string>& parameters, const std::string& requestContent);
            shared::CDataContainer deleteAllPages(const std::vector<std::string>& parameters, const std::string& requestContent);


            shared::CDataContainer addWidgetForPage(const std::vector<std::string>& parameters, const std::string& requestContent);
            shared::CDataContainer replaceAllWidgetsForPage(const std::vector<std::string>& parameters, const std::string& requestContent);
            shared::CDataContainer deleteAllWidgetsForPage(const std::vector<std::string>& parameters, const std::string& requestContent);

            shared::CDataContainer transactionalMethod(CRestDispatcher::CRestMethodHandler realMethod, const std::vector<std::string>& parameters, const std::string& requestContent);

         private:
            boost::shared_ptr<database::IDataProvider> m_dataProvider;
            static std::string m_restKeyword;
         };
      } //namespace service
   } //namespace rest
} //namespace web 


