#include "stdafx.h"
#include "AsyncTcpClient.h"
#include "AsyncTcpClientPimpl.h"

namespace shared { namespace communication {

   CAsyncTcpClient::CAsyncTcpClient(const std::string& serverAddress, const std::string& serverPort, boost::posix_time::time_duration connectRetryDelay)
      :m_pImpl(boost::make_shared<CAsyncTcpClientPimpl>(serverAddress, serverPort, connectRetryDelay))
   {
   }

   CAsyncTcpClient::~CAsyncTcpClient()
   {
      m_pImpl.reset(); //force implementation to delete
   }

   void CAsyncTcpClient::setReceiveBufferMaxSize(std::size_t size)
   {
      m_pImpl->setReceiveBufferMaxSize(size);
   }

   void CAsyncTcpClient::start()
   {
      m_pImpl->start();
   }

   void CAsyncTcpClient::stop()
   {
      m_pImpl->stop();
   }

   bool CAsyncTcpClient::isConnected() const
   {
      return m_pImpl->isConnected();
   }

   void CAsyncTcpClient::subscribeForConnectionEvents(event::CEventHandler& forEventHandler, int forId)
   {
      m_pImpl->subscribeForConnectionEvents(forEventHandler, forId);
   }

   void CAsyncTcpClient::setReceiveBufferHandler(boost::shared_ptr<IReceiveBufferHandler> handler)
   {
      m_pImpl->setReceiveBufferHandler(handler);
   }

   void CAsyncTcpClient::send(const CByteBuffer& buffer)
   {
      m_pImpl->send(buffer);
   }

   void CAsyncTcpClient::flush()
   {
      m_pImpl->flush();
   }

} } // namespace shared::communication
