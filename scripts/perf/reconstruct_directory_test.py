
from perf_test import PerfTest, PerfTestRunner
from client import OnlineSFMClient
import sys

class ReconstructDirectoryTest(PerfTest):
    def __init__(self, server_addr, directory):
        super(ReconstructDirectoryTest, self).__init__("Reconstruct Directory")
        self._client = OnlineSFMClient(server_addr)
        self._directory = directory
        
    def run(self):
        reconstruction = None
        with self.timer("NewReconstruction"):
            reconstruction = self._client.make_reconstruction()
        with self.timer("UploadDirectory"):
            reconstruction.set_agent_configuration("openmvg", {"num_threads": 1})
            reconstruction.upload_directory(self._directory)
        with self.timer("SparseReconstruction"):
            reconstruction.do_sparse_reconstruction()
        sparse = None
        with self.timer("GetSparseReconstruction"):
            sparse = reconstruction.get_sparse_reconstruction()
        self.store("sparse.ply", sparse.data)
        agent_config = None
        with self.timer("GetAgentConfiguration"):
            agent_config = reconstruction.get_active_agent_config()
        self.report("agent_config", agent_config)
        print("Closing connection...")
        reconstruction.close()
            

if __name__ == "__main__":
    runner = PerfTestRunner()
    server = sys.argv[1]
    directory = sys.argv[2]
    runner.add_test(ReconstructDirectoryTest(server, directory))
    runner.run()