  /// \file DQMLcioReaderListener.cc
/*
 *
 * DQMLcioReaderListener.cc source template automatically generated by a class generator
 * Creation date : jeu. nov. 6 2014
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
#include "dqm4hep/DQMEvent.h"
#include "dqm4hep/DQMLogging.h"
#include "dqm4hep/DQMEventClient.h"
#include "dqm4hep/lcio/DQMLcioReaderListener.h"
#include "dqm4hep/lcio/DQMLCEvent.h"

// -- lcio headers
#include "EVENT/LCEvent.h"
#include "EVENT/LCRunHeader.h"
#include "IO/LCReader.h"

namespace dqm4hep
{

DQMLcioReaderListener::DQMLcioReaderListener(IO::LCReader *pLCReader) :
		m_pLCReader(pLCReader),
		m_pEventClient(NULL),
		m_sleepTime(0),
		m_simulateSpill(false),
		m_previousTimeStamp(0)
{
	if(NULL == m_pLCReader)
		throw dqm4hep::StatusCodeException(dqm4hep::STATUS_CODE_INVALID_PTR);

	m_pLCReader->registerLCEventListener(this);
	m_pLCReader->registerLCRunListener(this);
}

//-------------------------------------------------------------------------------------------------

DQMLcioReaderListener::~DQMLcioReaderListener() 
{
	if(m_pLCReader)
	{
		m_pLCReader->removeLCEventListener(this);
		m_pLCReader->removeLCRunListener(this);
 }
}

//-------------------------------------------------------------------------------------------------

void DQMLcioReaderListener::processEvent(EVENT::LCEvent *pLCEvent)
{
	if(NULL == m_pEventClient)
		throw dqm4hep::StatusCodeException(dqm4hep::STATUS_CODE_NOT_INITIALIZED);

	DQMEvent *pDqmEvent = new DQMLCEvent();
	pDqmEvent->setEvent<EVENT::LCEvent>(pLCEvent, false);

	if(m_simulateSpill)
	{
		EVENT::long64 timeStamp = pLCEvent->getTimeStamp();
		EVENT::long64 timeStampDifference = (timeStamp - m_previousTimeStamp);

		if(timeStampDifference > 0 && m_previousTimeStamp != 0)
		{
			streamlog_out(DEBUG) << "Sleeping " << timeStampDifference << " sec ..." << std::endl;
			sleep(timeStampDifference);
		}

		m_previousTimeStamp = timeStamp;

		streamlog_out(DEBUG) << "Sending event no " << pLCEvent->getEventNumber() << std::endl;
		THROW_RESULT_IF(dqm4hep::STATUS_CODE_SUCCESS, !=, m_pEventClient->sendEvent(pDqmEvent));
		streamlog_out(DEBUG) << "Event no " << pLCEvent->getEventNumber() << " sent" << std::endl;
	}
	else
	{
		streamlog_out(DEBUG) << "Sending event no " << pLCEvent->getEventNumber() << std::endl;
		THROW_RESULT_IF(dqm4hep::STATUS_CODE_SUCCESS, !=, m_pEventClient->sendEvent(pDqmEvent));
		streamlog_out(DEBUG) << "Event no " << pLCEvent->getEventNumber() << " sent" << std::endl;

		if(m_sleepTime != 0)
		{
			timespec timesleep;
		    timesleep.tv_sec = 0;
		    timesleep.tv_nsec = 1000*m_sleepTime;
			nanosleep(&timesleep, NULL);
		}
	}

	delete pDqmEvent;
}

//-------------------------------------------------------------------------------------------------

void DQMLcioReaderListener::modifyEvent(EVENT::LCEvent *pLCEvent)
{
 /* nop */
}

//-------------------------------------------------------------------------------------------------

void DQMLcioReaderListener::processRunHeader(EVENT::LCRunHeader *pLCRunHeader)
{
	/* nop */
}

//-------------------------------------------------------------------------------------------------

void DQMLcioReaderListener::modifyRunHeader(EVENT::LCRunHeader *pLCRunHeader)
{
 /* nop */
}

//-------------------------------------------------------------------------------------------------

void DQMLcioReaderListener::setSleepTime(unsigned int usleepTime)
{
	m_sleepTime = usleepTime;
}

//-------------------------------------------------------------------------------------------------

void DQMLcioReaderListener::setSimulateSpill(bool simulate)
{
	m_simulateSpill = simulate;
}

//-------------------------------------------------------------------------------------------------

void DQMLcioReaderListener::setEventClient(DQMEventClient *pEventClient)
{
	m_pEventClient = pEventClient;
}

} 
