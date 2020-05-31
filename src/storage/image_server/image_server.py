from threading import Lock
from flask import Flask, jsonify, Response
import sys
import json
import mysql.connector
from server_pb2 import ImageMetaData
from flask_cors import CORS


app = Flask(__name__, static_url_path='/static')
CORS(app)

srv = None
bind_address = ""
store_root_path = ""


@app.route('/images/<reconstruction_id>/<image_id>')
def get_image_location(reconstruction_id, image_id):
    location = srv.get_image_location(reconstruction_id, image_id)
    location = ''.join(location.split(store_root_path))
    data = jsonify({"path": f"http://{bind_address}/static{location}"})
    return data


class ImageServer:
    def __init__(self, image_meta_storage):
        self._image_meta_storage = image_meta_storage

    def get_image_location(self, reconstruction_id, image_id):
        meta = self._image_meta_storage.GetImageMeta(
            reconstruction_id, image_id)
        return meta.path


class ImageMetaStorage():
    def GetImageMeta(self, reconstruction_id, image_id):
        raise Exception("Implement GetImageMeta")


GET_IMAGE_META_QUERY = (
    "SELECT * FROM {table} WHERE RECONSTRUCTION_ID = %s and ID = %s"
)


class SQLImageMetaStorage():
    def __init__(self, address, user, pswd, schema, table):
        self._connector = mysql.connector.connect(user=user,
                                                  password=pswd,
                                                  database=schema,
                                                  host=address.split(":")[
                                                      1][2:],
                                                  port=int(address.split(":")[2]))
        self._table = table
        self._lock = Lock()

    def GetImageMeta(self, reconstruction_id, image_id):
        with self._lock:
            cursor = self._connector.cursor()
            q = GET_IMAGE_META_QUERY.format(table=self._table)
            cursor.execute(q,
                           (reconstruction_id, image_id))

            for(reconstruction_id, image_id, path, form, distored_path) in cursor:
                return ImageMetaData(reconstruction=reconstruction_id,
                                     id=image_id,
                                     path=path,
                                     format=form,
                                     undistorted_path=distored_path)
            cursor.close()


if __name__ == "__main__":
    bind_address = sys.argv[1]
    config_path = sys.argv[2]
    store_root_path = sys.argv[3]
    address = bind_address.split(":")[0]
    port = bind_address.split(":")[1]
    config = {}
    with open(config_path) as config_file:
        config = json.load(config_file)
    stor = SQLImageMetaStorage(config["sql"]["address"],
                               config["sql"]["user"],
                               config["sql"]["password"],
                               config["sql"]["db"],
                               config["sql"]["views_table"])
    srv = ImageServer(stor)
    app.run(address, port)
