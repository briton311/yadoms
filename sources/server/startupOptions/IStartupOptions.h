//
// Yadoms startup options interface
//
#pragma once

#include "DatabaseEngine.h"
#include <Poco/Nullable.h>

namespace startupOptions {
   class IStartupOptions
   {
   public:
      //--------------------------------------------------------------
      /// \brief	    Destructor
      //--------------------------------------------------------------
      virtual ~IStartupOptions() {}

      //--------------------------------------------------------------
      /// \brief	    Get the log level
      /// \return     Configured log level
      //--------------------------------------------------------------
      virtual std::string getLogLevel() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Get the port number
      /// \return     Configured port number
      //--------------------------------------------------------------
      virtual unsigned short getWebServerPortNumber() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Get the SSL port number
      /// \return     Configured SSL port number
      //--------------------------------------------------------------
      virtual unsigned short getSSLWebServerPortNumber() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Determine if the SSL port is activated
      /// \return     true if SSL is activated 
      //--------------------------------------------------------------
      virtual bool getIsWebServerUseSSL() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Get the web server ip address
      /// \return     Configured web server ip address
      //--------------------------------------------------------------
      virtual std::string getWebServerIPAddress() const = 0;
      
      //--------------------------------------------------------------
      /// \brief	    Get the web server initial path
      /// \return     Configured web server initial path
      //--------------------------------------------------------------
      virtual std::string getWebServerInitialPath() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Get the database engine to use
      /// \return     The database engine (sqlite|postgresql)
      //--------------------------------------------------------------
      virtual EDatabaseEngine getDatabaseEngine() const = 0;

      //--------------------------------------------------------------
      /// \brief	    SQLite options
      //--------------------------------------------------------------
      virtual std::string getDatabaseSqliteFile() const = 0;
      virtual std::string getDatabaseSqliteBackupFile() const = 0;

      //--------------------------------------------------------------
      /// \brief	    PostgreSQL options
      //--------------------------------------------------------------
      virtual std::string getDatabasePostgresqlHost() const = 0;
      virtual unsigned int getDatabasePostgresqlPort() const = 0;
      virtual std::string getDatabasePostgresqlDbName() const = 0;
      virtual std::string getDatabasePostgresqlLogin() const = 0;
      virtual std::string getDatabasePostgresqlPassword() const = 0;
      virtual Poco::Nullable<std::string> getDatabasePostgresqlHostAddr() const = 0;
      virtual Poco::Nullable<int> getDatabasePostgresqlConnectTimeout() const = 0;
      virtual Poco::Nullable<std::string> getDatabasePostgresqlClientEncoding() const = 0;
      virtual Poco::Nullable<std::string> getDatabasePostgresqlOptions() const = 0;
      virtual Poco::Nullable<int> getDatabasePostgresqlKeepAlives() const = 0;
      virtual Poco::Nullable<int> getDatabasePostgresqlKeepAlivesIdle() const = 0;
      virtual Poco::Nullable<int> getDatabasePostgresqlKeepAlivesInterval() const = 0;
      virtual Poco::Nullable<int> getDatabasePostgresqlKeepAlivesCount() const = 0;
      virtual Poco::Nullable<std::string> getDatabasePostgresqlSslMode() const = 0;
      virtual Poco::Nullable<int> getDatabasePostgresqlRequireSsl() const = 0;
      virtual Poco::Nullable<int> getDatabasePostgresqlSslCompression() const = 0;
      virtual Poco::Nullable<std::string> getDatabasePostgresqlSslCert() const = 0;
      virtual Poco::Nullable<std::string> getDatabasePostgresqlSslKey() const = 0;
      virtual Poco::Nullable<std::string> getDatabasePostgresqlSslRootCert() const = 0;
      virtual Poco::Nullable<std::string> getDatabasePostgresqlSslRevocationList() const = 0;
      virtual Poco::Nullable<std::string> getDatabasePostgresqlRequirePeer() const = 0;
      virtual Poco::Nullable<std::string> getDatabasePostgresqlKerberos() const = 0;
      virtual Poco::Nullable<std::string> getDatabasePostgresqlGssLib() const = 0;
      virtual Poco::Nullable<std::string> getDatabasePostgresqlService() const = 0;


      //--------------------------------------------------------------
      /// \brief	    Get the plugin path
      /// \return     The plugin path
      /// \note       If not exist, it returns "plugins"
      //--------------------------------------------------------------
      virtual std::string getPluginsPath() const = 0;    
      
      //--------------------------------------------------------------
      /// \brief	    Get the script interpreters path
      /// \return     The script interpreters path
      /// \note       If not exist, it returns "scriptInterpreters"
      //--------------------------------------------------------------
      virtual std::string getScriptInterpretersPath() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Get the no password flag
      /// \return     flag indicating if password check is disabled for this instance
      //--------------------------------------------------------------
      virtual bool getNoPasswordFlag() const = 0;     
      
      //--------------------------------------------------------------
      /// \brief	    Indicate if yadoms is runnings as a service
      /// \return     true if yadoms is runnings as a service
      //--------------------------------------------------------------
      virtual bool getIsRunningAsService() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Get the update site URI
      /// \return     The update site URI
      /// \note       If not exist, it returns "http://www.yadoms.com/downloads/update/"
      //--------------------------------------------------------------
      virtual std::string getUpdateSiteUri() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Get the acquisition lifetime
      /// \return     The acquisition lifetime
      //--------------------------------------------------------------
      virtual int getDatabaseAcquisitionLifetime() const = 0;

      //--------------------------------------------------------------
      /// \brief	    Tell if the developer pode is enabled
      /// \return     true the developer pode is enabled
      //--------------------------------------------------------------
      virtual bool getDeveloperMode() const = 0;
   };

} // namespace startupOptions