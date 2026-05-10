#pragma once
#include <wx/wx.h>
#include <wx/scrolwin.h>
#include <wx/splitter.h>
#include <wx/statline.h>
#include <wx/filedlg.h>
#include <wx/filepicker.h>
#include <wx/spinctrl.h>
#include <vector>
#include "CardPanel.h"
#include "RaceActivity.h"

class MainFrame : public wxFrame
{
public:
    MainFrame();

private:
    std::vector<RaceActivity> m_activities;

    wxTextCtrl* m_raceNameCtrl{};
    wxSpinCtrl* m_teamsCtrl{};
    wxTextCtrl* m_outputCtrl{};

    wxTextCtrl* m_nameCtrl{};
    wxTextCtrl* m_locationCtrl{};
    wxSpinCtrl* m_rowsCtrl{};
    wxSpinCtrl* m_pointsCtrl{};
    wxCheckBox* m_makeWorksheetCheck{};
    wxChoice* m_scoringChoice{};

    wxScrolledWindow* m_cardsScroll{};
    wxBoxSizer* m_cardsSizer{};

    wxStaticText* m_summaryRace{};
    wxStaticText* m_summaryTeams{};
    wxStaticText* m_summaryActivities{};
    wxTextCtrl* m_previewCtrl{};

    enum
    {
        ID_IMPORT_JSON = wxID_HIGHEST + 1,
        ID_EXPORT_JSON,
        ID_GENERATE_EXCEL,
        ID_ADD_ACTIVITY
    };

    void BuildMenu();

    wxPanel* MakeSection(wxWindow* parent, const wxString& title, const wxString& subtitle = wxEmptyString);

    void BuildUi();

    void SeedActivities();

    void RefreshCards();

    void UpdatePreview();

    void UpdateSummary();

    void OnAddActivity(wxCommandEvent&);

    void OnImportJson(wxCommandEvent&);

    void OnExportJson(wxCommandEvent&);

    void OnGenerateExcel(wxCommandEvent&);
};