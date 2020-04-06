
from client import OnlineSFMClient
import sys
if __name__ == "__main__":
        server_addr = sys.argv[1]
        image_path = sys.argv[2]
        client = OnlineSFMClient(server_addr)
        reconstruction = client.make_reconstruction()
        reconstruction.upload_image(image_path)