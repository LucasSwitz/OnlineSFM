#include "sql_camera_intrinsics_storage.h"

#define SQL_GET_CAMERA_INTRINSIC(t) "SELECT * FROM " + t + " WHERE MODEL = ? OR (MAKER = ? AND NUMERIC_MODEL = ?)"
#define SQL_INSERT_CAMERA_INTRINSIC(t) "INSERT INTO " + t + " VALUES (?,?,?,?,?,?,?)"
#define SQL_DELETE_CAMERA_INTRINSIC(t) "DELETE FROM " + t + " WHERE MODEL = ?"

SQLCameraIntrinsicsStorage::SQLCameraIntrinsicsStorage(
    const std::string &table) : _table(table)
{
}

void SQLCameraIntrinsicsStorage::Store(const CameraIntrinsics &camera_instrinsics)
{
    std::string model_lower = camera_instrinsics.model();
    std::transform(model_lower.begin(), model_lower.end(), model_lower.begin(), ::tolower);
    // If maker not provided, extract it
    std::string maker = camera_instrinsics.maker().empty() ? ExtractMaker(model_lower) : camera_instrinsics.maker();
    // If model not provided, extract it
    std::string model_numeric = camera_instrinsics.numeric_model().empty() ? ExtractNumericModel(model_lower) : camera_instrinsics.numeric_model();
    this->IssueUpdate(SQL_INSERT_CAMERA_INTRINSIC(this->_table), [this, model_lower, maker, model_numeric, camera_instrinsics](sql::PreparedStatement *pstmt) {
        pstmt->setString(1, model_lower);
        pstmt->setString(2, maker);
        pstmt->setString(3, model_numeric);
        pstmt->setDouble(4, camera_instrinsics.sensor_size());
        pstmt->setString(5, camera_instrinsics.focus_mode());
        pstmt->setDouble(6, camera_instrinsics.horizontal_view_angle());
        pstmt->setInt64(7, camera_instrinsics.jpeg_quality());
    });
}

void SQLCameraIntrinsicsStorage::Delete(const std::string &model)
{
    CameraIntrinsics camera_instrinsic = Get(model);
    this->IssueUpdate(SQL_DELETE_CAMERA_INTRINSIC(this->_table),
                      [model](sql::PreparedStatement *stmt) {
                          stmt->setString(1, model);
                      });
}

CameraIntrinsics SQLCameraIntrinsicsStorage::Get(const std::string &model)
{
    auto connection_loan = this->GetConnection();
    std::string model_lower = model;
    std::transform(model_lower.begin(), model_lower.end(), model_lower.begin(), ::tolower);
    std::string maker = ExtractMaker(model_lower);
    std::string model_numeric = ExtractNumericModel(model_lower);
    sql::ResultSet *res = this->IssueQuery(SQL_GET_CAMERA_INTRINSIC(this->_table), connection_loan.con,
                                           [model, maker, model_numeric](sql::PreparedStatement *stmt) {
                                               stmt->setString(1, model);
                                               stmt->setString(2, maker);
                                               stmt->setString(3, model_numeric);
                                           });
    CameraIntrinsics intrinsics_data;
    if (res->next())
    {
        intrinsics_data.set_model(res->getString("MODEL"));
        intrinsics_data.set_maker(res->getString("MAKER"));
        intrinsics_data.set_numeric_model(res->getString("NUMERIC_MODEL"));
        intrinsics_data.set_sensor_size(res->getDouble("SENSOR_SIZE"));
        intrinsics_data.set_focus_mode(res->getString("FOCUS_MODE"));
        intrinsics_data.set_horizontal_view_angle(res->getDouble("HORIZONTAL_VIEW_ANGLE"));
        intrinsics_data.set_jpeg_quality(res->getInt64("JPEG_QUALITY"));
    }
    delete res;
    return intrinsics_data;
}

bool SQLCameraIntrinsicsStorage::Exists(const std::string &model)
{
    return !Get(model).model().empty();
}