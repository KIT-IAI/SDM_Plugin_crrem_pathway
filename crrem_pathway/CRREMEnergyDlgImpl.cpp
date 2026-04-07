#include "CRREMEnergyDlgImpl.h"

CRREMEnergyDlgImpl::CRREMEnergyDlgImpl(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style)
  : CRREMEnergyDlg(parent, id, title, pos, size, style)
{
  m_grid->Bind(wxEVT_SIZE, &CRREMEnergyDlgImpl::OnGridResize, this);

  SetSize(wxSize(600, 400));
  Centre();

  SetDoubleBuffered(true);

  wxSize minSize = this->GetBestSize();
  this->SetSize(minSize);
}

CRREMEnergyDlgImpl::~CRREMEnergyDlgImpl()
{
}

void CRREMEnergyDlgImpl::OnInitDialog(wxInitDialogEvent& event)
{
  m_grid->SetColFormatFloat(1, -1, 2);
  m_grid->SetColFormatNumber(3);

  wxGridCellNumberEditor* intEditor = new wxGridCellNumberEditor(0, 100);
  wxGridCellAttr* attr = new wxGridCellAttr();
  m_grid->SetColAttr(3, attr);
  attr->SetEditor(intEditor);

  for (int i = 0; i < static_cast<int>(EnergySource::Undefined); ++i)
  {
    EnergySource source = static_cast<EnergySource>(i);
    m_sourceChoices.Add(EnergyData::toString(source));
  }

  int row(0);

  for (const auto& energyData : *m_pEnergyDemands)
  {
    m_grid->AppendRows();

    // type
    m_grid->SetCellValue(row, 0, energyData.type);
    m_grid->SetReadOnly(row, 0, true);

    // energy demand
    m_grid->SetCellValue(row, 1, wxString::FromDouble(energyData.energyDemand));
    m_grid->SetCellAlignment(row, 1, wxALIGN_RIGHT, wxALIGN_CENTER);
    m_grid->SetReadOnly(row, 1, true);

    // source
    wxGridCellAttr* attr = new wxGridCellAttr();
    attr->SetEditor(new wxGridCellChoiceEditor(m_sourceChoices));
    m_grid->SetColAttr(2, attr);
    m_grid->SetCellValue(row, 2, EnergyData::toString(energyData.source));

    // emission factor
    m_grid->SetCellValue(row, 3, wxString::FromDouble(energyData.emissionFactor * 100));
    m_grid->SetCellAlignment(row, 3, wxALIGN_RIGHT, wxALIGN_CENTER);

    ++row;
  }

  TransferDataToWindow();

  m_grid->AutoSizeColumns();

  DoGetBestClientSize();
  DoGetBestSize();
}

void CRREMEnergyDlgImpl::OnGridResize(wxSizeEvent& event)
{
  int cols = m_grid->GetNumberCols();
  if (cols == 0) return;

  int width = m_grid->GetClientSize().GetWidth() - m_grid->GetRowLabelSize();
  int colWidth = width / cols;

  for (int c = 0; c < cols; c++)
  {
    m_grid->SetColSize(c, colWidth);
  }

  event.Skip();
}

void CRREMEnergyDlgImpl::OnGridCellChange(wxGridEvent& event)
{
  int row = event.GetRow();
  int col = event.GetCol();

  if (col == 2)
  {
    wxString value = m_grid->GetCellValue(row, 2);

    int index = m_sourceChoices.Index(value);

    EnergySource source = static_cast<EnergySource>(index);
    double emissionFactor = EnergyData::getDefaultEmissionFactor(source);

    m_pEnergyDemands->at(row).emissionFactor = emissionFactor;
    m_pEnergyDemands->at(row).source = source;

    m_grid->SetCellValue(row, 3, wxString::Format("%d", (int)std::round(emissionFactor * 100)));
  }
  else if (col == 3)
  {
    m_pEnergyDemands->at(row).emissionFactor = wxAtof(m_grid->GetCellValue(row, col)) / 100;
  }

  event.Skip();
}