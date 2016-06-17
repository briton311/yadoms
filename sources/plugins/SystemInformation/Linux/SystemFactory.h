#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <MemoryLoad.h>
#include <CPULoad.h>
#include <YadomsCPULoad.h>
#include <DiskUsage.h>
#include "YadomsRAMProcessMemory.h"
#include "YadomsVirtualProcessMemory.h"
#include "../ISIConfiguration.h"

// Shortcut to yPluginApi namespace
namespace yApi = shared::plugin::yPluginApi;

//--------------------------------------------------------------
/// \brief	Class where all Keywords are created and updated
/// \note
//--------------------------------------------------------------
class CSystemFactory
{
public:
   //--------------------------------------------------------------
   /// \brief	    Constructor
   /// \param[in] api                 yPluginApi API
   /// \param[in] device              The device name
   /// \param[in] ISIConfiguration    The plugin configuration
   /// \param[in] details             Details information for keyword
   //--------------------------------------------------------------
   CSystemFactory(boost::shared_ptr<yApi::IYPluginApi> api,
                  const std::string& device,
                  const ISIConfiguration& configuration,
                  shared::CDataContainer details);

   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~CSystemFactory();

   //--------------------------------------------------------------
   /// \brief	    Update high frequency keywords
   /// \param[in] api                 yPluginApi API
   //--------------------------------------------------------------
   void OnHighFrequencyUpdate(boost::shared_ptr<yApi::IYPluginApi> api) const;

   //--------------------------------------------------------------
   /// \brief	    Update low frequency keywords
   /// \param[in] api                 yPluginApi API
   /// \param[in] ISIConfiguration    The configuration state
   //--------------------------------------------------------------
   void OnLowFrequencyUpdate(boost::shared_ptr<yApi::IYPluginApi> api,
                             const ISIConfiguration& configuration);

   //--------------------------------------------------------------
   /// \brief	    OnConfigurationUpdate
   /// \param[in] api                 yPluginApi API
   /// \param[in] ISIConfiguration    The new configuration
   /// \param[in] details             Details information for keyword
   //--------------------------------------------------------------
   void OnConfigurationUpdate(boost::shared_ptr<yApi::IYPluginApi> api,
                              const ISIConfiguration& configuration,
                              shared::CDataContainer details);

private:
   //--------------------------------------------------------------
   /// \brief	    Device name
   //--------------------------------------------------------------
   std::string m_deviceName;

   //--------------------------------------------------------------
   /// \brief	    Keywords
   //--------------------------------------------------------------
   boost::shared_ptr<CMemoryLoad> m_memoryLoad;
   boost::shared_ptr<CCPULoad> m_cpuLoad;
   boost::shared_ptr<CYadomsCPULoad> m_yadomsCpuLoad;
   boost::shared_ptr<CYadomsRAMProcessMemory> m_ramProcessMemory;
   boost::shared_ptr<CYadomsVirtualProcessMemory> m_virtualProcessMemory;

   //--------------------------------------------------------------
   /// \brief	    List of keywords for disks
   //--------------------------------------------------------------
   std::vector<boost::shared_ptr<CDiskUsage> > m_diskUsageList;

   //--------------------------------------------------------------
   /// \brief	The keywords lists to historize in one step for better performances
   //--------------------------------------------------------------
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_highFreqencyUpdateKeywords;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable> > m_lowFreqencyUpdateKeywords;
};

