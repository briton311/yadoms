#pragma once
#include <shared/DataContainer.h>

namespace libPgsqlAdapter
{
   class CPgsqlOptions
   {
   public:
      explicit CPgsqlOptions(boost::shared_ptr<const shared::CDataContainer> options);
      virtual ~CPgsqlOptions();

      std::string getDatabaseHost() const;
      unsigned int getDatabasePort() const;
      std::string getDatabaseDbName() const;
      std::string getDatabaseLogin() const;
      std::string getDatabasePassword() const;
      Poco::Nullable<std::string> getDatabaseHostAddr() const;
      Poco::Nullable<int> getDatabaseConnectTimeout() const;
      Poco::Nullable<std::string> getDatabaseClientEncoding() const;
      Poco::Nullable<std::string> getDatabaseOptions() const;
      Poco::Nullable<int> getDatabaseKeepAlives() const;
      Poco::Nullable<int> getDatabaseKeepAlivesIdle() const;
      Poco::Nullable<int> getDatabaseKeepAlivesInterval() const;
      Poco::Nullable<int> getDatabaseKeepAlivesCount() const;
      Poco::Nullable<std::string> getDatabaseSslMode() const;
      Poco::Nullable<int> getDatabaseRequireSsl() const;
      Poco::Nullable<int> getDatabaseSslCompression() const;
      Poco::Nullable<std::string> getDatabaseSslCert() const;
      Poco::Nullable<std::string> getDatabaseSslKey() const;
      Poco::Nullable<std::string> getDatabaseSslRootCert() const;
      Poco::Nullable<std::string> getDatabaseSslRevocationList() const;
      Poco::Nullable<std::string> getDatabaseRequirePeer() const;
      Poco::Nullable<std::string> getDatabaseKerberos() const;
      Poco::Nullable<std::string> getDatabaseGssLib() const;
      Poco::Nullable<std::string> getDatabaseService() const;

   private:
      const boost::shared_ptr<const shared::CDataContainer> m_options;
   };
} //namespace libPgsqlAdapter 


