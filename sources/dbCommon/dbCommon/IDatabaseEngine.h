#pragma once
#include <shared/DataContainer.h>

namespace dbCommon
{
   class IDatabaseEngine
   {
   public:
      //--------------------------------------------------------------
      /// \Brief	Pass specific options to database engine (called just after engine construction, before initialize)
      /// \param[in] options  Options to pass to engine
      //--------------------------------------------------------------
      virtual void setOptions(boost::shared_ptr<const shared::CDataContainer> options) = 0;

      //--------------------------------------------------------------
      /// \Brief		Initialize the database system
      /// \return EInitState	The initialization state
      /// \throws 		
      //--------------------------------------------------------------
      virtual void initialize() = 0;

      //--------------------------------------------------------------
      /// \Brief	Finalize the database system
      //--------------------------------------------------------------
      virtual void finalize() = 0;

      //--------------------------------------------------------------
      /// \Brief	Get database system information
      /// \return shared::CDataContainer Some database information
      //--------------------------------------------------------------
      virtual shared::CDataContainer getInformation() = 0;

      //--------------------------------------------------------------
      /// \Brief	Virtual destructor
      //--------------------------------------------------------------   
      virtual ~IDatabaseEngine()
      {
      }
   };
} //namespace dbCommon 


