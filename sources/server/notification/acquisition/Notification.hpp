#pragma once

#include "../change/Notification.hpp"

namespace notification
{
   namespace acquisition
   {
      //-----------------------------
      ///\brief Interface for basic notification (handle a typed object)
      //-----------------------------
      class CNotification : public change::CNotification<dbCommon::entities::CAcquisition>
      {
      public:
         //-----------------------------
         ///\brief Constructor
         ///\param [in] obj   The notification object
         //-----------------------------
         explicit CNotification(boost::shared_ptr<dbCommon::entities::CAcquisition> acquisition)
            : change::CNotification<dbCommon::entities::CAcquisition>(acquisition, change::EChangeType::kCreate)
         {
         }

         //-----------------------------
         ///\brief Destructor
         //-----------------------------
         virtual ~CNotification()
         {
         }

         //-----------------------------
         ///\brief Get the acquisition
         ///\return The acquisition
         //-----------------------------
         boost::shared_ptr<dbCommon::entities::CAcquisition> getAcquisition()
         {
            return getObject();
         }
      };
   } //namespace acquisition
} //namespace notification


