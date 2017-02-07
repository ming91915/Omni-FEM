#include "UI/OmniFEMFrame.h"


/***************
 * Constructor *
 ***************/
 
bool OmniFEMApp::OnInit()
{
   OmniFEMMainFrame *frame = new OmniFEMMainFrame("Omni-FEM", wxPoint(50, 50), minSize);
   frame->Show(true);
   return true; 
}

/****************************
 * Function Implementations *
 ****************************/
 
OmniFEMMainFrame::OmniFEMMainFrame(const wxString &title, const wxPoint &pos, const wxSize &size) : wxFrame(NULL, wxID_ANY, title, pos, size)
{
	minSize = size;
	
    /* This creates the main menu Bar at the top */
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuEdit, "&Edit");
    menuBar->Append(menuView, "&View");
    menuBar->Append(menuProblem, "&Problem");
    menuBar->Append(menuGrid, "&Grid");
    menuBar->Append(menuProperties, "&Properties");
    menuBar->Append(menuMesh, "&Mesh");
    menuBar->Append(menuHelp, "&Help");
    
    
    /* Creating the menu listing of File menu */
    menuFile->Append(menubarID::ID_menubarNew, "&New\tCtrl-N");
    menuFile->Append(menubarID::ID_menubarSave, "&Save\tCtrl-S");
    menuFile->Append(menubarID::ID_menubarSaveAs, "&Save As");
	menuFile->Append(menubarID::ID_menubarOpen, "&Open");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);
    
    /* Creating the menu listinging of the Edit Menu */
	menuEdit->Append(menubarID::ID_menubarLUASCRIPT, "&Lua Script\tCtrl-L");
    menuEdit->Append(menubarID::ID_menubarPreferences, "&Preferences\tCtrl-P");
	
	/* Creting the menu listing of the View Menu */
	menuView->Append(menubarID::ID_menubarViewResults, "&View Results");
    menuView->AppendSeparator();
    menuView->Append(menubarID::ID_menubarDispBlockLabels, "&Show Block Name");
	menuView->Append(menubarID::ID_menubarDispStatusBar, "&Show Status Bar");
    menuView->Append(menubarID::ID_menubarDispLuaConsole, "&Lua Console");
    
    /* Create hte menu listing for the grid menu option */
    menuGrid->Append(menubarID::ID_menubarShowGrid, "&Display Grid");
    menuGrid->Append(menubarID::ID_menubarSnapGrid, "&Snap to Grid");
    menuGrid->Append(menubarID::ID_menubarSetGrid, "&Set Grid");
    
    /* Create the menu listing for the properties option */
    menuProperties->Append(menubarID::ID_menubarMaterials, "&Materials\tCtrl-M");
    menuProperties->Append(menubarID::ID_menubarBoundary, "&Boundary Conditions\tCtrl-B");
    menuProperties->Append(menubarID::ID_menubarPoint, "&Nodal Properties");
    menuProperties->Append(menubarID::ID_menubarCircuitsCond, "&Circuits/Conductors");
    menuProperties->AppendSeparator();
    menuProperties->Append(menubarID::ID_menubarMatLibrary, "&Materials Library\tCtrl-L");
    
    
	/* Create the menu listing for the mesh menu */
	menuMesh->Append(menubarID::ID_menubarCreateMesh, "&Create Mesh");
	menuMesh->Append(menubarID::ID_menubarShowMesh, "&Show Mesh");
	menuMesh->Append(menubarID::ID_menubarDeleteMesh, "&Delete Mesh");
    
    /* Creates the menu listing of the help menu */
    menuHelp->Append(menubarID::ID_menubarManual, "View Manual");
    menuHelp->AppendSeparator();
    menuHelp->Append(menubarID::ID_menubarLicense, "License");
    menuHelp->Append(wxID_ABOUT);
	
	/*Create the menu listing of the Problem Menu */
	menuProblem->Append(menubarID::ID_menubarPrecision, "&Set Precision");
    
    /* Create and display the menu bar */
    SetMenuBar(menuBar);
    CreateStatusBar();
    
    SetStatusText("Omni-FEM Simulator");

	createTopToolBar();
	this->GetClientSize(&clientSizeWidth, &clientSizeLength);
	
	createInitialStartupClient();
	enableToolMenuBar(false);
	
	
	this->SetMinSize(minSize);
	this->SetMaxSize(minSize);
	this->SetInitialSize(minSize);
	
	arrayPhysicsProblem.Add("Electrostatics");
	arrayPhysicsProblem.Add("Magnetics");
}





void OmniFEMMainFrame::enableToolMenuBar(bool enable)
{
	menuBar->Enable(menubarID::ID_menubarShowMesh,	enable);
	menuBar->Enable(menubarID::ID_menubarSave,		enable);
	menuBar->Enable(menubarID::ID_menubarSaveAs,		enable);
	menuBar->Enable(menubarID::ID_menubarPreferences,	enable);
	menuBar->Enable(menubarID::ID_menubarViewResults,	enable);
	menuBar->Enable(menubarID::ID_menubarCreateMesh,	enable);
	menuBar->Enable(menubarID::ID_menubarDeleteMesh,	enable);
	menuBar->Enable(menubarID::ID_menubarPrecision,	enable);
	
	mainFrameToolBar->EnableTool(toolbarID::ID_ToolBarSave,	enable);
}




void OmniFEMMainFrame::createInitialStartupClient()
{
	systemState currentState = controller.getOmniFEMState();
	
	/* First, the function will need to destoy any other panels that are currently active */
	if(currentState == systemState::dimensionChoosing)
	{
		dimSelectPanel->Destroy();
		initialStartPanel = new wxPanel();
	}
	
	// This seciton will create a new panel and apply 2 buttons on the panel. 
	// The 2 buttons are associated with the panel and when the panel is destoryed, so are the buttons.
	initialStartPanel->Create(this, wxID_ANY, wxDefaultPosition, wxSize(clientSizeWidth, clientSizeLength), wxBORDER_SIMPLE);
	
	wxButton *buttonNewFile = new wxButton(initialStartPanel, buttonID::ID_buttonNew, "New", wxPoint(10, 10), wxSize(100, 100));
	wxButton *buttonOpenFile = new wxButton(initialStartPanel, buttonID::ID_buttonOpen, "Open", wxPoint(10, 100 + (260 - 220)), wxSize(100, 100));

	controller.updateOmniFEMState(systemState::initialStartUp);
}



void OmniFEMMainFrame::createTopToolBar()
{
    wxStandardPaths path = wxStandardPaths::Get();
	wxImage::AddHandler(new wxPNGHandler);
	std::string resourcesDirectory = path.GetAppDocumentsDir().ToStdString() + std::string("/GitHub/Omni-FEM/src/UI/MainFrame/resources/");// equilivant to ~ in command line. This is for the path for the source code of the resources
    
    mainFrameToolBar->Create(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_TOP | wxNO_BORDER);

	/* This section will need to load the images into memory */
	wxImage saveImage(resourcesDirectory + "save.png", wxBITMAP_TYPE_PNG);
	wxImage openImage(resourcesDirectory + "Open.png", wxBITMAP_TYPE_PNG);
	wxImage newFileImage(resourcesDirectory + "new_file.png", wxBITMAP_TYPE_PNG);
	
	/* This section will convert the images into bitmaps */
	wxBitmap saveBitmap(saveImage);
	wxBitmap openImageBitmap(openImage);
	wxBitmap newFileBitmap(newFileImage);
	
	/* This section will add the tool to the toolbar */
	mainFrameToolBar->AddTool(toolbarID::ID_ToolBarNew, newFileBitmap, "New File");
	mainFrameToolBar->AddTool(toolbarID::ID_ToolBarOpen, openImageBitmap, "Open");
	mainFrameToolBar->AddTool(toolbarID::ID_ToolBarSave, saveBitmap, "Save");
	
	/* Enable the tooolbar and associate it with the main frame */
	mainFrameToolBar->Realize();
	this->SetToolBar(mainFrameToolBar);
}



void OmniFEMMainFrame::OnExit(wxCommandEvent &event)
{
    Close(true);
}



void OmniFEMMainFrame::createDimensionClient()
{
	systemState currentState = controller.getOmniFEMState();
	
	if(currentState == systemState::problemChooseing)
		problemSelectPanel->Destroy();
	else if(currentState == systemState::initialStartUp)
		initialStartPanel->Destroy();
	else if(currentState == systemState::problemDefining)
	{
		//problemDefiningPanel->Destroy();
//		this->SetSize(minSize);
//this
//this->SetSize(500, 500);
		enableToolMenuBar(false);
//		this->SetMinSize(minSize);
//		this->SetMaxSize(minSize);
	}
		
    dimSelectPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(clientSizeWidth, clientSizeLength), wxBORDER_SIMPLE);
	
    wxButton *TwoDimButton = new wxButton(dimSelectPanel, buttonID::ID_buttonTwoDim, "2-D", wxPoint(10, 50), wxSize(50, 50));
	wxButton *backButton = new wxButton(dimSelectPanel, buttonID::ID_buttonBack, "Back", wxPoint(5, clientSizeLength - 25 - 5), wxSize(100, 25));
	
	wxStaticText *text = new wxStaticText(dimSelectPanel, wxID_ANY, "Choose Spatial Dimension:", wxPoint(5, 5));
	
	
	controller.updateOmniFEMState(systemState::dimensionChoosing);
}



void OmniFEMMainFrame::createProblemChoosingClient()
{
	dimSelectPanel->Destroy();
	
	problemSelectPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(clientSizeWidth, clientSizeLength), wxBORDER_SIMPLE);
	
	wxButton *backButton = new wxButton(problemSelectPanel, buttonID::ID_buttonBack, "Back", wxPoint(5, clientSizeLength - 25 - 5), wxSize(100, 25));
	wxButton *finishButton = new wxButton(problemSelectPanel, buttonID::ID_buttonFinish, "Finish", wxPoint(125, clientSizeLength - 25 - 5), wxSize(100, 25));
	wxStaticText *text = new wxStaticText(problemSelectPanel, wxID_ANY, "Select Physics Problem:", wxPoint(5, 5));
	
	wxListBox *physicsProblems = new wxListBox(problemSelectPanel, comboListBoxID::ID_physicsProblems, wxPoint(5, 50), wxDefaultSize, arrayPhysicsProblem, wxLB_SINGLE); 
	physicsProblems->SetSelection(0);
	
	controller.updateOmniFEMState(systemState::problemChooseing);
}



void OmniFEMMainFrame::createModelDefiningClient()
{
	systemState currentState = controller.getOmniFEMState();
	
	
	if(currentState == systemState::initialStartUp)
		initialStartPanel->Destroy();
	else if(currentState == systemState::dimensionChoosing)
		dimSelectPanel->Destroy();
	else if(currentState == systemState::problemChooseing)
		problemSelectPanel->Destroy();
		
	enableToolMenuBar(true);
	
	this->SetMaxSize(wxSize(-1, -1));
	this->SetSize(960, 544);
	this->SetMinSize(wxSize(960, 544));
	
	
	wxSize modelBuilderPanelSize = wxSize((int)((double)0.17 * (double)clientSizeWidth - (double)20), (int)((double)0.66 * (double)clientSizeLength));
	
	/* This section is creating the general layout of the panel using he sizers in wxWidgets */
	modelBuilderTreePanel = new wxPanel(this, panelID::ID_modelBuilderTree, wxDefaultPosition, modelBuilderPanelSize, wxBORDER_SIMPLE | wxVSCROLL | wxHSCROLL);
	groupOneSizer->Add(modelBuilderTreePanel, 1, wxALIGN_LEFT | wxALL | wxEXPAND, controller.getBorderSize());

	//geometryBuilderPanel = new wxPanel(this, panelID::ID_geometryBuilder, wxDefaultPosition, wxSize((int)((double)0.66 * (double)clientSizeWidth), (int)((double)0.66 * (double)clientSizeLength)), wxBORDER_SIMPLE);
	twoDimGeometryEditor = new geometryEditor2DPresentation(this, wxDefaultPosition, wxSize((int)((double)0.66 * (double)clientSizeWidth), (int)((double)0.66 * (double)clientSizeLength)));
	
	groupOneSizer->Add(twoDimGeometryEditor, 3, wxALIGN_CENTER | wxALL | wxEXPAND, controller.getBorderSize());// THe middle frame will get the highest propoty when sizing becuase this is what the user will use most often
	
	settingsPanel = new wxPanel(this, panelID::ID_settings, wxDefaultPosition, wxSize((int)((double)0.17 * (double)clientSizeWidth - (double)20), (int)((double)0.66 * (double)clientSizeLength)), wxBORDER_SIMPLE);
	groupOneSizer->Add(settingsPanel, 1, wxALIGN_RIGHT | wxALL | wxEXPAND, controller.getBorderSize());
	
	vertBoxSizer->Add(groupOneSizer, 1, wxALL | wxEXPAND, controller.getBorderSize());
	
	statusInfoPanel = new wxPanel(this, panelID::ID_status, wxDefaultPosition, wxSize(clientSizeWidth - 20, clientSizeLength - (int)((double)0.66 * (double)clientSizeLength + (double)20) - 10), wxBORDER_SIMPLE);
	vertBoxSizer->Add(statusInfoPanel, 0, wxEXPAND | wxALL, controller.getBorderSize());
	
	
	/* This section is populating the layout with everything that is needed for the user */
	/*********************************
	* Tree Ctrl (Hiearchary Listbox) *
	**********************************/
	
	modelbuilderTreeCtrl = new wxTreeCtrl(modelBuilderTreePanel, wxID_ANY, wxDefaultPosition, modelBuilderPanelSize - wxSize(2, 0), wxTR_TWIST_BUTTONS | wxTR_NO_LINES | wxTR_FULL_ROW_HIGHLIGHT | wxTR_SINGLE | wxTR_HAS_BUTTONS);// The -2 appears in the size in order to give some extra room for hte scroll bars
	
	controller.setRootTreeIDAbstraction(modelbuilderTreeCtrl->AddRoot(controller.getWorkspaceNameAbstraction()));// This is the highest level
	
	controller.setAbstractProblemID(modelbuilderTreeCtrl->AppendItem(controller.getRootTreeIDAbstraction(), controller.getAbstractSimName()));// This will be for the project
	
	controller.setAbstractGeometryID(modelbuilderTreeCtrl->AppendItem(controller.getAbstractProblemID(), "Geometry"));
	controller.setAbstractMaterialsID(modelbuilderTreeCtrl->AppendItem(controller.getAbstractProblemID(), "Materials"));
	controller.setAbstractMeshID(modelbuilderTreeCtrl->AppendItem(controller.getAbstractProblemID(), "Mesh"));
	
	modelbuilderTreeCtrl->ExpandAll();
	
	/*******************
	* Geometry Builder *
	********************/
//	wxGLCanvas *test = new wxGLCanvas(twoDimDeometryEditor);
//	twoDimGeometryEditor->createOpenGLCanvas();
//	twoDimGeometryEditor->render();
	this->SetSizer(vertBoxSizer);
	this->Layout();
	
	controller.updateOmniFEMState(systemState::problemDefining);
	
}

	/********************
	 * Event Procedures *
	 ********************/
	 
void OmniFEMMainFrame::physicsProblemComboBox(wxCommandEvent &event)
{
	int physicsSelection = event.GetSelection();
	
	if(physicsSelection == 0)
		controller.setAbstractProblemPhysics(physicProblems::electrostatics);
	else if(physicsSelection == 1)
		controller.setAbstractProblemPhysics(physicProblems::magnetics);
}


void OmniFEMMainFrame::onResize(wxSizeEvent &event)
{
	systemState currentState = controller.getOmniFEMState();
	this->GetClientSize(&clientSizeWidth, &clientSizeLength);// This will update the client size
	
	if(currentState == systemState::initialStartUp || currentState == systemState::dimensionChoosing || currentState == systemState::problemChooseing)
	{
		return;
	}
	else if(currentState == systemState::problemDefining)
	{
		// geometryBuilderPanel = new wxPanel(this, panelID::ID_geometryBuilder, wxPoint((int)((double)0.17 * (double)clientSizeWidth), 10), wxSize((int)((double)0.66 * (double)clientSizeWidth), (int)((double)0.66 * (double)clientSizeLength)), wxBORDER_SIMPLE);
		// settingsPanel = new wxPanel(this, panelID::ID_settings, wxPoint((int)((double)0.82 * (double)clientSizeWidth + (double)20), 10), wxSize((int)((double)0.17 * (double)clientSizeWidth - (double)20), (int)((double)0.66 * (double)clientSizeLength)), wxBORDER_SIMPLE);
		// modelBuilderTreePanel = new wxPanel(this, panelID::ID_modelBuilderTree, wxPoint(10, 10), wxSize((int)((double)0.17 * (double)clientSizeWidth - (double)20), (int)((double)0.66 * (double)clientSizeLength)), wxBORDER_SIMPLE);
		// statusInfoPanel = new wxPanel(this, panelID::ID_status, wxPoint(10, (int)((double)0.66 * (double)clientSizeLength + (double)20)), wxSize(clientSizeWidth - 20, clientSizeLength - (int)((double)0.66 * (double)clientSizeLength + (double)20) - 10), wxBORDER_SIMPLE);
		this->Layout();
		
		wxSize newTreeCtrlDim = modelBuilderTreePanel->GetSize() - wxSize(2, 0);
		modelbuilderTreeCtrl->SetSize(newTreeCtrlDim);
	}
}




	/**************
	* Event Table *
	***************/



wxBEGIN_EVENT_TABLE(OmniFEMMainFrame, wxFrame)
	/**********************
	* Menu event Handling *
	***********************/
	
    /* This section is for the file menu */
    EVT_MENU(menubarID::ID_menubarNew,   OmniFEMMainFrame::onNewFile)
    EVT_MENU(menubarID::ID_menubarSave, OmniFEMMainFrame::OnSave)
    EVT_MENU(menubarID::ID_menubarSaveAs, OmniFEMMainFrame::onSaveAs)
	EVT_MENU(menubarID::ID_menubarOpen, OmniFEMMainFrame::onOpenFile)
    
    /* This section is for the Edit menu */
	EVT_MENU(menubarID::ID_menubarLUASCRIPT, OmniFEMMainFrame::onLuaRun)
    EVT_MENU(menubarID::ID_menubarPreferences, OmniFEMMainFrame::onPreferences)
	
	/* This section is for the View menu */
	EVT_MENU(menubarID::ID_menubarViewResults, OmniFEMMainFrame::onViewResults)
 /*   EVT_MENU(menubarID::ID_menubarDispBlockLabels, OmniFEMMainFrame::onDispGrid)
    EVT_MENU(menubarID::ID_menubarDispStatusBar, OmniFEMMainFrame::onSnapGrid)
    EVT_MENU(menubarID::ID_menubarDispLuaConsole, OmniFEMMainFrame::onSetGrid)*/
	
	/* This section is for the Problem menu */
	EVT_MENU(menubarID::ID_menubarPrecision, OmniFEMMainFrame::onPrecision)
	
    /* This section is for the Grid menu */
/*    EVT_MENU(menubarID::ID_menubarShowGrid, OmniFEMMainFrame::onDispGrid)
    EVT_MENU(menubarID::ID_menubarSnapGrid, OmniFEMMainFrame::onSnapGrid)
    EVT_MENU(menubarID::ID_menubarSetGrid, OmniFEMMainFrame::onSetGrid) */
    
    /* This section is for the Properties menu */
    EVT_MENU(menubarID::ID_menubarMaterials, OmniFEMMainFrame::onMaterials)
    EVT_MENU(menubarID::ID_menubarBoundary, OmniFEMMainFrame::onBoundary)
    EVT_MENU(menubarID::ID_menubarPoint, OmniFEMMainFrame::onPointProperty)
    EVT_MENU(menubarID::ID_menubarCircuitsCond, OmniFEMMainFrame::onCircuitsConductor)
    EVT_MENU(menubarID::ID_menubarMatLibrary, OmniFEMMainFrame::onMatLibrary)
    
	/*This section is for the mesh menu */
    EVT_MENU(menubarID::ID_menubarCreateMesh, OmniFEMMainFrame::onCreateMesh)
	EVT_MENU(menubarID::ID_menubarShowMesh, OmniFEMMainFrame::onShowMesh)
	EVT_MENU(menubarID::ID_menubarDeleteMesh, OmniFEMMainFrame::onDeleteMesh)
	
    /* This section is for the Help menu */
	EVT_MENU(menubarID::ID_menubarManual, OmniFEMMainFrame::onManual)
    EVT_MENU(menubarID::ID_menubarLicense, OmniFEMMainFrame::onLicense)
    EVT_MENU(wxID_ABOUT, OmniFEMMainFrame::OnAbout)
    
    /* Everything Else */
    EVT_MENU(wxID_EXIT,  OmniFEMMainFrame::OnExit)
	EVT_SIZE(OmniFEMMainFrame::onResize)
	
	/************************
	* Button Event Handling *
	*************************/
	
	EVT_BUTTON(buttonID::ID_buttonNew, OmniFEMMainFrame::onNewFile)
	EVT_BUTTON(buttonID::ID_buttonOpen, OmniFEMMainFrame::onOpenFile)
    EVT_BUTTON(buttonID::ID_buttonTwoDim, OmniFEMMainFrame::onTwoDimButton)
	EVT_BUTTON(buttonID::ID_buttonBack, OmniFEMMainFrame::onBackButton)
	EVT_BUTTON(buttonID::ID_buttonFinish, OmniFEMMainFrame::onFinishButton)
	
	/*************************
	* ToolBar Event Handling *
	**************************/	
	
	EVT_TOOL(toolbarID::ID_ToolBarNew, OmniFEMMainFrame::onNewFile)
	EVT_TOOL(toolbarID::ID_ToolBarOpen, OmniFEMMainFrame::onOpenFile)
	EVT_TOOL(toolbarID::ID_ToolBarSave, OmniFEMMainFrame::OnSave)
	
	/********
	* Other *
	*********/

	EVT_LISTBOX(comboListBoxID::ID_physicsProblems, OmniFEMMainFrame::physicsProblemComboBox)
    
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(OmniFEMApp);// This is where int main is located inside of