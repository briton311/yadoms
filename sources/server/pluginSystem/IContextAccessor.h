#pragma once
#include <shared/plugin/yPluginApi/IYPluginApi.h>


namespace pluginSystem
{
   //--------------------------------------------------------------
   /// \brief	yScriptApi context accessor, used by script to interact with Yadoms
   //--------------------------------------------------------------
   class IContextAccessor
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~IContextAccessor()
      {
      }

      //--------------------------------------------------------------
      /// \brief	Get the context accessor ID (unique on full system)
      /// \return The context accessor ID
      //--------------------------------------------------------------
      virtual std::string id() const = 0;

      //--------------------------------------------------------------
      /// \brief	Get the plugin API
      /// \return The plugin API
      //--------------------------------------------------------------
      virtual boost::shared_ptr<shared::plugin::yPluginApi::IYPluginApi> api() const = 0;
   };
} // namespace pluginSystem
