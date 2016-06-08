#include "stdafx.h"
#include "AsyncSerialPortPimpl.h"
#include <shared/Log.h>
#include <shared/Peripherals.h>
#include <shared/exception/InvalidParameter.hpp>
#include "PortException.hpp"
#include "Buffer.hpp"

namespace shared { namespace communication {

CAsyncSerialPortPimpl::CAsyncSerialPortPimpl(
   const std::string& port,
   boost::asio::serial_port_base::baud_rate baudrate,
   boost::asio::serial_port_base::parity parity,
   boost::asio::serial_port_base::character_size characterSize,
   boost::asio::serial_port_base::stop_bits stop_bits,
   boost::asio::serial_port_base::flow_control flowControl,
   boost::posix_time::time_duration connectRetryDelay,
   bool flushAtConnect)
   :m_boostSerialPort(m_ioService),
   m_port(port),
   m_baudrate(baudrate), m_parity(parity), m_characterSize(characterSize), m_stop_bits(stop_bits), m_flowControl(flowControl),
   m_asyncReadBuffer(512),
   m_connectStateEventHandler(NULL),
   m_connectStateEventId(event::kNoEvent),
   m_connectRetryDelay(connectRetryDelay),
   m_connectRetryTimer(m_ioService),
   m_flushAtConnect(flushAtConnect)
{
}

CAsyncSerialPortPimpl::~CAsyncSerialPortPimpl()
{
   stop();
}

void CAsyncSerialPortPimpl::setReceiveBufferMaxSize(std::size_t size)
{
   m_asyncReadBuffer.resize(size);
}

void CAsyncSerialPortPimpl::start()
{
   if (!!m_asyncThread)
      return;  // Already started

   // Try to connect
   tryConnect();
   m_asyncThread.reset(new boost::thread(boost::bind(&boost::asio::io_service::run, &m_ioService)));
}

void CAsyncSerialPortPimpl::stop()
{
   if (!m_asyncThread)
      return;  // Already stopped

   disconnect();

   m_ioService.stop();
   m_asyncThread->join();
   m_asyncThread.reset();
}

bool CAsyncSerialPortPimpl::connect()
{
   // Open the port
   try
   {
      m_boostSerialPort.open(m_port);
   }
   catch (boost::system::system_error& e)
   {
      YADOMS_LOG(error) << "Failed to open serial port " << e.what(); 
      return false;
   }

   // Set options to the port
   m_boostSerialPort.set_option(m_baudrate);
   m_boostSerialPort.set_option(m_parity);
   m_boostSerialPort.set_option(m_characterSize);
   m_boostSerialPort.set_option(m_stop_bits);
   m_boostSerialPort.set_option(m_flowControl);

   return true;
}

void CAsyncSerialPortPimpl::disconnect()
{
   if (!m_boostSerialPort.is_open())
      return;

   // Close the port
   try
   {
      m_boostSerialPort.close();
   }
   catch (boost::system::system_error& e)
   {
      YADOMS_LOG(error) << "Failed to close serial port " << e.what();
      return;
   }
}

bool CAsyncSerialPortPimpl::isConnected() const
{
   return m_boostSerialPort.is_open();
}

void CAsyncSerialPortPimpl::subscribeForConnectionEvents(event::CEventHandler& forEventHandler, int forId)
{
   m_connectStateEventHandler = &forEventHandler;
   m_connectStateEventId = forId;
}

void CAsyncSerialPortPimpl::setReceiveBufferHandler(boost::shared_ptr<IReceiveBufferHandler> handler)
{
   m_receiveBufferHandler = handler;
}

void CAsyncSerialPortPimpl::flush()
{
   // Hardware flush
   CPeripherals::flushSerialPort(m_boostSerialPort);

   // Clear the receive buffer
   if (!!m_receiveBufferHandler)
      m_receiveBufferHandler->flush();
}

void CAsyncSerialPortPimpl::reconnectTimerHandler(const boost::system::error_code& error)
{
   if (error != 0)
      throw exception::CException("Error code should be 0 here");

   tryConnect();
}

void CAsyncSerialPortPimpl::tryConnect()
{
   if (isConnected())
   {
      YADOMS_LOG(warning) << "Already connected";
   }
   else
   {
      if (!connect())
      {
         // Fail to reconnect, retry after a certain delay
         m_connectRetryTimer.expires_from_now(m_connectRetryDelay);
         m_connectRetryTimer.async_wait(boost::bind(&CAsyncSerialPortPimpl::reconnectTimerHandler, this, boost::asio::placeholders::error));
         return;
      }

      // Connected
      notifyEventHandler(true);

      // Flush buffers if required
      if (m_flushAtConnect)
         flush();
   }

   // Start listening on the port
   startRead();
}

void CAsyncSerialPortPimpl::startRead()
{
   // Start an asynchronous read and call readCompleted when it completes or fails 
   m_boostSerialPort.async_read_some(boost::asio::buffer(m_asyncReadBuffer.begin(), m_asyncReadBuffer.size()),
      boost::bind(&CAsyncSerialPortPimpl::readCompleted, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void CAsyncSerialPortPimpl::readCompleted(const boost::system::error_code& error, std::size_t bytesTransferred)
{
   if (error)
   {
      // boost::asio::error::operation_aborted is fired when stop is required
      if (error == boost::asio::error::operation_aborted)
         return;     // Normal stop

      // Error ==> disconnecting
      YADOMS_LOG(error) << "Serial port read error : " << error.message();
      disconnect();
      notifyEventHandler(false);
      return;
   }

   // Read OK
   CByteBuffer buffer(bytesTransferred);
   memcpy(buffer.begin(), m_asyncReadBuffer.begin(), bytesTransferred);

   if (!!m_receiveBufferHandler)
      m_receiveBufferHandler->push(buffer);

   // Restart read
   startRead();
}

void CAsyncSerialPortPimpl::send(const CByteBuffer& buffer)
{
   try
   {
      m_boostSerialPort.write_some(boost::asio::const_buffers_1(buffer.begin(), buffer.size()));
   }
   catch (boost::system::system_error& e)
   {
      // boost::asio::error::eof is the normal stop
      if (e.code() != boost::asio::error::eof)
      {
         YADOMS_LOG(error) << "Serial port write error : " << e.what();
         disconnect();
      }

      notifyEventHandler(false);

      throw CPortException(
         (e.code() == boost::asio::error::eof) ? CPortException::kConnectionClosed : CPortException::kConnectionError,
         e.what());
   }
}

void CAsyncSerialPortPimpl::notifyEventHandler(bool isConnected)
{
   if (m_connectStateEventHandler)
      m_connectStateEventHandler->postEvent<bool>(m_connectStateEventId, isConnected);
}

} } // namespace shared::communication
