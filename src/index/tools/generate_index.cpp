#include "vocab_index.h"

#include "openMVG/image/image_container.hpp"
#include "openMVG/features/sift/SIFT_Anatomy_Image_Describer.hpp"
#include "openMVG/image/image_io.hpp"
#include "boost/filesystem.hpp"

#include <mutex>

using namespace openMVG::image;
std::vector<std::string> get_file_list(const std::string& path);

int main(int argc, char** argv){
    VisualVocabularyGenerator generator;
    bool bUpRight = true;
    std::string sImage_Describer_Method = "SIFT_ANATOMY";
    bool bForce = false;
    std::string sFeaturePreset = "";
    std::string dataset_dir = argv[1];
    std::unique_ptr<Image_describer> image_describer = std::make_unique<SIFT_Anatomy_Image_describer>(SIFT_Anatomy_Image_describer::Params());
    std::mutex generator_lock; 
    std::vector<std::string> image_paths = get_file_list(dataset_dir);
    std::cout << "Found " << image_paths.size() << " images in " << dataset_dir;
    Image<unsigned char> imageGray;
    std::atomic<bool> preemptive_exit(false);
    const unsigned int nb_max_thread = omp_get_max_threads();
    int iNumThreads = 15;

    if (iNumThreads > 0) {
        omp_set_num_threads(iNumThreads);
    } else {
        omp_set_num_threads(nb_max_thread);
    }

    #pragma omp parallel for schedule(dynamic) if (iNumThreads > 0) private(imageGray)
    for(int i = 0; i < image_paths.size(); ++i){
        std::vector<std::string>::const_iterator iter_paths = image_paths.begin();
        std::advance(iter_paths, i);
        std::string image_path = *iter_paths;
        std::cout << "Generating features for " << image_path << std::endl;

        // If features or descriptors file are missing, compute them
        if (!preemptive_exit){
        if (!ReadImage(image_path.c_str(), &imageGray))
                continue;

            Image<unsigned char> * mask = nullptr; // The mask is null by default
            const std::string mask_filename_local = stlplus::create_filespec(dataset_dir,
                                                                            stlplus::basename_part(image_path) + "_mask", "png"),
                mask__filename_global = stlplus::create_filespec(dataset_dir, "mask", "png");
            Image<unsigned char> imageMask;
            // Try to read the local mask
            if (stlplus::file_exists(mask_filename_local))
            {
                if (!ReadImage(mask_filename_local.c_str(), &imageMask))
                {
                    std::cerr << "Invalid mask: " << mask_filename_local << std::endl
                                << "Stopping feature extraction." << std::endl;
                    preemptive_exit = true;
                }
                // Use the local mask only if it fits the current image size
                if (imageMask.Width() == imageGray.Width() && imageMask.Height() == imageGray.Height())
                mask = &imageMask;
            }
            else
            {
                // Try to read the global mask
                if (stlplus::file_exists(mask__filename_global))
                {
                if (!ReadImage(mask__filename_global.c_str(), &imageMask))
                {
                    std::cerr << "Invalid mask: " << mask__filename_global << std::endl
                            << "Stopping feature extraction." << std::endl;
                    preemptive_exit = true;
                }
                // Use the global mask only if it fits the current image size
                if (imageMask.Width() == imageGray.Width() && imageMask.Height() == imageGray.Height())
                    mask = &imageMask;
                }
            }

            auto regions = image_describer->Describe(imageGray, mask);
            std::vector<Descriptor<unsigned char, 128>, Eigen::aligned_allocator<Descriptor<unsigned char, 128>>> descriptors(regions->RegionCount());
            std::cout << "Adding descriptors for " << image_path << " to vocab generator" << std::endl;

            const Descriptor<unsigned char, 128>* desc_raw = reinterpret_cast<const Descriptor<unsigned char, 128>*>(regions->DescriptorRawData());
            // Copy computed descriptor into the vocabular generator

            memcpy(&descriptors[0], desc_raw, regions->RegionCount()*sizeof(Descriptor<unsigned char, 128>));
            generator_lock.lock();
            generator.AddDescriptors(descriptors); 
            generator_lock.unlock();  
        }
    }
    std::cout << "Generating vocabulary" << std::endl;
    generator.Generate();
    std::cout << "Saving index to " << argv[2] << std::endl;
    generator.Dump(argv[2]);
    return 0;
}


std::vector<std::string> get_file_list(const std::string& path)
{
    std::vector<std::string> file_list;
    if (!path.empty())
    {
        namespace fs = boost::filesystem;

        fs::path apk_path(path);
        fs::recursive_directory_iterator end;

        for (fs::recursive_directory_iterator i(apk_path); i != end; ++i)
        {
            const fs::path cp = (*i);
            file_list.push_back(cp.string());
        }
    }
    return file_list;
}