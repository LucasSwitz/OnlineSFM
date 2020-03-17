import grpc
from server_pb2 import DeleteReconstructionRequest
from server_pb2_grpc import ReconstructionServiceStub
import sys

if __name__ == "__main__":
        channel = grpc.insecure_channel('localhost:9999')
        client = ReconstructionServiceStub(channel)
        req = DeleteReconstructionRequest(id=sys.argv[1])
        client.DeleteReconstruction(req)