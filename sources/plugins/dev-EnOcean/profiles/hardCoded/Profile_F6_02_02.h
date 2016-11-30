#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <boost/dynamic_bitset.hpp>
#include "../IType.h"
#include "../../IMessageHandler.h"

namespace yApi = shared::plugin::yPluginApi;


class CProfile_F6_02_02 : public IType
{
public:
   CProfile_F6_02_02(const std::string& deviceId,
                     boost::shared_ptr<yApi::IYPluginApi> api);
   virtual ~CProfile_F6_02_02();

   // IType implementation
   const std::string& profile() const override;
   const std::string& title() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> allHistorizers() const override;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> states(unsigned char rorg,
                                                                                   const boost::dynamic_bitset<>& data,
                                                                                   const boost::dynamic_bitset<>& status) const override;
   void sendCommand(const std::string& keyword,
                    const std::string& commandBody,
                    const std::string& senderId,
                    boost::shared_ptr<IMessageHandler> messageHandler) const override;
   void sendConfiguration(const shared::CDataContainer& deviceConfiguration,
                          const std::string& senderId,
                          boost::shared_ptr<IMessageHandler> messageHandler) const override;
   // [END] IType implementation

private:
   boost::shared_ptr<yApi::IYPluginApi> m_api;
   const std::string& m_deviceId;
   boost::shared_ptr<yApi::historization::CSwitch> m_buttonA, m_buttonB, m_buttonA2ndAction, m_buttonB2ndAction;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_historizers;
   std::vector<boost::shared_ptr<const yApi::historization::IHistorizable>> m_secondActionHistorizers;
};
