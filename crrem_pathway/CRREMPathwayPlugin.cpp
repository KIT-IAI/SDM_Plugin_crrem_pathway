#include <iostream>
#include <string>
#include <vector>
#include <stdutils/executable.hpp>

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "CRREMPathwayPlugin.h"
#include "CRREMPathway.h"
#include "CRREMPathwayDlgImpl.h"

#include <ifcdb/IFCDBInterfaceVersion.hpp>
#include "PluginInterfaceVersion.hpp"
#include "ComponentRegistry.hpp"

using namespace std;
using namespace sdm::plugin;

IMPLEMENT_PLUGIN(CRREMPathwayPlugin);

CRREMPathwayAction::CRREMPathwayAction()
  : ActionFeatureHelper("CRREM Pathway")
{
  wxInitialize();
}

CRREMPathwayAction::~CRREMPathwayAction()
{
  wxUninitialize();
}

void CRREMPathwayAction::execute() const
{
  try
  {
    auto pDlg = new CRREMPathwayDlgImpl(nullptr, wxID_ANY, L"Carbon Risk Real Estate Monitor (CRREM)", wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE | wxSTAY_ON_TOP);
    pDlg->setCRREMPathwayAction(const_cast<CRREMPathwayAction*>(this));
    pDlg->setLogger(m_pLogger);
    pDlg->setDB(m_pDB);
    pDlg->setStates(m_pStates);
    pDlg->Show();

    const_cast<CRREMPathwayDlgImpl*>(m_pCRREMPathwayDlg) = pDlg;
  }
  catch (const std::exception& e)
  {
    const_cast<bool&>(m_isDialogOpen) = false;
    m_pLogger->log("CRREMPathway", e.what());
  }
}

bool CRREMPathwayAction::isActive() const
{
  if (m_pDB && !m_isDialogOpen &&
      (m_pDB->hasModelInfo(IfcDB::ModelInfo::MT_GML) ||
       m_pDB->hasModelInfo(IfcDB::ModelInfo::MT_IFC) ||
       m_pDB->hasModelInfo(IfcDB::ModelInfo::MT_GBXML)))
  {
    return true;
  }

  return false;
}

void CRREMPathwayAction::deleteDlg()
{
  m_pStates->unsubscribe(*m_pObserver);

  if (m_pCRREMPathwayDlg)
  {
    delete m_pCRREMPathwayDlg;
    m_pCRREMPathwayDlg = nullptr;
  }

  if (m_pObserver)
  {
    delete m_pObserver;
    m_pObserver = nullptr;
  }
}

void CRREMPathwayAction::setStates(IfcDB::utils::PopulationStates* pSates)
{
  m_pStates = pSates;

  if (!m_pObserver)
  {
    m_pObserver = new PluginObserver(*this);
    m_pStates->subscribe(*m_pObserver);
  }
}

CRREMPathwayPlugin::CRREMPathwayPlugin()
{
  m_MainFrameObserver.attach([&](sdm::plugin::MainFrameInterface* pMainFrameInterface) { m_CRREMPathwayAction.setMainFrameWnd(pMainFrameInterface->getParentWnd()); });
  m_documentObserver.attach([this](IfcDB::Populationi* pDB) { setDB(pDB); });
  m_documentObserver.attach([this](IfcDB::utils::PopulationSubject* pStates) { setStates(dynamic_cast<IfcDB::utils::PopulationStates*>(pStates)); });
  m_liveLogObserver.attach([this](sdm::plugin::LiveLogInterface* liveLogInterface) { m_CRREMPathwayAction.setLogger(liveLogInterface); });
}

CRREMPathwayPlugin::~CRREMPathwayPlugin()
{
}

sdm::plugin::Version CRREMPathwayPlugin::getInterfaceVersion() const
{
  return sdm::plugin::Version();
}

sdm::plugin::PluginInfo CRREMPathwayPlugin::getInfo() const
{
  sdm::plugin::PluginInfo info;
  info.name = "CRREMPathway";
  info.description = "CRREMPathway";
  info.version     = { 0, 1 };

  return info;
}

std::vector<sdm::plugin::Feature*> CRREMPathwayPlugin::getFeatures() const
{
  std::vector<sdm::plugin::Feature*> features;

  features.emplace_back(const_cast<CRREMPathwayAction*>(&m_CRREMPathwayAction));
  features.emplace_back(const_cast<MainFrameObserverImpl*>(&m_MainFrameObserver));
  features.emplace_back(const_cast<sdm::plugin::LiveLogObserver*>(&m_liveLogObserver));
  features.emplace_back(const_cast<DocumentObserverImpl*>(&m_documentObserver));

  return features;
}

ComponentInfo CRREMPathwayPlugin::getComponentInfo(const RequiredComponent& requiredComponent) const
{
  ComponentRegistry availableComponents;
  availableComponents.addAvailable(IFCDB_INTERFACE_COMPONENT_NAME, IFCDB_INTERFACE_COMPONENT_VERSION, IFCDB_INTERFACE_COMPONENT_HINT, std::atoi(IFCDB_INTERFACE_COMPONENT_DATE.data()));
  availableComponents.addAvailable(PLUGIN_INTERFACE_COMPONENT_NAME, PLUGIN_INTERFACE_COMPONENT_VERSION, PLUGIN_INTERFACE_COMPONENT_HINT, std::atoi(PLUGIN_INTERFACE_COMPONENT_DATE.data()));

  return availableComponents.getInfo(requiredComponent);
}

const sdm::plugin::InitializationState& CRREMPathwayPlugin::getInitializationState() const
{
  return m_initState;
}

void CRREMPathwayPlugin::setDB(IfcDB::Populationi* pDB)
{
  IfcDB::assignGlobalStates(pDB);
  m_CRREMPathwayAction.setDB(pDB);
}

void CRREMPathwayPlugin::setStates(IfcDB::utils::PopulationStates* pStates)
{
  m_CRREMPathwayAction.setStates(pStates);
}
