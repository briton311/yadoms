#pragma once

#include <dbCommon/Query.h>


namespace database
{
   namespace sqlite
   {
      //
      /// \brief  Class used to create queries
      //
      class CSQLiteQuery : public dbCommon::CQuery
      {
      public:
         //
         /// \brief           Constructor
         //
         CSQLiteQuery();

         //
         /// \brief           Destructor
         //
         virtual ~CSQLiteQuery();

         // Overriding dbCommon::CQuery
         std::string functionDateToIsoString(const std::string& sqlPart) override;
         // [END] - Overriding dbCommon::CQuery
      };
   } //namespace sqlite
} //namespace database 


