#pragma once

#include "IDataProvider.h"
#include "startupOptions/IStartupOptions.h"


namespace database
{
   //--------------------------------------------------------------
   /// \Brief		   Database factory
   //--------------------------------------------------------------
   class CFactory
   {
   public:
      //--------------------------------------------------------------
      /// \Brief		   Create the Database engine
      /// \param[in] startupOptions   The application startup options
      //--------------------------------------------------------------
      static boost::shared_ptr<IDataProvider> create(const boost::shared_ptr<startupOptions::IStartupOptions> startupOptions);

   private:
      //--------------------------------------------------------------
      /// \Brief		   Create the Database engine
      /// \param[in] startupOptions   The application startup options
      //--------------------------------------------------------------
      static boost::shared_ptr<dbCommon::IDatabaseRequester> createEngine(const boost::shared_ptr<startupOptions::IStartupOptions> startupOptions);

      //--------------------------------------------------------------
      /// \Brief		   Create the SQLite requester
      /// \param[in] startupOptions   The application startup options
      /// \return       The SQLite requester
      //--------------------------------------------------------------
      static boost::shared_ptr<dbCommon::IDatabaseRequester> createSQLiteRequester(const boost::shared_ptr<startupOptions::IStartupOptions> startupOptions);

      //--------------------------------------------------------------
      /// \Brief		   Create the SQLite options object
      /// \param[in] startupOptions   The application startup options
      /// \return       The SQLite options object
      //--------------------------------------------------------------
      static boost::shared_ptr<const shared::CDataContainer> buildSQLiteOptions(const boost::shared_ptr<startupOptions::IStartupOptions> startupOptions);

      //--------------------------------------------------------------
      /// \Brief		   Create the PostgreSQL requester
      /// \param[in] startupOptions   The application startup options
      /// \return       The PostgreSQL requester
      //--------------------------------------------------------------
      static boost::shared_ptr<dbCommon::IDatabaseRequester> createPqsqlRequester(const boost::shared_ptr<startupOptions::IStartupOptions> startupOptions);

      //--------------------------------------------------------------
      /// \Brief		   Create the PostgreSQL options object
      /// \param[in] startupOptions   The application startup options
      /// \return       The PostgreSQL options object
      //--------------------------------------------------------------
      static boost::shared_ptr<const shared::CDataContainer> buildPqsqlOptions(const boost::shared_ptr<startupOptions::IStartupOptions> startupOptions);
   };
} //namespace database 


