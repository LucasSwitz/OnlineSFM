#include "sql_openmvg_storage.h"
#include <glog/logging.h>
#include "openMVG/sfm/sfm_data_io_cereal.hpp"
#include "openMVG/sfm/sfm_landmark.hpp"
#include "openMVG/sfm/sfm_data_io.hpp"
#include "openMVG/cameras/cameras_io.hpp"
#include "openMVG/geometry/pose3_io.hpp"
#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/json.hpp>

#define SQL_GET_ALL_VIEWS(t) "SELECT * FROM " + t + " WHERE RECONSTRUCTION_ID = ? ORDER BY VIEW_IDX ASC"
#define SQL_GET_ALL_INTRINSICS(t) "SELECT * FROM " + t + " WHERE RECONSTRUCTION_ID = ? ORDER BY INTRINSICS_IDX ASC"

#define SQL_INTRINSICS_COUNT(t) "SELECT @intrinsic_count:= COUNT(*) FROM " + t + " WHERE RECONSTRUCTION_ID = ?;"
#define SQL_INSERT_OPENMVG_INTRINSIC(t) "INSERT INTO " + t + " VALUES (?,@intrinsic_count,?,?,?);"
#define SQL_VIEW_COUNT(t) "SELECT @view_count:=COUNT(*) FROM " + t + " WHERE RECONSTRUCTION_ID = ?;"
#define SQL_INSERT_OPENMVG_VIEW(t) "INSERT INTO " + t + " VALUES (?,?,@view_count,@intrinsic_count,@view_count,?);"
#define SQL_INSERT_OPENMVG_VIEW_SET_INTRINSIC(t) "INSERT INTO " + t + " VALUES (?,?,@view_count,?,@view_count,?);"
#define SQL_GET_INTRINSIC_BY_HASH(t) "SELECT * FROM " + t + " WHERE RECONSTRUCTION_ID = ? AND DATA_HASH = ?"

#define SQL_INSERT_MATCH(t) "INSERT INTO " + t + " VALUES (?,?,?,?,?)"
#define SQL_GET_MATCHES(t) "SELECT * FROM " + t + " WHERE RECONSTRUCTION_ID = ?"
#define SQL_GET_OPENMVG_META(t) "SELECT * FROM " + t + " WHERE RECONSTRUCTION_ID = ?"
#define SQL_INSERT_OPENMVG_METADATA(t) "INSERT IGNORE INTO " + t + " VALUES (?,?)"

#define SQL_INSERT_OPENMVG_POSE(t) "INSERT INTO " + t + " VALUES (?,?,?) ON DUPLICATE KEY UPDATE DATA = ?"
#define SQL_GET_OPENMVG_POSES(t) "SELECT * FROM " + t + " WHERE RECONSTRUCTION_ID = ? ORDER BY POSE_IDX ASC"

#define SQL_UPDATE_VIEW_POSE_IDX(t) "UPDATE " + t + " SET POSE_IDX = ? WHERE RECONSTRUCTION_ID = ? AND VIEW_IDX = ?"

#define SQL_GET_VIEW_IDX_BY_IMAGE_ID(t) "SELECT VIEW_IDX FROM " + t + " WHERE IMAGE_ID = (?)"

#define SQL_INSERT_OPENMVG_LANDMARK(t) "INSERT INTO " + t + " VALUES (?,?,?)"
#define SQL_GET_OPENMVG_LANDMARKS(t) "SELECT * FROM " + t + " WHERE RECONSTRUCTION_ID = ?"

using namespace openMVG;
using namespace openMVG::cameras;
using namespace openMVG::sfm;

SQLOpenMVGStorage::SQLOpenMVGStorage(
    const std::string &views_table,
    const std::string &intrinsics_table,
    const std::string &matches_table,
    const std::string &meta_table,
    const std::string &poses_table,
    const std::string &landmarks_table) : _views_table(views_table),
                                          _intrinsics_table(intrinsics_table),
                                          _matches_table(matches_table),
                                          _meta_table(meta_table),
                                          _poses_table(poses_table),
                                          _landmarks_table(landmarks_table)
{
}

Views SQLOpenMVGStorage::GetViews(const std::string &reconstruction_id)
{
    auto con_loan = this->GetConnection();
    sql::ResultSet *res = this->IssueQuery(SQL_GET_ALL_VIEWS(this->_views_table), con_loan.con,
                                           [this, reconstruction_id](sql::PreparedStatement *stmt) {
                                               stmt->setString(1, reconstruction_id);
                                           });

    if (!res)
    {
        LOG(ERROR) << "Failure to retrieve results";
        return Views();
    }

    Views views;
    while (res->next())
    {
        std::shared_ptr<View> view = std::make_shared<View>();
        {
            std::stringstream is(res->getString("DATA"));
            cereal::JSONInputArchive archive_in(is);
            try
            {
                archive_in(cereal::make_nvp("view", view));
            }
            catch (std::exception &e)
            {
                LOG(ERROR) << e.what();
            }
        }
        view->id_view = res->getInt64("VIEW_IDX");
        view->id_intrinsic = res->getInt64("INTRINSICS_IDX");
        view->id_pose = res->getInt64("POSE_IDX");
        views[view->id_view] = view;
    }
    delete res;
    return views;
}

Intrinsics SQLOpenMVGStorage::GetIntrinsics(const std::string &reconstruction_id)
{
    auto con_loan = this->GetConnection();
    sql::ResultSet *res = this->IssueQuery(SQL_GET_ALL_INTRINSICS(this->_intrinsics_table), con_loan.con,
                                           [this, reconstruction_id](sql::PreparedStatement *stmt) {
                                               stmt->setString(1, reconstruction_id);
                                           });

    if (!res)
    {
        LOG(ERROR) << "Failure to retrieve results";
        return Intrinsics();
    }

    Intrinsics intrinsics;
    while (res->next())
    {
        std::shared_ptr<IntrinsicBase> intrinsic;
        /*TODO Parse Intrinsic type and set it to the correct thing*/

        int intrinsic_type = res->getInt("INSTRINSIC_TYPE");
        switch (intrinsic_type)
        {
        case PINHOLE_CAMERA:
            intrinsic = std::make_shared<Pinhole_Intrinsic>();
            break;
        case PINHOLE_CAMERA_RADIAL1:
            intrinsic = std::make_shared<Pinhole_Intrinsic_Radial_K1>();
            break;
        case PINHOLE_CAMERA_RADIAL3:
            intrinsic = std::make_shared<Pinhole_Intrinsic_Radial_K3>();
            break;
        case PINHOLE_CAMERA_BROWN:
            intrinsic = std::make_shared<Pinhole_Intrinsic_Brown_T2>();
            break;
        case PINHOLE_CAMERA_FISHEYE:
            intrinsic = std::make_shared<Pinhole_Intrinsic_Fisheye>();
            break;
        case CAMERA_SPHERICAL:
            intrinsic = std::make_shared<Intrinsic_Spherical>();
            break;
        default:
            std::cerr << "Error: unknown camera model: " << (int)intrinsic_type << std::endl;
            return intrinsics;
        }
        std::string data = res->getString("DATA");
        IndexT intrinsic_idx = res->getInt64("INTRINSICS_IDX");
        std::stringstream is(data);
        {
            cereal::JSONInputArchive archive_in(is);
            archive_in(cereal::make_nvp("intrinsic", intrinsic));
        }
        intrinsics[intrinsic_idx] = intrinsic;
    }
    delete res;
    return intrinsics;
}

void SQLOpenMVGStorage::StoreViewAndIntrinsic(const std::string &reconstruction_id,
                                              const std::string &image_id,
                                              const std::shared_ptr<View> view,
                                              const std::shared_ptr<IntrinsicBase> intrinsic)
{
    auto con_loan = this->GetConnection();
    bool store_intrinsic = false;
    int64_t intrinsic_idx = -1;
    int intrinsic_type = -1;

    std::size_t intrinsic_hash = intrinsic->hashValue();

    sql::ResultSet *res = this->IssueQuery(SQL_GET_INTRINSIC_BY_HASH(this->_intrinsics_table), con_loan.con,
                                           [reconstruction_id, intrinsic_hash](sql::PreparedStatement *stmt) {
                                               stmt->setString(1, reconstruction_id);
                                               stmt->setInt64(2, intrinsic_hash);
                                           });

    if (!res || !res->next())
    {
        store_intrinsic = true;
        intrinsic_type = int(intrinsic->getType());
    }
    else
    {
        intrinsic_idx = res->getInt64("INTRINSICS_IDX");
    }
    delete res;

    std::stringstream view_os;
    {
        cereal::JSONOutputArchive archive_view_out(view_os);
        archive_view_out(cereal::make_nvp("view", view));
    }
    std::string view_json = view_os.str();
    try
    {
        this->Transaction([this,
                           reconstruction_id,
                           image_id,
                           view_json,
                           store_intrinsic,
                           intrinsic,
                           intrinsic_idx,
                           intrinsic_type,
                           intrinsic_hash](std::shared_ptr<sql::Connection> con) {
            if (store_intrinsic)
            {
                std::stringstream instrinsic_os;
                {
                    cereal::JSONOutputArchive archive_instrinsic_out(instrinsic_os);
                    archive_instrinsic_out(cereal::make_nvp("intrinsic", intrinsic));
                }
                std::string intrinsic_json = instrinsic_os.str();

                this->Execute(SQL_INTRINSICS_COUNT(this->_intrinsics_table),
                              con,
                              [reconstruction_id](sql::PreparedStatement *stmt) {
                                  stmt->setString(1, reconstruction_id);
                              });
                this->Execute(SQL_INSERT_OPENMVG_INTRINSIC(this->_intrinsics_table),
                              con,
                              [reconstruction_id, intrinsic_type, intrinsic_hash, intrinsic_json](sql::PreparedStatement *stmt) {
                                  stmt->setString(1, reconstruction_id);
                                  stmt->setInt64(2, intrinsic_type);
                                  stmt->setInt64(3, intrinsic_hash);
                                  stmt->setString(4, intrinsic_json);
                              });
                this->Execute(SQL_VIEW_COUNT(this->_views_table),
                              con,
                              [reconstruction_id](sql::PreparedStatement *stmt) {
                                  stmt->setString(1, reconstruction_id);
                              });
                this->Execute(SQL_INSERT_OPENMVG_VIEW(this->_views_table),
                              con,
                              [reconstruction_id, image_id, view_json](sql::PreparedStatement *stmt) {
                                  stmt->setString(1, reconstruction_id);
                                  stmt->setString(2, image_id);
                                  stmt->setString(3, view_json);
                              });
            }
            else
            {
                this->Execute(SQL_VIEW_COUNT(this->_views_table),
                              con,
                              [reconstruction_id](sql::PreparedStatement *stmt) {
                                  stmt->setString(1, reconstruction_id);
                              });
                this->Execute(SQL_INSERT_OPENMVG_VIEW_SET_INTRINSIC(this->_views_table),
                              con,
                              [reconstruction_id, image_id, view_json, intrinsic_idx](sql::PreparedStatement *stmt) {
                                  stmt->setString(1, reconstruction_id);
                                  stmt->setString(2, image_id);
                                  stmt->setInt64(3, intrinsic_idx);
                                  stmt->setString(4, view_json);
                              });
            }
        });
    }
    catch (const sql::SQLException &e)
    {
        if (e.getErrorCode() == 1062)
        {
            //duplicate key
            LOG(ERROR) << "Duplicate key when storing View/Intrinsic. Trying again...";
            this->StoreViewAndIntrinsic(reconstruction_id, image_id, view, intrinsic);
        }
    }
}

void SQLOpenMVGStorage::DeleteAll(const std::string &reconstruction_id)
{
}

using namespace matching;
void SQLOpenMVGStorage::StoreMatches(const std::string &reconstruction_id,
                                     char model,
                                     const PairWiseMatches &matches)
{
    for (auto e : matches)
    {
        std::pair<IndexT, IndexT> p = e.first;
        IndexT view_1 = std::get<0>(p);
        IndexT view_2 = std::get<1>(p);
        const std::vector<IndMatch> &pair_matches = e.second;
        std::stringstream stream;
        stream << view_1 << " " << view_2 << '\n'
               << pair_matches.size() << '\n';
        copy(pair_matches.begin(), pair_matches.end(), std::ostream_iterator<IndMatch>(stream, "\n"));

        std::string matches_data = stream.str();
        std::istringstream *blob_stream = new std::istringstream(matches_data);

        this->IssueUpdate(SQL_INSERT_MATCH(this->_matches_table),
                          [this, reconstruction_id, view_1, view_2, model, matches_data, blob_stream](sql::PreparedStatement *pstmt) {
                              pstmt->setString(1, reconstruction_id);
                              pstmt->setInt64(2, view_1);
                              pstmt->setInt64(3, view_2);
                              pstmt->setString(4, std::string(1, model));
                              pstmt->setBlob(5, blob_stream);
                          });
        delete blob_stream;
    }
}

std::unique_ptr<SfM_Data> SQLOpenMVGStorage::GetSFMData(const std::string &reconstruction_id, int mask)
{
    std::unique_ptr<SfM_Data> sfm_data = std::make_unique<SfM_Data>();
    //sfm_data->s_root_path = this->GetMeta(reconstruction_id).root_path;
    if (mask & ESfM_Data::VIEWS)
        sfm_data->views = this->GetViews(reconstruction_id);
    if (mask & ESfM_Data::INTRINSICS)
        sfm_data->intrinsics = this->GetIntrinsics(reconstruction_id);
    if (mask & ESfM_Data::EXTRINSICS)
        sfm_data->poses = this->GetPoses(reconstruction_id);
    if (mask & ESfM_Data::STRUCTURE)
        sfm_data->structure = this->GetLandmarks(reconstruction_id);
    return sfm_data;
}

PairWiseMatches SQLOpenMVGStorage::GetMatches(const std::string &reconstruction_id)
{
    auto con_loan = this->GetConnection();
    PairWiseMatches pariwise_matches;
    sql::ResultSet *res = this->IssueQuery(SQL_GET_MATCHES(this->_matches_table), con_loan.con,
                                           [this, reconstruction_id](sql::PreparedStatement *stmt) {
                                               stmt->setString(1, reconstruction_id);
                                           });

    if (!res)
    {
        LOG(ERROR) << "Failure to retrieve results";
        return pariwise_matches;
    }

    while (res->next())
    {
        std::pair<IndexT, IndexT> view_pair(res->getInt64("VIEW_1"), res->getInt64("VIEW_2"));
        std::vector<IndMatch> matches;
        std::stringstream stream(res->getString("MATCH_DATA"));
        size_t I, J, number;
        stream >> I >> J >> number;
        std::vector<IndMatch> read_matches(number);
        for (size_t i = 0; i < number; ++i)
        {
            stream >> read_matches[i];
        }
        pariwise_matches[{I, J}] = std::move(read_matches);
    }
    delete res;
    return pariwise_matches;
}

void SQLOpenMVGStorage::StoreMeta(const std::string &reconstruction_id, const OpenMVGMetadata &meta)
{
    this->IssueUpdate(SQL_INSERT_OPENMVG_METADATA(this->_meta_table),
                      [this, reconstruction_id, meta](sql::PreparedStatement *pstmt) {
                          pstmt->setString(1, reconstruction_id);
                          pstmt->setString(2, meta.root_path);
                      });
}

OpenMVGMetadata SQLOpenMVGStorage::GetMeta(const std::string &reconstruction_id)
{
    auto con_loan = this->GetConnection();
    OpenMVGMetadata meta;
    sql::ResultSet *res = this->IssueQuery(SQL_GET_OPENMVG_META(this->_meta_table), con_loan.con,
                                           [this, reconstruction_id](sql::PreparedStatement *stmt) {
                                               stmt->setString(1, reconstruction_id);
                                           });

    if (!res || !res->next())
    {
        LOG(ERROR) << "Failure to retrieve results";
        return meta;
    }

    meta.root_path = res->getString("ROOT_PATH");
    delete res;
    return meta;
}

void SQLOpenMVGStorage::StorePoseUpdateView(const std::string &reconstruction_id,
                                            const std::shared_ptr<openMVG::sfm::View> view,
                                            const openMVG::geometry::Pose3 &pose)
{

    std::stringstream os;
    {
        cereal::JSONOutputArchive archive_out(os);
        archive_out(cereal::make_nvp("pose", pose));
    }
    std::string pose_json = os.str();
    int pose_idx = view->id_pose;
    int view_idx = view->id_view;
    this->IssueUpdate(SQL_INSERT_OPENMVG_POSE(this->_poses_table),
                      [this, reconstruction_id, pose_idx, pose_json](sql::PreparedStatement *pstmt) {
                          pstmt->setString(1, reconstruction_id);
                          pstmt->setInt64(2, pose_idx);
                          pstmt->setString(3, pose_json);
                          pstmt->setString(4, pose_json);
                      });
    this->IssueUpdate(SQL_UPDATE_VIEW_POSE_IDX(this->_views_table),
                      [this, reconstruction_id, pose_idx, view_idx](sql::PreparedStatement *pstmt) {
                          pstmt->setInt64(1, pose_idx);
                          pstmt->setString(2, reconstruction_id);
                          pstmt->setInt64(3, view_idx);
                      });
}

Poses SQLOpenMVGStorage::GetPoses(const std::string &reconstruction_id)
{
    auto con_loan = this->GetConnection();
    Poses poses;
    sql::ResultSet *res = this->IssueQuery(SQL_GET_OPENMVG_POSES(this->_poses_table), con_loan.con,
                                           [this, reconstruction_id](sql::PreparedStatement *stmt) {
                                               stmt->setString(1, reconstruction_id);
                                           });

    if (!res)
    {
        LOG(ERROR) << "Failure to retrieve results";
        return poses;
    }

    while (res->next())
    {
        Pose3 pose;
        std::stringstream is(res->getString("DATA"));
        IndexT pose_idx = res->getInt64("POSE_IDX");
        cereal::JSONInputArchive archive_in(is);
        try
        {
            archive_in(cereal::make_nvp("pose", pose));
            poses[pose_idx] = pose;
        }
        catch (std::exception &e)
        {
            LOG(ERROR) << e.what();
        }
    }
    delete res;
    return poses;
}

openMVG::IndexT SQLOpenMVGStorage::GetViewIdxByImageID(const std::string &image_id)
{
    auto con_loan = this->GetConnection();
    IndexT index = -1;
    sql::ResultSet *res = this->IssueQuery(SQL_GET_VIEW_IDX_BY_IMAGE_ID(this->_views_table), con_loan.con,
                                           [this, image_id](sql::PreparedStatement *stmt) {
                                               stmt->setString(1, image_id);
                                           });

    if (!res)
    {
        LOG(ERROR) << "Failure to retrieve results";
        return index;
    }

    if (res->next())
    {
        index = res->getInt64("VIEW_IDX");
    }
    delete res;
    return index;
}

std::unique_ptr<std::unordered_map<std::string, IndexT>> SQLOpenMVGStorage::GetAllViewIdxByImageID(const std::vector<std::string> &image_ids)
{
    std::unique_ptr<std::unordered_map<std::string, IndexT>> idx_map = std::make_unique<std::unordered_map<std::string, IndexT>>();
    this->Transaction([this, &image_ids, &idx_map](std::shared_ptr<sql::Connection> con) mutable {
        for (std::string image_id : image_ids)
        {
            sql::ResultSet *res = this->IssueQuery(SQL_GET_VIEW_IDX_BY_IMAGE_ID(this->_views_table), con,
                                                   [image_id, &idx_map](sql::PreparedStatement *stmt) mutable {
                                                       stmt->setString(1, image_id);
                                                   });
            if (!res)
            {
                LOG(ERROR) << "Failure to retrieve results";
                return;
            }
            if (res->next())
            {
                (*idx_map)[image_id] = res->getInt64("VIEW_IDX");
            }
        }
    });
    return idx_map;
}

void SQLOpenMVGStorage::StoreLandmarks(const std::string &reconstruction_id,
                                       const openMVG::sfm::Landmarks &landmarks)
{
    for (auto e : landmarks)
    {
        IndexT track_id = e.first;
        Landmark landmark = e.second;
        std::ostringstream stream;
        {
            cereal::JSONOutputArchive archive_out(stream);
            archive_out(cereal::make_nvp("landmark", landmark));
        }
        std::string landmarks_data = stream.str();
        std::istringstream *blob_stream = new std::istringstream(landmarks_data);

        this->IssueUpdate(SQL_INSERT_OPENMVG_LANDMARK(this->_landmarks_table),
                          [this, reconstruction_id, track_id, blob_stream](sql::PreparedStatement *pstmt) {
                              pstmt->setString(1, reconstruction_id);
                              pstmt->setInt64(2, track_id);
                              pstmt->setBlob(3, blob_stream);
                          });
        delete blob_stream;
    }
}

#include "openMVG/sfm/sfm_landmark_io.hpp"

#include <cereal/types/map.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/vector.hpp>

Landmarks SQLOpenMVGStorage::GetLandmarks(const std::string &reconstruction_id)
{
    auto con_loan = this->GetConnection();
    Landmarks landmarks;
    sql::ResultSet *res = this->IssueQuery(SQL_GET_OPENMVG_LANDMARKS(this->_landmarks_table), con_loan.con,
                                           [this, reconstruction_id](sql::PreparedStatement *stmt) {
                                               stmt->setString(1, reconstruction_id);
                                           });

    if (!res)
    {
        LOG(ERROR) << "Failure to retrieve results";
        return landmarks;
    }

    while (res->next())
    {
        Landmark landmark;
        std::stringstream is(res->getString("DATA"));
        IndexT track = res->getInt64("TRACK");
        try
        {
            {
                cereal::JSONInputArchive archive_in(is);
                archive_in(cereal::make_nvp("landmark", landmark));
            }
            landmarks[track] = landmark;
        }
        catch (std::exception &e)
        {
            LOG(ERROR) << e.what();
        }
    }
    delete res;
    return landmarks;
}