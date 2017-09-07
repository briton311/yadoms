#pragma once

#include "database/IRecipientRequester.h"
#include <dbCommon/IDatabaseRequester.h>

namespace database
{
   namespace common
   {
      namespace requesters
      {
         //--------------------------------------------------------------
         /// \Brief		   Recipient requester for SQLite database
         //--------------------------------------------------------------
         class CRecipient: public IRecipientRequester
         {
         public:
            //--------------------------------------------------------------
            /// \Brief		   Constructor
            /// \param [in]	pDatabaseHandler: the database handler
            //--------------------------------------------------------------
            explicit CRecipient(boost::shared_ptr<dbCommon::IDatabaseRequester> databaseRequester);

            //--------------------------------------------------------------
            /// \Brief		   Destructor
            //--------------------------------------------------------------
            virtual ~CRecipient();

            // IRecipientRequester implementation
            virtual boost::shared_ptr<dbCommon::entities::CRecipient> addRecipient(const dbCommon::entities::CRecipient& recipient);
            virtual boost::shared_ptr<dbCommon::entities::CRecipient> updateRecipient(const dbCommon::entities::CRecipient& recipient);
            virtual std::vector<boost::shared_ptr<dbCommon::entities::CRecipient>> getRecipients();
            virtual boost::shared_ptr<dbCommon::entities::CRecipient> getRecipient(const int recipientId);
            virtual boost::shared_ptr<dbCommon::entities::CRecipient> getRecipient(const std::string& firstName, const std::string& lastName);
            virtual bool exists(const std::string& firstName, const std::string& lastName);
            virtual bool exists(const int id);
            virtual void removeRecipient(int recipientId);
            virtual void removeAllRecipients();
            virtual bool fieldExists(const std::string& fieldName, const std::string& pluginName) const;
            virtual boost::shared_ptr<dbCommon::entities::CRecipientField> createField(const dbCommon::entities::CRecipientField& field);
            virtual std::vector<boost::shared_ptr<dbCommon::entities::CRecipient>> findRecipientsFromField(const std::string& fieldName, const std::string& expectedFieldValue);
            virtual std::vector<boost::shared_ptr<dbCommon::entities::CRecipientField>> getFields();
            virtual std::vector<boost::shared_ptr<dbCommon::entities::CRecipientField>> getFieldsByName(const std::string& fieldName);
            // [END] IRecipientRequester implementation

         private:
            //--------------------------------------------------------------
            /// \Brief		   Read the recipient fields
            /// \param [in]	recipientToComplete: the recipient entity to complete
            //--------------------------------------------------------------
            void ReadRecipientFields(boost::shared_ptr<dbCommon::entities::CRecipient>& recipientToComplete) const;

            //--------------------------------------------------------------
            /// \Brief		   Write the recipient fields
            /// \param [in]	recipientId: the recipient id
            /// \param [in]	fields: the recipient fields
            //--------------------------------------------------------------
            void WriteRecipientFields(const int recipientId, const std::vector<boost::shared_ptr<dbCommon::entities::CRecipientField>>& fields) const;

            //--------------------------------------------------------------
            /// \Brief		   Reference to IDatabaseRequester
            //--------------------------------------------------------------
            boost::shared_ptr<dbCommon::IDatabaseRequester> m_databaseRequester;
         };
      } //namespace requesters
   } //namespace common
} //namespace database 


