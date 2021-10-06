#pragma once
#include <string>
#include <unordered_map>
#include <wrl.h>
#include <d3d12.h>

#include <Graphics\DX12\DX12Texture2D.h>

//TODO 
//オートロードの実装
//アンロードの実装(利用者に通知したり色々やらないといけない)

class TextureManager
{
public:
	void init();

	//テクスチャのロード
	void load(const std::string& key, const std::wstring& filePath);
	//全てアンロード
	void unloadAll();

	//テクスチャの取得
	DX12Texture2D* getTexture(const std::string& key) const;

private:
	std::unordered_map<std::string, DX12Texture2D*> m_Textures;
};