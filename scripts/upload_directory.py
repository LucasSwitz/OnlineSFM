import grpc
from server_pb2 import ImageData, ImageMetaData, ReconstructionUploadImageBatchRequest
from server_pb2_grpc import ReconstructionServiceStub
import sys
import random
import string
from glob import glob
import os

CHUNK_SIZE = 100*1000

if __name__ == "__main__":
        channel = grpc.insecure_channel('localhost:9999')
        client = ReconstructionServiceStub(channel)

        reconstruction_id = str(sys.argv[1])
        dir_path = sys.argv[2]

        files = glob(dir_path+"/*")
        img_idx = 0
        chunked = []
        for image_path in files:
            with open(image_path, "rb") as image:
                meta = ImageMetaData(reconstruction=reconstruction_id, format=os.path.splitext(image_path)[1][1:])
                image_bytes = image.read()
                for i in range(0, len(image_bytes), CHUNK_SIZE):
                    chunked.append(ReconstructionUploadImageBatchRequest(idx = int(img_idx), 
                                                                         data = ImageData(metadata=meta, 
                                                                                          data=image_bytes[i:i+CHUNK_SIZE])))
                print(f"Split image {image_path} into {len(chunked)} chunks {img_idx}")
            img_idx+=1
        client.ReconstructionUploadImageBatch(iter(chunked))
