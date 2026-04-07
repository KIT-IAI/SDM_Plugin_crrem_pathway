#pragma once

#include <ifcdb/utils/PopulationObserver.hpp>

class CRREMPathway;
class CRREMPathwayAction;

class PluginObserver final : public IfcDB::utils::PopulationObserver
{
  public:
    explicit PluginObserver(CRREMPathwayAction& action);
    ~PluginObserver() override = default;

    void activate(bool first) override;
    void deactivate(bool last) override;
    void selectEntity(IfcDB::ifcOid oid, bool selected) override;

  private:
    CRREMPathwayAction& m_CRREMPathwayAction;
};