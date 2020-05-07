
from perf_test import PerfTest, PerfTestRunner
from client import OnlineSFMClient
import sys
import grpc

class ReconstructDirectoryTest(PerfTest):
    def __init__(self, name, server_addr, directory, config = {}):
        super(ReconstructDirectoryTest, self).__init__(name)
        self._client = OnlineSFMClient(server_addr)
        self._directory = directory
        self._config = config
        
    def run(self):
        try:
            #bbd98d7e-857d-4532-9ba8-bd105c4a10e8
            reconstruction = self._client.get_reconstruction("bbd98d7e-857d-4532-9ba8-bd105c4a10e8")
            '''with self.timer("NewReconstruction"):
                reconstruction = self._client.make_reconstruction()
            with self.timer("UploadDirectory"):
                if len(self._config):
                    reconstruction.set_agent_configuration(self._config)
                reconstruction.upload_directory(self._directory)'''
            images = reconstruction.get_all_image_ids()
            reconstruction.compute_matches(images)
            with self.timer("SparseReconstruction"):
                reconstruction.do_sparse_reconstruction()
            sparse = None
            with self.timer("GetSparseReconstruction"):
                sparse = reconstruction.get_sparse_reconstruction()
            self.store("sparse.ply", sparse.data)
            '''with self.timer("MVS"):
                reconstruction.do_mvs()'''
            agent_config = None
            with self.timer("GetAgentConfiguration"):
                agent_config = reconstruction.get_active_agent_config()
            self.report("agent_config", agent_config)
        except grpc.RpcError as rpc_error:
            print("Test failed with rpc_error")
            print(rpc_error)
    
if __name__ == "__main__":
    runner = PerfTestRunner()
    server = sys.argv[1]
    directory = sys.argv[2]
    runner.add_test(ReconstructDirectoryTest("default", server, directory, {}))
    #runner.add_test(ReconstructDirectoryTest("default", server, directory, {"openmvg" : {}}))
    runner.run()