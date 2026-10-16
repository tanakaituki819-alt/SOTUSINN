#pragma once
#include <iostream>
#include <fstream>  // ファイル操作に必要
#include <iomanip>  // std::setw（インデント整形）に必要
#include "json.hpp"

#include <string>
#include <vector>
#include <map>
//プロジェクト（P）、既存の項目を追加nlohmann_json.natvisをするとデバッカーでみやすくなる


//jsonファイルの読み書きをする
class JsonFileIO
{
public:
	JsonFileIO();
	~JsonFileIO();
	
	static bool Load(const std::string& filePath, nlohmann::json& loadedData);

	static bool SAVE(const std::string& filePath, nlohmann::json& loadedData);

	static void OTAMESILoad();
	static void OTAMESISAVE();

private:

};

