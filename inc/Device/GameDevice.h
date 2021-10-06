#pragma once
#include <Device\Input.h>
#include <Device\GameTime.h>
#include <Device\Resource\TextureManager.h>
#include <Device\Resource\ModelManager.h>
#include <Device\Resource\ShaderManager.h>
#include <Device\Resource\AudioManager.h>
#include <Utility\Random.h>
#include <Component\Graphics\RenderManager.h>

class GameDevice
{
public:
	static void init(const HINSTANCE& hInstance, const HWND& hwnd);
	static void update();
	static void shutdown();

	static Input& getInput();
	static GameTime& getGameTime();
	static TextureManager& getTextureManager();
	static ModelManager& getModelManager();
	static ShaderManager& getShaderManager();
	static RenderManager& getRenderManager();
	static AudioManager& getAudioManager();
	static Random& getRandom();

	//static Renderer* getRenderer();

private:
	GameDevice(const HINSTANCE& hinstance, const HWND& hwnd);
	~GameDevice();

	//ÉRÉsÅ[ã÷é~
	GameDevice(const GameDevice& other) = delete;
	GameDevice& operator = (const GameDevice& other) = delete;

private:
	static GameDevice* pInstance;

	Input m_Input;
	GameTime m_GameTime;
	TextureManager m_TextureManager;
	ModelManager m_ModelManager;
	ShaderManager m_ShaderManager;
	RenderManager m_RenderManager;
	AudioManager m_AudioManager;
	Random m_Random;

	//Renderer* m_pRenderer;
	//AudioManager* m_pAudioManager;
	//FontManager* m_pFontManager;
};