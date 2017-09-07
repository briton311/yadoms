#pragma once
#include "FrameBase.h"
#include <dbCommon/entities/Entities.h>

namespace web
{
   namespace ws
   {
      //-----------------------------
      ///\brief Frame which can be send to GUI to notify for log event
      //-----------------------------
      class CLogEventFrame : public CFrameBase
      {
      public:
         //-----------------------------
         ///\brief Constructor
         ///\param [in]    logEvent     The log event data
         //-----------------------------
         explicit CLogEventFrame(boost::shared_ptr<const dbCommon::entities::CEventLogger> logEvent);

         //-----------------------------
         ///\brief Destructor
         //-----------------------------
         virtual ~CLogEventFrame();
      };
   } //namespace ws
} //namespace web


