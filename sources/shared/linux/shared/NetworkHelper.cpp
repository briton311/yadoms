#include "stdafx.h"

#include "../../shared/exception/Exception.hpp"
#include "../../shared/Log.h"
#include "NetworkHelper.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <ifaddrs.h>

namespace shared
{

CNetworkHelper::CNetworkHelper()
{
}

CNetworkHelper::~CNetworkHelper()
{
}

std::vector<std::string> CNetworkHelper::getLocalIps()
{
   std::vector<std::string> result;
   boost::asio::io_service io_service;
   boost::asio::ip::udp::resolver resolver(io_service);
   boost::asio::ip::udp::resolver::query query(boost::asio::ip::host_name(), "");
   boost::asio::ip::udp::resolver::iterator iter = resolver.resolve(query);
   boost::asio::ip::udp::resolver::iterator end; // End marker.
   
   //we push every ip in the vector
   while (iter != end)
   {
      boost::asio::ip::address addr = iter->endpoint().address();
      //we manage only ipv4
      if(addr.is_v4())
         result.push_back(addr.to_string());
      
      ++iter;
   }

   return result;
}

} // namespace shared
