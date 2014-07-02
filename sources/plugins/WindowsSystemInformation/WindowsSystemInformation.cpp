#include "stdafx.h"
#include "WindowsSystemInformation.h"
#include <shared/Log.h>
#include <shared/event/EventTimer.h>
#include <shared/plugin/ImplementationHelper.h>
#include "WindowsSystemMemoryLoad.h"
#include "WindowsSystemCPULoad.h"
#include "WindowsSystemYadomsCPULoad.h"
#include "WindowsSystemDiskUsage.h"
#include "WindowsSystemDisksList.h"

// Use this macro to define all necessary to make your DLL a Yadoms valid plugin.
// Note that you have to provide some extra files, like package.json, and icon.png
// This macro also defines the static PluginInformations value that can be used by plugin to get information values

IMPLEMENT_PLUGIN(CWindowsSystemInformation)


CWindowsSystemInformation::CWindowsSystemInformation()
{
}

CWindowsSystemInformation::~CWindowsSystemInformation()
{
}

// Event IDs
enum
{
   kEvtTimerSendMessage = yApi::IYadomsApi::kPluginFirstEventId,   // Always start from shared::event::CEventHandler::kUserFirstId
};

void CWindowsSystemInformation::doWork(boost::shared_ptr<yApi::IYadomsApi> context)
{
   try
   {
      YADOMS_LOG(debug) << "CWindowsSystemInformation is starting...";

      CWindowsSystemMemoryLoad    MemoryLoad   ("MemoryLoad");
      CWindowsSystemCPULoad       CPULoad      ("CPULoad");
      CWindowsSystemYadomsCPULoad YadomsCPULoad("YadomsCPULoad");
      
      CWindowsSystemDisksList     DisksList;

      CPULoad.declareDevice(context);
      MemoryLoad.declareDevice(context);
      YadomsCPULoad.declareDevice(context);

      // Timer used to send a message periodically
      context->getEventHandler().createTimer(kEvtTimerSendMessage, shared::event::CEventTimer::kPeriodic, boost::posix_time::seconds(10));

      // the main loop
      YADOMS_LOG(debug) << "WindowsSystemInformation plugin is running...";

      while(1)
      {
         // Wait for an event
         switch(context->getEventHandler().waitForEvents())
         {
         case kEvtTimerSendMessage:
            {
               // Timer used here to send a message periodically

               YADOMS_LOG(debug) << "WindowsSystem plugin :  Read a value...";

                  std::ostringstream ss;
                  std::ostringstream ss1;
                  std::ostringstream ss2;
                  std::ostringstream ss3;

                  try
                  {
                     ss << std::fixed << std::setprecision(2) << MemoryLoad.getValue();

                     YADOMS_LOG(debug) << "WindowsSystemInformation plugin :  Memory Load : " << ss.str();

                     MemoryLoad.historizeData(context);

                     ss1 << std::fixed << std::setprecision(2) << CPULoad.getValue();

                     YADOMS_LOG(debug) << "WindowsSystemInformation plugin :  CPU Load : " << ss1.str();

                     CPULoad.historizeData(context);

                     ss2 << std::fixed << std::setprecision(2) << YadomsCPULoad.getValue();

                     YADOMS_LOG(debug) << "WindowsSystemInformation plugin :  Yadoms CPU Load : " << ss2.str();

                     std::vector<std::string>::const_iterator DisksListIterator;
                     std::vector<std::string> TempList;
                     
                     TempList = DisksList.getList();

                     for(DisksListIterator=TempList.begin(); DisksListIterator!=TempList.end(); ++DisksListIterator)
	                  {  
                        //TODO : Modifier cette déclaration, car les disques ne sont pas historisés pour le moment
                        CWindowsSystemDiskUsage DiskUsage("DiskUsage", *DisksListIterator);

                        ss3 << std::fixed << std::setprecision(2) << DiskUsage.getValue();

                        YADOMS_LOG(debug) << "WindowsSystemInformation plugin :  Yadoms Disk Usage " << *DisksListIterator << " :" << ss3.str();
                     }
                  }
                  catch (boost::system::system_error& e)
                  {
                     YADOMS_LOG(error) << "WindowsSystemInformation plugin :  Exception" << e.what();
                     return;
                  }

               break;
            }
         default:
            {
               YADOMS_LOG(error) << "Unknown message id";
               break;
            }
         }
      };
   }
   // Plugin must catch this end-of-thread exception to make its cleanup.
   // If no cleanup is necessary, still catch it, or Yadoms will consider
   // as a plugin failure.
   catch (boost::thread_interrupted&)
   {
      YADOMS_LOG(info) << "WindowsSystemInformation is stopping..."  << std::endl;
   }
}

