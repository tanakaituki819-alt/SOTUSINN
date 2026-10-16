#pragma once
#include <iostream>
#include <fstream>  // ファイル操作に必要
#include <iomanip>  // std::setw（インデント整形）に必要
#include "json.hpp"

#include <string>
#include <vector>
#include <map>
#include <string_view>
#include <cstdint>
//プロジェクト（P）、既存の項目を追加nlohmann_json.natvisをするとデバッカーでみやすくなる

#include <filesystem>  

//jsonファイルの読み書きをする
class JsonFileIO
{
private:
	static inline const std::string Key = "Key";
	static inline constexpr std::string_view MAGIC = "ITUKI";//string_view(読み取り専用)//一番最初につく
public:
	JsonFileIO();
	~JsonFileIO();
	
	static bool Load(const std::string& filePath, nlohmann::json& loadedData);

	static bool SAVE(const std::string& filePath, nlohmann::json& loadedData);

	// ---- 暗号化版 ----
// JSONを暗号化してバイナリファイルとして保存する
	static bool SaveEncrypted(const std::string& filePath, const nlohmann::json& saveData, const std::string& key= Key);
	// 暗号化されたファイルを復号してJSONとして読み込む
	static bool LoadEncrypted(const std::string& filePath, nlohmann::json& loadedData, const std::string& key = Key);


	static void OTAMESILoad();
	static void OTAMESISAVE();


private:
	// XORベースの暗号化・復号（同じ処理で往復できる）
	static void XorCrypt(std::string& data, const std::string& key);
	// 改ざん・鍵違い検出用の簡易ハッシュ(FNV-1a)
	static uint32_t Hash(const std::string& data);
	// UTF-8 の文字列を、Windows で日本語ファイル名が使える path に変換する
	static std::filesystem::path ToPath(const std::string& utf8Path);
};

