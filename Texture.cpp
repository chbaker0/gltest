#include "Texture.hpp"

bool Texture::loadFromFile(const std::string& infile)
{
    using namespace std;
    using namespace glimg;

    try
    {
        unique_ptr<ImageSet> imageSet(loaders::stb::LoadFromFile(infile));
        CreateTexture(handle, imageSet.get(), FORCE_DSA | FORCE_SRGB_COLORSPACE_FMT);
    }
    catch(loaders::stb::StbLoaderException& e)
    {
        #ifdef DEBUG
        cerr << "Error loading image " << infile << endl;
        #endif // DEBUG
        return false;
    }

    return true;
}
