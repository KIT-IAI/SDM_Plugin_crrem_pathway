///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/choice.h>
#include "CRREMDiagram.h"
#include <wx/dialog.h>
#include <wx/grid.h>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class CRREMPathwayDlg
///////////////////////////////////////////////////////////////////////////////
class CRREMPathwayDlg : public wxDialog
{
	private:

	protected:
		wxStaticText* m_nameText;
		wxTextCtrl* m_nameCtrl;
		wxStaticText* m_yearOfConstructionText;
		wxTextCtrl* m_yearOfConstructionCtrl;
		wxStaticText* m_buildingTypeText;
		wxTextCtrl* m_buildingTypeCtrl;
		wxStaticText* m_areaText;
		wxTextCtrl* m_areaCtrl;
		wxCheckBox* m_areaOverrideCheckBox;
		wxStaticText* m_annualValueText;
		wxTextCtrl* m_annualValueCtrl;
		wxButton* m_energyButton;
		wxCheckBox* m_annualValueOverrideCheckBox;
		wxStaticText* m_scenarioText;
		wxChoice* m_scenarioChoice;
		wxStaticText* m_countryText;
		wxChoice* m_countryChoice;
		wxStaticText* m_buildingTypeChoiseText;
		wxChoice* m_buildingTypeChoice;
		wxStaticText* m_impactTypeText;
		wxChoice* m_impactTypeChoice;
		wxStaticText* m_startYearText;
		wxTextCtrl* m_startYearCtrl;
		wxStaticText* m_targetYearText;
		wxTextCtrl* m_targetYearCtrl;
		wxStaticText* m_startValueText;
		wxTextCtrl* m_startValueCtrl;
		wxStaticText* m_annualReductionRateText;
		wxTextCtrl* m_annualReductionRateCtrl;
		CrremDiagram* m_crremDiagram;
		wxStdDialogButtonSizer* m_sdbSizer1;
		wxButton* m_sdbSizer1OK;
		wxButton* m_sdbSizer1Apply;
		wxButton* m_sdbSizer1Cancel;

		// Virtual event handlers, override them in your derived class
		virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnInitDialog( wxInitDialogEvent& event ) { event.Skip(); }
		virtual void OnAreaCheck( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnEnergyButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnEnergyCheck( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnImpactTypeChoice( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnApply( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCancel( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnOK( wxCommandEvent& event ) { event.Skip(); }


	public:

		CRREMPathwayDlg( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Carbon Risk Real Estate Monitor (CRREM)"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 369,540 ), long style = wxCLOSE_BOX|wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );

		~CRREMPathwayDlg();

};

///////////////////////////////////////////////////////////////////////////////
/// Class CRREMEnergyDlg
///////////////////////////////////////////////////////////////////////////////
class CRREMEnergyDlg : public wxDialog
{
	private:

	protected:
		wxGrid* m_grid;
		wxStdDialogButtonSizer* m_sdbSizer2;
		wxButton* m_sdbSizer2OK;
		wxButton* m_sdbSizer2Cancel;

		// Virtual event handlers, override them in your derived class
		virtual void OnInitDialog( wxInitDialogEvent& event ) { event.Skip(); }
		virtual void OnGridCellChange( wxGridEvent& event ) { event.Skip(); }


	public:

		CRREMEnergyDlg( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("CRREM Energy Dialog"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 533,221 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );

		~CRREMEnergyDlg();

};

