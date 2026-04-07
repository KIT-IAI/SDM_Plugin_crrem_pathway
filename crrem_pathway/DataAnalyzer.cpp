#include "DataAnalyzer.h"
#include <stdutils/utf8.hpp>

/************************************************************************/
/* DataAnalyzer                                                         */
/************************************************************************/
DataAnalyzer::DataAnalyzer(IfcDB::Populationi* pDB, IfcDB::IfcEntity* pBuilding)
  : m_pDB(pDB), m_pBuilding(pBuilding)
{
}

IfcDB::Polylines DataAnalyzer::createFootprint(std::vector<IfcDB::Geometry*> geometries)
{
  // Matrix for projection
  IfcDB::Matrix4 matrix(1.0, 0.0, 0.0, 0.0, 0.0, 1.0);

  IfcDB::ClipperCurve footprint(matrix, (IfcDB::Curve*)nullptr);

  for (auto pGeometry : geometries)
  {
    std::vector<IfcDB::Face*> vFaces;
    IfcDB::Geometry::getFaces(pGeometry, vFaces);

    if (vFaces.empty())
    {
      continue;
    }

    for (auto pFace : vFaces)
    {
      IfcDB::ClipperCurve curveElement(matrix, pFace);

      if (footprint.isEmpty())
      {
        footprint = std::move(curveElement);
      }
      else
      {
        footprint.unite(&curveElement);
      }
    }
  }

  if (footprint.isEmpty())
  {
    return {};
  }

  std::vector<IfcDB::CPolyline*> polylines = footprint.toPolylines();

  for (auto pPolyline : polylines)
  {
    pPolyline->deleteCollinearPoints();
  }

  return polylines;
}


double DataAnalyzer::calcTimseriesSum(IfcDB::AbstractTimeSeries* pTimeSeries)
{
  switch (pTimeSeries->getTimeSeriesType())
  {
    case IfcDB::AbstractTimeSeries::Regular:
    {
      IfcDB::RegularTimeSeries* pRegularTimeSeries = dynamic_cast<IfcDB::RegularTimeSeries*>(pTimeSeries);

      return pRegularTimeSeries->calcSum();
    }
    case IfcDB::AbstractTimeSeries::Irregular:
    {
      assert(0);
      break;
    }
    case IfcDB::AbstractTimeSeries::RegularFile:
    {
      assert(0);
      break;
    }
    case IfcDB::AbstractTimeSeries::IrregularFile:
    {
      assert(0);
      break;
    }
    default:
      assert(0);
  }

  return 0.0;
}

/************************************************************************/
/* CityGmlDataAnalyzer                                                  */
/************************************************************************/
CityGmlDataAnalyzer::CityGmlDataAnalyzer(IfcDB::Populationi* pDB, IfcDB::IfcEntity* pBuilding)
  : DataAnalyzer(pDB, pBuilding)
{
}

void CityGmlDataAnalyzer::analyseBuildingParameter()
{
  IfcDB::Feature* pBuildingFeature = dynamic_cast<IfcDB::Feature*>(m_pBuilding);

  m_buildingParameter.buildingName = toUtf8(m_pBuilding->getName());

  std::wstring namespaceCode(pBuildingFeature->getNamespaceCode() + L":");

  int64_t storeys(1);
  pBuildingFeature->getIntegerAttributWert(namespaceCode + L"storeysAboveGround", storeys);

  std::wstring buildingClass;
  pBuildingFeature->getStringAttributWert(namespaceCode + L"class", buildingClass);

  std::wstring buildingFunction;
  pBuildingFeature->getStringAttributWert(namespaceCode + L"function", buildingFunction);

  if (!buildingClass.empty())
  {
    m_buildingParameter.buildingType = toUtf8(buildingClass);

    if (!buildingFunction.empty())
    {
      m_buildingParameter.buildingType += " - " + toUtf8(buildingFunction);
    }
  }
  else
  {
    m_buildingParameter.buildingType = toUtf8(buildingFunction);
  }

  IfcDB::AbstractTime* pTime = pBuildingFeature->getDateAttributWert(namespaceCode + L"yearOfConstruction");

  if (pTime != nullptr && pTime->getTimeType() == IfcDB::AbstractTime::TIME_POSITION)
  {
    IfcDB::TimePosition* pTimePosition = dynamic_cast<IfcDB::TimePosition*>(pTime);
    m_buildingParameter.yearOfConstruction = pTimePosition->getYear();
  }

  m_buildingParameter.totalArea = getBuildingArea(m_pBuilding) * storeys;

  processSimulationResults(m_pBuilding);
}

double CityGmlDataAnalyzer::getBuildingArea(IfcDB::IfcEntity* pBuilding)
{
  double area(0.0);

  bool state = analyzeRepresentations(pBuilding, area);

  IfcDB::IfcEntityList buildingParts;
  m_pDB->getChildren(IfcDB::CITYGML_BUILDING_PART, pBuilding->getOid(), buildingParts);

  if (!buildingParts.empty())
  {
    for (auto pBuildingPart : buildingParts)
    {
      state &= analyzeRepresentations(pBuildingPart, area);
    }
  }

  return area;
}

bool CityGmlDataAnalyzer::processSimulationResults(IfcDB::IfcEntity* pBuilding)
{
  double totalEnergyDemand(0.0);

  // check for energy:ThermalZone
  std::vector<IfcDB::Feature*> thermalZones = m_pDB->getRelatedFeatures(dynamic_cast<IfcDB::Feature*>(pBuilding), L"energy:thermalZone");

  for (const auto& pThermalZone : thermalZones)
  {
    for (auto& pTimeseries : pThermalZone->getTimeSeries())
    {
      if (pTimeseries->getTimeSeriesCategory() == IfcDB::AbstractTimeSeries::HEATING_ENERGY ||
          pTimeseries->getTimeSeriesCategory() == IfcDB::AbstractTimeSeries::COOLING_ENERGY ||
          pTimeseries->getTimeSeriesCategory() == IfcDB::AbstractTimeSeries::THERMAL_GAIN_LIGHTING ||
          pTimeseries->getTimeSeriesCategory() == IfcDB::AbstractTimeSeries::THERMAL_GAIN_ELECTRIC_APPLIANCES)
      {
        double energyDemand = calcTimseriesSum(pTimeseries);
        totalEnergyDemand += energyDemand;

        EnergyData energyData;
        energyData.type = stdutils::toUtf8(pTimeseries->getTimeSeriesCategoryAsString());
        energyData.source = Electricity;
        energyData.energyDemand = energyDemand;
        energyData.emissionFactor = EnergyData::getDefaultEmissionFactor(Electricity);

        m_buildingParameter.totalEnergyDemand = totalEnergyDemand;
        m_buildingParameter.energyDemands.emplace_back(energyData);
      }
    }
  }

  if (totalEnergyDemand > 0.0)
  {
    return true;
  }

  // check for energy:SimulationResults
  std::vector<IfcDB::Feature*> simulationResults = m_pDB->getRelatedFeatures(dynamic_cast<IfcDB::Feature*>(pBuilding), L"energy:simulationResults");

  for (const auto& pSimulationResults : simulationResults)
  {

  }

  // check for energy:EnergyDemand
  std::vector<IfcDB::Feature*> energyDemands = m_pDB->getRelatedFeatures(dynamic_cast<IfcDB::Feature*>(pBuilding), L"energy:energyDemand");

  for (const auto& pEnergyDemand : energyDemands)
  {

  }

  return false;
}

bool CityGmlDataAnalyzer::processEnergySimulationResults(IfcDB::IfcEntity* pBuilding)
{
  IfcDB::Feature* pSimulationResults = m_pDB->getRelatedFeature(dynamic_cast<IfcDB::Feature*>(pBuilding), L"energy:simulationResults");

  double totalEnergyDemand(0.0);

  if (pSimulationResults)
  {
    for (auto& pTimeseries : pSimulationResults->getTimeSeries())
    {
      if (pTimeseries->getTimeSeriesCategory() == IfcDB::AbstractTimeSeries::TOTAL_HEATING_ENERGY  ||
          pTimeseries->getTimeSeriesCategory() == IfcDB::AbstractTimeSeries::TOTAL_COOLING_ENERGY  ||
          pTimeseries->getTimeSeriesCategory() == IfcDB::AbstractTimeSeries::HEATING_ENERGY        ||
          pTimeseries->getTimeSeriesCategory() == IfcDB::AbstractTimeSeries::COOLING_ENERGY        || 
          pTimeseries->getTimeSeriesCategory() == IfcDB::AbstractTimeSeries::THERMAL_GAIN_LIGHTING ||
          pTimeseries->getTimeSeriesCategory() == IfcDB::AbstractTimeSeries::THERMAL_GAIN_ELECTRIC_APPLIANCES)
      {
        double energyDemand = calcTimseriesSum(pTimeseries);
        totalEnergyDemand += energyDemand;

        EnergyData energyData;
        energyData.type = stdutils::toUtf8(pTimeseries->getTimeSeriesCategoryAsString());
        energyData.source = Electricity;
        energyData.energyDemand = energyDemand;
        energyData.emissionFactor = EnergyData::getDefaultEmissionFactor(Electricity);

        m_buildingParameter.totalEnergyDemand = totalEnergyDemand;
        m_buildingParameter.energyDemands.emplace_back(energyData);
      }
    }

    if (totalEnergyDemand > 0.0)
    {
      return true;
    }
  }

  IfcDB::IfcEntityList energyDemands;
  m_pDB->getChildren(L"energy:EnergyDemand", pBuilding->getOid(), energyDemands);

  if (!energyDemands.empty())
  {
    for (auto pEntity : energyDemands)
    {
      IfcDB::Feature* pEnergyDemand = dynamic_cast<IfcDB::Feature*>(pEntity);

      if (pEnergyDemand != nullptr && pEnergyDemand->hasTimeeries())
      {
        for (auto pTimeSeries : pEnergyDemand->getTimeSeries(L"energy:energyAmount"))
        {
          double energyDemand = calcTimseriesSum(pTimeSeries);
          totalEnergyDemand += energyDemand;

          EnergyData energyData;
          energyData.type = stdutils::toUtf8(pTimeSeries->getTimeSeriesCategoryAsString());
          energyData.source = Electricity;
          energyData.energyDemand = energyDemand;

          m_buildingParameter.totalEnergyDemand = totalEnergyDemand;
          m_buildingParameter.energyDemands.emplace_back(energyData);
        }
      }
    }

    if (totalEnergyDemand > 0.0)
    {
      return true;
    }
  }

  return false;
}

bool CityGmlDataAnalyzer::analyzeRepresentations(IfcDB::IfcEntity* pBuilding, double& area)
{
  bool state(false);
  IfcDB::Geometry* pSolidGeometry = nullptr;
  IfcDB::Geometry* pFootPrintGeometry = nullptr;

  for (const auto& representation : pBuilding->getRepresentations())
  {
    if (representation.m_representationIdentifier == L"lod0" && representation.m_representationType.find(L"FootPrint") != std::string::npos)
    {
      pFootPrintGeometry = representation.m_geometry;
    }
    else if (!pSolidGeometry && representation.m_representationIdentifier == L"lod1" && IfcDB::Geometry::isSolid(representation.m_geometry))
    {
      pSolidGeometry = representation.m_geometry;
    }
    else if (representation.m_representationIdentifier == L"lod2" && IfcDB::Geometry::isSolid(representation.m_geometry))
    {
      pSolidGeometry = representation.m_geometry;
    }
  }

  bool removeGeometry(false);

  if (!pSolidGeometry)
  {
    removeGeometry = true;
    pSolidGeometry = createBuildingSolid(dynamic_cast<IfcDB::Feature*>(pBuilding));
  }

  IfcDB::Polylines polylines;

  if (pFootPrintGeometry)
  {
    polylines = createFootprint({ pFootPrintGeometry });
  }
  else if (pSolidGeometry)
  {
    polylines = createFootprint({ pSolidGeometry });
  }
  else
  {
    assert(0);
  }

  for (auto pPolyline : polylines)
  {
    area += IfcDB::Curve::calcArea(pPolyline);
    delete pPolyline;
  }

  if (pSolidGeometry && area > 0.0)
  {
    state = true;
  }

  if (removeGeometry == true && pSolidGeometry)
  {
    removeGeometry = false;
    delete pSolidGeometry;
    pSolidGeometry = nullptr;
  }

  return state;
}

IfcDB::Geometry* CityGmlDataAnalyzer::createBuildingSolid(IfcDB::Feature* pFeature)
{
  // create solid from boundary surfaces
  std::vector<IfcDB::Feature*> vBuildingElements;
  m_pDB->getRelatedFeatures(pFeature, L"bldg:boundedBy", vBuildingElements);

  IfcDB::Brep* pBuildingSolid = new IfcDB::Brep();

  for (auto pBoundarySurface : vBuildingElements)
  {
    IfcDB::Geometry* pGeometry = pBoundarySurface->getGeometry();

    std::vector<IfcDB::Face*> vFaces;
    IfcDB::Geometry::getFaces(pGeometry, vFaces);

    for (auto pFace : vFaces)
    {
      pBuildingSolid->addFace(dynamic_cast<IfcDB::Face*>(pFace->clone()));
    }
  }

  return pBuildingSolid;
}


/************************************************************************/
/* IfcDataAnalyzer                                                      */
/************************************************************************/
IfcDataAnalyzer::IfcDataAnalyzer(IfcDB::Populationi* pDB, IfcDB::IfcEntity* pBuilding)
  : DataAnalyzer(pDB, pBuilding)
{
}

void IfcDataAnalyzer::analyseBuildingParameter()
{
  m_buildingParameter.buildingName = toUtf8(m_pBuilding->getName());

  std::wstring propertyValue;

  if (m_pDB->findPropertyValue(m_pBuilding->getOid(), L"Pset_BuildingCommon", L"YearOfConstruction", propertyValue))
  {
    m_buildingParameter.yearOfConstruction = std::stoi(propertyValue);
  }

  if (m_pDB->findPropertyValue(m_pBuilding->getOid(), L"Pset_BuildingCommon", L"OccupancyType", propertyValue))
  {
    m_buildingParameter.buildingType = toUtf8(propertyValue);
  }

  std::vector<IfcDB::IfcEntity*> spaces;
  m_pDB->getAllChildren(IfcDB::IFC_SPACE, m_pBuilding->getOid(), spaces);

  double unitFactor = m_pDB->getUnitFactor(_T("LENGTHUNIT"));
  double areaFactor = unitFactor * unitFactor / 1000000;

  for (auto pSpace : spaces)
  {
    bool hasArea(false);

    std::tstring value;
    IfcDB::Property* pBaseQuantities = m_pDB->findProperty(pSpace->getOid(), L"BaseQuantities", L"");

    if (pBaseQuantities->findValue(L"BaseQuantities", L"GrossFloorArea", value))
    {
      hasArea = true;
      m_buildingParameter.totalArea += std::stod(value) * areaFactor;
    }

    if (!hasArea)
    {
      IfcDB::cEntityQuantities quantities(m_pDB, pSpace);
      quantities.calcQuantities(pSpace->getGeometry(L"Body"));

      double baseArea;
      if (quantities.getQuantity(L"BaseArea", baseArea))
      {
        m_buildingParameter.totalArea += baseArea * areaFactor;
      }
    }
  }
}


/************************************************************************/
/* GbxmlDataAnalyzer                                                    */
/************************************************************************/
GbxmlDataAnalyzer::GbxmlDataAnalyzer(IfcDB::Populationi* pDB, IfcDB::IfcEntity* pBuilding)
  : DataAnalyzer(pDB, pBuilding)
{
}

void GbxmlDataAnalyzer::analyseBuildingParameter()
{
  m_buildingParameter.buildingName = toUtf8(m_pBuilding->getName());

  std::vector<IfcDB::IfcEntity*> spaces;
  m_pDB->getAllChildren(IfcDB::GBXML_SPACE, m_pBuilding->getOid(), spaces);

  double unitFactor = m_pDB->getUnitFactor(_T("LENGTHUNIT"));
  double areaFactor = unitFactor * unitFactor / 1000000;

  for (auto pSpace : spaces)
  {
    bool hasArea(false);

    std::wstring value;

    if (pSpace->getAttribute(_T("Area"), value))
    {
      hasArea = true;
      m_buildingParameter.totalArea += std::stod(value) * areaFactor;
    }

    if (!hasArea)
    {
      IfcDB::cEntityQuantities quantities(m_pDB, pSpace);
      quantities.calcQuantities(pSpace->getGeometry(L"PlanarGeometry"));

      double baseArea;
      if (quantities.getQuantity(L"BaseArea", baseArea))
      {
        m_buildingParameter.totalArea += baseArea * areaFactor;
      }
    }
  }
}
