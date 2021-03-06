#pragma once

#include "IRestService.h"
#include "dataAccessLayer/IConfigurationManager.h"

namespace web
{
   namespace rest
   {
      namespace service
      {
         class CConfiguration : public IRestService
         {
         public:
            explicit CConfiguration(boost::shared_ptr<dataAccessLayer::IConfigurationManager> configurationManager);
            virtual ~CConfiguration();

         public:
            // IRestService implementation
            void configureDispatcher(CRestDispatcher& dispatcher) override;
            // [END] IRestService implementation


            static const std::string& getRestKeyword();

         public:
            shared::CDataContainer getConfiguration(const std::vector<std::string>& parameters,
                                                    const std::string& requestContent) const;
            shared::CDataContainer getSectionConfigurations(const std::vector<std::string>& parameters,
                                                            const std::string& requestContent) const;
            shared::CDataContainer getAllConfigurations(const std::vector<std::string>& parameters,
                                                        const std::string& requestContent) const;

            shared::CDataContainer createOneConfiguration(const std::vector<std::string>& parameters
                                                          , const std::string& requestContent) const;
            shared::CDataContainer updateOneConfiguration(const std::vector<std::string>& parameters,
                                                          const std::string& requestContent) const;
            shared::CDataContainer updateAllConfigurations(const std::vector<std::string>& parameters,
                                                           const std::string& requestContent) const;
            shared::CDataContainer deleteOneConfiguration(const std::vector<std::string>& parameters,
                                                          const std::string& requestContent) const;

         private:
            boost::shared_ptr<dataAccessLayer::IConfigurationManager> m_configurationManager;
            static std::string m_restKeyword;
         };
      } //namespace service
   } //namespace rest
} //namespace web 


