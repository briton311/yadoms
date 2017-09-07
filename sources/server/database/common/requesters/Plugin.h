#pragma once

#include "database/IPluginRequester.h"
#include <dbCommon/IDatabaseRequester.h>

namespace database
{
   namespace common
   {
      namespace requesters
      {
         //--------------------------------------------------------------
         /// \Brief		   Plugin requester for SQLite database
         //--------------------------------------------------------------
         class CPlugin : public IPluginRequester
         {
         public:
            //--------------------------------------------------------------
            /// \Brief		   Constructor
            /// \param [in]	pDatabaseHandler: the database handler
            //--------------------------------------------------------------
            explicit CPlugin(boost::shared_ptr<dbCommon::IDatabaseRequester> databaseRequester);

            //--------------------------------------------------------------
            /// \Brief		   Destructor
            //--------------------------------------------------------------
            virtual ~CPlugin();

            // IPluginRequester implementation
            int addInstance(const dbCommon::entities::CPlugin& newPlugin) override;
            void removeInstance(int pluginId) override;
            boost::shared_ptr<dbCommon::entities::CPlugin> getInstance(int pluginId) override;
            boost::shared_ptr<dbCommon::entities::CPlugin> getSystemInstance() override;
            std::vector<boost::shared_ptr<dbCommon::entities::CPlugin>> getInstances() override;
            void updateInstance(const dbCommon::entities::CPlugin& newPlugin) override;
            void disableAutoStartForAllPluginInstances(const std::string& pluginName) override;
            // [END] IPluginRequester implementation

         private:
            //--------------------------------------------------------------
            /// \Brief		   Reference to IDatabaseRequester
            //--------------------------------------------------------------
            boost::shared_ptr<dbCommon::IDatabaseRequester> m_databaseRequester;
         };
      } //namespace requesters
   } //namespace common
} //namespace database 


