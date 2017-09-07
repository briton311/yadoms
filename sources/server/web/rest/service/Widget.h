#pragma once

#include "IRestService.h"
#include "database/IDataProvider.h"
#include "web/rest/RestDispatcher.h"

namespace web
{
   namespace rest
   {
      namespace service
      {
         class CWidget : public IRestService
         {
         public:
            explicit CWidget(boost::shared_ptr<database::IDataProvider> dataProvider, const std::string& webServerPath);
            virtual ~CWidget();

            // IRestService implementation
            void configureDispatcher(CRestDispatcher& dispatcher) override;
            // [END] IRestService implementation


            static const std::string& getRestKeyword();

         public:
            shared::CDataContainer getOneWidget(const std::vector<std::string>& parameters, const std::string& requestContent) const;
            shared::CDataContainer getAllWidgets(const std::vector<std::string>& parameters, const std::string& requestContent) const;
            shared::CDataContainer addWidget(const std::vector<std::string>& parameters, const std::string& requestContent) const;
            shared::CDataContainer deleteOneWidget(const std::vector<std::string>& parameters, const std::string& requestContent) const;
            shared::CDataContainer deleteAllWidgets(const std::vector<std::string>& parameters, const std::string& requestContent) const;
            shared::CDataContainer replaceAllWidgets(const std::vector<std::string>& parameters, const std::string& requestContent) const;
            shared::CDataContainer updateOneWidget(const std::vector<std::string>& parameters, const std::string& requestContent) const;
            shared::CDataContainer findWidgetPackages(const std::vector<std::string>& parameters, const std::string& requestContent) const;

            shared::CDataContainer transactionalMethod(CRestDispatcher::CRestMethodHandler realMethod, const std::vector<std::string>& parameters, const std::string& requestContent) const;
         private:
            boost::shared_ptr<database::IDataProvider> m_dataProvider;
            static std::string m_restKeyword;
            std::string m_webServerPath;
         };
      } //namespace service
   } //namespace rest
} //namespace web 


