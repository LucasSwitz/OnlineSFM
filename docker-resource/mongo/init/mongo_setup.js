conn = new Mongo();
db = conn.getDB("reconstruction")
db.createCollection("reconstruction_configurations")
db.createCollection("default_reconstruction_configurations")