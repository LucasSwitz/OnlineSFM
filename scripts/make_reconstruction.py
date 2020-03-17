import grpc
from server_pb2 import ImageData, ImageMetaData, NewReconstructionRequest
from server_pb2_grpc import ReconstructionServiceStub

if __name__ == "__main__":
        channel = grpc.insecure_channel('localhost:9999')
        client = ReconstructionServiceStub(channel)
        req = NewReconstructionRequest()
        print(client.NewReconstruction(req))
