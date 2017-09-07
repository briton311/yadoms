#pragma once

#include "IRestService.h"
#include <dbCommon/IDatabaseRequester.h>
#include "web/rest/RestDispatcher.h"
#include "task/Scheduler.h"

namespace web
{
   namespace rest
   {
      namespace service
      {
         class CMaintenance : public IRestService
         {
         public:
            explicit CMaintenance(boost::shared_ptr<dbCommon::IDatabaseRequester> databaseRequester, boost::shared_ptr<task::CScheduler> taskScheduler);
            virtual ~CMaintenance();

            // IRestService implementation
            void configureDispatcher(CRestDispatcher& dispatcher) override;
            // [END] IRestService implementation

            static const std::string& getRestKeyword();

            shared::CDataContainer getDatabaseInformation(const std::vector<std::string>& parameters, const std::string& requestContent) const;
            shared::CDataContainer startDatabaseBackup(const std::vector<std::string>& parameters, const std::string& requestContent) const;
            shared::CDataContainer getLastDatabaseBackup(const std::vector<std::string>& parameters, const std::string& requestContent) const;
            shared::CDataContainer deleteLastDatabaseBackup(const std::vector<std::string>& parameters, const std::string& requestContent) const;

         private:
            static std::string m_restKeyword;

            boost::shared_ptr<dbCommon::IDatabaseRequester> m_databaseRequester;
            boost::shared_ptr<task::CScheduler> m_taskScheduler;
         };
      } //namespace service
   } //namespace rest
} //namespace web 


