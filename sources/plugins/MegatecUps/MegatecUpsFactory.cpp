#include "stdafx.h"
#include "MegatecUpsFactory.h"
#include <shared/Log.h>
#include <shared/communication/AsyncSerialPort.h>
#include <shared/communication/EOFReceiveBufferHandler.h>


CMegatecUpsFactory::~CMegatecUpsFactory()
{
}


boost::shared_ptr<shared::communication::IAsyncPort> CMegatecUpsFactory::constructPort(
   const IMegatecUpsConfiguration& configuration,
   shared::event::CEventHandler& eventHandler,
   int evtPortConnectionId,
   int evtPortDataReceived)
{
   YADOMS_LOG(information) << "Connecting UPS on serial port " << configuration.getSerialPort() << "...";
   boost::shared_ptr<shared::communication::IAsyncPort> port(new shared::communication::CAsyncSerialPort(
      configuration.getSerialPort(),
      2400,
      shared::communication::CAsyncSerialPort::parity_none,
      8,
      shared::communication::CAsyncSerialPort::stopbits_one,
      shared::communication::CAsyncSerialPort::flow_none));

   port->subscribeForConnectionEvents(eventHandler, evtPortConnectionId);

   boost::shared_ptr<shared::communication::IReceiveBufferHandler> receiveBufferHandler(
      new shared::communication::CEOFReceiveBufferHandler(eventHandler, evtPortDataReceived, MEGATEC_EOF));
   port->setReceiveBufferHandler(receiveBufferHandler);

   return port;
}