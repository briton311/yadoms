#pragma once
#include "IEventLogger.h"
#include "database/IEventLoggerRequester.h"

namespace dataAccessLayer
{
   class CEventLogger : public IEventLogger
   {
   public:
      //--------------------------------------------------------------
      /// \brief                          Constructor
      /// \param [in] requester           The event logger requester
      //--------------------------------------------------------------
      explicit CEventLogger(boost::shared_ptr<database::IEventLoggerRequester> requester);

      //--------------------------------------------------------------
      /// \brief       Destructor
      //--------------------------------------------------------------
      virtual ~CEventLogger();

      // IEventLogger Implementation
      int addEvent(const dbCommon::entities::ESystemEventCode& eventType, const std::string& who, const std::string& what) override;
      int addEvent(const dbCommon::entities::CEventLogger& logEntry) override;
      std::vector<boost::shared_ptr<dbCommon::entities::CEventLogger>> getEvents() override;
      boost::shared_ptr<dbCommon::entities::CEventLogger> getEvent(const int eventId) override;
      boost::shared_ptr<dbCommon::entities::CEventLogger> getLastEvent() override;
      std::vector<boost::shared_ptr<dbCommon::entities::CEventLogger>> getEventsFrom(const int eventId) override;
      std::vector<boost::shared_ptr<dbCommon::entities::CEventLogger>> getEventsRange(const int offset, const int count) override;
      // [END] IEventLogger Implementation

   protected:
      //--------------------------------------------------------------
      /// \brief           The requester
      //--------------------------------------------------------------
      boost::shared_ptr<database::IEventLoggerRequester> m_requester;
   };
} //namespace dataAccessLayer 


