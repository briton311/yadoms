#pragma once
#include <shared/Export.h>
#include "IAsyncPort.h"

namespace shared { namespace communication {

   //--------------------------------------------------------------
   /// \brief	This class manage a serial port
   //--------------------------------------------------------------
   class YADOMS_SHARED_EXPORT CAsyncSerialPort : public IAsyncPort
   {  
   public:
      enum E_Parity
      {
         parity_none,
         parity_even,
         parity_odd
      };

      enum E_Stopbits
      { 
         stopbits_one, 
         stopbits_onepointfive,
         stopbits_two
      };

      enum E_FlowControl
      { 
         flow_none, 
         flow_software,
         flow_hardware
      };

      //--------------------------------------------------------------
      /// \brief	Constructor
      /// \param[in] port                 Serial port name
      /// \param[in] baudrate             Baudrate (in bauds)
      /// \param[in] parity               Parity 
      /// \param[in] characterSize        Character size 
      /// \param[in] stop_bits            Nb of stop bits
      /// \param[in] flowControl          Flow control
      /// \param[in] connectRetryDelay    Delay between 2 connection retries
      /// \param[in] flushAtConnect       If true (default), flush serial port buffers before listening on port
      //--------------------------------------------------------------
      CAsyncSerialPort(
         const std::string& port,
         const unsigned int baudRate, 
         E_Parity parity = parity_none,
         const unsigned int characterSize = 8,
         E_Stopbits stop_bits = stopbits_one,
         E_FlowControl flowControl = flow_none,
         const unsigned int connectRetryDelay = 30,
         bool flushAtConnect = true);

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CAsyncSerialPort();

      // IAsyncPort Implementation
      virtual void setReceiveBufferMaxSize(std::size_t size);
      virtual void start();
      virtual void stop();
      virtual bool isConnected() const;
      virtual void subscribeForConnectionEvents(event::CEventHandler& forEventHandler, int forId);
      virtual void setReceiveBufferHandler(boost::shared_ptr<IReceiveBufferHandler> handler);
      virtual void send(const CByteBuffer& buffer);
      virtual void flush();
      // [END] IAsyncPort Implementation

   private:
      //--------------------------------------------------------------
      /// \brief	Private implementation
      //--------------------------------------------------------------
      boost::shared_ptr<IAsyncPort> m_pImpl;
   };

} } // namespace shared::communication


