#pragma once

#include "IRestService.h"
#include "database/IDataProvider.h"


namespace web
{
   namespace rest
   {
      namespace service
      {
         class CRecipient : public IRestService
         {
         public:
            explicit CRecipient(boost::shared_ptr<database::IDataProvider> dataProvider);
            virtual ~CRecipient();

            // IRestService implementation
            void configureDispatcher(CRestDispatcher& dispatcher) override;
            // [END] IRestService implementation

            static const std::string& getRestKeyword();
            shared::CDataContainer getOneRecipient(const std::vector<std::string>& parameters, const std::string& requestContent) const;
            shared::CDataContainer getAllRecipients(const std::vector<std::string>& parameters, const std::string& requestContent) const;
            shared::CDataContainer addRecipient(const std::vector<std::string>& parameters, const std::string& requestContent) const;
            shared::CDataContainer updateRecipient(const std::vector<std::string>& parameters, const std::string& requestContent) const;
            shared::CDataContainer removeOneRecipient(const std::vector<std::string>& parameters, const std::string& requestContent) const;
            shared::CDataContainer removeAllRecipients(const std::vector<std::string>& parameters, const std::string& requestContent) const;

            shared::CDataContainer getAllRecipientFields(const std::vector<std::string>& parameters, const std::string& requestContent) const;
            shared::CDataContainer getAllRecipientsByField(const std::vector<std::string>& parameters, const std::string& requestContent) const;

         private:
            static std::string m_restKeyword;
            static std::string m_restFieldKeyword;
            boost::shared_ptr<database::IDataProvider> m_dataProvider;
            shared::CDataContainer transactionalMethod(CRestDispatcher::CRestMethodHandler realMethod, const std::vector<std::string>& parameters, const std::string& requestContent);
         };
      } //namespace service
   } //namespace rest
} //namespace web 


