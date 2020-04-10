
from client import OnlineSFMClient
import sys
if __name__ == "__main__":
        server_addr = sys.argv[1]
        dir_path = sys.argv[2]
        client = OnlineSFMClient(server_addr)
        reconstruction = client.make_reconstruction()
        reconstruction.set_agent_configuration("openmvg", {"num_threads": 10})
        reconstruction.upload_directory(dir_path)
        reconstruction.do_sparse_reconstruction()