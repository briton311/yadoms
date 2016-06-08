#pragma once
#include <shared/Export.h>
#include "IAsyncPort.h"

namespace shared { namespace communication {

   //--------------------------------------------------------------
   /// \brief	This class manage a serial port
   //--------------------------------------------------------------
   class YADOMS_SHARED_EXPORT CAsyncTcpClient : public IAsyncPort
   {  
   public:
      //--------------------------------------------------------------
      /// \brief	Constructor
      /// \param[in] serverAddress        Server IP address or DNS name
      /// \param[in] serverPort           Server portNumber
      /// \param[in] connectRetryDelay    Delay between 2 connection retries
      //--------------------------------------------------------------
      CAsyncTcpClient(
         const std::string& serverAddress,
         const std::string& serverPort,
         boost::posix_time::time_duration connectRetryDelay = boost::posix_time::seconds(30));

      //--------------------------------------------------------------
      /// \brief	Destructor
      //--------------------------------------------------------------
      virtual ~CAsyncTcpClient();

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


