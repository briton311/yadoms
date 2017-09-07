#include "stdafx.h"
#include "BasicQualifier.h"
#include <shared/exception/EmptyResult.hpp>
#include <shared/Log.h>

namespace pluginSystem
{
   CBasicQualifier::CBasicQualifier(boost::shared_ptr<database::IPluginEventLoggerRequester> pluginLogger,
                                    boost::shared_ptr<dataAccessLayer::IEventLogger> mainLogger)
      : m_pluginLogger(pluginLogger),
        m_mainLogger(mainLogger)
   {
   }

   CBasicQualifier::~CBasicQualifier()
   {
   }

   void CBasicQualifier::signalLoad(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation)
   {
      // Insert event in database
      AddEventToDatabase(pluginInformation, dbCommon::entities::EEventType::kLoad);
   }

   void CBasicQualifier::signalUnload(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation)
   {
      // Insert event in database
      AddEventToDatabase(pluginInformation, dbCommon::entities::EEventType::kUnload);
   }

   void CBasicQualifier::signalCrash(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation, const std::string& reason)
   {
      // Insert event in database
      AddEventToDatabase(pluginInformation, dbCommon::entities::EEventType::kCrash, reason);
   }

   void CBasicQualifier::AddEventToDatabase(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation,
                                            dbCommon::entities::EEventType eventType,
                                            const std::string& reason) const
   {
      try
      {
         // Add event into plugin event logger table
         m_pluginLogger->addEvent(
            pluginInformation->getType(),
            pluginInformation->getVersion().toString(),
            eventType,
            reason);

         // Only crashes have to be logged in the main event logger table
         if (eventType == dbCommon::entities::EEventType::kCrash)
            m_mainLogger->addEvent(dbCommon::entities::ESystemEventCode::kPluginCrash, "plugin " + pluginInformation->getIdentity(), reason);
      }
      catch (shared::exception::CEmptyResult& e)
      {
         // Just log the error
         YADOMS_LOG(error) << "Error when adding plugin " << eventType << " event in database : " << e.what();
      }
   }

   bool CBasicQualifier::isSafe(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation)
   {
      // With basic qualifier, plugin is always considered as safe
      return true;
   }

   int CBasicQualifier::getQualityLevel(const boost::shared_ptr<const shared::plugin::information::IInformation> pluginInformation)
   {
      // With basic qualifier, plugin is always considered as perfect
      return 100;
   }
} // namespace pluginSystem


