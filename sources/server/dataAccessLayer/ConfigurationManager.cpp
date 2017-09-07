#include "stdafx.h"
#include "ConfigurationManager.h"
#include <shared/Log.h>
#include "notification/Helpers.hpp"

namespace dataAccessLayer
{
   CConfigurationManager::CConfigurationManager(boost::shared_ptr<database::IConfigurationRequester> configurationRequester)
      : m_configurationRequester(configurationRequester)
   {
   }

   CConfigurationManager::~CConfigurationManager()
   {
   }

   void CConfigurationManager::postUpdateNotification(const std::string& section, const std::string& name) const
   {
      //post notification
      try
      {
         auto notificationData(boost::make_shared<dbCommon::entities::CConfiguration>());
         notificationData->Section = section;
         notificationData->Name = name;

         notification::CHelpers::postChangeNotification(notificationData, notification::change::EChangeType::kUpdate);
      }
      catch (shared::exception::CException& ex)
      {
         YADOMS_LOG(error) << "Fail to retreive new configuration : " << ex.what();
      }
   }

   void CConfigurationManager::create(dbCommon::entities::CConfiguration& configurationToCreate)
   {
      m_configurationRequester->create(configurationToCreate);
      postUpdateNotification(configurationToCreate.Section, configurationToCreate.Name);
   }

   bool CConfigurationManager::exists(const std::string& section, const std::string& name)
   {
      return m_configurationRequester->exists(section, name);
   }

   boost::shared_ptr<dbCommon::entities::CConfiguration> CConfigurationManager::getConfiguration(const std::string& section, const std::string& name)
   {
      return m_configurationRequester->getConfiguration(section, name);
   }

   std::vector<boost::shared_ptr<dbCommon::entities::CConfiguration>> CConfigurationManager::getConfigurations(const std::string& section)
   {
      return m_configurationRequester->getConfigurations(section);
   }

   std::vector<boost::shared_ptr<dbCommon::entities::CConfiguration>> CConfigurationManager::getConfigurations()
   {
      return m_configurationRequester->getConfigurations();
   }

   void CConfigurationManager::updateConfiguration(dbCommon::entities::CConfiguration& configurationToUpdate)
   {
      m_configurationRequester->updateConfiguration(configurationToUpdate);
      postUpdateNotification(configurationToUpdate.Section, configurationToUpdate.Name);
   }

   void CConfigurationManager::removeConfiguration(dbCommon::entities::CConfiguration& configurationToRemove)
   {
      m_configurationRequester->removeConfiguration(configurationToRemove);
      postUpdateNotification(configurationToRemove.Section, configurationToRemove.Name);
   }
} //namespace dataAccessLayer 


