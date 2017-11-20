#ifndef SFML_EXAMPLE_SOUNDSTREAMER_H
#define SFML_EXAMPLE_SOUNDSTREAMER_H

#include <bitset>
#include <cmath>
#include <string>
#include <vector>
#include <SFML/Audio/SoundStream.hpp>

class SoundStreamer : public sf::SoundStream
{
public:
    SoundStreamer(int sampleCount_){
        sampleCount = sampleCount_;
    }
    ~SoundStreamer()
    {
        delete[] samples;
    }
    void passJniData(short* samples_){
        samples = samples_;
        sf::SoundStream::initialize(2, sampleCount);
    }
protected:
    bool onGetData(sf::SoundStream::Chunk& chunk)
    {
        chunk.samples = samples;
        //chunk.sampleCount = static_cast<std::size_t>(myFile.read(samples,sampleCount));
        chunk.sampleCount = static_cast<std::size_t>(sampleCount);
        return (chunk.sampleCount>=sampleCount);
    }
    void onSeek(sf::Time time)
    {
        //myFile.seek(time);
    }
private:
    sf::Int16* samples;
    int sampleCount;
};

#endif //SFML_EXAMPLE_SOUNDSTREAMER_H
