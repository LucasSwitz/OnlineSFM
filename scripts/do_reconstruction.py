import grpc
from server_pb2 import ReconstructRequest
from server_pb2_grpc import ReconstructionServiceStub
import sys

if __name__ == "__main__":
        channel = grpc.insecure_channel('localhost:9999')
        client = ReconstructionServiceStub(channel)
        req = ReconstructRequest(reconstruction_id=sys.argv[1])
        client.Reconstruct(req)
