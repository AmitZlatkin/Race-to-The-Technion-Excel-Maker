#include "MainFrame.h"
#include <wx/display.h>

MainFrame::MainFrame() : wxFrame(nullptr, wxID_ANY, "RaceXL", wxDefaultPosition, wxSize(1480, 920))
{
    SetMinSize(wxSize(1180, 760));

    //// Get the display where the window is or will be
    //wxDisplay display(wxDisplay::GetFromWindow(this));
    //wxRect rect = display.GetClientArea();

    //// Calculate desired size
    //int w = rect.GetWidth() * 0.6;
    //int h = rect.GetHeight() * 0.75;

    //// Apply size and center
    //SetSize(w, h);

    SetBackgroundColour(wxColour(246, 243, 237));
    BuildMenu();
    BuildUi();
    SeedActivities();
    RefreshCards();
    UpdateSummary();

    Centre();
}

void MainFrame::BuildMenu()
{
    auto* menuBar = new wxMenuBar();
    auto* fileMenu = new wxMenu();
    fileMenu->Append(ID_IMPORT_JSON, "Import JSON...\tCtrl+I");
    fileMenu->Append(ID_EXPORT_JSON, "Export JSON...\tCtrl+E");
    fileMenu->AppendSeparator();
    fileMenu->Append(ID_GENERATE_EXCEL, "Generate Excel\tCtrl+G");
    fileMenu->AppendSeparator();
    fileMenu->Append(wxID_EXIT, "Exit");
    menuBar->Append(fileMenu, "");
    SetMenuBar(menuBar);
    CreateStatusBar();
    SetStatusText("RaceXL v2.0.0 by Amit Zlatkin");

    Bind(wxEVT_MENU, &MainFrame::OnImportJson, this, ID_IMPORT_JSON);
    Bind(wxEVT_MENU, &MainFrame::OnExportJson, this, ID_EXPORT_JSON);
    Bind(wxEVT_MENU, &MainFrame::OnGenerateExcel, this, ID_GENERATE_EXCEL);
    Bind(wxEVT_MENU, [this](wxCommandEvent&) { Close(); }, wxID_EXIT);
}

wxPanel* MainFrame::MakeSection(wxWindow* parent, const wxString& title, const wxString& subtitle)
{
    auto* panel = new wxPanel(parent);
    panel->SetBackgroundColour(wxColour(255, 253, 249));
    auto* sizer = new wxBoxSizer(wxVERTICAL);
    auto* titleText = new wxStaticText(panel, wxID_ANY, title);
    titleText->SetFont(wxFontInfo(14).Bold());
    sizer->Add(titleText, 0, wxLEFT | wxRIGHT | wxTOP, 18);
    if (!subtitle.empty())
    {
        auto* sub = new wxStaticText(panel, wxID_ANY, subtitle);
        sub->SetForegroundColour(wxColour(110, 101, 91));
        sizer->Add(sub, 0, wxLEFT | wxRIGHT | wxTOP | wxBOTTOM, 18);
    }
    panel->SetSizer(sizer);
    return panel;
}

void MainFrame::BuildUi()
{
    auto* root = new wxBoxSizer(wxVERTICAL);
    auto* header = new wxPanel(this);
    header->SetBackgroundColour(wxColour(250, 248, 243));
    auto* headerSizer = new wxBoxSizer(wxHORIZONTAL);
    auto* appTitle = new wxStaticText(header, wxID_ANY, "RaceXL - Visual Builder");
    appTitle->SetFont(wxFontInfo(18).Bold());
    auto* appSub = new wxStaticText(header, wxID_ANY, "Create Excel score sheets without editing JSON by hand");
    appSub->SetForegroundColour(wxColour(108, 99, 89));
    auto* titleStack = new wxBoxSizer(wxVERTICAL);
    titleStack->Add(appTitle, 0, wxBOTTOM, 2);
    titleStack->Add(appSub, 0);
    headerSizer->Add(titleStack, 1, wxALL | wxALIGN_CENTER_VERTICAL, 18);
    auto* importBtn = new wxButton(header, ID_IMPORT_JSON, "Import JSON");
    auto* exportBtn = new wxButton(header, ID_EXPORT_JSON, "Export JSON");
    auto* generateBtn = new wxButton(header, ID_GENERATE_EXCEL, "Generate Excel");
    headerSizer->Add(importBtn, 0, wxALL | wxALIGN_CENTER_VERTICAL, 8);
    headerSizer->Add(exportBtn, 0, wxALL | wxALIGN_CENTER_VERTICAL, 8);
    headerSizer->Add(generateBtn, 0, wxALL | wxALIGN_CENTER_VERTICAL, 8);
    header->SetSizer(headerSizer);
    root->Add(header, 0, wxEXPAND | wxALL, 10);

    auto* splitter = new wxSplitterWindow(this, wxID_ANY);
    splitter->SetSashGravity(0.18);
    splitter->SetMinimumPaneSize(260);

    auto* left = new wxPanel(splitter);
    left->SetBackgroundColour(wxColour(242, 238, 230));
    auto* leftSizer = new wxBoxSizer(wxVERTICAL);

    auto* setupPanel = MakeSection(left, "Race setup", "Basic project settings");
    auto* setupSizer = static_cast<wxBoxSizer*>(setupPanel->GetSizer());
    auto* setupGrid = new wxFlexGridSizer(2, 8, 10);
    setupGrid->AddGrowableCol(1, 1);
    setupGrid->Add(new wxStaticText(setupPanel, wxID_ANY, "Race name"), 0, wxALIGN_CENTER_VERTICAL);
    m_raceNameCtrl = new wxTextCtrl(setupPanel, wxID_ANY, "2026 Technion Race");
    setupGrid->Add(m_raceNameCtrl, 1, wxEXPAND);
    setupGrid->Add(new wxStaticText(setupPanel, wxID_ANY, "Teams"), 0, wxALIGN_CENTER_VERTICAL);
    m_teamsCtrl = new wxSpinCtrl(setupPanel, wxID_ANY);
    m_teamsCtrl->SetRange(1, 999);
    m_teamsCtrl->SetValue(12);
    setupGrid->Add(m_teamsCtrl, 1, wxEXPAND);
    setupGrid->Add(new wxStaticText(setupPanel, wxID_ANY, "Output file"), 0, wxALIGN_CENTER_VERTICAL);
    m_outputCtrl = new wxTextCtrl(setupPanel, wxID_ANY, "race_to_the_technion.xlsx");
    setupGrid->Add(m_outputCtrl, 1, wxEXPAND);
    setupSizer->Add(setupGrid, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 18);
    leftSizer->Add(setupPanel, 0, wxEXPAND | wxALL, 10);

    auto* notesPanel = MakeSection(left, "Workflow", "Builder first, JSON optional");
    auto* notesSizer = static_cast<wxBoxSizer*>(notesPanel->GetSizer());
    notesSizer->Add(new wxStaticText(notesPanel, wxID_ANY, "• Add activities one by one as cards"), 0, wxLEFT | wxRIGHT | wxBOTTOM, 18);
    notesSizer->Add(new wxStaticText(notesPanel, wxID_ANY, "• Import/export JSON only when needed"), 0, wxLEFT | wxRIGHT | wxBOTTOM, 18);
    notesSizer->Add(new wxStaticText(notesPanel, wxID_ANY, "• Review workbook preview before generation"), 0, wxLEFT | wxRIGHT | wxBOTTOM, 18);
    leftSizer->Add(notesPanel, 0, wxEXPAND | wxALL, 10);

    leftSizer->AddStretchSpacer();
    left->SetSizer(leftSizer);

    auto* right = new wxPanel(splitter);
    right->SetBackgroundColour(wxColour(246, 243, 237));
    auto* rightSizer = new wxBoxSizer(wxVERTICAL);

    auto* contentSplitter = new wxSplitterWindow(right, wxID_ANY);
    contentSplitter->SetSashGravity(0.72);
    contentSplitter->SetMinimumPaneSize(320);

    auto* builderPane = new wxPanel(contentSplitter);
    builderPane->SetBackgroundColour(wxColour(246, 243, 237));
    auto* builderSizer = new wxBoxSizer(wxVERTICAL);

    auto* formPanel = MakeSection(builderPane, "Add a new activity", "Translate the underlying schema into friendly controls");
    auto* formSizer = static_cast<wxBoxSizer*>(formPanel->GetSizer());
    auto* formGrid = new wxFlexGridSizer(2, 10, 12);
    formGrid->AddGrowableCol(1, 1);
    formGrid->Add(new wxStaticText(formPanel, wxID_ANY, "Activity name"), 0, wxALIGN_CENTER_VERTICAL);
    m_nameCtrl = new wxTextCtrl(formPanel, wxID_ANY, "Human Pyramid");
    formGrid->Add(m_nameCtrl, 1, wxEXPAND);
    formGrid->Add(new wxStaticText(formPanel, wxID_ANY, "Location"), 0, wxALIGN_CENTER_VERTICAL);
    m_locationCtrl = new wxTextCtrl(formPanel, wxID_ANY, "Central Courtyard");
    formGrid->Add(m_locationCtrl, 1, wxEXPAND);
    formGrid->Add(new wxStaticText(formPanel, wxID_ANY, "Rows"), 0, wxALIGN_CENTER_VERTICAL);
    m_rowsCtrl = new wxSpinCtrl(formPanel, wxID_ANY);
    m_rowsCtrl->SetRange(0, 100);
    m_rowsCtrl->SetValue(5);
    formGrid->Add(m_rowsCtrl, 1, wxEXPAND);
    formGrid->Add(new wxStaticText(formPanel, wxID_ANY, "Points per row"), 0, wxALIGN_CENTER_VERTICAL);
    m_pointsCtrl = new wxSpinCtrl(formPanel, wxID_ANY);
    m_pointsCtrl->SetRange(0, 1000);
    m_pointsCtrl->SetValue(10);
    formGrid->Add(m_pointsCtrl, 1, wxEXPAND);
    formSizer->Add(formGrid, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 18);

    auto* optionsRow = new wxBoxSizer(wxHORIZONTAL);
    m_makeWorksheetCheck = new wxCheckBox(formPanel, wxID_ANY, "Create worksheet");
    m_makeWorksheetCheck->SetValue(true);
    m_scoringChoice = new wxChoice(formPanel, wxID_ANY);
    m_scoringChoice->Append("Numeric scoring");
    m_scoringChoice->Append("Checkbox scoring");
    m_scoringChoice->SetSelection(0);
    optionsRow->Add(m_makeWorksheetCheck, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 16);
    optionsRow->Add(new wxStaticText(formPanel, wxID_ANY, "Scoring type"), 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 8);
    optionsRow->Add(m_scoringChoice, 1, wxRIGHT, 8);
    auto* addBtn = new wxButton(formPanel, ID_ADD_ACTIVITY, "Add activity card");
    optionsRow->Add(addBtn, 0);
    formSizer->Add(optionsRow, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 18);
    builderSizer->Add(formPanel, 0, wxEXPAND | wxALL, 10);

    auto* cardsPanel = MakeSection(builderPane, "Activities", "Visual cards instead of raw JSON objects");
    auto* cardsPanelSizer = static_cast<wxBoxSizer*>(cardsPanel->GetSizer());
    m_cardsScroll = new wxScrolledWindow(cardsPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxVSCROLL);
    m_cardsScroll->SetScrollRate(0, 10);
    m_cardsScroll->SetBackgroundColour(wxColour(255, 253, 249));
    m_cardsSizer = new wxBoxSizer(wxVERTICAL);
    m_cardsScroll->SetSizer(m_cardsSizer);
    cardsPanelSizer->Add(m_cardsScroll, 1, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 18);
    builderSizer->Add(cardsPanel, 1, wxEXPAND | wxALL, 10);
    builderPane->SetSizer(builderSizer);

    auto* summaryPane = new wxPanel(contentSplitter);
    summaryPane->SetBackgroundColour(wxColour(246, 243, 237));
    auto* summarySizer = new wxBoxSizer(wxVERTICAL);

    auto* summaryPanel = MakeSection(summaryPane, "Summary", "Live race overview");
    auto* summaryPanelSizer = static_cast<wxBoxSizer*>(summaryPanel->GetSizer());
    m_summaryRace = new wxStaticText(summaryPanel, wxID_ANY, "Race: -");
    m_summaryTeams = new wxStaticText(summaryPanel, wxID_ANY, "Teams: -");
    m_summaryActivities = new wxStaticText(summaryPanel, wxID_ANY, "Activities: -");
    summaryPanelSizer->Add(m_summaryRace, 0, wxLEFT | wxRIGHT | wxBOTTOM, 18);
    summaryPanelSizer->Add(m_summaryTeams, 0, wxLEFT | wxRIGHT | wxBOTTOM, 18);
    summaryPanelSizer->Add(m_summaryActivities, 0, wxLEFT | wxRIGHT | wxBOTTOM, 18);
    summarySizer->Add(summaryPanel, 0, wxEXPAND | wxALL, 10);

    auto* previewPanel = MakeSection(summaryPane, "Workbook preview", "Stations and scoring mode");
    auto* previewSizer = static_cast<wxBoxSizer*>(previewPanel->GetSizer());
    m_previewCtrl = new wxTextCtrl(previewPanel, wxID_ANY, "", wxDefaultPosition, wxSize(-1, 420), wxTE_MULTILINE | wxTE_READONLY | wxBORDER_NONE);
    m_previewCtrl->SetBackgroundColour(wxColour(248, 244, 236));
    previewSizer->Add(m_previewCtrl, 1, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 18);
    summarySizer->Add(previewPanel, 1, wxEXPAND | wxALL, 10);
    summaryPane->SetSizer(summarySizer);

    contentSplitter->SplitVertically(builderPane, summaryPane, 920);
    rightSizer->Add(contentSplitter, 1, wxEXPAND | wxALL, 8);
    right->SetSizer(rightSizer);

    splitter->SplitVertically(left, right, 280);
    root->Add(splitter, 1, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);
    SetSizer(root);

    Bind(wxEVT_BUTTON, &MainFrame::OnAddActivity, this, ID_ADD_ACTIVITY);
    Bind(wxEVT_BUTTON, &MainFrame::OnImportJson, this, ID_IMPORT_JSON);
    Bind(wxEVT_BUTTON, &MainFrame::OnExportJson, this, ID_EXPORT_JSON);
    Bind(wxEVT_BUTTON, &MainFrame::OnGenerateExcel, this, ID_GENERATE_EXCEL);
    Bind(wxEVT_TEXT, [this](wxCommandEvent&) { UpdateSummary(); }, m_raceNameCtrl->GetId());
    Bind(wxEVT_SPINCTRL, [this](wxSpinEvent&) { UpdateSummary(); }, m_teamsCtrl->GetId());
}

void MainFrame::SeedActivities()
{
    m_activities.push_back({ "Activity A", "Location A", true, true, 3, 0 });
    m_activities.push_back({ "Treasure Hunt", "Library Plaza", true, false, 5, 8 });
    m_activities.push_back({ "Human Pyramid", "Central Courtyard", false, false, 0, 10 });
}

void MainFrame::RefreshCards()
{
    m_cardsSizer->Clear(true);
    for (size_t i = 0; i < m_activities.size(); ++i)
    {
        auto* card = new CardPanel(m_cardsScroll, m_activities[i], i);
        m_cardsSizer->Add(card, 0, wxEXPAND | wxBOTTOM, 12);
    }
    m_cardsSizer->Layout();
    m_cardsScroll->FitInside();
    m_cardsScroll->Layout();
    UpdatePreview();
    UpdateSummary();
}

void MainFrame::UpdatePreview()
{
    wxString text;
    text << "Station\tLocation\tMode\tWorksheet\n";
    text << "-----------------------------------------------\n";
    for (const auto& a : m_activities)
    {
        text << a.name << "\t" << a.location << "\t"
            << (a.numberTable ? "Numeric" : "Checkbox") << "\t"
            << (a.makeWorksheet ? "Yes" : "No") << "\n";
    }
    m_previewCtrl->SetValue(text);
}

void MainFrame::UpdateSummary()
{
    m_summaryRace->SetLabel("Race: " + m_raceNameCtrl->GetValue());
    m_summaryTeams->SetLabel(wxString::Format("Teams: %d", m_teamsCtrl->GetValue()));
    m_summaryActivities->SetLabel(wxString::Format("Activities: %zu", m_activities.size()));
    Layout();
}

void MainFrame::OnAddActivity(wxCommandEvent&)
{
    RaceActivity a;
    a.name = m_nameCtrl->GetValue();
    a.location = m_locationCtrl->GetValue();
    a.makeWorksheet = m_makeWorksheetCheck->GetValue();
    a.numberTable = m_scoringChoice->GetSelection() == 0;
    a.rows = m_rowsCtrl->GetValue();
    a.pointsPerRow = m_pointsCtrl->GetValue();

    if (a.name.Trim().IsEmpty())
    {
        wxMessageBox("Please enter an activity name.", "Missing data", wxOK | wxICON_WARNING, this);
        return;
    }

    m_activities.push_back(a);
    RefreshCards();
    SetStatusText("Activity card added");
}

void MainFrame::OnImportJson(wxCommandEvent&)
{
    wxFileDialog dlg(this, "Import JSON", "", "", "JSON files (*.json)|*.json|All files (*.*)|*.*", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
    if (dlg.ShowModal() == wxID_OK)
    {
        wxMessageBox("JSON import hook placeholder:\n\nSelected file:\n" + dlg.GetPath() + "\n\nIn the next step, this should parse the configuration and convert it into editable activity cards.", "Import JSON", wxOK | wxICON_INFORMATION, this);
        SetStatusText("JSON import selected: " + dlg.GetFilename());
    }
}

void MainFrame::OnExportJson(wxCommandEvent&)
{
    wxFileDialog dlg(this, "Export JSON", "", "race-config.json", "JSON files (*.json)|*.json", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    if (dlg.ShowModal() == wxID_OK)
    {
        wxString json;
        json << "{\n";
        json << "  \"teams\": " << m_teamsCtrl->GetValue() << ",\n";
        json << "  \"activities\": [\n";
        for (size_t i = 0; i < m_activities.size(); ++i)
        {
            const auto& a = m_activities[i];
            json << "    {\n";
            json << "      \"name\": \"" << a.name << "\",\n";
            json << "      \"location\": \"" << a.location << "\",\n";
            json << "      \"makeWorksheet\": " << (a.makeWorksheet ? "true" : "false") << ",\n";
            json << "      \"rows\": " << a.rows << ",\n";
            json << "      \"numberTable\": " << (a.numberTable ? "true" : "false") << ",\n";
            json << "      \"pointsPerRow\": " << a.pointsPerRow << "\n";
            json << "    }" << (i + 1 < m_activities.size() ? "," : "") << "\n";
        }
        json << "  ]\n}";

        wxFile file;
        if (file.Open(dlg.GetPath(), wxFile::write)) file.Close();
        if (file.Create(dlg.GetPath(), true) || file.Open(dlg.GetPath(), wxFile::write))
        {
            file.Write(json);
            file.Close();
            wxMessageBox("JSON exported successfully.", "Export JSON", wxOK | wxICON_INFORMATION, this);
            SetStatusText("JSON exported");
        }
        else
        {
            wxMessageBox("Could not write the JSON file.", "Export error", wxOK | wxICON_ERROR, this);
        }
    }
}

void MainFrame::OnGenerateExcel(wxCommandEvent&)
{
    wxString msg;
    msg << "This GUI is ready for integration with the existing RaceXL generation engine.\n\n";
    msg << "Race: " << m_raceNameCtrl->GetValue() << "\n";
    msg << "Teams: " << m_teamsCtrl->GetValue() << "\n";
    msg << "Activities: " << m_activities.size() << "\n";
    msg << "Output: " << m_outputCtrl->GetValue();
    wxMessageBox(msg, "Generate Excel", wxOK | wxICON_INFORMATION, this);
    SetStatusText("Generate Excel clicked");
}