#pragma once

#include <shared/plugin/yPluginApi/IYPluginApi.h>
#include <shared/DataContainer.h>
#include "ILighting5Subtype.h"

namespace yApi = shared::plugin::yPluginApi;

namespace rfxcomMessages
{
   //--------------------------------------------------------------
   /// \brief	The Lighting5 On/off keyword
   //--------------------------------------------------------------
   class CLighting5MdRemoteKeyword : public ILighting5Subtype
   {
   public:
      //--------------------------------------------------------------
      /// \brief	                        Constructor
      //--------------------------------------------------------------
      CLighting5MdRemoteKeyword();
      
      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CLighting5MdRemoteKeyword();
       
      // ILighting5Subtype implementation
      virtual std::string getModel() const;
      virtual void declare(boost::shared_ptr<yApi::IYPluginApi> context, const std::string& deviceName) const;
      virtual void historize(boost::shared_ptr<yApi::IYPluginApi> context, const std::string& deviceName) const;
      virtual void set(const std::string& yadomsCommand);
      virtual void reset();
      virtual size_t getMessageNb() const;
      virtual void setFromProtocolState(unsigned char cmdByte, unsigned char levelByte);
      virtual void toProtocolState(size_t idxMessage, unsigned char& cmdByte, unsigned char& levelByte) const;
      // [END] ILighting5Subtype implementation

   private:
      //--------------------------------------------------------------
      /// \brief	                        The keyword
      //--------------------------------------------------------------
      yApi::historization::CDimmable m_keyword;
   };

} // namespace rfxcomMessages