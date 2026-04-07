#pragma once

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "CRREMPathwayDlgImpl.h"
#include "CRREMData.h"
#include <vector>

class CRREMEnergyDlgImpl : public CRREMEnergyDlg
{
	public:
		CRREMEnergyDlgImpl( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Energy Dialog"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
		~CRREMEnergyDlgImpl();

    virtual void OnInitDialog(wxInitDialogEvent& event);
		virtual void OnGridCellChange(wxGridEvent& event);

		void OnGridResize(wxSizeEvent& event);

		void setEnergyDemands(std::vector<EnergyData>* pEnergyDemands) { m_pEnergyDemands = pEnergyDemands; }

	private:
		std::vector<EnergyData>* m_pEnergyDemands = nullptr;
    wxArrayString m_sourceChoices;
};
