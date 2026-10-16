#include "JsonFileIO.h"

#define NOMINMAX
#include "windows.h"


JsonFileIO::JsonFileIO()
{
}

JsonFileIO::~JsonFileIO()
{
}

bool JsonFileIO::Load(const std::string& filePath, nlohmann::json& loadedData)
{
    // コンソールの出力文字コードを UTF-8 に変更する
    SetConsoleOutputCP(65001);



    // 1. ファイルが存在するか調べる
    if (!std::filesystem::exists(filePath)) {
        std::cerr << "[Error] ファイルが存在しません: " << filePath << std::endl;
        return false;
    }
    // 2. 読み込みモードでファイルを開く
    std::ifstream file(filePath);

    // 3. ファイルが開けたかチェック
    if (!file.is_open()) {
        std::cerr << "[Error] ファイルを開けませんでした: " << filePath << std::endl;
        return false;
    }

    // 4. JSONデータのパース（解析・読み込み）
    try {
        file >> loadedData; // ファイルの内容を json オブジェクトへ流し込む
    }
    catch (const  nlohmann::json::parse_error& e) {
        // カンマ忘れや構文エラーなど、JSON形式として壊れている場合のキャッチ
        std::cerr << "[Error] JSONのパース（解析）に失敗しました: " << e.what() << std::endl;

        file.close();
        return false;
    }


    // 5. ファイルを閉じる(無くても関数が終わると閉じる)
    file.close();

    std::cout << "[Success] ファイルの読み込みが完了しました: " << filePath << std::endl;

    return true;


}

bool JsonFileIO::SAVE(const std::string& filePath, nlohmann::json& SAVEData)
{

    // コンソールの出力文字コードを UTF-8 に変更する
    SetConsoleOutputCP(65001);

    // 1. ファイルを開く（存在しなければ新規作成される）
    std::ofstream file(filePath);

    // 2. 開けたかチェック（フォルダが存在しないなどの場合は失敗する）
    if (!file.is_open()) {
        std::cerr << "[Error] ファイルを作成できませんでした: " << filePath << std::endl;
        return false;
    }
    
    // 3. JSONデータを整形して出力 (4スペースインデント)
    file << std::setw(4) << SAVEData << std::endl;

    // 4. ファイルを閉じる
    file.close();

    std::cout << "[Success] ファイルの書き込みが完了しました" << std::endl;
    return true;
}

void JsonFileIO::OTAMESILoad()
{
    //Load関数の使い方
    nlohmann::json loadedData;

    Load("TEST.json", loadedData);

 
 
    // --- 中身のデータを取り出す ---
    // .value("キー名", 初期値) を使うと安全に取り出せます
    std::string gameName = loadedData.value("game_name", "不明");
    //または score が無くても 0 が入る
    int score = loadedData["player"].value("score", 0);

    std::cout << "ゲーム名: " << gameName << std::endl;
    std::cout << "スコア: " << score << std::endl;

    return;
    // player オブジェクトが存在しない場合に安全な書き方
    //int score = 0;
    //      プレイヤーがあって               それがオブジェクトなら 
    //if (loadedData.contains("player") && loadedData["player"].is_object()) {
    //    score = loadedData["player"].value("score", 0);
    //}

    //一度に取る（_json_pointerを書く必要あり）
    //int score = loadedData.value("/player/score"_json_pointer, 0);
}

void JsonFileIO::OTAMESISAVE()
{
    // 1. 実験用に書き込むテストデータを準備
    //コード上で代入した順番は関係なく、キー（文字列）の頭文字でソートされて保存される
    nlohmann::json testData;
    testData["game_name"] = "勇者の冒険";//文字列
    testData["player"]["score"] = 1000;//数字（int型）
    testData["Timu"] = 1000.f;//数字（float型）
    testData["player"]["is_completed"] = false;//bool型
    testData["player"]["nowLife"] = "0";//文字列の数字
    testData["player"]["items"] = { "sword", "potion" };
    testData["player"]["isWeapon"] = nullptr;//NULL型
    // std::vector をそのまま代入（配列になる）
    std::vector<std::string> itemList = { "sword", "shield", "potion" };
    testData["inventory"] = itemList; // 配列型

    // std::map をそのまま代入（連想配列/オブジェクトになる）
    std::map<std::string, int> statusMap = { {"STR", 15}, {"DEF", 10} };
    testData["status"] = statusMap; // オブジェクト型

    //nlohmann::json enemy1 = { {"name", "スライム"}, {"hp", 10} };
    //nlohmann::json enemy2 = { {"name", "ドラゴン"}, {"hp", 500} };

    //testData["enemies"] = { enemy1, enemy2 }; // オブジェクトの配列
    // オブジェクトの配列
    testData["enemies"] = nlohmann::json::array();   // 空の配列を作る
    testData["enemies"].push_back({ {"name", "スライム"}, {"hp", 10} });
    testData["enemies"].push_back({ {"name", "ドラゴン"}, {"hp", 500} });

    SAVE("TEST.json", testData);
}
