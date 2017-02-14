#ifndef LUAINTERFACE_H
#define LUAINTERFACE_H

#include "SingletonTemplate.h"

#include <lua.hpp>
#include "Vector3.h"

#include <map>
using std::map;

struct LuaInfo
{
	lua_State* luaState;
	const char* fileDirectory;
};

class LuaInterface : public Singleton<LuaInterface>
{
	friend Singleton<LuaInterface>;
public:
	virtual ~LuaInterface();

	bool Init();
	void Run();
	void Drop();

	int GetIntValue(std::string stringKey, const char* varName);
	float GetFloatValue(std::string stringKey, const char* varName);
	char GetCharValue(std::string stringKey, const char* varName);
	Vector3 GetVector3Value(std::string stringKey, const char* varName);
	float GetField(std::string stringKey, const char* key);
	int GetVariableValues(std::string stringKey, const char* varName, int &a, int &b, int&c, int&d);

	float GetDistanceSquareValue(const char* varName, Vector3 source, Vector3 destination);

	void SaveIntValue(std::string stringKey, const char* varName, const int value, const bool overwrite = false);
	void SaveFloatValue(std::string stringKey, const char* varName, const float value, const bool overwrite = false);
	void SaveCharValue(std::string stringKey, const char* varName, const char value, const bool overwrite = false);
	void SaveVector3Value(std::string stringKey, const char* varName, const Vector3 value, const bool overwrite = false);

	void LoadLua(std::string stringKey, const char* fileDirectory);
	inline lua_State* GetLuaState(std::string stringKey){ return this->luaMap[stringKey].luaState; }

	void Error(const char* errorCode);

protected:
	LuaInterface();

	std::map<std::string, LuaInfo> luaMap;
};

#endif