#include "AudioManager.h"

AudioManager::AudioManager()
{
    // init variables
    m_music = NULL;

    int audio_rate = 22050;
    Uint16 audio_format = AUDIO_S16SYS;
    int audio_channels = 2;
    int audio_buffers = 4096;

    if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initizale audio: %s", Mix_GetError());
        exit(-1);
    }
}

AudioManager::~AudioManager()
{
    SDL_Quit();
}


void AudioManager::AddSound(const char* path)
{
    Mix_Chunk* tmpChunk = Mix_LoadWAV(path);

    if (tmpChunk != nullptr)
    {
        m_soundHolder.push_back(tmpChunk);
        std::cout << (m_soundHolder.size() - 1) << " Sound is Ready, path: " << path << '\n';
    }
    else
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't init audio: %s", Mix_GetError());
    }
}

void AudioManager::AddAudio(const char* path)
{
    m_music = Mix_LoadMUS(path);

    if (m_music == NULL)
    {
        printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
    }
}

void AudioManager::PlaySound(const int x) const
{
    if (x > m_soundHolder.size() - 1)
    {
        return;
    }

    Mix_PlayChannel(-1, m_soundHolder[x], 0);
}

void AudioManager::PlayAudio()
{
    if (Mix_PlayingMusic() == 0)
    {
        //Play the music
        Mix_PlayMusic(m_music, -1);
    }
}