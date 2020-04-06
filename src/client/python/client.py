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
                        GetAgentConfigRequest)
from server_pb2_grpc import ReconstructionServiceStub
import sys
import random
import string
import json
from glob import glob
import os

class OnlineSFMSession:
    def __init__(self, client, reconstruction_id):
        self._client = client
        self._reconstrution_id = reconstruction_id

class OnlineSFMReconstruction:
    def __init__(self, client, id):
        self._client = client
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

    def upload_directory(self, directory_path):
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
        
    def do_sparse_reconstruction(self):
        self._client.SparseReconstruct(SparseReconstructRequest(reconstruction_id=self._id))

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

class OnlineSFMClient:
    def __init__(self, server_addr):
        self._channel = grpc.insecure_channel(server_addr)
        self._client = ReconstructionServiceStub(self._channel)
    
    def get_reconstruction(self, id):
        pass

    def make_reconstruction(self):
        reconstruction_id = self._client.NewReconstruction(NewReconstructionRequest()).reconstruction_id
        return OnlineSFMReconstruction(self._client, reconstruction_id)

    def make_session(self, reconstruction_id):
        return OnlineSFMSession(self._client, reconstruction_id)

CHUNK_SIZE = 100*1000
        
