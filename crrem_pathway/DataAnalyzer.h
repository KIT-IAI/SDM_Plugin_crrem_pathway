#pragma once

#include "CRREMData.h"
#include <ifcdb/IfcDbInclude.h>
#include <ifcdb/utils/PopulationObserver.hpp>
#include <stdutils/utf8.hpp>

struct BuildingParameter
{
  std::string buildingName;
  int yearOfConstruction = 0;
  std::string buildingType;
  double totalArea = 0.0;
  double totalEnergyDemand = 0.0;
  std::vector<EnergyData> energyDemands;
};

class DataAnalyzer
{
  public:
    DataAnalyzer(IfcDB::Populationi* pDB, IfcDB::IfcEntity* pBuilding);
    ~DataAnalyzer() = default;

    virtual void analyseBuildingParameter() = 0;

    BuildingParameter getBuildingParameter() { return m_buildingParameter; }

  protected:
    IfcDB::Polylines createFootprint(std::vector<IfcDB::Geometry*> geometries);
    double calcTimseriesSum(IfcDB::AbstractTimeSeries* pTimeSeries);

  protected:
    IfcDB::Populationi* m_pDB = nullptr;
    IfcDB::IfcEntity* m_pBuilding = nullptr;
    BuildingParameter m_buildingParameter;
};

class CityGmlDataAnalyzer : public DataAnalyzer
{
  public:
    CityGmlDataAnalyzer(IfcDB::Populationi* pDB, IfcDB::IfcEntity* pBuilding);
    ~CityGmlDataAnalyzer() = default;

    void analyseBuildingParameter() override;

  private:
    double getBuildingArea(IfcDB::IfcEntity* pBuilding);
    bool processSimulationResults(IfcDB::IfcEntity* pBuilding);

    bool processEnergySimulationResults(IfcDB::IfcEntity* pBuilding);
    bool processBuildingSimulationResults(IfcDB::IfcEntity* pBuilding);
    bool processEnergyDemands(IfcDB::IfcEntity* pEntity);
    bool analyzeRepresentations(IfcDB::IfcEntity* pBuilding, double& area);
    IfcDB::Geometry* createBuildingSolid(IfcDB::Feature* pFeature);
};

class IfcDataAnalyzer : public DataAnalyzer
{
  public:
    IfcDataAnalyzer(IfcDB::Populationi* pDB, IfcDB::IfcEntity* pBuilding);
    ~IfcDataAnalyzer() = default;

    void analyseBuildingParameter() override;
};

class GbxmlDataAnalyzer : public DataAnalyzer
{
  public:
    GbxmlDataAnalyzer(IfcDB::Populationi* pDB, IfcDB::IfcEntity* pBuilding);
    ~GbxmlDataAnalyzer() = default;

    void analyseBuildingParameter() override;
};

namespace DataAnalyzerFactory
{
  static bool isBuilding(IfcDB::IfcEntity* pEntity)
  {
    if (pEntity &&
        (pEntity->isTypeOf(IfcDB::IFC_BUILDING)     ||
         pEntity->isTypeOf(IfcDB::CITYGML_BUILDING) ||
         pEntity->isTypeOf(IfcDB::GBXML_BUILDING)))
    {
      return true;
    }

    return false;
  }

  static IfcDB::IfcEntity* getBuilding(IfcDB::Populationi* pDB, IfcDB::ifcOid oid)
  {
    // searching building(s)
    IfcDB::IfcEntity* pEntity = pDB->get(oid);

    IfcDB::ifcOid buildingOid(0);

    if (pEntity && isBuilding(pEntity))
    {
      return pEntity;
    }
    else if (pDB->hasEntityParentOfType(IfcDB::IFC_BUILDING, oid, buildingOid)     == true ||
             pDB->hasEntityParentOfType(IfcDB::CITYGML_BUILDING, oid, buildingOid) == true ||
             pDB->hasEntityParentOfType(IfcDB::GBXML_BUILDING, oid, buildingOid)   == true)
    {
      return pDB->get(buildingOid);
    }

    return {};
  }

  static IfcDB::IfcEntity* getBuilding(IfcDB::Populationi* pDB, IfcDB::utils::PopulationStates* pStates)
  {
    // only first building is used!
    for (auto oid : pStates->getSelectedEntities())
    {
      IfcDB::IfcEntity* pBuilding = getBuilding(pDB, oid);

      if (pBuilding)
      {
        return pBuilding;
      }
    }

    std::vector<IfcDB::IfcEntity*> buildings;
    pDB->getList(IfcDB::CITYGML_BUILDING, buildings);
    pDB->getList(IfcDB::IFC_BUILDING, buildings);
    pDB->getList(IfcDB::GBXML_BUILDING, buildings);

    if (!buildings.empty())
    {
      return buildings.front();
    }

    return {};
  }

  static std::unique_ptr<DataAnalyzer> create(IfcDB::Populationi* pDB, IfcDB::IfcEntity* pBuilding)
  {
    if (pBuilding->isTypeOf(IfcDB::CITYGML_BUILDING))
    {
      return std::make_unique<CityGmlDataAnalyzer>(pDB, pBuilding);
    }
    else if (pBuilding->isTypeOf(IfcDB::IFC_BUILDING))
    {
      return std::make_unique<IfcDataAnalyzer>(pDB, pBuilding);
    }
    else if (pBuilding->isTypeOf(IfcDB::GBXML_BUILDING))
    {
      return std::make_unique<GbxmlDataAnalyzer>(pDB, pBuilding);
    }

    throw std::logic_error("Building type not implemented: " + stdutils::toUtf8(IfcDB::getTypeOfEntityAsString(pBuilding->getType())));
  }

  static std::unique_ptr<DataAnalyzer> create(IfcDB::Populationi* pDB, IfcDB::ifcOid buildingOid)
  {
    IfcDB::IfcEntity* pBuilding = getBuilding(pDB, buildingOid);

    if (pBuilding)
    {
      return create(pDB, pBuilding);
    }

    throw std::logic_error("Element not found");
  }

  static std::unique_ptr<DataAnalyzer> create(IfcDB::Populationi* pDB, IfcDB::utils::PopulationStates* pStates)
  {
    IfcDB::IfcEntity* pBuilding = getBuilding(pDB, pStates);

    if (pBuilding)
    {
      return create(pDB, pBuilding);
    }

    throw std::logic_error("Element not found");
  }
}
