import grpc
from server_pb2 import ImageData, GetSparseRequest, SparsePointCloudData, ImageMetaData, UploadImageRequest, ReconstructRequest, GetOBJRequest, NewReconstructionRequest, SessionUploadImageRequest, StartSessionRequest
from server_pb2_grpc import ReconstructionServiceStub
import sys
import random
import string
from glob import glob
import os
from time import sleep
import random
from threading import Thread
from plyfile import PlyData, PlyElement  
import io
from mayavi import mlab
from tvtk.tools import visual

CHUNK_SIZE = 100*1000

def do_upload(client, reconstruction_id, session_id, dataset_path):
    print(f"Uploading dataset from {dir_path}")
    files = glob(dataset_path+"/*")
    for image_path in files:
        sleep_time = random.uniform(1.0, 5.0)
        print(f"Uploading next image in {sleep_time} seconds")
        sleep(sleep_time)
        with open(image_path, "rb") as image:
            meta = ImageMetaData(reconstruction=reconstruction_id, format=os.path.splitext(image_path)[1][1:])
            image_bytes = image.read()
            chunked = []
            for i in range(0, len(image_bytes), CHUNK_SIZE):
                chunked.append(SessionUploadImageRequest(session_id= session_id, 
                                                            upload_image=UploadImageRequest(reconstruction_id=reconstruction_id, 
                                                                                            image = ImageData(metadata=meta, 
                                                                                                            data=image_bytes[i:i+CHUNK_SIZE]))))
            client.SessionUploadImage(iter(chunked))

if __name__ == "__main__":
        dir_path = sys.argv[1]
        channel = grpc.insecure_channel('localhost:9999')
        client = ReconstructionServiceStub(channel)

        print("Requesting new reconstruction...")
        new_reconstruction_resp = client.NewReconstruction(NewReconstructionRequest())
        reconstruction_id = new_reconstruction_resp.reconstruction_id
        print(f"Created new reconstruction {reconstruction_id}")
        print(f"Starting a new session for reconstruction {reconstruction_id}")
        session_id = client.StartSession(StartSessionRequest(reconstruction_id=reconstruction_id)).session_id
        print(f"Started session {session_id}")

        mlab.figure(bgcolor=(0, 0, 0))
        Thread(target=do_upload, args=[client, reconstruction_id, session_id, dir_path]).start()
        '''@mlab.show
        @mlab.animate
        def anim(delay=3000, ui=False):
            """Animate the b1 box."""
            while 1:
                sparse_response = client.GetSparse(GetSparseRequest(reconstruction_id=reconstruction_id))
                sparse_final = SparsePointCloudData()
                for sparse in sparse_response:
                    sparse_final.metadata.CopyFrom(sparse.sparse.metadata)
                    sparse_final.data += sparse.sparse.data
                if len(sparse_final.data):
                    plydata = PlyData.read(io.BytesIO(sparse_final.data))
                    vertex = plydata['vertex']
                    (x, y, z) = (vertex[t] for t in ('x', 'y', 'z'))
                    mlab.points3d(x, y, z, color=(1, 1, 1), mode='point')
                yield'''
        while(1):
            continue        
