
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string>
#include <iostream>

#include <FCollada.h>
#include <FUtils/FUUri.h>
#include <FCDocument/FCDocument.h>
#include <FCDocument/FCDLibrary.h>
#include <FCDocument/FCDGeometry.h>
#include <FCDocument/FCDGeometrySource.h>
#include <FCDocument/FCDGeometryPolygonsTools.h>
#include <FCDocument/FCDGeometryPolygonsInput.h>
#include <FCDocument/FCDAsset.h>
#include <FCDocument/FCDGeometryMesh.h>
#include <FCDocument/FCDGeometryPolygons.h>
#include <FCDocument/FCDGeometryInstance.h>
#include <FCDocument/FCDSceneNode.h>
#include <FCDocument/FCDImage.h>
#include <FCDocument/FCDMaterial.h>
#include <FCDocument/FCDMaterialInstance.h>
#include <FCDocument/FCDEffect.h>
#include <FCDocument/FCDEffectProfile.h>
#include <FCDocument/FCDEffectStandard.h>
#include <FCDocument/FCDTexture.h>
#include <FCDocument/FCDController.h>
#include <FCDocument/FCDSkinController.h>

/**
 * 
 * @param argc 
 * @param argv[] 
 * @return 
 */
int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		std::cout << "Usage: nelcollada <colladafile>" << std::endl;
		return EXIT_FAILURE;
	}

	std::string filename(argv[1]);
	//std::string file = "bar.dae";//"sphere.dae";

	std::cout << "Initializing FCollada" << std::endl;
	FCollada::Initialize();

	std::cout << "Creating new FCollada top document" << std::endl;
	FCDocument* doc = FCollada::NewTopDocument();

	std::cout << "Attempting to load file " << filename << std::endl;
	if (!FCollada::LoadDocumentFromFile(doc, filename.c_str()) )
	{
		std::cout << "Failed to load file " << filename << ": please check format." << std::endl;
		return EXIT_FAILURE;
	}
	std::cout << "FCollada Document Load returned true" << std::endl;

	if(doc==NULL)
		std::cout<<"Doc is null" << std::endl;

	// log the asset info
	FCDAsset *asset = doc->GetAsset();
	std::cout << "Asset: Title: " << asset->GetTitle() << std::endl;
	std::cout << "Grab the geometry library" << std::endl;
	FCDGeometryLibrary *pGeometryLibrary = doc->GetGeometryLibrary();
	std::cout << "Loading " << pGeometryLibrary->GetEntityCount() << " entities from Collada." << std::endl;
	for (int i=0; i< (int) pGeometryLibrary->GetEntityCount(); i++)
	{
		if (pGeometryLibrary->GetEntity(i)->IsMesh()==true)
		{
			FCDGeometryMesh *pMesh = pGeometryLibrary->GetEntity(i)->GetMesh();
			FCDGeometryPolygonsTools::Triangulate(pMesh);
			printf("found mesh: %lu polygons\n", (long unsigned)pMesh->GetPolygonsCount());
		}
	}
	return EXIT_SUCCESS;
}
