#include "stdafx.h"
#include "ZWave.h"
#include <shared/plugin/ImplementationHelper.h>
#include <shared/Log.h>
#include <shared/plugin/yPluginApi/StandardCapacities.h>
#include <shared/exception/Exception.hpp>
#include "ZWaveControllerFactory.h"
#include "KeywordContainer.h"

// Use this macro to define all necessary to make your DLL a Yadoms valid plugin.
// Note that you have to provide some extra files, like package.json, and icon.png
// This macro also defines the static PluginInformations value that can be used by plugin to get information values
IMPLEMENT_PLUGIN(CZWave)


CZWave::CZWave()
{
}

CZWave::~CZWave()
{
}


void CZWave::doWork(boost::shared_ptr<yApi::IYPluginApi> context)
{
   try
   {
      YADOMS_LOG_CONFIGURE("ZWave");

      // Load configuration values (provided by database)
      m_configuration.initializeWith(context->getConfiguration());

      m_configuration.setPath(context->getPluginPath().string());

      // the main loop
      YADOMS_LOG(debug) << "CZWave is running...";

      m_controller = CZWaveControllerFactory::Create();
      m_controller->configure(&m_configuration, &context->getEventHandler());
      if (m_controller->start())
      {
         while (1)
         {
            // Wait for an event
            switch (context->getEventHandler().waitForEvents())
            {
            case yApi::IYPluginApi::kEventDeviceCommand:
            {
               // Command was received from Yadoms
               boost::shared_ptr<const yApi::IDeviceCommand> command = context->getEventHandler().getEventData<boost::shared_ptr<const yApi::IDeviceCommand> >();

               YADOMS_LOG(debug) << "Command received from Yadoms :" << command->toString();
               try
               {
                  m_controller->SendCommand(command->getTargetDevice(), command->getKeyword(), command->getBody());
               }
               catch (shared::exception::CException & ex)
               {
                  YADOMS_LOG(error) << "Fail to send command : " << ex.what();
               }

               break;
            }
            case yApi::IYPluginApi::kEventUpdateConfiguration:
            {
               // Configuration was updated
               shared::CDataContainer newConfiguration = context->getEventHandler().getEventData<shared::CDataContainer>();
               YADOMS_LOG(debug) << "configuration was updated...";
               BOOST_ASSERT(!newConfiguration.empty());  // newConfigurationValues shouldn't be empty, or kEventUpdateConfiguration shouldn't be generated

               // Take into account the new configuration
               // - Restart the plugin if necessary,
               // - Update some resources,
               // - etc...
               m_configuration.initializeWith(newConfiguration);
               break;
            }
            case kDeclareDevice:
            {
               try
               {
                  shared::CDataContainer deviceData = context->getEventHandler().getEventData<shared::CDataContainer>();
                  if (!context->deviceExists(deviceData.get<std::string>("name")))
                     context->declareDevice(deviceData.get<std::string>("name"), deviceData.get<std::string>("friendlyName"), deviceData.get<shared::CDataContainer>("details").serialize());
               }
               catch (shared::exception::CException & ex)
               {
                  YADOMS_LOG(error) << "Fail to declare device : " << ex.what();
               }

               break;
            }

            case kUpdateKeyword:
            {
               try
               {

                  boost::shared_ptr<CKeywordContainer> keywordData  = context->getEventHandler().getEventData< boost::shared_ptr<CKeywordContainer> >();

                  std::string deviceId = keywordData->getDeviceId();
                  std::string keywordId = keywordData->getKeyword().getKeyword();

                  if (!context->keywordExists(deviceId, keywordId))
                     context->declareKeyword(deviceId, keywordData->getKeyword());

                  context->historizeData(deviceId, keywordData->getKeyword());
               }
               catch (shared::exception::CException & ex)
               {
                  YADOMS_LOG(error) << "Fail to update keyword : " << ex.what();
               }

               break;
            }
               break;

            default:
            {
               YADOMS_LOG(error) << "Unknown message id";
               break;
            }
            }
         };
      }
      else
      {
         YADOMS_LOG(error) << "Cannot start ZWave controller. Plugin ends.";
      }

   }
   // Plugin must catch this end-of-thread exception to make its cleanup.
   // If no cleanup is necessary, still catch it, or Yadoms will consider
   // as a plugin failure.
   catch (boost::thread_interrupted&)
   {
   }
   catch (shared::exception::CException & ex)
   {
      YADOMS_LOG(fatal) << "The XPL plugin fails. Unknown expcetion : " << ex.what();
   }
}


