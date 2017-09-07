#pragma once

#include "database/IRuleRequester.h"
#include <dbCommon/IDatabaseRequester.h>

namespace database
{
   namespace common
   {
      namespace requesters
      {
         //--------------------------------------------------------------
         /// \Brief		   Rule requester for SQLite database
         //--------------------------------------------------------------
         class CRule : public IRuleRequester
         {
         public:
            //--------------------------------------------------------------
            /// \Brief		   Constructor
            /// \param [in]	pDatabaseHandler: the database handler
            //--------------------------------------------------------------
            explicit CRule(boost::shared_ptr<dbCommon::IDatabaseRequester> databaseRequester);

            //--------------------------------------------------------------
            /// \Brief		   Destructor
            //--------------------------------------------------------------
            virtual ~CRule();

            // IRuleRequester implementation
            std::vector<boost::shared_ptr<dbCommon::entities::CRule>> getRules() const override;
            std::vector<boost::shared_ptr<dbCommon::entities::CRule>> getRules(const std::string& interpreterName) const override;
            boost::shared_ptr<dbCommon::entities::CRule> getRule(int ruleId) const override;
            int addRule(boost::shared_ptr<const dbCommon::entities::CRule> ruleData) override;
            void updateRule(boost::shared_ptr<const dbCommon::entities::CRule> ruleData) override;
            void deleteRule(int ruleId) override;
            // [END] IRuleRequester implementation

         private:
            //--------------------------------------------------------------
            /// \Brief		   Reference to IDatabaseRequester
            //--------------------------------------------------------------
            boost::shared_ptr<dbCommon::IDatabaseRequester> m_databaseRequester;
         };
      } //namespace requesters
   } //namespace common
} //namespace database 


