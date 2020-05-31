import os
import sys
import json

from elasticsearch import Elasticsearch

if __name__ == "__main__":
    address = sys.argv[1]
    indexes_path = sys.argv[2]
    es = Elasticsearch([address])
    index_dirs = [dI for dI in os.listdir(indexes_path) if os.path.isdir(os.path.join(indexes_path,dI))]
    for index_dir in index_dirs:
        index_name = index_dir
        index_dir_path = indexes_path + '/' + index_dir
        response = es.indices.create(
            index=index_name,
            body={},
            ignore=400
        )
        for type_file in os.listdir(index_dir_path):
            with open(index_dir_path + '/' + type_file, "r") as tf:
                typename = type_file.split(".")[0]
                type_data = json.load(tf)
                print(type_data)
                response = es.indices.put_mapping(
                    index= index_name,
                    doc_type= typename,
                    body= type_data,
                    include_type_name = True,
                    ignore=400
                )
                print(response)
    print("Finished creating indicies.")




