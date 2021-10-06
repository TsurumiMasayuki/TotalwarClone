#pragma once
#include <typeindex>
#include <unordered_map>
#include <cassert>
#include <Graphics\DX12\Shader\AbstractVertexShader.h>
#include <Graphics\DX12\Shader\AbstractGeometryShader.h>
#include <Graphics\DX12\Shader\AbstractPixelShader.h>

class ShaderManager
{
public:
	template<class T>
	T* loadVS();
	template<class T>
	T* loadGS();
	template<class T>
	T* loadPS();

	void unloadAll();

	template<class T>
	T* getVertexShader();
	template<class T>
	T* getGeometryShader();
	template<class T>
	T* getPixelShader();

private:
	std::unordered_map<std::type_index, AbstractVertexShader*> m_VertexShaders;
	std::unordered_map<std::type_index, AbstractGeometryShader*> m_GeometryShaders;
	std::unordered_map<std::type_index, AbstractPixelShader*> m_PixelShaders;
};

template<class T>
inline T* ShaderManager::loadVS()
{
	auto pVS = new T();
	//シェーダーの型をチェック
	auto pCheck = dynamic_cast<AbstractVertexShader*>(pVS);
	assert(pCheck != nullptr);

	pCheck->init();
	m_VertexShaders.emplace(typeid(T), pVS);

	return pVS;
}

template<class T>
inline T* ShaderManager::loadGS()
{
	auto pGS = new T();
	//シェーダーの型をチェック
	auto pCheck = dynamic_cast<AbstractGeometryShader*>(pGS);
	assert(pCheck != nullptr);

	pCheck->init();
	m_GeometryShaders.emplace(typeid(T), pGS);

	return pGS;
}

template<class T>
inline T* ShaderManager::loadPS()
{
	auto pPS = new T();
	//シェーダーの型をチェック
	auto pCheck = dynamic_cast<AbstractPixelShader*>(pPS);
	assert(pCheck != nullptr);

	pCheck->init();
	m_PixelShaders.emplace(typeid(T), pPS);

	return pPS;
}

template<class T>
inline T* ShaderManager::getVertexShader()
{
	//見つからなかったらロード
	if (m_VertexShaders.count(typeid(T)) == 0)
	{
		return loadVS<T>();
	}

	return dynamic_cast<T*>(m_VertexShaders.at(typeid(T)));
}

template<class T>
inline T* ShaderManager::getGeometryShader()
{
	//見つからなかったらロード
	if (m_GeometryShaders.count(typeid(T)) == 0)
	{
		return loadGS<T>();
	}

	return dynamic_cast<T*>(m_GeometryShaders.at(typeid(T)));
}

template<class T>
inline T* ShaderManager::getPixelShader()
{
	//見つからなかったらロード
	if (m_PixelShaders.count(typeid(T)) == 0)
	{
		return loadPS<T>();
	}

	return dynamic_cast<T*>(m_PixelShaders.at(typeid(T)));
}
