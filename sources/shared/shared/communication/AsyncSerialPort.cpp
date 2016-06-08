#include "stdafx.h"
#include "AsyncSerialPort.h"
#include "AsyncSerialPortPimpl.h"

namespace shared { namespace communication {


   CAsyncSerialPort::CAsyncSerialPort(
      const std::string& port,
      const unsigned int baudRate,
      E_Parity parity,
      const unsigned int characterSize,
      E_Stopbits stop_bits,
      E_FlowControl flowControl,
      const unsigned int connectRetryDelay,
      bool flushAtConnect)
   {
      boost::asio::serial_port_base::parity boostparity;

      switch (parity)
      {
      case parity_none:
         boostparity = boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none);
         break;
      case parity_even:
         boostparity = boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::even);
         break;
      case parity_odd:
         boostparity = boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::odd);
         break;
      }


      boost::asio::serial_port_base::stop_bits booststopbits;
      switch (stop_bits)
      {
      case stopbits_one:
         booststopbits = boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one);
         break;
      case stopbits_onepointfive:
         booststopbits = boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::onepointfive);
         break;
      case stopbits_two:
         booststopbits = boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::two);
         break;
      }

      boost::asio::serial_port_base::flow_control boostflowControl;
      switch (flowControl)
      {
      case flow_none:
         boostflowControl = boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none);
         break;
      case flow_software:
         boostflowControl = boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::software);
         break;
      case flow_hardware:
         boostflowControl = boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::hardware);
         break;
      }

      m_pImpl =  boost::make_shared<CAsyncSerialPortPimpl>(port,
                                             boost::asio::serial_port_base::baud_rate(baudRate),
                                             boostparity,
                                             boost::asio::serial_port_base::character_size(characterSize),
                                             booststopbits,
                                             boostflowControl,
                                             boost::posix_time::seconds(connectRetryDelay),
                                             flushAtConnect);

   }

   CAsyncSerialPort::~CAsyncSerialPort()
   {
   }

   void CAsyncSerialPort::setReceiveBufferMaxSize(std::size_t size)
   {
      m_pImpl->setReceiveBufferMaxSize(size);
   }

   void CAsyncSerialPort::start()
   {
      m_pImpl->start();
   }

   void CAsyncSerialPort::stop()
   {
      m_pImpl->stop();
   }

   bool CAsyncSerialPort::isConnected() const
   {
      return m_pImpl->isConnected();
   }

   void CAsyncSerialPort::subscribeForConnectionEvents(event::CEventHandler& forEventHandler, int forId)
   {
      m_pImpl->subscribeForConnectionEvents(forEventHandler, forId);
   }

   void CAsyncSerialPort::setReceiveBufferHandler(boost::shared_ptr<IReceiveBufferHandler> handler)
   {
      m_pImpl->setReceiveBufferHandler(handler);
   }

   void CAsyncSerialPort::send(const CByteBuffer& buffer)
   {
      m_pImpl->send(buffer);
   }
   
   void CAsyncSerialPort::flush()
   {
      m_pImpl->flush();
   }

} } // namespace shared::communication
