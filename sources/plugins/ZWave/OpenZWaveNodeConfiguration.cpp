#include "stdafx.h"
#include "OpenZWaveNodeConfiguration.h"
#include "OpenZWaveNodeKeywordFactory.h"
#include "OpenZWaveHelpers.h"

COpenZWaveNodeConfiguration::COpenZWaveNodeConfiguration(const uint32 homeId, const uint8 nodeId)
   : m_homeId(homeId), m_nodeId(nodeId)
{
}


COpenZWaveNodeConfiguration::~COpenZWaveNodeConfiguration()
{
}

void COpenZWaveNodeConfiguration::registerConfiguration(OpenZWave::ValueID& value)
{
   std::string keyword = COpenZWaveHelpers::GenerateKeywordName(value);

   if (m_configurationItems.find(keyword) == m_configurationItems.end())
      m_configurationItems[keyword] = COpenZWaveNodeKeywordFactory::createKeyword(value, m_homeId, m_nodeId, false);

}

boost::shared_ptr<IOpenZWaveNodeKeyword> COpenZWaveNodeConfiguration::getConfigurationItem(OpenZWave::ValueID& value)
{
   std::string keyword = COpenZWaveHelpers::GenerateKeywordName(value);

   if (m_configurationItems.find(keyword) == m_configurationItems.end())
   {
      registerConfiguration(value);
      if (m_configurationItems.find(keyword) != m_configurationItems.end())
      {
         return boost::shared_ptr<IOpenZWaveNodeKeyword>();
      }
   }
   return m_configurationItems[keyword];
}

bool COpenZWaveNodeConfiguration::sendCommand(const std::string& keyword, const std::string& commandData)
{
   if (m_configurationItems.find(keyword) != m_configurationItems.end())
      return m_configurationItems[keyword]->sendCommand(commandData);

   throw shared::exception::CException("The keyword is not registered for this zwave node");
}

shared::CDataContainer COpenZWaveNodeConfiguration::generateConfigurationSchema()
{
   shared::CDataContainer result;
   for (auto i = m_configurationItems.begin(); i != m_configurationItems.end(); ++i)
   {
      if (i->second != NULL)
      {
         result.set(i->first, i->second->serialize());
      }
   }

   return result;
}

shared::CDataContainer COpenZWaveNodeConfiguration::saveValuesToDatabase()
{
   return shared::CDataContainer();
}