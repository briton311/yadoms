#pragma once

#include "database/IConfigurationRequester.h"
#include <dbCommon/IDatabaseRequester.h>

namespace database
{
   namespace common
   {
      namespace requesters
      {
         //--------------------------------------------------------------
         /// \Brief		   Configuration requester for database
         //--------------------------------------------------------------
         class CConfiguration: public IConfigurationRequester
         {
         public:
            //--------------------------------------------------------------
            /// \Brief		   Constructor
            /// \param [in]	pDatabaseHandler: the database handler
            //--------------------------------------------------------------
            explicit CConfiguration(boost::shared_ptr<dbCommon::IDatabaseRequester> databaseRequester);

            //--------------------------------------------------------------
            /// \Brief		   Destructor
            //--------------------------------------------------------------
            virtual ~CConfiguration();

            // IConfigurationRequester implementation
            void create(dbCommon::entities::CConfiguration& configurationToCreate) override;
            bool exists(const std::string& section, const std::string& name) override;
            boost::shared_ptr<dbCommon::entities::CConfiguration> getConfiguration(const std::string& section, const std::string& name) override;
            std::vector<boost::shared_ptr<dbCommon::entities::CConfiguration>> getConfigurations(const std::string& section) override;
            std::vector<boost::shared_ptr<dbCommon::entities::CConfiguration>> getConfigurations() override;
            void updateConfiguration(dbCommon::entities::CConfiguration& configurationToUpdate) override;
            void removeConfiguration(dbCommon::entities::CConfiguration& configurationToRemove) override;
            // [END] IConfigurationRequester implementation

         private:
            //--------------------------------------------------------------
            /// \Brief		   Reference to IDatabaseRequester
            //--------------------------------------------------------------
            boost::shared_ptr<dbCommon::IDatabaseRequester> m_databaseRequester;
         };
      } //namespace requesters
   } //namespace common
} //namespace database 


