import grpc
from server_pb2 import ImageData, ImageMetaData, UploadImageRequest
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

        dir_path = sys.argv[1]
        reconstruction_id = str(sys.argv[2])

        files = glob(dir_path+"/*")
        for image_path in files:
            with open(image_path, "rb") as image:
                meta = ImageMetaData(reconstruction=reconstruction_id, format=os.path.splitext(image_path)[1][1:])
                image_bytes = image.read()
                chunked = []
                for i in range(0, len(image_bytes), CHUNK_SIZE):
                    chunked.append(UploadImageRequest(reconstruction_id=reconstruction_id, 
                                                    image = ImageData(metadata=meta, 
                                                                        data=image_bytes[i:i+CHUNK_SIZE])))
                print(f"Split image into {len(chunked)} chunks")
                client.UploadImage(iter(chunked))