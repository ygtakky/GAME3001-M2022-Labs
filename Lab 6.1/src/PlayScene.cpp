#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"
#include "InputType.h"
#include "Renderer.h"
#include "Util.h"
#include "SoundManager.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"

PlayScene::PlayScene()
{
	PlayScene::Start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::Draw()
{
	DrawDisplayList();
	SDL_SetRenderDrawColor(Renderer::Instance().GetRenderer(), 255, 255, 255, 255);
}

void PlayScene::Update()
{
	UpdateDisplayList();
}

void PlayScene::Clean()
{
	RemoveAllChildren();
}


void PlayScene::HandleEvents()
{
	EventManager::Instance().Update();

	GetPlayerInput();

	GetKeyboardInput();
}

void PlayScene::GetPlayerInput()
{
	switch (m_pCurrentInputType)
	{
	case static_cast<int>(InputType::GAME_CONTROLLER):
	{
		// handle player movement with GameController
		if (SDL_NumJoysticks() > 0)
		{
			if (EventManager::Instance().GetGameController(0) != nullptr)
			{
				constexpr auto dead_zone = 10000;
				if (EventManager::Instance().GetGameController(0)->STICK_LEFT_HORIZONTAL > dead_zone)
				{
					
				}
				else if (EventManager::Instance().GetGameController(0)->STICK_LEFT_HORIZONTAL < -dead_zone)
				{
					
				}
				else
				{
					
				}
			}
		}
	}
	break;
	case static_cast<int>(InputType::KEYBOARD_MOUSE):
	{
		// handle player movement with mouse and keyboard
		if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_A))
		{
			
		}
		else if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_D))
		{
			
		}
		else
		{
			
		}
	}
	break;
	case static_cast<int>(InputType::ALL):
	{
		if (SDL_NumJoysticks() > 0)
		{
			if (EventManager::Instance().GetGameController(0) != nullptr)
			{
				constexpr auto dead_zone = 10000;
				if (EventManager::Instance().GetGameController(0)->STICK_LEFT_HORIZONTAL > dead_zone
					|| EventManager::Instance().IsKeyDown(SDL_SCANCODE_D))
				{
					
				}
				else if (EventManager::Instance().GetGameController(0)->STICK_LEFT_HORIZONTAL < -dead_zone
					|| EventManager::Instance().IsKeyDown(SDL_SCANCODE_A))
				{
					
				}
				else
				{
					
				}
			}
		}
		else if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_A))
		{
			
		}
		else if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_D))
		{
			
		}
		else
		{
			
		}
	}
	break;
	}
}

void PlayScene::GetKeyboardInput()
{
	if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_ESCAPE))
	{
		Game::Instance().Quit();
	}

	if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_1))
	{
		Game::Instance().ChangeSceneState(SceneState::START);
	}

	if (EventManager::Instance().IsKeyDown(SDL_SCANCODE_2))
	{
		Game::Instance().ChangeSceneState(SceneState::END);
	}
}

void PlayScene::BuildObstaclePool()
{
	for (int i = 0; i < 3; ++i)
	{
		m_pObstacles.push_back(new Obstacle());
	}
}

void PlayScene::m_buildGrid()
{
}

void PlayScene::m_toggleGrid(bool state)
{
}

void PlayScene::m_checkShipLOS(DisplayObject* target_object) const
{
}

void PlayScene::m_storeObstacles()
{
}

void PlayScene::m_clearNodes()
{
}


void PlayScene::Start()
{
	// Set GUI Title
	m_guiTitle = "Play Scene";

	// Set Input Type
	m_pCurrentInputType = static_cast<int>(InputType::KEYBOARD_MOUSE);

	// Add Game Objects
	m_pTarget = new Target();
	m_pTarget->GetTransform()->position = glm::vec2(600.0f, 300.0f);
	AddChild(m_pTarget);

	m_pStarship = new Starship();
	m_pStarship->GetTransform()->position = glm::vec2(150.0f, 300.0f);
	AddChild(m_pStarship, 2);

	// Add Obstacles
	BuildObstaclePool();

	m_pObstacles[0]->GetTransform()->position = glm::vec2(380.0f, 80.0f);
	m_pObstacles[0]->SetHeight(50);
	AddChild(m_pObstacles[0]);

	m_pObstacles[1]->GetTransform()->position = glm::vec2(380.0f, 280.0f);
	m_pObstacles[1]->SetWidth(100);
	AddChild(m_pObstacles[1]);

	m_pObstacles[2]->GetTransform()->position = glm::vec2(380.0f, 480.0f);
	AddChild(m_pObstacles[2]);

	// preload sounds
	SoundManager::Instance().Load("../Assets/audio/yay.ogg", "yay", SoundType::SOUND_SFX);
	SoundManager::Instance().Load("../Assets/audio/thunder.ogg", "thunder", SoundType::SOUND_SFX);

	/* DO NOT REMOVE */
	ImGuiWindowFrame::Instance().SetGuiFunction([this] { GUI_Function(); });
}

void PlayScene::GUI_Function()
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();

	ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
	ImGui::Begin(m_guiTitle.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove);

	ImGui::Text("Player Input");
	ImGui::RadioButton("Keyboard / Mouse", &m_pCurrentInputType, static_cast<int>(InputType::KEYBOARD_MOUSE)); ImGui::SameLine();
	ImGui::RadioButton("Game Controller", &m_pCurrentInputType, static_cast<int>(InputType::GAME_CONTROLLER)); ImGui::SameLine();
	ImGui::RadioButton("Both", &m_pCurrentInputType, static_cast<int>(InputType::ALL));

	ImGui::Separator();

	if (ImGui::Checkbox("Toggle Grid", &m_isGridEnabled))
	{
		m_toggleGrid(m_isGridEnabled);
	}

	ImGui::Separator();

	// spaceship properties

	static int shipPosition[] = { static_cast<int>(m_pStarship->GetTransform()->position.x), static_cast<int>(m_pStarship->GetTransform()->position.y)};
	if (ImGui::SliderInt2("Ship Position", shipPosition, 0, 800))
	{
		m_pStarship->GetTransform()->position.x = static_cast<float>(shipPosition[0]);
		m_pStarship->GetTransform()->position.y = static_cast<float>(shipPosition[1]);
	}

	// allow the ship to rotate
	static int angle;
	if (ImGui::SliderInt("Ship Direction", &angle, -360, 360))
	{
		m_pStarship->SetCurrentHeading(static_cast<float>(angle));
	}

	// Target properties

	static int targetPosition[] = { static_cast<int>( m_pTarget->GetTransform()->position.x), static_cast<int>(m_pTarget->GetTransform()->position.y) };
	if (ImGui::SliderInt2("Target Position", targetPosition, 0, 800))
	{
		m_pTarget->GetTransform()->position.x = static_cast<float>(targetPosition[0]);
		m_pTarget->GetTransform()->position.y = static_cast<float>(targetPosition[1]);
	}

	ImGui::Separator();

	// Add Obstacle Position Control for all obstacles
	for (unsigned i = 0; i < m_pObstacles.size(); ++i)
	{
		int obstaclePosition[] = { static_cast<int>(m_pObstacles[i]->GetTransform()->position.x), static_cast<int>(m_pObstacles[i]->GetTransform()->position.y) };
		std::string label = "Obstacle" + std::to_string(i + 1) + " Position";
		if (ImGui::SliderInt2(label.c_str(), obstaclePosition, 0, 800))
		{
			m_pObstacles[i]->GetTransform()->position.x = static_cast<float>(obstaclePosition[0]);
			m_pObstacles[i]->GetTransform()->position.y = static_cast<float>(obstaclePosition[1]);
			m_buildGrid();
		}
	}

	ImGui::Separator();

	
	ImGui::End();
}