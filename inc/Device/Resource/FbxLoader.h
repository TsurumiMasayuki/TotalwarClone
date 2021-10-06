#pragma once

#include "fbxsdk.h"

#include <d3d12.h>
#include <d3dx12.h>

#include <Device\Resource\Model.h>

class FbxLoader
{
public:
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static FbxLoader* getInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="device">D3D12デバイス</param>
	void initialize(ID3D12Device* pDevice);

	/// <summary>
	/// 後始末
	/// </summary>
	void finalize();

	/// <summary>
	/// ファイルからFBXモデル読み込み
	/// </summary>
	/// <param name="modelName">モデル名</param>
	Model* loadModelFromFile(const std::string& folderPath, const std::string& modelName);

	/// <summary>
	/// 再帰的にノード構成を解析
	/// </summary>
	/// <param name="pModel"></param>
	/// <param name="pFbxNode"></param>
	void parseNodeRecursive(Model* pModel, FbxNode* pFbxNode, Node* pParent = nullptr);

	/// <summary>
	/// メッシュ読み取り
	/// </summary>
	/// <param name="pModel">読み込み先のモデルオブジェクト</param>
	/// <param name="pFbxNode">解析対象のノード</param>
	void parseMesh(Model* pModel, Node* pNode, FbxNode* pFbxNode);

private:
	// privateなコンストラクタ（シングルトンパターン）
	FbxLoader() = default;
	// privateなデストラクタ（シングルトンパターン）
	~FbxLoader() = default;
	// コピーコンストラクタを禁止（シングルトンパターン）
	FbxLoader(const FbxLoader& obj) = delete;
	// コピー代入演算子を禁止（シングルトンパターン）
	void operator=(const FbxLoader& obj) = delete;

	//ファイルパスからファイル名を抽出する
	std::string extractFileName(const std::string& path);

	//FBXの行列をDirectXMathの行列に変換する
	void convertMatrixFromFbx(const FbxAMatrix& src, DirectX::XMMATRIX& dest);

private:
	//D3D12デバイス
	ID3D12Device* m_pDevice;
	//Fbxマネージャー
	fbxsdk::FbxManager* m_pFbxManager;
	//Fbxインポーター
	fbxsdk::FbxImporter* m_pFbxImporter;
};