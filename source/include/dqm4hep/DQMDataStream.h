/*
 *
 * DQMDataStream.h header template automatically generated by a class generator
 * Creation date : mar. janv. 27 2015
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


#ifndef DQMDATASTREAM_H
#define DQMDATASTREAM_H

// -- dqm4hep headers
#include "dqm4hep/DQM4HEP.h"

// -- std headers
#include <cstring>

namespace dqm4hep
{

/** DQMDataStream class
 */ 
class DQMDataStream 
{
public:
	/** Constructor with an initial buffer size.
	 *
	 *  The buffer can be reallocated if there is not
	 *  enough place to write bytes. Then maxBufferSize
	 *  will change according to needed size
	 */
	DQMDataStream(dqm_uint maxBufferSize);

	/** Destructor
	 */
	virtual ~DQMDataStream();

	/** Reset the buffer ptr to beginning of the buffer
	 */
	void reset();

	/** Get the buffer
	 */
	char *getBuffer() const;

	/** Set the buffer with its size
	 */
	StatusCode setBuffer(char *pBuffer, dqm_uint size);

	/** Get the maximum size of the allocated buffer
	 */
	dqm_uint getMaxBufferSize() const;

	/** Get the current buffer size
	 */
	dqm_uint getBufferSize() const;

	/** Get remaining size in the buffer
	 */
	dqm_uint getAvailableSize() const;

	/** Whether the buffer is valid.
	 *  Returns true if the set buffer has
	 *  been called after a reset call
	 */
	bool isValid() const;

	/** Seek the current buffer ptr to given position (in bytes)
	 */
	StatusCode seek(dqm_uint pos = 0);

	/**
	 */
	template <typename T>
	  StatusCode write(const T &value);

	template <typename T>
	  StatusCode write(const T *pValue, dqm_uint nVals);

	template <typename T>
	  StatusCode read(T &value);

	template <typename T>
	  StatusCode read(T *&pValue, dqm_uint &nVals);

protected:

	/** Reallocate the buffer with a new size.
	 * 	The buffer is copied if the flag is true (default)
	 */
	StatusCode reallocBuffer(dqm_uint newSize, bool copyCurrentBuffer = true);

	dqm_uint    m_maxBufferSize;
	char       *m_pBuffer;
	char       *m_pBufferPtr;
	dqm_uint    m_bufferSize;
	bool        m_isValid;
}; 

//-----------------------------------------------------------------
//-----------------------------------------------------------------

template <typename T>
inline StatusCode DQMDataStream::write(const T &value)
{
	return write<T>(&value, 1);
}

//-----------------------------------------------------------------

template <>
inline StatusCode DQMDataStream::write(const std::string &value)
{
	return write<char>(value.c_str(), value.size());
}

//-----------------------------------------------------------------

template <typename T>
inline StatusCode DQMDataStream::write(const T *pValue, dqm_uint nVals)
{
	if(NULL == pValue)
		return STATUS_CODE_INVALID_PARAMETER;

	dqm_uint size = sizeof(T)*nVals;
	dqm_uint availableSize = getMaxBufferSize() - getBufferSize();

	if(availableSize <= size+sizeof(dqm_uint))
	{
		dqm_uint newSize = m_maxBufferSize + 1*1024*1024;

		if(size + sizeof(dqm_uint) > 1*1024*1024)
			newSize = size + sizeof(dqm_uint);

		RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, reallocBuffer(newSize));
	}

	// get the current buffer ptr
	dqm_uint *pCurrBuff = (dqm_uint *) m_pBufferPtr;

	// write the packet size
	pCurrBuff[0] = size;

	// increment the buffer
	m_pBufferPtr += sizeof(dqm_uint);
	m_bufferSize += sizeof(dqm_uint);

	// write the packet in the buffer
	memcpy(m_pBufferPtr, (const void*)pValue, size);

	// increment again the buffer
	m_pBufferPtr += size;
	m_bufferSize += size;

	return STATUS_CODE_SUCCESS;
}

//-----------------------------------------------------------------

template <>
inline StatusCode DQMDataStream::read(dqm_char *&pValue, dqm_uint &nVals)
{
	dqm_uint *pCurrBuff = (dqm_uint *) m_pBufferPtr;

	// get the size of the next values to read
	dqm_uint size = pCurrBuff[0] & 0xFFFFFF;
	nVals = size;

	// increment the buffer ptr
	m_pBufferPtr += sizeof(dqm_uint);

	// allocate the new buffer
	pValue = new dqm_char[size+1];
	pValue[size] = '\0';
	memset(pValue, 0, size);

	// read and copy the asked buffer region
	memcpy(pValue, m_pBufferPtr, size);

	// increment the buffer ptr
	m_pBufferPtr += size;

	return STATUS_CODE_SUCCESS;
}

//-----------------------------------------------------------------

template <typename T>
inline StatusCode DQMDataStream::read(T &value)
{
	dqm_uint *pCurrBuff = (dqm_uint *) m_pBufferPtr;

	// get the size of the next values to read
	dqm_uint size = pCurrBuff[0] & 0xFFFFFF;

	if(sizeof(T) != size)
		return STATUS_CODE_INVALID_PARAMETER;

	// increment the buffer ptr
	m_pBufferPtr += sizeof(dqm_uint);

	// read and copy the asked buffer region
	memcpy(&value, m_pBufferPtr, size);

	// increment the buffer ptr
	m_pBufferPtr += size;

	return STATUS_CODE_SUCCESS;
}

//-----------------------------------------------------------------

template <>
inline StatusCode DQMDataStream::read(std::string &value)
{
	dqm_uint *pCurrBuff = (dqm_uint *) m_pBufferPtr;

	// get the size of the next values to read
	dqm_uint size = pCurrBuff[0] & 0xFFFFFF;

	char *pString = NULL;
	StatusCode statusCode = read<char>(pString, size);

	if(STATUS_CODE_SUCCESS != statusCode)
	{
		delete [] pString;
		return statusCode;
	}

	if(NULL == pString)
		return STATUS_CODE_FAILURE;

	if(0 == size)
		value.clear();
	else
		value = pString;

	delete [] pString;
	
	return STATUS_CODE_SUCCESS;
}

//-----------------------------------------------------------------

template <typename T>
inline StatusCode DQMDataStream::read(T *&pValue, dqm_uint &nVals)
{
	pValue = NULL;

	dqm_uint *pCurrBuff = (dqm_uint *) m_pBufferPtr;

	// get the size of the next values to read
	dqm_uint size = pCurrBuff[0] & 0xFFFFFF;

	if(size%sizeof(T) != 0)
		return STATUS_CODE_INVALID_PARAMETER;

	// valid operation because of the previous check
	nVals = size/sizeof(T);

	// increment the buffer ptr
	m_pBufferPtr += sizeof(dqm_uint);

	// valid even if size is 0, then nothing happens while copying memory
	pValue = new T[nVals];

	// read and copy the asked buffer region
	memcpy(pValue, m_pBufferPtr, size);

	// increment the buffer ptr
	m_pBufferPtr += size;

	return STATUS_CODE_SUCCESS;
}

//-----------------------------------------------------------------

} 

#endif  //  DQMDATASTREAM_H