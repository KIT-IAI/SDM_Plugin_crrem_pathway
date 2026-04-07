#pragma once

#include <Plugin.hpp>
#include <ActionFeatureHelper.hpp>
#include <MainFrameObserverImpl.hpp>
#include <DocumentObserver.hpp>
#include <ifcdb/utils/PopulationObserver.hpp>
#include <LiveLogFeature.hpp>
#include <MessageDialogFeature.hpp>
#include <MessageDialogFeatureHelper.hpp>
#include <LogDialogFeature.hpp>
#include <LogDialogFeatureHelper.hpp>
#include "PluginObserver.h"

class CRREMPathwayDlgImpl;

class CRREMPathwayAction : public sdm::plugin::ActionFeatureHelper
{
  public:
    CRREMPathwayAction();
    virtual ~CRREMPathwayAction();

    void execute() const override;
    bool isActive() const override;

    void deleteDlg();

    void setMainFrameWnd(intptr_t hParentWnd)
    {
    }
    void setLogger(sdm::plugin::LiveLogInterface* pLiveLogInterface) { m_pLogger = pLiveLogInterface; }
    void setDB(IfcDB::Populationi* pDB) { m_pDB = pDB; }
    void setStates(IfcDB::utils::PopulationStates* pSates);

    bool isDialogOpen() { return m_isDialogOpen; }
    void setIsDialogOpen(bool state) { m_isDialogOpen = state; }
    CRREMPathwayDlgImpl* getDlg() { return m_pCRREMPathwayDlg; }

    sdm::plugin::LiveLogInterface* getLogger() { return m_pLogger; }
    IfcDB::Populationi* getDB() { return m_pDB; }
    IfcDB::utils::PopulationStates* getStates() { return m_pStates; }

  protected:
    bool m_isDialogOpen = false;
    intptr_t m_hParentWnd = 0;
    CRREMPathwayDlgImpl* m_pCRREMPathwayDlg = nullptr;
    sdm::plugin::LiveLogInterface* m_pLogger = nullptr;
    IfcDB::Populationi* m_pDB = nullptr;
    IfcDB::utils::PopulationStates* m_pStates = nullptr;
    PluginObserver* m_pObserver = nullptr;
};

class CRREMPathwayPlugin : public sdm::plugin::Plugin
{
  public:
    CRREMPathwayPlugin();
    ~CRREMPathwayPlugin();

    sdm::plugin::Version getInterfaceVersion() const override;
    sdm::plugin::PluginInfo getInfo() const override;
    std::vector<sdm::plugin::Feature*> getFeatures() const override;

    sdm::plugin::ComponentInfo getComponentInfo(const sdm::plugin::RequiredComponent& requiredComponent) const override;
    const sdm::plugin::InitializationState& getInitializationState() const override;

    void setDB(IfcDB::Populationi* pDB);
    void setStates(IfcDB::utils::PopulationStates* pStates);

  private:
    sdm::plugin::MainFrameObserverImpl m_MainFrameObserver;
    sdm::plugin::DocumentObserverImpl m_documentObserver;
    sdm::plugin::LiveLogObserver m_liveLogObserver;
    CRREMPathwayAction m_CRREMPathwayAction;
    sdm::plugin::InitializationState m_initState;
    IfcDB::Populationi* m_pDB = nullptr;
    IfcDB::utils::PopulationStates* m_pStates = nullptr;
};