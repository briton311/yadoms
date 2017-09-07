#pragma once

#include "database/IPluginEventLoggerRequester.h"
#include <dbCommon/IDatabaseRequester.h>

namespace database
{
   namespace common
   {
      namespace requesters
      {
         //--------------------------------------------------------------
         /// \Brief		   PluginEventLogger requester for SQLite database
         //--------------------------------------------------------------
         class CPluginEventLogger : public IPluginEventLoggerRequester
         {
         public:
            //--------------------------------------------------------------
            /// \Brief		   Constructor
            /// \param [in]	pDatabaseHandler: the database handler
            //--------------------------------------------------------------
            explicit CPluginEventLogger(boost::shared_ptr<dbCommon::IDatabaseRequester> databaseRequester);

            //--------------------------------------------------------------
            /// \Brief		   Destructor
            //--------------------------------------------------------------
            virtual ~CPluginEventLogger();

            // IPluginEventLoggerRequester implementation
            int addEvent(const std::string& pluginName, const std::string& pluginVersion, const dbCommon::entities::EEventType& eventType, const std::string& message = std::string()) override;
            int addEvent(const dbCommon::entities::CPluginEventLogger& pluginLogEntry) override;
            std::vector<boost::shared_ptr<dbCommon::entities::CPluginEventLogger>> getPluginEvents(const std::string& pluginName, const std::string& pluginVersion) override;
            std::vector<boost::shared_ptr<dbCommon::entities::CPluginEventLogger>> getPluginEvents(const std::string& pluginName, const std::string& pluginVersion, const boost::posix_time::ptime& fromDate) override;
            // [END] IPluginEventLoggerRequester implementation

         private:
            //--------------------------------------------------------------
            /// \Brief		   Reference to IDatabaseRequester
            //--------------------------------------------------------------
            boost::shared_ptr<dbCommon::IDatabaseRequester> m_databaseRequester;
         };
      } //namespace requesters
   } //namespace common
} //namespace database 


