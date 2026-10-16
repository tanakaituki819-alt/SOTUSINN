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




    // 1. ファイルが存在するか調べる
    if (!std::filesystem::exists(ToPath(filePath))) {
        std::cerr << "[Error] ファイルが存在しません: " << filePath << std::endl;
        return false;
    }
    // 2. 読み込みモードでファイルを開く
    std::ifstream file(ToPath(filePath));

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

 

    // 1. ファイルを開く（存在しなければ新規作成される）
    std::ofstream file(ToPath(filePath));

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
// ---- 暗号化版 ----
//  ファイル形式: [4byte マジック "JENC"][4byte 平文のハッシュ][暗号化された本文]
bool JsonFileIO::SaveEncrypted(const std::string& filePath, const nlohmann::json& saveData, const std::string& key)
{


    if (key.empty()) {
        std::cerr << "[Error] 暗号化キーが空です" << std::endl;
        return false;
    }

    //  JSON を文字列に変換する
    //    -1    : インデントなし（1行にまとめる。ファイルサイズが小さくなる）
    //    ' '   : インデントに使う文字（-1 なので実際は使われない）
    //    false : 日本語を \uXXXX に変換せず、UTF-8 のまま出力する
    //    replace : 不正な UTF-8 が混ざっていても、例外を出さず代替文字に置き換える
    std::string plain = saveData.dump(-1, ' ', false, nlohmann::json::error_handler_t::replace);

    // 平文のハッシュを先に計算（復号後の検証用）
    // 暗号化する「前」の文字列から、ハッシュを計算しておく
    //  読み込み時に「正しく復号できたか」を確認するために使う
    uint32_t hash = Hash(plain);

    // 文字列のコピーを作って暗号化する
    //（XorCrypt は渡した文字列を直接書き換えるので、コピーを渡している）
    std::string encrypted = plain;
    XorCrypt(encrypted, key);

    // 4. バイナリモードで書き込む（テキストモードだと改行コードが変換されて壊れる）
    std::ofstream file(ToPath(filePath), std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "[Error] ファイルを作成できませんでした: " << filePath << std::endl;
        return false;
    }
// ファイルの先頭から順に書き込む
// [4バイト: "JENC"][4バイト: ハッシュ][暗号化された本文]

// マジック文字列：「このアプリが作った暗号化ファイルか」を判別するための目印
    //先頭から文字分書く
    file.write(MAGIC.data(), static_cast<std::streamsize>(MAGIC.size()));
   

    // ハッシュ値（uint32_t = 4バイト）をそのままのバイト列として書く
    file.write(reinterpret_cast<const char*>(&hash), sizeof(hash));
    // 暗号化された本文
    file.write(encrypted.data(), static_cast<std::streamsize>(encrypted.size()));
    // 書き込みに失敗していないか確認（ディスク容量不足など）
    if (!file) {
        std::cerr << "[Error] ファイルの書き込みに失敗しました: " << filePath << std::endl;
        return false;
    }

    std::cout << "[Success] 暗号化して保存しました: " << filePath << std::endl;
    return true;
}

bool JsonFileIO::LoadEncrypted(const std::string& filePath, nlohmann::json& loadedData, const std::string& key)
{

    if (key.empty()) {
        std::cerr << "[Error] 暗号化キーが空です" << std::endl;
        return false;
    }

    if (!std::filesystem::exists(ToPath(filePath))) {
        std::cerr << "[Error] ファイルが存在しません: " << filePath << std::endl;
        return false;
    }

    // 1. バイナリモードで開く
    std::ifstream file(ToPath(filePath), std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "[Error] ファイルを開けませんでした: " << filePath << std::endl;
        return false;
    }

    // 2. ヘッダ(マジック + ハッシュ)を読む
    std::string magic(MAGIC.size(), '\0');     // MAGIC と同じ長さの入れ物を用意
    // ハッシュを入れる変数（4バイトの整数）
    uint32_t savedHash = 0;
    // ② ファイルの先頭から、magic のバイト数ぶんを読み込む
//    magic.data() は入れ物の先頭アドレス。ここに直接書き込まれる
    file.read(magic.data(), static_cast<std::streamsize>(magic.size()));
    // ③ 続けて、次の4バイトを savedHash に読み込む
    //    ファイルの読み取り位置は、直前の read の続きから進む
    file.read(reinterpret_cast<char*>(&savedHash), sizeof(savedHash));

    // ④ 確認する
//    !file      : 読み込みに失敗した（ファイルが短すぎる、など）
//    magic != MAGIC : 読み取った文字列が、期待するマジックと違う
    if (!file || magic != MAGIC) {             // 1行で全文字を比較できる
        std::cerr << "[Error] 暗号化ファイルの形式が正しくありません: " << filePath << std::endl;
        return false;
    }

    // 3. 残りの本文をすべて読む
    std::string data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    // 4. 復号
    XorCrypt(data, key);

    // 5. ハッシュ照合（鍵が違う・ファイルが改ざんされた場合はここで弾く）
    if (Hash(data) != savedHash) {
        std::cerr << "[Error] 復号に失敗しました（キー違い、またはファイルが改ざんされています）" << std::endl;
        return false;
    }

    // 6. JSONとしてパース
    try {
        loadedData = nlohmann::json::parse(data);
    }
    catch (const nlohmann::json::parse_error& e) {
        std::cerr << "[Error] JSONのパース（解析）に失敗しました: " << e.what() << std::endl;
        return false;
    }

    std::cout << "[Success] 復号して読み込みました: " << filePath << std::endl;
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

void JsonFileIO::XorCrypt(std::string& data, const std::string& key)
{
    if (key.empty()) return;
    // データを1バイトずつ順番に処理する（i は先頭からの位置）
    for (size_t i = 0; i < data.size(); ++i) {
        //  鍵から1文字取り出す
        //    i % key.size() で、鍵の長さを超えたら先頭に戻る（鍵を繰り返し使う）
        //    例: 鍵が "ABC" なら A,B,C,A,B,C,... の順に使われる
        uint8_t k = static_cast<uint8_t>(key[i % key.size()]);

        //  元のデータ・鍵の文字・位置の値の3つを XOR して、その位置の文字を上書きする
        data[i] = static_cast<char>(static_cast<uint8_t>(data[i]) ^ k );
    }
}
//保存するとき、暗号化する前の JSON 文字列のハッシュを計算して、ファイルの先頭に書いています。
// 読み込むときは、復号したデータのハッシュをもう一度計算して、保存してあった値と比べます。
uint32_t JsonFileIO::Hash(const std::string& data)
{
    uint32_t h = 2166136261u;      // ① 決まった初期値からスタート
    for (unsigned char c : data) {
        h ^= c;                    // ② 1バイトずつ XOR で混ぜる
        h *= 16777619u;            // ③ 決まった数を掛けて散らす
    }
    return h;
}

std::filesystem::path JsonFileIO::ToPath(const std::string& utf8Path)
{
    if (utf8Path.empty()) return {};

    // 変換後に必要な文字数（wchar_t の数）を調べる
    int len = MultiByteToWideChar(CP_UTF8, 0,
        utf8Path.data(), static_cast<int>(utf8Path.size()),
        nullptr, 0);
    if (len <= 0) return std::filesystem::path(utf8Path);   // 変換できなければそのまま返す

    // その長さの入れ物を作り、UTF-8 → UTF-16 に変換して書き込む
    std::wstring wide(len, L'\0');
    MultiByteToWideChar(CP_UTF8, 0,
        utf8Path.data(), static_cast<int>(utf8Path.size()),
        wide.data(), len);

    // wstring から作った path は、Windows が日本語として正しく扱える
    return std::filesystem::path(wide);
}
