#pragma once

#include "Version_3_0_1.h"
#include <dbCommon/IDatabaseRequester.h>


namespace database
{
   namespace common
   {
      namespace versioning
      {
         //
         /// \brief Database version 3.0 update manager
         //
         class CVersion_4_0_0 : public CVersion_3_0_1
         {
         public:
            //
            /// \brief Constructor
            //
            CVersion_4_0_0();

            //
            /// \brief Destructor
            //
            virtual ~CVersion_4_0_0();

            // ISQLiteVersionUpgrade implementation
            void checkForUpgrade(const boost::shared_ptr<dbCommon::IDatabaseRequester>& pRequester, const shared::versioning::CVersion& currentVersion) override;
            // [END] ISQLiteVersionUpgrade implementation

         private:
            //-----------------------------------
            /// \brief     The version (4.0.0)
            //-----------------------------------
            static const shared::versioning::CVersion Version;

            //-----------------------------------
            /// \brief     Create the database (when tables are not found)
            ///\param [in] pRequester : database requester object
            //-----------------------------------
            static void UpdateFrom3_0_1(const boost::shared_ptr<dbCommon::IDatabaseRequester>& pRequester);
         };
      } //namespace versioning
   } //namespace common
} //namespace database 


