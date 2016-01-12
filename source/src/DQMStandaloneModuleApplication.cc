  /// \file DQMStandaloneModuleApplication.cc
/*
 *
 * DQMStandaloneModuleApplication.cc source template automatically generated by a class generator
 * Creation date : mer. ao�t 26 2015
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

// -- dqm4hep headers
#include "dqm4hep/DQMStandaloneModuleApplication.h"
#include "dqm4hep/DQMPluginManager.h"
#include "dqm4hep/DQMMonitorElementManager.h"
#include "dqm4hep/DQMStandaloneModule.h"
#include "dqm4hep/DQMMonitorElementSender.h"
#include "dqm4hep/DQMXmlHelper.h"
#include "dqm4hep/tinyxml.h"

// -- std headers
#include <fstream>
#include <stdexcept>

// -- dim headers
#include "dis.hxx"
#include "dic.hxx"

namespace dqm4hep
{

DQMStandaloneModuleApplication::DQMStandaloneModuleApplication() :
		DQMModuleApplication(),
		m_type("StandaloneModule"),
		m_name("Unknown"),
		m_sleepTimeBetweenTwoProcess(100),
		m_publicationPeriod(10),
		m_resetAtEndOfPeriod(true),
		m_returnStatusCode(STATUS_CODE_SUCCESS)
{
	/* nop */
}

//-------------------------------------------------------------------------------------------------

DQMStandaloneModuleApplication::~DQMStandaloneModuleApplication() 
{
	/* nop */
}

//-------------------------------------------------------------------------------------------------

const std::string &DQMStandaloneModuleApplication::getType() const
{
	return m_type;
}

//-------------------------------------------------------------------------------------------------

const std::string &DQMStandaloneModuleApplication::getName() const
{
	return m_name;
}

//-------------------------------------------------------------------------------------------------

StatusCode DQMStandaloneModuleApplication::readSettings( const std::string &settingsFileName )
{
	if(this->isInitialized())
		return STATUS_CODE_ALREADY_INITIALIZED;

    TiXmlDocument xmlDocument(settingsFileName);

    if (!xmlDocument.LoadFile())
    {
        std::cout << "DQMAnalysisModuleApplication::readSettings - Invalid xml file." << std::endl;
        return STATUS_CODE_FAILURE;
    }

    const TiXmlHandle xmlDocumentHandle(&xmlDocument);
    const TiXmlHandle xmlHandle = TiXmlHandle(xmlDocumentHandle.FirstChildElement().Element());

	TiXmlElement *const pMeCollectorElement = xmlHandle.FirstChildElement("monitorelementcollector").Element();

	if(NULL == pMeCollectorElement)
	{
		streamlog_out(ERROR) << "<monitorelementcollector> element not found !" << std::endl;
		return STATUS_CODE_NOT_FOUND;
	}

	std::string monitorElementCollector;
	RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, DQMXmlHelper::getAttribute(pMeCollectorElement, "name", monitorElementCollector));
	this->getMonitorElementSender()->setCollectorName(monitorElementCollector);

	m_sleepTimeBetweenTwoProcess = 100;
	m_publicationPeriod = 2;
	m_resetAtEndOfPeriod = true;

	TiXmlElement *const pCycleElement = xmlHandle.FirstChildElement("cycle").Element();

	if(pCycleElement)
	{
		RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, DQMXmlHelper::getAttribute(pCycleElement, "sleep", m_sleepTimeBetweenTwoProcess));
		RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, DQMXmlHelper::getAttribute(pCycleElement, "period", m_publicationPeriod));
		RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, DQMXmlHelper::getAttribute(pCycleElement, "reset", m_resetAtEndOfPeriod));
	}

	// module settings
	TiXmlElement *const pXmlElement = xmlHandle.FirstChildElement("module").Element();

	std::string type;
	std::string name;

	RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, DQMXmlHelper::getAttribute(pXmlElement, "type", type));
	RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, DQMXmlHelper::getAttribute(pXmlElement, "name", name));

	if(this->getModuleType().empty())
		this->setModuleType(type);

	if(this->getModuleName().empty())
		this->setModuleName(name);

	DQMStandaloneModule *pStandaloneModule = NULL;

	streamlog_out( MESSAGE ) << "Query analysis module to PluginManager ... " << std::endl;

	RETURN_RESULT_IF( STATUS_CODE_SUCCESS, !=,
			DQMPluginManager::instance()->getCastedPluginClone< DQMStandaloneModule >( this->getModuleType(), pStandaloneModule ) );

	this->setModule(pStandaloneModule);

	streamlog_out( MESSAGE ) << "Query analysis module to PluginManager ... OK" << std::endl;

	TiXmlHandle moduleHandle(pXmlElement);

//	TiXmlElement *const pQualityTestsElement = moduleHandle.FirstChildElement("qualitytests").Element();
//
//	if(pQualityTestsElement)
//	{
//		TiXmlHandle qualityTestsHandle(pQualityTestsElement);
//		RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, this->getMonitorElementManager()->readSettings(qualityTestsHandle));
//	}

//	TiXmlElement *const pSettingsElement = moduleHandle.FirstChildElement("settings").Element();
//
//	if(pSettingsElement)
//	{
//		TiXmlHandle settingsHandle(pSettingsElement);

	streamlog_out( MESSAGE ) << "Reading settings of active module '" << this->getModule()->getName() << "' ..." << std::endl;
	RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, this->getModule()->readSettings(moduleHandle));
	streamlog_out( MESSAGE ) << "Reading settings of active module '" << this->getModule()->getName() << "' ... OK" << std::endl;
//	}

	streamlog_out( MESSAGE ) << "Initializing active module '" << this->getModule()->getName() << "' ..." << std::endl;
	RETURN_RESULT_IF( STATUS_CODE_SUCCESS, !=, this->getModule()->initModule() );
	streamlog_out( MESSAGE ) << "Initializing active module '" << this->getModule()->getName() << "' ... OK" << std::endl;

	m_name = this->getModule()->getType() + "/" + this->getModule()->getName();

	DimBrowser browser;
	int nServers = browser.getServers();

	std::string moduleServerName = "DQM4HEP/Module/" + this->getModule()->getName();

	if(nServers)
	{
		char *serverName;
		char *node;

		while(browser.getNextServer(serverName, node))
		{
			if(moduleServerName == serverName)
			{
				streamlog_out(ERROR) << "Module '" << this->getModule()->getName() << "' already registered over the network.\n"
						<< "Please, change the module name or stop the other module application with the same name !" << std::endl;
				return STATUS_CODE_ALREADY_PRESENT;
			}
		}
	}

	DimServer::start( moduleServerName.c_str() );

	this->setInitialized(true);

	return STATUS_CODE_SUCCESS;
}

//-------------------------------------------------------------------------------------------------

StatusCode DQMStandaloneModuleApplication::run()
{
	if(!this->isInitialized())
		return STATUS_CODE_NOT_INITIALIZED;

	time_t publicationTimer = time(0);
	bool callStartOfCycle = true;

	// get casted module for easier manipulation
	DQMStandaloneModule *pStandaloneModule = dynamic_cast<DQMStandaloneModule *>(this->getModule());

	while(!this->shouldStopApplication())
	{
		if(callStartOfCycle)
		{
			streamlog_out(MESSAGE) << "Start of cycle !!!" << std::endl;
			RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, pStandaloneModule->startOfCycle());
			callStartOfCycle = false;
		}

		RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, pStandaloneModule->process());

		if(difftime(time(0), publicationTimer) >= m_publicationPeriod)
		{
			streamlog_out(MESSAGE) << "End of of cycle !!!" << std::endl;
			RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, pStandaloneModule->endOfCycle());

			streamlog_out(MESSAGE) << "Sending monitor elements to collector" << std::endl;

			DQMMonitorElementList monitorElementListToPublish;
			RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, this->getMonitorElementManager()
					->getMonitorElementListToPublish(monitorElementListToPublish));

			streamlog_out(MESSAGE) << "Number of monitor elements sent : " << monitorElementListToPublish.size() << std::endl;

			RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, this->getMonitorElementSender()->sendMonitorElements(pStandaloneModule->getName(), monitorElementListToPublish));

			if(m_resetAtEndOfPeriod)
			{
				streamlog_out(MESSAGE) << "Reseting monitor elements" << std::endl;
				RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, this->getMonitorElementManager()
						->resetMonitorElements());
			}

			publicationTimer = time(0);
			callStartOfCycle = true;
		}

		timespec timesleep;
	    timesleep.tv_sec = 0;
	    timesleep.tv_nsec = m_sleepTimeBetweenTwoProcess*1000;
		nanosleep(&timesleep, NULL);
	}

	RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, pStandaloneModule->endModule());

	return STATUS_CODE_SUCCESS;
}

bool DQMStandaloneModuleApplication::shouldStopCycle() const
{
	// unused
	return true;
}

} 
