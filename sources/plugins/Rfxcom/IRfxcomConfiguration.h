#pragma once
#include <shared/DataContainer.h>

//--------------------------------------------------------------
/// \brief	Interface of plugin configuration
//--------------------------------------------------------------
class IRfxcomConfiguration
{
public:
   //--------------------------------------------------------------
   /// \brief	    Destructor
   //--------------------------------------------------------------
   virtual ~IRfxcomConfiguration() {}

   //--------------------------------------------------------------
   /// \brief		   Load configuration data
   /// \param [in] data The data container
   //--------------------------------------------------------------
   void initializeWith(const shared::CDataContainer &data);

   //--------------------------------------------------------------
   /// \brief	    Virtual serial port connected to the rfxcom
   //--------------------------------------------------------------
   virtual std::string getSerialPort() const = 0;
};

