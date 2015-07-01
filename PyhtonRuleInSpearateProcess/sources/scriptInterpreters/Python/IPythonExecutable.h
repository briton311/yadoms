#pragma once
#include <shared/script/ILogger.h>

//--------------------------------------------------------------
/// \brief	Python executable interface
//--------------------------------------------------------------
class IPythonExecutable
{
public:
   //--------------------------------------------------------------
   /// \brief	Destructor
   //--------------------------------------------------------------
   virtual ~IPythonExecutable() {}

   //--------------------------------------------------------------
   /// \brief	To know if Python interpreter was found in the system
   /// \return true if found
   //--------------------------------------------------------------
   virtual bool found() const = 0;

   //--------------------------------------------------------------
   /// \brief	Installed Python version
   /// \details string containing the Python version, as returned by Python (something like "Python 2.7.9")
   //--------------------------------------------------------------
   virtual std::string version() const = 0;

   //--------------------------------------------------------------
   /// \brief	Start a module (in separated process)
   /// \param[in] module The module to start
   /// \param[in] absoluteParentPath The script parent path
   /// \param[in] scriptLogger The script logger
   /// \return The start process handle (empty if error)
   /// \throw CPythonException if error
   //--------------------------------------------------------------
   virtual boost::shared_ptr<Poco::ProcessHandle> startModule(const std::string& module, const boost::filesystem::path& absoluteParentPath, boost::shared_ptr<shared::script::ILogger> scriptLogger) const = 0;
};
