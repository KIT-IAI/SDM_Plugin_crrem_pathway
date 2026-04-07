#include "PluginObserver.h"
#include "CRREMPathwayPlugin.h"
#include "CRREMPathway.h"
#include "CRREMPathwayDlgImpl.h"

PluginObserver::PluginObserver(CRREMPathwayAction& action)
  : m_CRREMPathwayAction(action)
{
}

void PluginObserver::activate(bool first)
{
}

void PluginObserver::deactivate(bool last)
{
  if (last)
  {
    m_CRREMPathwayAction.setIsDialogOpen(false);

    auto pCRREMPathwayDlg = m_CRREMPathwayAction.getDlg();

    if (pCRREMPathwayDlg)
    {
      pCRREMPathwayDlg->Close();
      m_CRREMPathwayAction.deleteDlg();
    }
  }
}

void PluginObserver::selectEntity(IfcDB::ifcOid oid, bool selected)
{
  if (m_CRREMPathwayAction.getDlg() && m_CRREMPathwayAction.isDialogOpen())
  {
    if (!selected)
    {
      oid = 0;
    }

    m_CRREMPathwayAction.getDlg()->clearDialogData();
    m_CRREMPathwayAction.getDlg()->calculate(oid);
  }
}
