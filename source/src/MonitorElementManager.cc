/// \file MonitorElementManager.cc
/*
 *
 * MonitorElementManager.cc source template automatically generated by a class generator
 * Creation date : lun. ao�t 17 2015
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
#include <dqm4hep/Directory.h>
#include <dqm4hep/MonitorElement.h>
#include <dqm4hep/MonitorElementManager.h>
#include <dqm4hep/PluginManager.h>
#include <dqm4hep/QualityTest.h>
#include <dqm4hep/Storage.h>

// -- std headers
#include <stdexcept>

namespace dqm4hep {

  namespace core {

    MonitorElementManager::MonitorElementManager() {
      PluginManager *pPluginMgr(PluginManager::instance());
      StringVector factoryNames(pPluginMgr->pluginNamesMatchingType<QualityTestFactory>());

      for (auto factoryName : factoryNames) {
        auto factory(pPluginMgr->create<QualityTestFactory>(factoryName));

        if (nullptr == factory) {
          dqm_error("MonitorElementManager::MonitorElementManager: Couldn't create qtest factory '{0}'. This is "
                    "normally not possible !",
                    factoryName);
          continue;
        }

        m_qualityTestFactoryMap.insert(QualityTestFactoryMap::value_type(factoryName, factory));
      }
      // the default one
      m_defaultXMLAllocator = std::make_shared<DefaultXMLAllocator>();
      // TH1
      m_xmlAllocatorMap["TH1D"] = std::make_shared<TH1XMLAllocator>();
      m_xmlAllocatorMap["TH1F"] = std::make_shared<TH1XMLAllocator>();
      m_xmlAllocatorMap["TH1I"] = std::make_shared<TH1XMLAllocator>();
      m_xmlAllocatorMap["TH1C"] = std::make_shared<TH1XMLAllocator>();
      m_xmlAllocatorMap["TH1S"] = std::make_shared<TH1XMLAllocator>();
      // TH2
      m_xmlAllocatorMap["TH2D"] = std::make_shared<TH2XMLAllocator>();
      m_xmlAllocatorMap["TH2F"] = std::make_shared<TH2XMLAllocator>();
      m_xmlAllocatorMap["TH2I"] = std::make_shared<TH2XMLAllocator>();
      m_xmlAllocatorMap["TH2C"] = std::make_shared<TH2XMLAllocator>();
      m_xmlAllocatorMap["TH2S"] = std::make_shared<TH2XMLAllocator>(); 
      // TH2
      m_xmlAllocatorMap["TH3D"] = std::make_shared<TH3XMLAllocator>();
      m_xmlAllocatorMap["TH3F"] = std::make_shared<TH3XMLAllocator>();
      m_xmlAllocatorMap["TH3I"] = std::make_shared<TH3XMLAllocator>();
      m_xmlAllocatorMap["TH3C"] = std::make_shared<TH3XMLAllocator>();
      m_xmlAllocatorMap["TH3S"] = std::make_shared<TH3XMLAllocator>();
      // scalars
      m_xmlAllocatorMap["int"] = std::make_shared<TScalarXMLAllocator>();
      m_xmlAllocatorMap["real"] = std::make_shared<TScalarXMLAllocator>();
      m_xmlAllocatorMap["float"] = std::make_shared<TScalarXMLAllocator>();
      m_xmlAllocatorMap["double"] = std::make_shared<TScalarXMLAllocator>();
      m_xmlAllocatorMap["short"] = std::make_shared<TScalarXMLAllocator>();
      m_xmlAllocatorMap["long"] = std::make_shared<TScalarXMLAllocator>();
      m_xmlAllocatorMap["long64"] = std::make_shared<TScalarXMLAllocator>();
      // others
      m_xmlAllocatorMap["THStack"] = std::make_shared<THStackXMLAllocator>();
      m_xmlAllocatorMap["TH2Poly"] = std::make_shared<TH2PolyXMLAllocator>();
      m_xmlAllocatorMap["TProfile"] = std::make_shared<TProfileXMLAllocator>();
      m_xmlAllocatorMap["TProfile2D"] = std::make_shared<TProfile2DXMLAllocator>();
    }

    //-------------------------------------------------------------------------------------------------

    MonitorElementManager::~MonitorElementManager() {
      m_qualityTestFactoryMap.clear();
      m_qualityTestMap.clear();
    }

    //-------------------------------------------------------------------------------------------------

    void MonitorElementManager::cd() {
      m_storage.cd();
    }

    //-------------------------------------------------------------------------------------------------

    StatusCode MonitorElementManager::cd(const std::string &dirName) {
      return m_storage.cd(dirName);
    }

    //-------------------------------------------------------------------------------------------------

    StatusCode MonitorElementManager::mkdir(const std::string &dirName) {
      return m_storage.mkdir(dirName);
    }

    //-------------------------------------------------------------------------------------------------

    // void MonitorElementManager::ls(bool recursive)
    // {
    //   m_pMonitorElementStorage->ls(recursive);
    // }

    //-------------------------------------------------------------------------------------------------

    const std::string &MonitorElementManager::pwd() {
      return m_storage.pwd();
    }

    //-------------------------------------------------------------------------------------------------

    const std::string &MonitorElementManager::fullPwd() {
      return m_storage.current()->fullPath().getPath();
    }

    //-------------------------------------------------------------------------------------------------

    StatusCode MonitorElementManager::goUp() {
      return m_storage.goUp();
    }

    //-------------------------------------------------------------------------------------------------

    StatusCode MonitorElementManager::rmdir(const std::string &dirName) {
      return m_storage.rmdir(dirName);
    }

    //-------------------------------------------------------------------------------------------------

    bool MonitorElementManager::dirExists(const std::string &dirName) const {
      return m_storage.dirExists(dirName);
    }

    //-------------------------------------------------------------------------------------------------

    void MonitorElementManager::resetMonitorElements() {
      m_storage.iterate([](const MonitorElementDir &, MonitorElementPtr monitorElement) {
        monitorElement->reset();
        return true;
      });
    }
    
    //-------------------------------------------------------------------------------------------------
    
    void MonitorElementManager::monitorElementsToJson(json &object) const {
      m_storage.iterate([&object](const MonitorElementDir &, MonitorElementPtr monitorElement) {
        json meObject;
        monitorElement->toJson(meObject);
        object.push_back(meObject);
        return true;
      });
    }

    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------

    StatusCode MonitorElementManager::removeMonitorElement(const std::string &path, const std::string &name) {
      MonitorElementPtr monitorElement;
      RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, this->getMonitorElement(path, name, monitorElement));

      // look in qtest map first
      auto iter = m_monitorElementToQTestMap.find(monitorElement);

      if (m_monitorElementToQTestMap.end() != iter)
        m_monitorElementToQTestMap.erase(iter);

      // remove the element form storage. Call delete operator
      RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, m_storage.remove(path, [&name](const MonitorElementPtr &element) {
        return (element->name() == name);
      }));

      return STATUS_CODE_SUCCESS;
    }

    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------

    StatusCode MonitorElementManager::createQualityTests(TiXmlElement *const pXmlElement) {
      
      if (nullptr == pXmlElement) {
        dqm_error("MonitorElementManager::createQualityTests: invalid xml element!");
        return STATUS_CODE_INVALID_PTR;
      }
      
      float warningLimit(QualityTest::defaultWarningLimit()), errorLimit(QualityTest::defaultWarningLimit());
      
      RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, XmlHelper::getAttribute(pXmlElement, "error", errorLimit, [](const float &value){
        return (value >= 0.f && value < 1.f);
      }));
      
      RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, XmlHelper::getAttribute(pXmlElement, "warning", warningLimit, [&errorLimit](const float &value){
        return (value >=0.f && value > errorLimit && value <= 1.f);
      }));
      
      for (TiXmlElement *qtest = pXmlElement->FirstChildElement("qtest"); qtest != nullptr; qtest = qtest->NextSiblingElement("qtest")) {
        RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, this->createQualityTest(qtest, warningLimit, errorLimit));
      }
      
      return STATUS_CODE_SUCCESS;
    }
    
    //-------------------------------------------------------------------------------------------------

    StatusCode MonitorElementManager::createQualityTest(TiXmlElement *const pXmlElement, float warning, float error) {
      std::string name, type;
      RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, XmlHelper::getAttribute(pXmlElement, "name", name));
      RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, XmlHelper::getAttribute(pXmlElement, "type", type));
      
      float warningLimit(warning), errorLimit(error);
      
      RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, XmlHelper::getAttribute(pXmlElement, "error", errorLimit, [](const float &value){
        return (value >= 0.f && value < 1.f);
      }));
      
      RETURN_RESULT_IF_AND_IF(STATUS_CODE_SUCCESS, STATUS_CODE_NOT_FOUND, !=, XmlHelper::getAttribute(pXmlElement, "warning", warningLimit, [&errorLimit](const float &value){
        return (value >=0.f && value > errorLimit && value <= 1.f);
      }));

      auto findIter = m_qualityTestMap.find(name);

      if (m_qualityTestMap.end() != findIter)
        return STATUS_CODE_ALREADY_PRESENT;

      auto findFactoryIter = m_qualityTestFactoryMap.find(type);

      if (m_qualityTestFactoryMap.end() == findFactoryIter)
        return STATUS_CODE_NOT_FOUND;

      QualityTestPtr qualityTest = findFactoryIter->second->createQualityTest(name);

      qualityTest->setLimits(warningLimit, errorLimit);
      RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, qualityTest->readSettings(TiXmlHandle(pXmlElement)));
      RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, qualityTest->init());

      if (!m_qualityTestMap.insert(QualityTestMap::value_type(name, qualityTest)).second)
        return STATUS_CODE_FAILURE;

      return STATUS_CODE_SUCCESS;
    }

    //-------------------------------------------------------------------------------------------------

    StatusCode MonitorElementManager::addQualityTest(const std::string &path, const std::string &name,
                                                     const std::string &qualityTestName) {
      auto findIter = m_qualityTestMap.find(qualityTestName);

      if (m_qualityTestMap.end() == findIter)
        return STATUS_CODE_NOT_FOUND;

      MonitorElementPtr monitorElement;
      RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, this->getMonitorElement(path, name, monitorElement));
      RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, monitorElement->addQualityTest(findIter->second));

      return STATUS_CODE_SUCCESS;
    }

    //-------------------------------------------------------------------------------------------------

    StatusCode MonitorElementManager::removeQualityTest(const std::string &path, const std::string &name,
                                                        const std::string &qualityTestName) {
      MonitorElementPtr monitorElement;
      RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, this->getMonitorElement(path, name, monitorElement));
      RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, monitorElement->removeQualityTest(qualityTestName));

      return STATUS_CODE_SUCCESS;
    }

    //-------------------------------------------------------------------------------------------------

    StatusCode MonitorElementManager::runQualityTests(QReportStorage &reports) {
      try {
        m_storage.iterate([&reports](const MonitorElementDir &, MonitorElementPtr monitorElement) {
          QReportMap reportMap;
          THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, monitorElement->runQualityTests(reportMap));
          reports.addReports(reportMap);
          return true;
        });
      } catch (StatusCodeException &exception) {
        dqm_error("Failed to process qtests: {0}", exception.toString());
        return exception.getStatusCode();
      } catch (...) {
        dqm_error("Failed to process qtests: Unknown exception");
        return STATUS_CODE_FAILURE;
      }

      return STATUS_CODE_SUCCESS;
    }

    //-------------------------------------------------------------------------------------------------

    StatusCode MonitorElementManager::runQualityTests(const std::string &path, const std::string &name,
                                                      QReportStorage &reports) {
      MonitorElementPtr monitorElement;
      QReportMap reportMap;
      RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, this->getMonitorElement(path, name, monitorElement));
      RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, monitorElement->runQualityTests(reportMap));
      reports.addReports(reportMap);

      return STATUS_CODE_SUCCESS;
    }

    //-------------------------------------------------------------------------------------------------

    StatusCode MonitorElementManager::runQualityTest(const std::string &path, const std::string &name,
                                                     const std::string &qualityTestName, QReportStorage &reports) {
      MonitorElementPtr monitorElement;
      QReport report;
      RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, this->getMonitorElement(path, name, monitorElement));
      RETURN_RESULT_IF(STATUS_CODE_SUCCESS, !=, monitorElement->runQualityTest(qualityTestName, report));
      reports.addReport(report);

      return STATUS_CODE_SUCCESS;
    }

    //-------------------------------------------------------------------------------------------------

    const Storage<MonitorElement> &MonitorElementManager::getStorage() const {
      return m_storage;
    }
    
    //-------------------------------------------------------------------------------------------------
    
    bool MonitorElementManager::checkClass(TClass *cls) {
      if(nullptr == cls) {
        dqm_error("MonitorElementManager::checkClass: class is nullptr! Please check for class existence !");
        return false;
      }
      if(not cls->HasDictionary()) {
        dqm_error("MonitorElementManager::checkClass: class '{0}' has no dictionnary!", cls->GetName());
        return false;
      }
      if (cls->IsForeign()) {
        dqm_error(
            "MonitorElementManager::checkClass: class '{0}' is foreign (does not have a Streamer method)!",
            cls->GetName());
        return false;
      }
      if (not cls->InheritsFrom("TNamed")) {
        dqm_error("MonitorElementManager::checkClass: class '{0}' doesn't inherit TNamed (required to call "
                  "SetName())!",
                  cls->GetName());
        return false;
      }
      return true;
    }
    
    //-------------------------------------------------------------------------------------------------
    
    StatusCode MonitorElementManager::addToStorage(const std::string &path, MonitorElementPtr monitorElement) {
      // check for existence
      if (nullptr != m_storage.findObject(path, [&](MonitorElementPtr obj) { 
        return obj->name() == monitorElement->name(); 
      })) {
        dqm_error("Monitor element '{0}' in directory '{1}' already booked !", monitorElement->name(), path);
        return STATUS_CODE_ALREADY_PRESENT;
      }
      // try to add it
      try {
        THROW_RESULT_IF(STATUS_CODE_SUCCESS, !=, m_storage.add(path, monitorElement));
        monitorElement->setPath(path);
      } 
      catch (StatusCodeException &e) {
        if (nullptr == monitorElement) {
          // TObject should have been owned by the MonitorElement
          // but creation failed, we need to delete it manually
          delete monitorElement->object();
        }
        return e.getStatusCode();
      } 
      catch (...) {
        if (nullptr == monitorElement) {
          // TObject should have been owned by the MonitorElement
          // but creation failed, we need to delete it manually
          delete monitorElement->object();
        }
        return STATUS_CODE_FAILURE;
      }
      return STATUS_CODE_SUCCESS;
    }
    
    //-------------------------------------------------------------------------------------------------
    
    void MonitorElementManager::dumpStorage() {
      m_storage.dump([](MonitorElementPtr monitorElement){
        return monitorElement->name() + " (" + monitorElement->type() + ") - " + monitorElement->title();
      });
    }
    
  }
}
