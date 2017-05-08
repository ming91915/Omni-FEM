#ifndef OMNIFEMFrame_H_
#define OMNIFEMFrame_H_

#include <string.h>
#include <algorithm>

#include <wx/wx.h>
#include <wx/aboutdlg.h>
#include <wx/stdpaths.h>
#include <wx/listbox.h>
#include <wx/sizer.h>
#include <wx/treectrl.h>

#include <UI/GeometryEditor2D.h>
#include <UI/common.h>

#include <UI/MaterialsDialog/MaterialDialog.h>
#include <UI/MaterialsDialog/MaterialsLibrary.h>
#include <UI/BoundaryDialog/BoundaryDialog.h>
#include <UI/NodalProperty/PropertyDialog.h>
#include <UI/ConductorsDialog/ConductorPropSetDialog.h>

#include <UI/PreferencesDialog.h>
#include <UI/ExteriorRegion.h>

#include <UI/EditMenu/MoveCopyDialog.h>
#include <UI/EditMenu/ScalingDialog.h>
#include <UI/EditMenu/MirrorDialog.h>
#include <UI/EditMenu/OpenBoundaryDialog.h>
#include <UI/EditMenu/GlobalPreferencesDialog.h>
#include <UI/EditMenu/SelectGroupDialog.h>

#include <UI/ViewMenu/ZoomWindow.h>
#include <UI/ViewMenu/LuaConsole.h>

#include <UI/GridPreferencesDialog.h>

#include <UI/ModelDefinition/ModelDefinition.h>

#include <common/enums.h>



// For documenting code, see: https://www.stack.nl/~dimitri/doxygen/manual/docblocks.html

class OmniFEMApp : public wxApp
{
	private:
	
		wxSize minSize = wxSize(450, 340);
	
    public:
        virtual bool OnInit();
};






/*! \class OmniFEMMainFrame
 *  \brief The main class for everything
 * 
 *  This class is also the presentation layer of the main frame
 */
class OmniFEMMainFrame : public wxFrame
{
public:
    OmniFEMMainFrame(const wxString &title, const wxPoint &pos);
private:

    /************
	* Variables *
	*************/
	
    modelDefinition *_model;
    
    problemDefinition _problemDefinition;
    
    bool _displayStatusMenu = true;
	
	//! The menu bar for the main window
	wxMenuBar *_menuBar = new wxMenuBar;
	
	//! This would be the file menu in the menu bar
    wxMenu *_menuFile = new wxMenu;
	
	//! This would be the edit menu in the menu bar
    wxMenu *_menuEdit = new wxMenu;
	
	//! This would be the view menu in the menu bar
    wxMenu *_menuView = new wxMenu;
	
	//! This would be the problem menu in the menu bar
    wxMenu *_menuProblem = new wxMenu;
	
    //! The Grid menu in the menu bar
    wxMenu *_menuGrid = new wxMenu;
    
    //! The properties menu in the menu bar
    wxMenu *_menuProperties = new wxMenu;
    
    //! This would be the mesh menu in the menu bar
    wxMenu *_menuMesh = new wxMenu;
    
    //! This is the menu entry for hte analsis
    wxMenu *_analysisMenu = new wxMenu;
    
	//! This would be the help menu in the menu bar
    wxMenu *_menuHelp = new wxMenu;
    
    wxListBox *_physicsProblemsListBox;
    
    systemState _UIState = systemState::ON_START_UP_STATE;


	/***********************************
	* Prototypes for creating the menu *
	************************************/
	
    /* This section is for the File menu */
    void onNewFile(wxCommandEvent &event);
    void OnSave(wxCommandEvent &event);
    void onSaveAs(wxCommandEvent &event);
	void onOpenFile(wxCommandEvent &event);
    
    /* This section is for the Edit menu */
	void onLuaRun(wxCommandEvent &event);
    void onPreferences(wxCommandEvent &event);
    void onCopy(wxCommandEvent &event);
    void onScale(wxCommandEvent &event);
    void onMirror(wxCommandEvent &event);
    void onUndo(wxCommandEvent &event);
    void onDelete(wxCommandEvent &event);
    void onMove(wxCommandEvent &event);
    void onCreateRadius(wxCommandEvent &event);
    void onCreateOpenBoundary(wxCommandEvent &event);
    void onSelectGroup(wxCommandEvent &event);
    void onEditProperty(wxCommandEvent &event);
	
	/* This section is for the View Menu */
    void onZoomIn(wxCommandEvent &event);
    void onZoomOut(wxCommandEvent &event);
    void onZoomWindow(wxCommandEvent &event);
    void onBlockName(wxCommandEvent &event);
    void onOrphans(wxCommandEvent &event);
    void onStatusBar(wxCommandEvent &event);
    void onLua(wxCommandEvent &event);
    
    /* This section is for the Problem Menu */
    void onProblemPreferences(wxCommandEvent &event);
	
    /* This section is for the Grid Menu */
    void onDispGrid(wxCommandEvent &event);
    void onSnapGrid(wxCommandEvent &event);
    void onSetGridPreferences(wxCommandEvent &event);
    
    /* Thos section is for the Properties Menu */
    void onMaterials(wxCommandEvent &event);
    void onBoundary(wxCommandEvent &event);
    void onPointProperty(wxCommandEvent &event);
    void onCircuitsConductor(wxCommandEvent &event);
    void onExteriorRegion(wxCommandEvent &event);
    void onMatLibrary(wxCommandEvent &event);
    
	/* This section is for the Mesh menu */
	void onCreateMesh(wxCommandEvent &event);
	void onShowMesh(wxCommandEvent &event);
	void onDeleteMesh(wxCommandEvent &event);
    
    /* This section is for the Analysis menu */
    void onAnalyze(wxCommandEvent &event);
    void onViewResults(wxCommandEvent &event);
    
	//! Event called to view the manual
    void onManual(wxCommandEvent &event);
	
	//! Event called to view the manual
    void onLicense(wxCommandEvent &event);
	
	//! Event called to view the manual
    void OnAbout(wxCommandEvent &event);
    
	//! Event called to view the manual
    void OnExit(wxCommandEvent &event);
    
    //! Event that is called when the user wants to toggle between node and block label creation
    void onToggleNodeCreation(wxCommandEvent &event)
    {
        if(_UIState == systemState::MODEL_DEFINING)
        {
            _model->setCreateNodeState(!_model->getCreateNodeState());
        }
    }
    
    //! Event that is called when the user wants to toggle between line and arc label creation
    void onToggleLineCreation(wxCommandEvent &event)
    {
        if(_UIState == systemState::MODEL_DEFINING)
        {
            _model->setCreateLinesState(!_model->getCreateLineState());
        }
    }
    
    void onSolveProblem(wxCommandEvent &event)
    {
        return;
    }
	
    void onDisplayResults(wxCommandEvent &event)
    {
        return;
    }
    
    void onGLCanvasMouseMove(wxCommandEvent &event);

 	/*****************************
	* Prototypes for client area *
	******************************/   
	
	//! Function that is called to create the initial client area
	void createInitialStartupClient();
	
	
	//! This function is called when the user is ready to choose their physics simulation
	void createProblemChoosingClient();
	
	
	//! This function is called to create the client for defining the physics problem
	/*
		Defining would be creating the geometry, creating the mesh, determining boundary conditions, etc.
	*/
	void createModelDefiningClient();
	
	
	//! This function is called when the user would like to view the results.
	/*
		When the view wants to view results, this function is called in order to create the client frame for viewing
		the results
	*/
	void createResultsViewingClient();
	
	/*************************
	* Prototypes for toolbar *
	**************************/
	
	//! Function called that will create the toolbar for the main frame
	void createTopToolBar();
	
    /*************************
	* Prototypes for buttons *
	**************************/
	
	//! Function called when the back button is pressed 
	void onBackButton(wxCommandEvent &event);
	
	void onFinishButton(wxCommandEvent &event);
	
    /********************
	* Prototypes Others *
	*********************/	
	
	//! Event called when a resize event occurs
	void onResize(wxSizeEvent &event);	
	
	//! This is a function that will be for the initial state of Omni-FEM. With items on the toolbar greyed out and menus not accessible
	void enableToolMenuBar(bool enable);

    wxDECLARE_EVENT_TABLE();
};












#endif /* OMNIFEM_H_ */
