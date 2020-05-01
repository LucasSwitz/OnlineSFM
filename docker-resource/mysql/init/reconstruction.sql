USE reconstruction;
CREATE TABLE IF NOT EXISTS RECONSTRUCTIONS(
    ID CHAR(36) NOT NULL PRIMARY KEY,
    ROOT_PATH VARCHAR(200) NOT NULL,
    IMAGES_PATH VARCHAR(200) NOT NULL,
    FEATURES_PATH VARCHAR(200) NOT NULL,
    MATCHES_PATH  VARCHAR(200) NOT NULL,
    SFM_PATH VARCHAR(200) NOT NULL,
    MVS_PATH VARCHAR(200) NOT NULL
);
CREATE TABLE IF NOT EXISTS VIEWS(
    ID CHAR(36) NOT NULL PRIMARY KEY,
    RECONSTRUCTION_ID CHAR(36) NOT NULL REFERENCES RECONSTRUCTIONS,
    PATH VARCHAR(200) NOT NULL,
    FORMAT VARCHAR(20) NOT NULL,
    UNDISTORTED_PATH VARCHAR(200)
);
CREATE TABLE IF NOT EXISTS OPENMVG_VIEWS(
    RECONSTRUCTION_ID CHAR(36) NOT NULL REFERENCES RECONSTRUCTIONS,
    IMAGE_ID CHAR(36) NOT NULL REFERENCES VIEWS,
    VIEW_IDX BIGINT NOT NULL,
    INTRINSICS_IDX BIGINT NOT NULL,
    POSE_IDX BIGINT NOT NULL,
    DATA JSON NOT NULL,
    PRIMARY KEY (RECONSTRUCTION_ID, VIEW_IDX) 
);
CREATE TABLE IF NOT EXISTS OPENMVG_INTRINSICS(
    RECONSTRUCTION_ID CHAR(36) NOT NULL REFERENCES RECONSTRUCTIONS,
    INTRINSICS_IDX BIGINT NOT NULL,
    INSTRINSIC_TYPE INT, 
    DATA_HASH BIGINT,
    DATA JSON NOT NULL,
    PRIMARY KEY (RECONSTRUCTION_ID, INTRINSICS_IDX) 
);
CREATE TABLE IF NOT EXISTS OPENMVG_LANDMARKS(
    RECONSTRUCTION_ID CHAR(36) NOT NULL REFERENCES RECONSTRUCTIONS,
    TRACK BIGINT NOT NULL,
    DATA BLOB NOT NULL
);
CREATE TABLE IF NOT EXISTS OPENMVG_MATCHES(
    RECONSTRUCTION_ID CHAR(36) NOT NULL REFERENCES RECONSTRUCTIONS,
    VIEW_1 BIGINT,
    VIEW_2 BIGINT,
    GEOMETRIC_MODEL CHAR(2),
    MATCH_DATA BLOB NOT NULL,
    PRIMARY KEY (RECONSTRUCTION_ID, VIEW_1, VIEW_2)
);
CREATE TABLE IF NOT EXISTS OPENMVG_POSES(
    RECONSTRUCTION_ID CHAR(36) NOT NULL REFERENCES RECONSTRUCTIONS,
    POSE_IDX BIGINT NOT NULL,
    DATA JSON NOT NULL,
    PRIMARY KEY (RECONSTRUCTION_ID, POSE_IDX) 
);
CREATE TABLE IF NOT EXISTS OPENMVG_META(
    RECONSTRUCTION_ID CHAR(36) NOT NULL PRIMARY KEY REFERENCES RECONSTRUCTIONS,
    ROOT_PATH varchar(200) NOT NULL
);
CREATE TABLE IF NOT EXISTS SPARSE(
    ID CHAR(36) NOT NULL PRIMARY KEY,
    RECONSTRUCTION_ID CHAR(36) NOT NULL REFERENCES RECONSTRUCTIONS,
    PLY_PATH VARCHAR(200) NOT NULL,
    MVS_PATH VARCHAR(200) NOT NULL
);
CREATE TABLE IF NOT EXISTS OBJ(
    ID CHAR(36) NOT NULL PRIMARY KEY,
    RECONSTRUCTION_ID CHAR(36) NOT NULL REFERENCES RECONSTRUCTIONS,
    PATH VARCHAR(200) NOT NULL,
    TEXTURE_PATH VARCHAR(200) NOT NULL,
    MTL_PATH VARCHAR(200) NOT NULL
);
CREATE TABLE CAMERA_INTRINSICS(
    MODEL VARCHAR(50) NOT NULL PRIMARY KEY,
    MAKER VARCHAR(50),
    NUMERIC_MODEL VARCHAR(50),
    SENSOR_SIZE FLOAT,
    FOCUS_MODE VARCHAR(40),
    HORIZONTAL_VIEW_ANGLE FLOAT,
    JPEG_QUALITY INT
);
CREATE TABLE IF NOT EXISTS IMAGE_REGIONS(
    RECONSTRUCTION_ID CHAR(36) NOT NULL REFERENCES RECONSTRUCTIONS,
    IMAGE_ID CHAR(36) NOT NULL REFERENCES VIEWS,
    DESCRIPTOR BINARY(128) NOT NULL,
    FEATURE BLOB,
    INDEX reconstruction_idx(RECONSTRUCTION_ID, IMAGE_ID)
);
CREATE TABLE IF NOT EXISTS IMAGE_WORDS(
    RECONSTRUCTION_ID CHAR(36) NOT NULL REFERENCES RECONSTRUCTIONS,
    IMAGE_ID CHAR(36) NOT NULL REFERENCES VIEWS,
    DESCRIPTOR BINARY(128) NOT NULL,
    N INT NOT NULL,
    PRIMARY KEY (RECONSTRUCTION_ID, IMAGE_ID, DESCRIPTOR),
    INDEX descriptor_index (DESCRIPTOR)
);