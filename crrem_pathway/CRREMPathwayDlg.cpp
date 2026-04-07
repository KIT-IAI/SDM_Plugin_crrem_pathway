///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "CRREMPathwayDlg.h"

///////////////////////////////////////////////////////////////////////////

CRREMPathwayDlg::CRREMPathwayDlg( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer;
	bSizer = new wxBoxSizer( wxVERTICAL );

	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer1->AddGrowableCol( 1 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_nameText = new wxStaticText( this, wxID_ANY, wxT("Name"), wxDefaultPosition, wxDefaultSize, 0 );
	m_nameText->Wrap( -1 );
	fgSizer1->Add( m_nameText, 0, wxALL, 5 );

	m_nameCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	fgSizer1->Add( m_nameCtrl, 1, wxALL|wxEXPAND, 5 );

	m_yearOfConstructionText = new wxStaticText( this, wxID_ANY, wxT("Year of construction"), wxDefaultPosition, wxDefaultSize, 0 );
	m_yearOfConstructionText->Wrap( -1 );
	fgSizer1->Add( m_yearOfConstructionText, 0, wxALL, 5 );

	m_yearOfConstructionCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	fgSizer1->Add( m_yearOfConstructionCtrl, 1, wxALL|wxEXPAND, 5 );

	m_buildingTypeText = new wxStaticText( this, wxID_ANY, wxT("Building type"), wxDefaultPosition, wxDefaultSize, 0 );
	m_buildingTypeText->Wrap( -1 );
	fgSizer1->Add( m_buildingTypeText, 0, wxALL, 5 );

	m_buildingTypeCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	fgSizer1->Add( m_buildingTypeCtrl, 1, wxALL|wxEXPAND, 5 );

	m_areaText = new wxStaticText( this, wxID_ANY, wxT("Total area [m²]"), wxDefaultPosition, wxDefaultSize, 0 );
	m_areaText->Wrap( -1 );
	fgSizer1->Add( m_areaText, 0, wxALL, 5 );

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );

	m_areaCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	bSizer4->Add( m_areaCtrl, 1, wxALL|wxEXPAND, 5 );

	m_areaOverrideCheckBox = new wxCheckBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( m_areaOverrideCheckBox, 0, wxALL, 5 );


	fgSizer1->Add( bSizer4, 1, wxEXPAND, 5 );

	m_annualValueText = new wxStaticText( this, wxID_ANY, wxT("Annual energy demand [kWh]"), wxDefaultPosition, wxDefaultSize, 0 );
	m_annualValueText->Wrap( -1 );
	fgSizer1->Add( m_annualValueText, 0, wxALL, 5 );

	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxHORIZONTAL );

	m_annualValueCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	bSizer5->Add( m_annualValueCtrl, 1, wxALL|wxEXPAND, 5 );

	m_energyButton = new wxButton( this, wxID_ANY, wxT("..."), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer5->Add( m_energyButton, 0, wxALL, 5 );

	m_annualValueOverrideCheckBox = new wxCheckBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer5->Add( m_annualValueOverrideCheckBox, 0, wxALL, 5 );


	fgSizer1->Add( bSizer5, 1, wxEXPAND, 5 );

	m_scenarioText = new wxStaticText( this, wxID_ANY, wxT("Scenario"), wxDefaultPosition, wxDefaultSize, 0 );
	m_scenarioText->Wrap( -1 );
	fgSizer1->Add( m_scenarioText, 0, wxALL, 5 );

	wxString m_scenarioChoiceChoices[] = { wxT("1,5°"), wxT("2,0 °") };
	int m_scenarioChoiceNChoices = sizeof( m_scenarioChoiceChoices ) / sizeof( wxString );
	m_scenarioChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_scenarioChoiceNChoices, m_scenarioChoiceChoices, 0 );
	m_scenarioChoice->SetSelection( 0 );
	m_scenarioChoice->Enable( false );

	fgSizer1->Add( m_scenarioChoice, 1, wxALL|wxEXPAND, 5 );

	m_countryText = new wxStaticText( this, wxID_ANY, wxT("Country"), wxDefaultPosition, wxDefaultSize, 0 );
	m_countryText->Wrap( -1 );
	fgSizer1->Add( m_countryText, 0, wxALL, 5 );

	wxArrayString m_countryChoiceChoices;
	m_countryChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_countryChoiceChoices, 0 );
	m_countryChoice->SetSelection( 0 );
	fgSizer1->Add( m_countryChoice, 1, wxALL|wxEXPAND, 5 );

	m_buildingTypeChoiseText = new wxStaticText( this, wxID_ANY, wxT("Building type"), wxDefaultPosition, wxDefaultSize, 0 );
	m_buildingTypeChoiseText->Wrap( -1 );
	fgSizer1->Add( m_buildingTypeChoiseText, 0, wxALL, 5 );

	wxArrayString m_buildingTypeChoiceChoices;
	m_buildingTypeChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_buildingTypeChoiceChoices, 0 );
	m_buildingTypeChoice->SetSelection( 0 );
	fgSizer1->Add( m_buildingTypeChoice, 1, wxALL|wxEXPAND, 5 );

	m_impactTypeText = new wxStaticText( this, wxID_ANY, wxT("Impact type"), wxDefaultPosition, wxDefaultSize, 0 );
	m_impactTypeText->Wrap( -1 );
	fgSizer1->Add( m_impactTypeText, 0, wxALL, 5 );

	wxString m_impactTypeChoiceChoices[] = { wxT("kWh"), wxT("kgCO²") };
	int m_impactTypeChoiceNChoices = sizeof( m_impactTypeChoiceChoices ) / sizeof( wxString );
	m_impactTypeChoice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_impactTypeChoiceNChoices, m_impactTypeChoiceChoices, 0 );
	m_impactTypeChoice->SetSelection( 0 );
	fgSizer1->Add( m_impactTypeChoice, 1, wxALL|wxEXPAND, 5 );

	m_startYearText = new wxStaticText( this, wxID_ANY, wxT("Start year"), wxDefaultPosition, wxDefaultSize, 0 );
	m_startYearText->Wrap( -1 );
	fgSizer1->Add( m_startYearText, 0, wxALL, 5 );

	m_startYearCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_startYearCtrl, 1, wxALL|wxEXPAND, 5 );

	m_targetYearText = new wxStaticText( this, wxID_ANY, wxT("Target year"), wxDefaultPosition, wxDefaultSize, 0 );
	m_targetYearText->Wrap( -1 );
	fgSizer1->Add( m_targetYearText, 0, wxALL, 5 );

	m_targetYearCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_targetYearCtrl, 1, wxALL|wxEXPAND, 5 );

	m_startValueText = new wxStaticText( this, wxID_ANY, wxT("Start value [kWh/m²]"), wxDefaultPosition, wxDefaultSize, 0 );
	m_startValueText->Wrap( -1 );
	fgSizer1->Add( m_startValueText, 0, wxALL, 5 );

	m_startValueCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_startValueCtrl, 1, wxALL|wxEXPAND, 5 );

	m_annualReductionRateText = new wxStaticText( this, wxID_ANY, wxT("Annual reduction rate [%]"), wxDefaultPosition, wxDefaultSize, 0 );
	m_annualReductionRateText->Wrap( -1 );
	fgSizer1->Add( m_annualReductionRateText, 0, wxALL, 5 );

	m_annualReductionRateCtrl = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_annualReductionRateCtrl, 1, wxALL|wxEXPAND, 5 );


	bSizer->Add( fgSizer1, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );

	m_crremDiagram = new CrremDiagram(this);
	bSizer2->Add( m_crremDiagram, 1, wxALL|wxEXPAND, 5 );

	m_sdbSizer1 = new wxStdDialogButtonSizer();
	m_sdbSizer1OK = new wxButton( this, wxID_OK );
	m_sdbSizer1->AddButton( m_sdbSizer1OK );
	m_sdbSizer1Apply = new wxButton( this, wxID_APPLY );
	m_sdbSizer1->AddButton( m_sdbSizer1Apply );
	m_sdbSizer1Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer1->AddButton( m_sdbSizer1Cancel );
	m_sdbSizer1->Realize();

	bSizer2->Add( m_sdbSizer1, 0, wxEXPAND|wxRIGHT, 5 );


	bSizer->Add( bSizer2, 1, wxEXPAND, 5 );


	this->SetSizer( bSizer );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( CRREMPathwayDlg::OnClose ) );
	this->Connect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( CRREMPathwayDlg::OnInitDialog ) );
	m_areaOverrideCheckBox->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CRREMPathwayDlg::OnAreaCheck ), NULL, this );
	m_energyButton->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CRREMPathwayDlg::OnEnergyButtonClick ), NULL, this );
	m_annualValueOverrideCheckBox->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CRREMPathwayDlg::OnEnergyCheck ), NULL, this );
	m_impactTypeChoice->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CRREMPathwayDlg::OnImpactTypeChoice ), NULL, this );
	m_sdbSizer1Apply->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CRREMPathwayDlg::OnApply ), NULL, this );
	m_sdbSizer1Cancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CRREMPathwayDlg::OnCancel ), NULL, this );
	m_sdbSizer1OK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CRREMPathwayDlg::OnOK ), NULL, this );
}

CRREMPathwayDlg::~CRREMPathwayDlg()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( CRREMPathwayDlg::OnClose ) );
	this->Disconnect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( CRREMPathwayDlg::OnInitDialog ) );
	m_areaOverrideCheckBox->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CRREMPathwayDlg::OnAreaCheck ), NULL, this );
	m_energyButton->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CRREMPathwayDlg::OnEnergyButtonClick ), NULL, this );
	m_annualValueOverrideCheckBox->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CRREMPathwayDlg::OnEnergyCheck ), NULL, this );
	m_impactTypeChoice->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CRREMPathwayDlg::OnImpactTypeChoice ), NULL, this );
	m_sdbSizer1Apply->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CRREMPathwayDlg::OnApply ), NULL, this );
	m_sdbSizer1Cancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CRREMPathwayDlg::OnCancel ), NULL, this );
	m_sdbSizer1OK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CRREMPathwayDlg::OnOK ), NULL, this );

}

CRREMEnergyDlg::CRREMEnergyDlg( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );

	m_grid = new wxGrid( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );

	// Grid
	m_grid->CreateGrid( 2, 4 );
	m_grid->EnableEditing( true );
	m_grid->EnableGridLines( true );
	m_grid->EnableDragGridSize( false );
	m_grid->SetMargins( 0, 0 );

	// Columns
	m_grid->AutoSizeColumns();
	m_grid->EnableDragColMove( false );
	m_grid->EnableDragColSize( true );
	m_grid->SetColLabelValue( 0, wxT("Type") );
	m_grid->SetColLabelValue( 1, wxT("Amount [kWh]") );
	m_grid->SetColLabelValue( 2, wxT("Source") );
	m_grid->SetColLabelValue( 3, wxT("Factor [%]") );
	m_grid->SetColLabelSize( wxGRID_AUTOSIZE );
	m_grid->SetColLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Rows
	m_grid->EnableDragRowSize( true );
	m_grid->SetRowLabelSize( 0 );
	m_grid->SetRowLabelAlignment( wxALIGN_CENTER, wxALIGN_CENTER );

	// Label Appearance

	// Cell Defaults
	m_grid->SetDefaultCellAlignment( wxALIGN_LEFT, wxALIGN_TOP );
	bSizer5->Add( m_grid, 0, wxALL|wxEXPAND, 5 );

	m_sdbSizer2 = new wxStdDialogButtonSizer();
	m_sdbSizer2OK = new wxButton( this, wxID_OK );
	m_sdbSizer2->AddButton( m_sdbSizer2OK );
	m_sdbSizer2Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer2->AddButton( m_sdbSizer2Cancel );
	m_sdbSizer2->Realize();

	bSizer5->Add( m_sdbSizer2, 0, wxEXPAND, 5 );


	this->SetSizer( bSizer5 );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( CRREMEnergyDlg::OnInitDialog ) );
	m_grid->Connect( wxEVT_GRID_CELL_CHANGED, wxGridEventHandler( CRREMEnergyDlg::OnGridCellChange ), NULL, this );
}

CRREMEnergyDlg::~CRREMEnergyDlg()
{
	// Disconnect Events
	this->Disconnect( wxEVT_INIT_DIALOG, wxInitDialogEventHandler( CRREMEnergyDlg::OnInitDialog ) );
	m_grid->Disconnect( wxEVT_GRID_CELL_CHANGED, wxGridEventHandler( CRREMEnergyDlg::OnGridCellChange ), NULL, this );

}
