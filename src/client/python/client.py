import grpc
from server_pb2 import (ImageData, 
                        ImageMetaData, 
                        ReconstructionUploadImageBatchRequest, 
                        NewReconstructionRequest, 
                        MVSRequest, 
                        SparseReconstructRequest,
                        SetAgentConfigFieldsRequest,
                        SetReconstructionConfigFieldsRequest,
                        GetSparseRequest,
                        SparsePointCloudData,
                        GetReconstructionConfigRequest,
                        GetAgentConfigRequest,
                        ReconstructionUploadImageRequest,
                        ComputeMatchesRequest,
                        GetAllImagesRequest)
from server_pb2_grpc import ReconstructionServiceStub
import sys
import random
import string
import json
from glob import glob
import os
import time

class OnlineSFMSession:
    def __init__(self, client, reconstruction_id):
        self._client = client
        self._reconstrution_id = reconstruction_id

class OnlineSFMReconstruction:
    def __init__(self, client, channel, id):
        self._client = client
        self._channel = channel
        self._id = id

    def upload_image(self, image_path):
        with open(image_path, "rb") as image:
                meta = ImageMetaData(reconstruction=self._id, format=os.path.splitext(image_path)[1][1:])
                image_bytes = image.read()
                chunked = []
                for i in range(0, len(image_bytes), CHUNK_SIZE):
                    chunked.append(ReconstructionUploadImageBatchRequest(idx = 0, 
                                                                         data = ImageData(metadata=meta, 
                                                                                          data=image_bytes[i:i+CHUNK_SIZE])))
                self._client.ReconstructionUploadImageBatch(iter(chunked))

    def upload_directory_batch(self, directory_path):
        files = glob(directory_path+"/*")
        img_idx = 0
        chunked = []
        for image_path in files:
            with open(image_path, "rb") as image:
                meta = ImageMetaData(reconstruction=self._id, format=os.path.splitext(image_path)[1][1:])
                image_bytes = image.read()
                for i in range(0, len(image_bytes), CHUNK_SIZE):
                    chunked.append(ReconstructionUploadImageBatchRequest(idx = int(img_idx), 
                                                                         data = ImageData(metadata=meta, 
                                                                                          data=image_bytes[i:i+CHUNK_SIZE])))
            img_idx+=1
        self._client.ReconstructionUploadImageBatch(iter(chunked))


    def upload_directory(self, directory_path):
        files = glob(directory_path+"/*")
        image_ids = []

        print("Uploading Images")
        for i in range(0, len(files), 10):
            print(f"Uploaded {i} images")
            upload_futures = []
            for image_path in files[i:i+10]:
                chunked = []
                with open(image_path, "rb") as image:
                    meta = ImageMetaData(reconstruction=self._id, format=os.path.splitext(image_path)[1][1:])
                    image_bytes = image.read()
                    for i in range(0, len(image_bytes), CHUNK_SIZE):
                        chunked.append(ReconstructionUploadImageRequest(reconstruction_id = self._id,
                                                                        compute_matches = False,
                                                                        image = ImageData(metadata=meta, 
                                                                                        data=image_bytes[i:i+CHUNK_SIZE])))
                upload_futures.append(self._client.ReconstructionUploadImage.future(iter(chunked), timeout=1000))
            for future in upload_futures:
                image_ids.append(future.result().image_id)

        matches_futures = []
        for image_id in image_ids:
            matches_futures.append(self._client.ComputeMatches.future(ComputeMatchesRequest(reconstruction_id = self._id, 
                                                                                            image_id=image_id),timeout=10000))
        for match_future in matches_futures:
            match_future.result()


    def compute_matches(self, images):
        matches_futures = []
        for i in range(0, len(images), 10):
            for image in images[i:i+10]:
                matches_futures.append(self._client.ComputeMatches.future(ComputeMatchesRequest(reconstruction_id = self._id, 
                                                                                            image_id=image),timeout=10000))
        for match_future in matches_futures:
            match_future.result()
    
    def get_all_image_ids(self):
        return self._client.GetAllImages(GetAllImagesRequest(reconstruction_id=self._id)).images 
        
    def do_sparse_reconstruction(self):
        return self._client.SparseReconstruct(SparseReconstructRequest(reconstruction_id=self._id))

    def get_sparse_reconstruction(self):
        sparse_response = self._client.GetSparse(GetSparseRequest(reconstruction_id=self._id))
        sparse_final = SparsePointCloudData()
        for sparse in sparse_response:
            sparse_final.metadata.CopyFrom(sparse.sparse.metadata)
            sparse_final.data += sparse.sparse.data
        return sparse_final

    def do_mvs(self):
        self._client.MVS(MVSRequest(reconstruction_id=self._id))

    def set_configuration(self, config):
        self._client.SetReconstructionConfigFields(SetReconstructionConfigFieldsRequest(reconstruction_id=self._id, 
                                                                                        config_json=config))

    def set_agent_configuration(self, agent, config):
        self._client.SetAgentConfigFields(SetAgentConfigFieldsRequest(reconstruction_id=self._id, 
                                                                      agent_name=agent, 
                                                                      config_json=json.dumps(config)))

    def get_active_agent_config(self):
        rresp = self._client.GetReconstructionConfig(GetReconstructionConfigRequest(reconstruction_id=self._id))
        config = json.loads(rresp.config_json)
        agent_name = config["reconstruction_agent"]
        return self.get_agent_config(agent_name)

    def get_agent_config(self, agent_name):
        aresp = self._client.GetAgentConfig(GetAgentConfigRequest(reconstruction_id=self._id, agent_name=agent_name))
        return json.loads(aresp.config_json)

    def close(self):
        self._channel.close()

class OnlineSFMClient:
    def __init__(self, server_addr):
        self._channel = grpc.insecure_channel(server_addr)
        self._client = ReconstructionServiceStub(self._channel)
    
    def get_reconstruction(self, id):
        return OnlineSFMReconstruction(self._client, self._channel, id)

    def make_reconstruction(self):
        reconstruction_id = self._client.NewReconstruction(NewReconstructionRequest()).reconstruction_id
        return OnlineSFMReconstruction(self._client, self._channel, reconstruction_id)

    def make_session(self, reconstruction_id):
        return OnlineSFMSession(self._client, reconstruction_id)

CHUNK_SIZE = 100*1000
        
