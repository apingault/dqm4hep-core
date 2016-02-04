  /// \file DQMAnalysisModuleApplication.h
/*
 *
 * DQMAnalysisModuleApplication.h header template automatically generated by a class generator
 * Creation date : dim. nov. 9 2014
 *
 * This file is part of DQM4HEP libraries.
 * 
 * DQM4HEP is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * based upon these libraries are permitted. Any copy of these libraries
 * must include this copyright notice.
 * 
 * DQM4HEP is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with DQM4HEP.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * @author Remi Ete
 * @copyright CNRS , IPNL
 */


#ifndef DQMANALYSISMODULEAPPLICATION_H
#define DQMANALYSISMODULEAPPLICATION_H

// -- dqm4hep headers
#include "dqm4hep/DQM4HEP.h"
#include "dqm4hep/DQMModuleApplication.h"

namespace dqm4hep
{

class DQMEventClient;
class DQMRunControlClient;
class DQMCycle;
class DQMArchiver;
class DQMEvent;
class TiXmlHandle;

/** DQMAnalysisModuleApplication class
 */ 
class DQMAnalysisModuleApplication : public DQMModuleApplication
{
	friend class DQMModuleApi;

public:
	/** Settings class
 	 */
	class Settings
	{
	public:
		/** Constructor
		 */
		Settings();

		/** Print the parameters
		 */
		void print();

	public:
		std::string         m_settingsFileName;
		std::string         m_archiveDirectory;
		bool                m_shouldOpenArchive;
		std::string         m_runControlType;
		std::string         m_runControlName;
		std::string         m_eventCollector;
		std::string         m_subEventIdentifier;
		std::string         m_eventStreamer;
		std::string         m_monitorElementCollector;
		std::string         m_cycleType;
		float               m_cycleValue;
		unsigned int       m_cycleTimeout;
	};

public:
	/** Constructor
	 */
	DQMAnalysisModuleApplication();

	/** Destructor
	 */
	~DQMAnalysisModuleApplication();

	/** Get the application type ("AnalysisModule")
	 */
	const std::string &getType() const;

	/** Get the application name ("ModuleType/ModuleName")
	 */
	const std::string &getName() const;

	/** Read settings from a xml file.
	 *  Initialize the application. Declare all the services,
	 *  load the shared libraries, configure the active module
	 */
	StatusCode readSettings( const std::string &settingsFile );

	/** Run the application.
	 */
	StatusCode run();

	/** Whether the module application has to stop processing cycles
	 */
	bool shouldStopCycle() const;

public:
	/** Get the event client
	 */
	DQMEventClient *getEventClient() const;

	/** Get the run control client
	 */
	DQMRunControlClient *getRunControlClient() const;

private:
	/** Configure the module
	 */
	StatusCode configureModule(const TiXmlHandle xmlHandle);

	/** Configure network interface
	 */
	StatusCode configureNetwork(const TiXmlHandle xmlHandle);

	/** Configure the workflow cycle
	 */
	StatusCode configureCycle(const TiXmlHandle xmlHandle);

	/** Configure the archiver
	 */
	StatusCode configureArchiver(const TiXmlHandle xmlHandle);

	/** Start all services. Should be called after init
	 */
	StatusCode startServices();

	/** Get the current run number that was started (not from run control)
	 */
	int getCurrentRunNumber() const;

private:
	// members
	Settings                       m_settings;

	DQMEventClient                *m_pEventClient;
	DQMRunControlClient           *m_pRunControlClient;
	DQMCycle                      *m_pCycle;
	DQMArchiver                   *m_pArchiver;

	std::string                   m_applicationType;
	std::string                   m_applicationName;

	int                           m_runNumber;
}; 

} 

#endif  //  DQMANALYSISMODULEAPPLICATION_H
