  /// \file DQMQualityTest.h
/*
 *
 * DQMQualityTest.h header template automatically generated by a class generator
 * Creation date : ven. f�vr. 20 2015
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


#ifndef DQMQUALITYTEST_H
#define DQMQUALITYTEST_H

// -- dqm4hep headers
#include "dqm4hep/DQM4HEP.h"
#include "dqm4hep/DQMPlugin.h"
#include "dqm4hep/DQMStreamable.h"

namespace dqm4hep
{

class DQMMonitorElement;
class TiXmlHandle;

//-------------------------------------------------------------------------------------------------

/** DQMQualityTestResult class
 *  Hanlde the result of a quality test
 */
class DQMQualityTestResult : public DQMStreamable
{
public:
	/** Constructor
	 */
	DQMQualityTestResult();

	/** Copy constructor
	 */
	DQMQualityTestResult(const DQMQualityTestResult &qualityTestResult);

	/** Assignment operator
	 */
	DQMQualityTestResult &operator=(const DQMQualityTestResult &qualityTestResult);

	StatusCode serialize(DQMDataStream *const pDataStream) const;
	StatusCode deserialize(DQMDataStream *const pDataStream);

public:
	std::string           m_qualityTestName;
	std::string           m_qualityTestType;
	std::string           m_message;
	DQMQuality            m_quality;
	bool                 m_isSuccessful;
};

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

/** DQMQualityTest class
 */ 
class DQMQualityTest
{
public:

	/** Constructor with quality test name (unique identifier)
	 */
	DQMQualityTest(const std::string &name);

	/** Destructor
	 */
	virtual ~DQMQualityTest();

	/** Get the quality test type
	 */
	const std::string &getType() const;

	/** Get the quality test name (unique identifier)
	 */
	const std::string &getName() const;

	/** Get the message after quality test
	 */
	const std::string &getMessage() const;

	/** Get the quality flag
	 */
	DQMQuality getQuality() const;

	/** Whether the quality test is successful
	 */
	bool isSuccessful() const;

protected:

	/** Read the settings from the xml handle
	 */
	virtual StatusCode readSettings(const TiXmlHandle xmlHandle) = 0;

	/** Initialize the quality test.
	 */
	virtual StatusCode init() = 0;

	/** Runs a quality test on the given monitor element
	 */
	virtual StatusCode run(DQMMonitorElement *pMonitorElement) = 0;

	/** Whether the quality test can be run on the monitor element
	 */
	virtual bool canRun(DQMMonitorElement *pMonitorElement) const = 0;

private:

	/** Perform the quality test result and fill the quality test result object
	 */
	StatusCode run(DQMMonitorElement *pMonitorElement, DQMQualityTestResult &result);

protected:

	std::string           m_message;
	bool                 m_isSuccessful;
	DQMQuality            m_quality;

private:

	std::string    m_type;
	std::string    m_name;

	// friendship
	friend class DQMMonitorElementManager; // to create quality test
	friend class DQMMonitorElement;     // to run quality test
};

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

/** DQMQualityTestFactory class
 */
class DQMQualityTestFactory
{
public:
	/** Destructor
	 */
	virtual ~DQMQualityTestFactory();

	/** Create a quality test
	 */
	virtual DQMQualityTest *createQualityTest(const std::string &name) const = 0;
};

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

inline DQMQualityTestFactory::~DQMQualityTestFactory()
{
	/* nop */
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

///////////////////////////
// BUILTIN QUALITY TESTS //
///////////////////////////

/** DQMMeanWithinExpectedTest class
 */
class DQMMeanWithinExpectedTest : public DQMQualityTest
{
public:
	/** Factory class
	 */
	class Factory : public DQMQualityTestFactory
	{
	public:
		/** Create a quality test
		 */
		DQMQualityTest *createQualityTest(const std::string &name) const;
	};

	/** Constructor with name
	 */
	DQMMeanWithinExpectedTest(const std::string &name);

	/** Destructor
	 */
	virtual ~DQMMeanWithinExpectedTest();

	/** Read the settings from the xml handle
	 */
	virtual StatusCode readSettings(const TiXmlHandle xmlHandle);

	/** Initialize the quality test.
	 */
	virtual StatusCode init();

	/** Runs a quality test on the given monitor element
	 */
	virtual StatusCode run(DQMMonitorElement *pMonitorElement);

	/** Whether the quality test can be run on the monitor element
	 */
	virtual bool canRun(DQMMonitorElement *pMonitorElement) const;

protected:

	unsigned int       m_strategy;
	float               m_xMin;
	float               m_xMax;
	float               m_expectedMean;
	float               m_sigma;
};

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

inline DQMQualityTest *DQMMeanWithinExpectedTest::Factory::createQualityTest(const std::string &name) const
{
	return new DQMMeanWithinExpectedTest(name);
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

/** DQMChi2FitFunctionTest class
 */
class DQMChi2FitFunctionTest : public DQMQualityTest
{
public:
	/** Factory class
	 */
	class Factory : public DQMQualityTestFactory
	{
	public:
		/** Create a quality test
		 */
		DQMQualityTest *createQualityTest(const std::string &name) const;
	};

	/** Constructor with name
	 */
	DQMChi2FitFunctionTest(const std::string &name);

	/** Destructor
	 */
	virtual ~DQMChi2FitFunctionTest();

	/** Read the settings from the xml handle
	 */
	virtual StatusCode readSettings(const TiXmlHandle xmlHandle);

	/** Initialize the quality test.
	 */
	virtual StatusCode init();

	/** Runs a quality test on the given monitor element
	 */
	virtual StatusCode run(DQMMonitorElement *pMonitorElement);

	/** Whether the quality test can be run on the monitor element
	 */
	virtual bool canRun(DQMMonitorElement *pMonitorElement) const;

protected:

	std::string                m_formula;
	std::vector<double>       m_inputParameterList;
	unsigned int             m_functionNParameters;
	double                    m_functionXMin;
	double                    m_functionXMax;
};

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

inline DQMQualityTest *DQMChi2FitFunctionTest::Factory::createQualityTest(const std::string &name) const
{
	return new DQMChi2FitFunctionTest(name);
}

} 

#endif  //  DQMQUALITYTEST_H