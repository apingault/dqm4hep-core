/// \file DQMLocalFileHandler.h
/*
 *
 * DQMLocalFileHandler.h header template automatically generated by a class generator
 * Creation date : lun. janv. 11 2016
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


#ifndef DQM4HEP_LOCALFILEHANDLER_H
#define DQM4HEP_LOCALFILEHANDLER_H

#include "dqm4hep/FileHandler.h"

namespace dqm4hep {

  namespace core {

    /** LocalFileHandler class
     */
    class LocalFileHandler : public FileHandler
    {
    public:
      /** Constructor
       */
      LocalFileHandler();

      /** Destructor
       */
      ~LocalFileHandler();

      /** Get the file handler type as string
       */
      const std::string &type() const;

      /** Download the file locally using the target pattern
       */
      StatusCode download(const std::string &fileName);

      /** Get the full file name after the download step
       */
      const std::string &getLocalFileName() const;

    private:
      std::string              m_type;
      std::string              m_localFileName;
    };

  }

} 

#endif  //  DQM4HEP_LOCALFILEHANDLER_H