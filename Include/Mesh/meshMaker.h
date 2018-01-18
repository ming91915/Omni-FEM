#ifndef MESH_MAKER_H_
#define MESH_MAKER_H_

#include <vector>
#include <algorithm>

#include <UI/geometryShapes.h>
#include <UI/ModelDefinition/ModelDefinition.h>

#include <common/plfcolony.h>
#include <common/Vector.h>
#include <common/enums.h>

#include <common/MeshSettings.h>
#include <common/ClosedPath.h>
#include <common/ProblemDefinition.h>

#include <common/GeometryProperties/BlockProperty.h>

#include <Mesh/Gmsh.h>
#include <Mesh/Context.h>
#include <Mesh/GVertex.h>
#include <Mesh/GEdge.h>
#include <Mesh/GFace.h>
#include <Mesh/GModel.h>

#include <Mesh/SBoundingBox3d.h>


/**
 * @class meshMaker
 * @author phillip
 * @date 21/09/17
 * @file meshMaker.h
 * @brief 	The purpose of this class is to handle the creation of the 2D Mesh from the geometry
 * 			that the user has created. This class contains the function needed in order to
 * 			determine the closed contours within the geometry. This class will interface with
 * 			the GMSH source in order to create the mesh.
 */
class meshMaker
{
private:
	std::vector<closedPath> p_closedContourPaths;
	
	std::vector<GVertex*> p_vertexModelList;
	
	//! Pointer to the global nodal list
	plf::colony<node> *p_nodeList;
	
	//! Pointer to the global block label list
	plf::colony<blockLabel> *p_blockLabelList;
	
	//! Pointer to the global line list
	plf::colony<edgeLineShape> *p_lineList;
	
	//! Pointer to the global arc list
	plf::colony<arcShape> *p_arcList;
	
	meshSettings *p_settings;
	
	wxString p_simulationName;
	
	wxString p_folderPath;
	
	GModel *p_meshModel;
	
	//! This is the total number of lines within the geometry. This is the number of lines and arcs
	unsigned long p_numberofLines = 0;
	
	//! This data type is incremented when the program visists a new line
	unsigned long p_numberVisited = 0;
	
	unsigned int p_blockLabelsUsed = 0;
	
	/**
	 * @brief 	This function is called in order to find all of the closed contours connected to 
	 * 			a common edge. The function will then determine what the geometry pieces are within
	 * 			the closed contour. This function is modeled off of the depth-first search algorthim
	 * @return Returns a closed path if avaiable
	 */
	closedPath findContour(edgeLineShape *startingEdge = nullptr, rectangleShape *point = nullptr);
	
	/**
	 * @brief This function is called in order to create the mesh for hte geometry model.
	 * 			This function needs to be called after the program sorts through and determines
	 * 			the geometry pieces within the model
	 */
	void createMesh();
	
	/**
	 * @brief This function will take a edge segment and find all of the banches that are connected to that edge and return as a vector
	 * @param currentSegment The line segment that the program will find the connected branches to
	 * @param pathVector
	 * @return Returns a vector containing all of the connected branches to the segment
	 */
	std::vector<edgeLineShape*> getConnectedPaths(std::vector<edgeLineShape*>::reference currentSegment, std::vector<edgeLineShape*> *pathVector);
	
	/**
	 * @brief 	This function will test if two paths share a common edge. It does not determine which edge is common. But
	 * 			it tests if there is a common edge. The function does not determine how many common edges. Just that if
	 * 			a common edge exists.
	 * @param path1 The first path
	 * @param path2 The second path
	 * @return Returns true if a common edge exists. Otherwise, returns false
	 */
	bool shareCommonEdge(std::vector<edgeLineShape> path1, std::vector<edgeLineShape> path2);
	
	double calculateShortestDistance(blockLabel selectedLabel, edgeLineShape segment);
	
	void createGMSHGeometryOld(std::vector<closedPath> *pathContour = nullptr);
	
	void createGMSHGeometry(std::vector<closedPath> *pathContour = nullptr);
	
	/**
	 * @brief 	The purpose of this function to to check if a specfied rectangle shape lies within the path.
	 * 			The function accomplishes this through the use of a modified version of the winding number method.
	 * 			In this method, the algorithm calculates the number of times that the point wraps around the 
	 * 			closed path. The current version of this algorithm does not use arc sin or cos. But instead checks to see
	 * 			if the point is to the left or right of an edge within the closed path. If teh number of times that the point
	 * 			is to the left of the edges is greater then the number of rights, then this means that the point lies withing the
	 * 			closed path.
	 * @param label The rectangle shape that the algorithm will be checking to see if it lies within the closed path
	 * @param path The closed path which will be checked.
	 * @return Returns true if the rectangle shape lies within the closed path. Otherwise, returns false.
	 */
	bool checkPointInContour(wxRealPoint point, closedPath &path);
	
	bool checkPointInContour(rectangleShape &point, closedPath &path)
	{
		return checkPointInContour(point.getCenter(), path);
	}
	
	bool checkPointInContour(rectangleShape *point, closedPath *path)
	{
		return checkPointInContour(point->getCenter(), *path);
	}
	
	void holeDetection();
	
	void assignBlockLabel(std::vector<closedPath> *pathContour = nullptr);
public:
	
	meshMaker(problemDefinition &definition, modelDefinition *model)
	{
		p_meshModel = model->getMeshModel();
		
		p_nodeList = model->getModelNodeList();
		p_blockLabelList = model->getModelBlockList();
		p_lineList = model->getModelLineList();
		p_arcList = model->getModelArcList();
		
		p_numberofLines = p_lineList->size() + p_arcList->size();
		
		p_settings = definition.getMeshSettingsPointer();
		p_simulationName = definition.getName();
		p_folderPath = definition.getSaveFilePath();
	}
	
	/**
	 * @brief Function that is used to check if the system has found all geometry pieces 
	 * @return Returns true when the class has finished the procedure. Otherwise returns false
	 */
	bool geometryIsFound()
	{
		if(p_numberVisited == p_numberofLines)
			return true;
		else
			return false;
	}
	
	void mesh();
	
	~meshMaker()
	{
		//free(p_nodeList);
		//delete p_blockLabelList;
		//delete p_lineList;
		//delete p_arcList;
	}
	
	
};


#endif