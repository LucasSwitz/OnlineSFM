
/*#include "openmvs_ext.h"

using namespace MVS;

bool DistributedScene::LoadFromeRemoteScene(const std::string& reconstruction_id){
	//TODO: Get the scene data from remote
	Release();
    std::istringstream ss;

	char szHeader[4];
	ss.read(szHeader, 4);
	uint32_t nVer;
	ss.read((char*)&nVer, sizeof(uint32_t));
	uint32_t nType;
	ss.read((char*)&nType, sizeof(uint32_t));
	uint64_t nReserved;
	ss.read((char*)&nReserved, sizeof(uint64_t));
	// serialize in the current state
	if (!SerializeLoad(*this, ss, (ARCHIVE_TYPE)nType))
		return false;
	nCalibratedImages = 0;
	size_t nTotalPixels(0);
	FOREACH(ID, images) {
		Image& imageData = images[ID];
		if (imageData.poseID == NO_ID)
			continue;
		imageData.UpdateCamera(platforms);
		++nCalibratedImages;
		nTotalPixels += imageData.width * imageData.height;
	}
	DEBUG_EXTRA("Scene loaded (%s):\n"
				"\t%u images (%u calibrated) with a total of %.2f MPixels (%.2f MPixels/image)\n"
				"\t%u points, %u vertices, %u faces",
				TD_TIMER_GET_FMT().c_str(),
				images.GetSize(), nCalibratedImages, (double)nTotalPixels/(1024.0*1024.0), (double)nTotalPixels/(1024.0*1024.0*nCalibratedImages),
				pointcloud.points.GetSize(), mesh.vertices.GetSize(), mesh.faces.GetSize());
	return true;
}

bool DistributedScene::DenseReconstruction(int nFusionMode=0){

}



bool DistributedScene::ComputeDepthMap(DenseDepthMapData& data){

{
	IIndexArr imagesMap;
	{
		data.images.Reserve(1);
		imagesMap.Resize(1);
        Image& imageData = //TODO Load image data
        if (!imageData.IsValid()){
            return
        }
        imagesMap[idxImage] = data.images.GetSize();
        data.images.Insert(0);
        const unsigned nMaxResolution(imageData.RecomputeMaxResolution(OPTDENSE::nResolutionLevel, 
                                                                        OPTDENSE::nMinResolution, 
                                                                        OPTDENSE::nMaxResolution));
        if (!imageData.ReloadImage(nMaxResolution)) {
            return false;
        }
        imageData.UpdateCamera(platforms);	
		if (data.images.IsEmpty()) {
			return false;
		}
	
	{
		IIndexArr invalidIDs;
		const IIndex idxImage(data.images[0]);
		ASSERT(imagesMap[idxImage] != NO_ID);
		DepthData& depthData(data.depthMaps.arrDepthData[idxImage]);
		if (!data.depthMaps.SelectViews(depthData)) {
				invalidIDs.InsertSort(0);
        }
		RFOREACH(i, invalidIDs) {
			const IIndex idx(invalidIDs[i]);
			imagesMap[data.images.Last()] = idx;
			imagesMap[data.images[idx]] = NO_ID;
			data.images.RemoveAt(idx);
		}
		if (OPTDENSE::nNumViews == 1 && !data.depthMaps.SelectViews(data.images, imagesMap, data.neighborsMap)) {
			VERBOSE("error: no valid images to be dense reconstructed");
			return false;
		}
		ASSERT(!data.images.IsEmpty());
		VERBOSE("Selecting images for dense reconstruction completed: %d images (%s)", data.images.GetSize(), TD_TIMER_GET_FMT().c_str());
	}
	}
	data.idxImage = 0;
	ASSERT(data.events.IsEmpty());
	data.events.AddEvent(new EVTProcessImage(0));
	data.progress = new Util::Progress("Estimated depth-maps", data.images.GetSize());
	DenseReconstructionEstimate((void*)&data);
	if (!data.events.IsEmpty())
		return false;
	data.progress.Release();
	if ((OPTDENSE::nOptimize & OPTDENSE::ADJUST_FILTER) != 0) {
		data.sem.Clear();
		data.idxImage = data.images.GetSize();
		ASSERT(data.events.IsEmpty());
        auto i =  data.images[0];
		data.events.AddEvent(new EVTFilterDepthMap(i));
		data.progress = new Util::Progress("Filtered depth-maps", data.images.GetSize());
		DenseReconstructionFilter((void*)&data);
		if (!data.events.IsEmpty())
			return false;
		data.progress.Release();
	}
	return true;
}

bool DistributedScene::ComputeDepthMaps(DenseDepthMapData& data)
{
    
}*/