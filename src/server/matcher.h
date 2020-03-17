#pragma once


typedef struct MatcherConfigT {
  std::string sSfM_Data_Filename;
  std::string sMatchesDirectory = "";
  std::string sGeometricModel = "f";
  float fDistRatio = 0.8f;
  int iMatchingVideoMode = -1;
  std::string sPredefinedPairList = "";
  std::string sNearestMatchingMethod = "AUTO";
  bool bForce = false;
  bool bGuided_matching = false;
  int imax_iteration = 2048;
  unsigned int ui_max_cache_size = 0;
} MatcherConfig;

class ContinuousMatcher {
    public:
        ContinuousMatcher(const MatcherConfig& config);
        bool Match(Pair_Set& pairs);
    private:
        MatcherConfig _config;
};