#pragma once

#include "IRestService.h"
#include "task/Scheduler.h"

namespace web
{
   namespace rest
   {
      namespace service
      {
         class CTask : public IRestService
         {
         public:
            explicit CTask(boost::shared_ptr<task::CScheduler> taskManager);
            virtual ~CTask();

            // IRestService implementation
            void configureDispatcher(CRestDispatcher& dispatcher) override;
            // [END] IRestService implementation

            static const std::string& getRestKeyword();
            shared::CDataContainer getOneTask(const std::vector<std::string>& parameters, const std::string& requestContent) const;
            shared::CDataContainer getAllTasks(const std::vector<std::string>& parameters, const std::string& requestContent) const;

            shared::CDataContainer addTask(const std::vector<std::string>& parameters, const std::string& requestContent) const;

         private:
            boost::shared_ptr<task::CScheduler> m_taskManager;
            static std::string m_restKeyword;

            static shared::CDataContainer serialize(boost::shared_ptr<task::IInstance> instance);
         };
      } //namespace service
   } //namespace rest
} //namespace web 


