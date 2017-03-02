#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include "SingletonTemplate.h"

#include <map>
#include <string>

#include <irrKlang.h>
using namespace irrklang;

struct Vector3;

class SoundManager : public Singleton<SoundManager>
{
	friend Singleton<SoundManager>;
public:
	void Init();

	void PlayBGM(const std::string &soundName);
	void PlaySound2D(const std::string &soundName);
	void PlaySound3D(const std::string &soundName, Vector3 soundPos);

	void LoadMedia(const std::string &soundName, char* fileLocation, double defaultVolume = 0.5, float minDist = 0.0f, float maxDist = 0.0f);
	void LoadBGM(const std::string &soundName, char* fileLocation, double defaultVolume = 0.5, float minDist = 0.0f, float maxDist = 0.0f);

private:
	SoundManager();
	~SoundManager();

	ISoundEngine* activeSoundEngine;
	ISound* bgm;
	
	std::map<std::string, ISoundSource*> soundMap;
	std::map<std::string, ISound*> bgmMap;
};

#endif