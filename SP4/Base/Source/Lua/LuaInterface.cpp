#include "LuaInterface.h"

#include <iostream>

using std::cout;
using std::cin;
using std::endl;

LuaInterface::LuaInterface()
{
}

LuaInterface::~LuaInterface()
{
}

bool LuaInterface::Init()
{
	bool result = false;

	luaMap["LuaState"].luaState = lua_open();
	if (luaMap["LuaState"].luaState)
	{
		//Load lua auxiliary libraries
		luaL_openlibs(luaMap["LuaState"].luaState);
		//Load the error lua script
		luaMap["LuaState"].fileDirectory = "LuaScripts//DM2240.lua";
		luaL_dofile(luaMap["LuaState"].luaState, luaMap["LuaState"].fileDirectory);
		
		result = true;
	}

	LoadLua("ErrorState", "LuaScripts//errorLookup.lua");

	LoadLua("PlayerSettings", "LuaScripts//PlayerSettings.lua");

	LoadLua("ApplicationSettings", "LuaScripts//ApplicationSettings.lua");

	return result;
}

void LuaInterface::LoadLua(std::string stringKey, const char* fileDirectory)
{
	luaMap[stringKey].luaState = lua_open();
	if (luaMap["LuaState"].luaState && luaMap[stringKey].luaState)
	{
		//Load lua auxiliary libraries
		luaL_openlibs(luaMap["LuaState"].luaState);
		//Load the error lua script
		luaMap[stringKey].fileDirectory = fileDirectory;
		luaL_dofile(luaMap[stringKey].luaState, fileDirectory);
	}
}

void LuaInterface::Run()
{
	//if (luaMap["LuaState"].luaState == NULL)
	//	return;

	//lua_getglobal(luaMap["LuaState"], "title");
	//const char* title = lua_tostring(luaMap["LuaState"], -1);

	//lua_getglobal(luaMap["LuaState"], "width");
	//int width = lua_tointeger(luaMap["LuaState"], -1);

	//lua_getglobal(luaMap["LuaState"], "height");
	//int height = lua_tointeger(luaMap["LuaState"], -1);

	//std::cout << title << std::endl;
	//std::cout << "-----------------------------------------------------------" << std::endl;
	//std::cout << "Width of screen : " << width << std::endl;
	//std::cout << "Height of screen : " << height << std::endl;
}

void LuaInterface::Drop()
{
	for (auto q : luaMap)
	{
		if (q.second.luaState)
			lua_close(q.second.luaState);
	}
}

int LuaInterface::GetIntValue(std::string stringKey, const char* varName)
{
	lua_getglobal(luaMap[stringKey].luaState, varName);
	return lua_tointeger(luaMap[stringKey].luaState, -1);
}

float LuaInterface::GetFloatValue(std::string stringKey, const char* varName)
{
	lua_getglobal(luaMap[stringKey].luaState, varName);
	return lua_tonumber(luaMap[stringKey].luaState, -1);
}

char LuaInterface::GetCharValue(std::string stringKey, const char* varName){
	lua_getglobal(luaMap[stringKey].luaState, varName);

	size_t len;
	const char* cstr = lua_tolstring(luaMap[stringKey].luaState, -1, &len);

	//if string not empty, return first char
	if (len > 0)
		return cstr[0];

	//esle return default vlaue of <Space>
	else
		return ' ';
}

Vector3 LuaInterface::GetVector3Value(std::string stringKey, const char* varName){
	lua_getglobal(luaMap[stringKey].luaState, varName);
	lua_rawgeti(luaMap[stringKey].luaState, -1, 1);
	float x = lua_tonumber(luaMap[stringKey].luaState, -1);
	lua_pop(luaMap[stringKey].luaState, 1);
	lua_rawgeti(luaMap[stringKey].luaState, -1, 2);
	float y = lua_tonumber(luaMap[stringKey].luaState, -1);
	lua_pop(luaMap[stringKey].luaState, 1);
	lua_rawgeti(luaMap[stringKey].luaState, -1, 3);
	float z = lua_tonumber(luaMap[stringKey].luaState, -1);
	lua_pop(luaMap[stringKey].luaState, 1);

	return Vector3(x, y, z);
}

float LuaInterface::GetField(std::string stringKey, const char* key)
{
	float result = 0.0f;

	//Check if the variables in the lua stack belongs to a table
	if (!lua_istable(luaMap[stringKey].luaState, -1))
		Error("error100");

	lua_pushstring(luaMap[stringKey].luaState, key);
	lua_gettable(luaMap[stringKey].luaState, -2);
	if (!lua_isnumber(luaMap[stringKey].luaState, -1))
		Error("error101");
	result = (float)lua_tonumber(luaMap[stringKey].luaState, -1);
	lua_pop(luaMap[stringKey].luaState, 1); //remove number
	
	return result;
}

int LuaInterface::GetVariableValues(std::string stringKey, const char* varName, int &a, int &b, int&c, int&d){
	lua_getglobal(luaMap[stringKey].luaState, varName);
	lua_pushnumber(luaMap[stringKey].luaState, 1000);
	lua_pushnumber(luaMap[stringKey].luaState, 2000);
	lua_pushnumber(luaMap[stringKey].luaState, 3000);
	lua_pushnumber(luaMap[stringKey].luaState, 4000);
	lua_call(luaMap[stringKey].luaState, 4, 4);
	a = lua_tonumber(luaMap[stringKey].luaState, -1);
	lua_pop(luaMap[stringKey].luaState, 1);
	b = lua_tonumber(luaMap[stringKey].luaState, -1);
	lua_pop(luaMap[stringKey].luaState, 1);
	c = lua_tonumber(luaMap[stringKey].luaState, -1);
	lua_pop(luaMap[stringKey].luaState, 1);
	d = lua_tonumber(luaMap[stringKey].luaState, -1);
	lua_pop(luaMap[stringKey].luaState, 1);

	return true;
}

void LuaInterface::SaveIntValue(std::string stringKey, const char* varName, const int value, const bool overwrite)
{
	lua_getglobal(luaMap["LuaState"].luaState, "SaveToLuaFile");
	char outputString[80];
	sprintf(outputString, "%s = %d\n", varName, value);
	lua_pushstring(luaMap["LuaState"].luaState, luaMap[stringKey].fileDirectory);
	lua_pushstring(luaMap["LuaState"].luaState, outputString);
	lua_pushboolean(luaMap["LuaState"].luaState, overwrite);
	lua_call(luaMap["LuaState"].luaState, 3, 0); std::cout << "......................";
}

void LuaInterface::SaveFloatValue(std::string stringKey, const char* varName, const float value, const bool overwrite)
{
	lua_getglobal(luaMap["LuaState"].luaState, "SaveToLuaFile");
	char outputString[80];
	sprintf(outputString, "%s = %6.4f\n", varName, value);
	lua_pushstring(luaMap["LuaState"].luaState, luaMap[stringKey].fileDirectory);
	lua_pushstring(luaMap["LuaState"].luaState, outputString);
	lua_pushboolean(luaMap["LuaState"].luaState, overwrite);
	lua_call(luaMap["LuaState"].luaState, 3, 0); std::cout << "......................";
}

void LuaInterface::SaveCharValue(std::string stringKey, const char* varName, const char value, const bool overwrite)
{
	lua_getglobal(luaMap["LuaState"].luaState, "SaveToLuaFile");
	char outputString[80];
	sprintf(outputString, "%s = \"%c\"\n", varName, value);
	std::cout << outputString << std::endl;
	lua_pushstring(luaMap["LuaState"].luaState, luaMap[stringKey].fileDirectory);
	lua_pushstring(luaMap["LuaState"].luaState, outputString);
	lua_pushboolean(luaMap["LuaState"].luaState, overwrite);
	lua_call(luaMap["LuaState"].luaState, 3, 0); std::cout << "......................";
}

void LuaInterface::SaveVector3Value(std::string stringKey, const char* varName, const Vector3 value, const bool overwrite)
{
	lua_getglobal(luaMap["LuaState"].luaState, "SaveToLuaFile");
	char outputString[80];
	sprintf(outputString, "%s = {%6.4f, %6.4f, %6.4f}\n", varName, value.x, value.y, value.z);
	lua_pushstring(luaMap["LuaState"].luaState, luaMap[stringKey].fileDirectory);
	lua_pushstring(luaMap["LuaState"].luaState, outputString);
	lua_pushboolean(luaMap["LuaState"].luaState, overwrite);
	lua_call(luaMap["LuaState"].luaState, 3, 0); std::cout << "......................";
}

float LuaInterface::GetDistanceSquareValue(const char* varName, Vector3 source, Vector3 destination){
	lua_getglobal(luaMap["LuaState"].luaState, varName);
	lua_pushnumber(luaMap["LuaState"].luaState, source.x);
	lua_pushnumber(luaMap["LuaState"].luaState, source.y);
	lua_pushnumber(luaMap["LuaState"].luaState, source.z);
	lua_pushnumber(luaMap["LuaState"].luaState, destination.x);
	lua_pushnumber(luaMap["LuaState"].luaState, destination.y);
	lua_pushnumber(luaMap["LuaState"].luaState, destination.z);
	lua_call(luaMap["LuaState"].luaState, 6, 1);
	float distanceSquare = (float)lua_tonumber(luaMap["LuaState"].luaState, -1);
	lua_pop(luaMap["LuaState"].luaState, 1);

	return distanceSquare;
}

void LuaInterface::Error(const char* errorCode)
{
	if (luaMap["ErrorState"].luaState == NULL)
		return;

	lua_getglobal(luaMap["ErrorState"].luaState, errorCode);
	const char* errorMsg = lua_tostring(luaMap["ErrorState"].luaState, -1);
	if (errorMsg != NULL)
		std::cout << errorMsg << std::endl;
	else
		std::cout << errorCode << " is not valid.\n*** Please contact the developer ***" << std::endl;

}