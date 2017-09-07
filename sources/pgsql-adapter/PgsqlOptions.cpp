#include "stdafx.h"
#include "PgsqlOptions.h"


namespace libPgsqlAdapter
{
   CPgsqlOptions::CPgsqlOptions(boost::shared_ptr<const shared::CDataContainer> options)
      : m_options(options)
   {
   }

   CPgsqlOptions::~CPgsqlOptions()
   {
   }

   std::string CPgsqlOptions::getDatabaseHost() const
   {
      return m_options->getWithDefault<std::string>("host", "127.0.0.1");
   }

   unsigned CPgsqlOptions::getDatabasePort() const
   {
      return m_options->getWithDefault<unsigned int>("port", 5432);
   }

   std::string CPgsqlOptions::getDatabaseDbName() const
   {
      return boost::to_lower_copy(m_options->getWithDefault<std::string>("dbname", "yadoms"));
   }

   std::string CPgsqlOptions::getDatabaseLogin() const
   {
      return m_options->getWithDefault<std::string>("login", "");
   }

   std::string CPgsqlOptions::getDatabasePassword() const
   {
      return m_options->getWithDefault<std::string>("password", "");
   }

   Poco::Nullable<std::string> CPgsqlOptions::getDatabaseHostAddr() const
   {
      if (m_options->exists("hostaddr"))
         return m_options->getWithDefault<std::string>("hostaddr", "");
      return Poco::NULL_GENERIC;
   }

   Poco::Nullable<int> CPgsqlOptions::getDatabaseConnectTimeout() const
   {
      if (m_options->exists("connect-timeout"))
         return m_options->getWithDefault<int>("connect-timeout", 60);
      return Poco::NULL_GENERIC;
   }

   Poco::Nullable<std::string> CPgsqlOptions::getDatabaseClientEncoding() const
   {
      if (m_options->exists("client-encoding"))
         return m_options->getWithDefault<std::string>("client-encoding", "");
      return Poco::NULL_GENERIC;
   }

   Poco::Nullable<std::string> CPgsqlOptions::getDatabaseOptions() const
   {
      if (m_options->exists("options"))
         return m_options->getWithDefault<std::string>("options", "");
      return Poco::NULL_GENERIC;
   }

   Poco::Nullable<int> CPgsqlOptions::getDatabaseKeepAlives() const
   {
      if (m_options->exists("keep-alives"))
         return m_options->getWithDefault<int>("keep-alives", 1);
      return Poco::NULL_GENERIC;
   }

   Poco::Nullable<int> CPgsqlOptions::getDatabaseKeepAlivesIdle() const
   {
      if (m_options->exists("keep-alives-idle"))
         return m_options->getWithDefault<int>("keep-alives-idle", 0 /* = use system default*/);
      return Poco::NULL_GENERIC;
   }

   Poco::Nullable<int> CPgsqlOptions::getDatabaseKeepAlivesInterval() const
   {
      if (m_options->exists("keep-alives-interval"))
         return m_options->getWithDefault<int>("keep-alives-interval", 0 /* = use system default*/);
      return Poco::NULL_GENERIC;
   }

   Poco::Nullable<int> CPgsqlOptions::getDatabaseKeepAlivesCount() const
   {
      if (m_options->exists("keep-alives-count"))
         return m_options->getWithDefault<int>("keep-alives-count", 0 /* = use system default*/);
      return Poco::NULL_GENERIC;
   }

   Poco::Nullable<std::string> CPgsqlOptions::getDatabaseSslMode() const
   {
      if (m_options->exists("ssl-mode"))
         return m_options->getWithDefault<std::string>("ssl-mode", "prefer");
      return Poco::NULL_GENERIC;
   }

   Poco::Nullable<int> CPgsqlOptions::getDatabaseRequireSsl() const
   {
      if (m_options->exists("require-ssl"))
         return m_options->getWithDefault<int>("require-ssl", 0);
      return Poco::NULL_GENERIC;
   }

   Poco::Nullable<int> CPgsqlOptions::getDatabaseSslCompression() const
   {
      if (m_options->exists("ssl-compression"))
         return m_options->getWithDefault<int>("ssl-compression", 1);
      return Poco::NULL_GENERIC;
   }

   Poco::Nullable<std::string> CPgsqlOptions::getDatabaseSslCert() const
   {
      if (m_options->exists("ssl-cert"))
         return m_options->getWithDefault<std::string>("ssl-cert", "");
      return Poco::NULL_GENERIC;
   }

   Poco::Nullable<std::string> CPgsqlOptions::getDatabaseSslKey() const
   {
      if (m_options->exists("ssl-key"))
         return m_options->getWithDefault<std::string>("ssl-key", "");
      return Poco::NULL_GENERIC;
   }

   Poco::Nullable<std::string> CPgsqlOptions::getDatabaseSslRootCert() const
   {
      if (m_options->exists("ssl-root"))
         return m_options->getWithDefault<std::string>("ssl-root", "");
      return Poco::NULL_GENERIC;
   }

   Poco::Nullable<std::string> CPgsqlOptions::getDatabaseSslRevocationList() const
   {
      if (m_options->exists("ssl-revocation"))
         return m_options->getWithDefault<std::string>("ssl-revocation", "");
      return Poco::NULL_GENERIC;
   }

   Poco::Nullable<std::string> CPgsqlOptions::getDatabaseRequirePeer() const
   {
      if (m_options->exists("require-peer"))
         return m_options->getWithDefault<std::string>("require-peer", "");
      return Poco::NULL_GENERIC;
   }

   Poco::Nullable<std::string> CPgsqlOptions::getDatabaseKerberos() const
   {
      if (m_options->exists("kerberos"))
         return m_options->getWithDefault<std::string>("kerberos", "");
      return Poco::NULL_GENERIC;
   }

   Poco::Nullable<std::string> CPgsqlOptions::getDatabaseGssLib() const
   {
      if (m_options->exists("gss-lib"))
         return m_options->getWithDefault<std::string>("gss-lib", "");
      return Poco::NULL_GENERIC;
   }

   Poco::Nullable<std::string> CPgsqlOptions::getDatabaseService() const
   {
      if (m_options->exists("service"))
         return m_options->getWithDefault<std::string>("service", "");
      return Poco::NULL_GENERIC;
   }
} //namespace libPgsqlAdapter 


