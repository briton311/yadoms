#pragma once

#include <Poco/Util/TimerTask.h>
#include "database/sqlite/SQLiteDataProvider.h"

namespace database { 
namespace sqlite { 

   class CSQLiteSummaryDataTask : public Poco::Util::TimerTask
   {
   public:
      //--------------------------------------------------------------
      /// \brief	Constructor
      /// \param [in]	databaseProvider: the database provider
      //--------------------------------------------------------------
      CSQLiteSummaryDataTask(boost::shared_ptr<IAcquisitionRequester> acquisitionRequester, boost::shared_ptr<IKeywordRequester> keywordRequester);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CSQLiteSummaryDataTask();

      // BEGIN Poco::Util::TimerTask implementation 
      virtual void run();
      // END Poco::Util::TimerTask implementation 

   private:
      //--------------------------------------------------------------
      /// \Brief		   Method for implementing first run process
      //--------------------------------------------------------------
      void executeFirstRunPass();

      //--------------------------------------------------------------
      /// \Brief		   Method for implementing common process (not first run)
      //--------------------------------------------------------------
      void executeCommonPass();
      
      //--------------------------------------------------------------
      /// \Brief		   Acquisition requester
      //--------------------------------------------------------------
      boost::shared_ptr<IAcquisitionRequester> m_acquisitionRequester;

      //--------------------------------------------------------------
      /// \Brief		   Keyword requester
      //--------------------------------------------------------------
      boost::shared_ptr<IKeywordRequester> m_keywordRequester;

      //--------------------------------------------------------------
      /// \Brief		   Keep the last date time used to compute summary day data
      //--------------------------------------------------------------
      bool m_firstRun;
   };

} //namespace sqlite
} //namespace database 

