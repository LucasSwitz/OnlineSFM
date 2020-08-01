#pragma once
#include "job.h"

class ComputeFeaturesJob : public Job
{
public:
    ComputeFeaturesJob(){

    };
    static json Serialize(ComputeFeaturesJob &tj)
    {
        json j;
        j["image_id"] = tj.image_id;
        j["reconstruction_id"] = tj.reconstruction_id;
        return j;
    }
    static std::string Type()
    {
        return "Reconstruction_ComputeFeatures";
    }
    static ComputeFeaturesJob Parse(const json &j)
    {
        ComputeFeaturesJob job;
        job.ParseHeader(j);

        json job_json = j["job"];
        job.image_id = job_json["image_id"];
        job.reconstruction_id = job_json["reconstruction_id"];
        return job;
    }
    std::string image_id;
    std::string reconstruction_id;
};

class AddImageJob : public Job
{
public:
    AddImageJob(){

    };
    static json Serialize(AddImageJob &tj)
    {
        json j;
        j["image_id"] = tj.image_id;
        j["reconstruction_id"] = tj.reconstruction_id;
        return j;
    }
    static std::string Type()
    {
        return "Reconstruction_AddImage";
    }
    static AddImageJob Parse(const json &j)
    {
        AddImageJob job;
        job.ParseHeader(j);

        json job_json = j["job"];
        job.image_id = job_json["image_id"];
        job.reconstruction_id = job_json["reconstruction_id"];
        return job;
    }
    std::string image_id;
    std::string reconstruction_id;
};

class ComputeMatchesJob : public Job
{
public:
    ComputeMatchesJob(){

    };
    static json Serialize(ComputeMatchesJob &tj)
    {
        json j;
        j["image_id"] = tj.image_id;
        j["reconstruction_id"] = tj.reconstruction_id;
        return j;
    }
    static std::string Type()
    {
        return "Reconstruction_ComputeMatches";
    }
    static ComputeMatchesJob Parse(const json &j)
    {
        ComputeMatchesJob job;
        job.ParseHeader(j);

        json job_json = j["job"];
        job.image_id = job_json["image_id"];
        job.reconstruction_id = job_json["reconstruction_id"];
        return job;
    }
    std::string image_id;
    std::string reconstruction_id;
};

class SparseReconstructionJob : public Job
{
public:
    SparseReconstructionJob(){

    };
    static json Serialize(SparseReconstructionJob &tj)
    {
        json j;
        j["reconstruction_id"] = tj.reconstruction_id;
        return j;
    }
    static std::string Type()
    {
        return "Reconstruction_SparseReconstruction";
    }
    static SparseReconstructionJob Parse(const json &j)
    {
        SparseReconstructionJob job;
        job.ParseHeader(j);

        json job_json = j["job"];
        job.reconstruction_id = job_json["reconstruction_id"];
        return job;
    }
    std::string reconstruction_id;
};

class ComputeStructureJob : public Job
{
public:
    ComputeStructureJob(){

    };
    static json Serialize(ComputeStructureJob &tj)
    {
        json j;
        j["reconstruction_id"] = tj.reconstruction_id;
        return j;
    }
    static std::string Type()
    {
        return "Reconstruction_ComputeStructure";
    }
    static ComputeStructureJob Parse(const json &j)
    {
        ComputeStructureJob job;
        job.ParseHeader(j);

        json job_json = j["job"];
        job.reconstruction_id = job_json["reconstruction_id"];
        return job;
    }
    std::string reconstruction_id;
};

class MVSJob : public Job
{
public:
    MVSJob(){

    };
    static json Serialize(MVSJob &tj)
    {
        json j;
        j["reconstruction_id"] = tj.reconstruction_id;
        return j;
    }
    static std::string Type()
    {
        return "Reconstruction_MVS";
    }
    static MVSJob Parse(const json &j)
    {
        MVSJob job;
        job.ParseHeader(j);

        json job_json = j["job"];
        job.reconstruction_id = job_json["reconstruction_id"];
        return job;
    }
    std::string reconstruction_id;
};

class IndexImageJob : public Job
{
public:
    IndexImageJob(){

    };
    static json Serialize(IndexImageJob &tj)
    {
        json j;
        j["image_id"] = tj.image_id;
        j["reconstruction_id"] = tj.reconstruction_id;
        return j;
    }
    static std::string Type()
    {
        return "Index_IndexImage";
    }
    static IndexImageJob Parse(const json &j)
    {
        IndexImageJob job;
        job.ParseHeader(j);

        json job_json = j["job"];
        job.image_id = job_json["image_id"];
        job.reconstruction_id = job_json["reconstruction_id"];
        return job;
    }
    std::string image_id;
    std::string reconstruction_id;
};

class ScoreImageJob : public Job
{
public:
    ScoreImageJob(){

    };
    ScoreImageJob(ScoreImageJob *const){

    };
    static json Serialize(ScoreImageJob &tj)
    {
        json j;
        j["image_id"] = tj.image_id;
        j["reconstruction_id"] = tj.reconstruction_id;
        return j;
    }
    static std::string Type()
    {
        return "Index_ScoreImage";
    }
    static ScoreImageJob Parse(const json &j)
    {
        ScoreImageJob job;
        job.ParseHeader(j);

        json job_json = j["job"];
        job.image_id = job_json["image_id"];
        job.reconstruction_id = job_json["reconstruction_id"];
        return job;
    }
    std::string image_id;
    std::string reconstruction_id;
};

class SearchClosestNJob : public Job
{
public:
    SearchClosestNJob(){

    };
    static json Serialize(SearchClosestNJob &tj)
    {
        json j;
        j["image_id"] = tj.image_id;
        j["reconstruction_id"] = tj.reconstruction_id;
        return j;
    }
    static std::string Type()
    {
        return "Reconstruction_SearchClosestN";
    }
    static SearchClosestNJob Parse(const json &j)
    {
        SearchClosestNJob job;
        job.ParseHeader(j);

        json job_json = j["job"];
        job.image_id = job_json["image_id"];
        job.reconstruction_id = job_json["reconstruction_id"];
        job.n = job_json["n"];
        return job;
    }
    std::string image_id;
    std::string reconstruction_id;
    int n;
};