#pragma once
#include "database/IDataProvider.h"
#include "database/IKeywordRequester.h"

namespace database
{
   namespace common
   {
      namespace requesters
      {
         class CAcquisition;

         //--------------------------------------------------------------
         /// \Brief		   Keyword requester for SQLite database
         //--------------------------------------------------------------
         class CKeyword : public IKeywordRequester
         {
         public:
            //--------------------------------------------------------------
            /// \Brief		   Constructor
            /// \param [in]	databaseRequester    The database requester
            //--------------------------------------------------------------
            explicit CKeyword(boost::shared_ptr<dbCommon::IDatabaseRequester> databaseRequester);

            //--------------------------------------------------------------
            /// \Brief		   Destructor
            //--------------------------------------------------------------
            virtual ~CKeyword();

            // IKeywordRequester implementation
            void addKeyword(const dbCommon::entities::CKeyword& newKeyword) override;
            std::vector<boost::shared_ptr<dbCommon::entities::CKeyword> > getKeywordsMatchingCapacity(const std::string& capacity) const override;
            boost::shared_ptr<dbCommon::entities::CKeyword> getKeyword(int deviceId, const std::string& keyword) const override;
            boost::shared_ptr<dbCommon::entities::CKeyword> getKeyword(int keywordId) const override;
            std::vector<boost::shared_ptr<dbCommon::entities::CKeyword> > getKeywordIdFromFriendlyName(int deviceId, const std::string& friendlyName) const override;
            std::vector<boost::shared_ptr<dbCommon::entities::CKeyword> > getKeywords(int deviceId) const override;
            std::vector<boost::shared_ptr<dbCommon::entities::CKeyword> > getAllKeywords() const override;
            std::vector<boost::shared_ptr<dbCommon::entities::CKeyword> > getDeviceKeywordsWithCapacity(int deviceId, const std::string& capacityName, const shared::plugin::yPluginApi::EKeywordAccessMode& accessMode) const override;
            void updateKeywordBlacklistState(int keywordId, const bool blacklist) override;
            void removeKeyword(int keywordId) override;
            void updateKeywordFriendlyName(int keywordId, const std::string& newFriendlyName) override;
            // [END] IKeywordRequester implementation

         private:
            //--------------------------------------------------------------
            /// \Brief		   The database requester
            //--------------------------------------------------------------
            boost::shared_ptr<dbCommon::IDatabaseRequester> m_databaseRequester;
         };
      } //namespace requesters
   } //namespace common
} //namespace database 


