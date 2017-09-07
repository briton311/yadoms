#pragma once

#include "IRestService.h"
#include "database/IDataProvider.h"

namespace web
{
   namespace rest
   {
      namespace service
      {
         class CPluginEventLogger : public IRestService
         {
         public:
            explicit CPluginEventLogger(boost::shared_ptr<database::IDataProvider> dataProvider);
            virtual ~CPluginEventLogger();

         public:
            // IRestService implementation
            void configureDispatcher(CRestDispatcher& dispatcher) override;
            // [END] IRestService implementation

            const std::string& getRestKeyword() const;

            shared::CDataContainer getLogsForPluginName(const std::vector<std::string>& parameters, const std::string& requestContent) const;
            shared::CDataContainer getLogsForPluginNameFromDate(const std::vector<std::string>& parameters, const std::string& requestContent) const;

         private:
            boost::shared_ptr<database::IDataProvider> m_dataProvider;
            std::string m_restKeyword;
         };
      } //namespace service
   } //namespace rest
} //namespace web 


