import grpc
from server_pb2 import ImageData, ImageMetaData, UploadImageRequest, ReconstructRequest, GetOBJRequest, NewReconstructionRequest
from server_pb2_grpc import ReconstructionServiceStub
import sys
import random
import string
from glob import glob
import os

CHUNK_SIZE = 100*1000

if __name__ == "__main__":
        dir_path = sys.argv[1]
        channel = grpc.insecure_channel('localhost:9999')
        client = ReconstructionServiceStub(channel)

        print("Requesting new reconstruction...")
        new_reconstruction_resp = client.NewReconstruction(NewReconstructionRequest())
        reconstruction_id = new_reconstruction_resp.reconstruction_id
        print(f"Created new reconstruction {reconstruction_id}")

        files = glob(dir_path+"/*")
        
        print(f"Uploading dataset from {dir_path}")
        for image_path in files:
            with open(image_path, "rb") as image:
                meta = ImageMetaData(reconstruction=reconstruction_id, format=os.path.splitext(image_path)[1][1:])
                image_bytes = image.read()
                chunked = []
                for i in range(0, len(image_bytes), CHUNK_SIZE):
                    chunked.append(UploadImageRequest(reconstruction_id=reconstruction_id, 
                                                    image = ImageData(metadata=meta, 
                                                                        data=image_bytes[i:i+CHUNK_SIZE])))
                client.UploadImage(iter(chunked))

        print(f"Requesting reconstruction of {reconstruction_id}")
        reconstruct_response = client.Reconstruct(ReconstructRequest(reconstruction_id=reconstruction_id))
        obj_meta = None
        if reconstruct_response.success:
            print("Getting reconstructed OBJ...")
            with open(f"{reconstruction_id}.obj", "wb+") as output_obj, \
                open("texture.tmp", "wb+") as output_texture, \
                open("mtl.tmp", "wb+") as output_mtl:
                for get_obj_response in client.GetOBJ(GetOBJRequest(reconstruction_id=reconstruction_id)):
                    obj_meta = get_obj_response.obj.metadata
                    output_obj.write(get_obj_response.obj.obj_data)
                    output_mtl.write(get_obj_response.obj.mtl_data)
                    output_texture.write(get_obj_response.obj.texture_data)
                os.rename("texture.tmp", os.path.basename(obj_meta.texture_path))
                os.rename("mtl.tmp", os.path.basename(obj_meta.mtl_path))
                print(f"Wrote OBJ to {reconstruction_id}.obj")
            os.execv("/usr/bin/meshlab", ['/usr/bin/meshlab', f"{reconstruction_id}.obj"])
        else:
            print("Reconstruction Failed")
                        
