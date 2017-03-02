#include "SoundManager.h"
#include "Vector3.h"
#include "GraphicsManager.h"
#include "Vector3.h"

SoundManager::SoundManager():
activeSoundEngine(NULL),
bgm(NULL)
{
}

SoundManager::~SoundManager()
{
	//{//clears soundmMap
	//	std::map<std::string, ISoundSource*>::iterator it;
	//	while (it != soundMap.end())
	//		it = soundMap.erase(it);
	//}
	//{//clears bgmMap
	//	std::map<std::string, ISound*>::iterator it;
	//	while (it != bgmMap.end())
	//		it = bgmMap.erase(it);
	//}
	if (activeSoundEngine != NULL)
		activeSoundEngine->drop();
}

void SoundManager::Init()
{
	activeSoundEngine = createIrrKlangDevice();
	if (!activeSoundEngine)
	{
		printf("Error starting up irrKlang sound engine\n");
		return; // error starting up the sound engine
	}
}

void SoundManager::PlayBGM(const std::string &soundName)
{
	//std::map<std::string, ISound*>::iterator it;
	//it = std::find(bgmMap.begin(), bgmMap.end(), soundName);
	//if (it != bgmMap.end())
	{//if found
		if (bgm == bgmMap[soundName])
			return;

		if (bgm)
		{
			bgm->setIsPaused(true);
			bgm->setPlayPosition(0);
		}
		bgm = bgmMap[soundName];
		bgm->setIsPaused(false);
	}
}

void SoundManager::PlaySound2D(const std::string &soundName)
{
	activeSoundEngine->play2D(soundMap[soundName]);
}

void SoundManager::PlaySound3D(const std::string &soundName, Vector3 soundPos)
{
	irrklang::vec3df pos(soundPos.x, soundPos.y, soundPos.z);
	Mtx44 viewMatrix = GraphicsManager::GetInstance()->GetViewMatrix();

	irrklang::vec3df listenerPos(viewMatrix.a[3], viewMatrix.a[7], viewMatrix.a[11]);
	irrklang::vec3df listenerView(viewMatrix.a[2], viewMatrix.a[6], viewMatrix.a[10]);
	activeSoundEngine->setListenerPosition(listenerPos, listenerView);

	activeSoundEngine->play3D(soundMap[soundName], pos);
}

void SoundManager::LoadMedia(const std::string &soundName, char* fileLocation, double defaultVolume, float minDist, float maxDist)
{
	if (!activeSoundEngine)
		return;

	soundMap[soundName] = activeSoundEngine->addSoundSourceFromFile(fileLocation);
	soundMap[soundName]->setDefaultMinDistance(minDist);
	soundMap[soundName]->setDefaultMaxDistance(maxDist);
	soundMap[soundName]->setDefaultVolume(defaultVolume);
}

void SoundManager::LoadBGM(const std::string &soundName, char* fileLocation, double defaultVolume, float minDist, float maxDist)
{
	bgmMap[soundName] = activeSoundEngine->play2D(fileLocation, true, true);
}