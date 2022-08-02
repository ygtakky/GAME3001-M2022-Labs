#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"
#include "InputType.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"

// required for Scene
#include "Renderer.h"
#include "Util.h"
#include "PathNode.h"
#include "Config.h"
#include <fstream>

PlayScene::PlayScene()
{
	PlayScene::Start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::Draw()
{
	DrawDisplayList();

	if(m_isGridEnabled)
	{
		// draws the collision bounds of each obstacle
		for (const auto obstacle : m_pObstacles)
		{
			auto offset = glm::vec2(obstacle->GetWidth() * 0.5f, obstacle->GetHeight() * 0.5f);
			Util::DrawRect(obstacle->GetTransform()->position - offset, obstacle->GetWidth(), obstacle->GetHeight());
		}

		// Radius
		auto detected = m_pStarShip->GetTree()->GetPlayerDetectedNode()->GetDetected();
		Util::DrawCircle(m_pStarShip->GetTransform()->position, 300.0f, detected ? glm::vec4(0, 1, 0, 1) : glm::vec4(1, 0, 0, 1));
	}

	SDL_SetRenderDrawColor(Renderer::Instance().GetRenderer(), 255, 255, 255, 255);
}

void PlayScene::Update()
{
	UpdateDisplayList();

	// Sets up Ranged Combat Enemy
	m_pStarShip->GetTree()->GetEnemyHealthNode()->SetHealth(m_pStarShip->GetHealth() > 25);
	m_pStarShip->GetTree()->GetEnemyHitNode()->SetIsHit(false);
	m_pStarShip->CheckAgentLOSToTarget(m_pStarShip, m_pTarget, m_pObstacles);

	// Distance Check between Starship and Target for Detection Radius
	float distance = Util::Distance(m_pStarShip->GetTransform()->position, m_pTarget->GetTransform()->position);

	// Radius detection...just outside of LOS range (around 300 px)
	m_pStarShip->GetTree()->GetPlayerDetectedNode()->SetDetected(distance < 300);

	// Within LOS distance...but not too close (optimum firing range)
	m_pStarShip->GetTree()->GetRangedCombatNode()->SetIsWithinCombatRange(distance >= 200 && distance <= 350);

	switch(m_LOSMode)
	{
	case LOSMode::TARGET:
		m_checkAllNodesWithTarget(m_pTarget);
		break;
	case LOSMode::SHIP:
		m_checkAllNodesWithTarget(m_pStarShip); 
		break;
	case LOSMode::BOTH:
		m_checkAllNodesWithBoth(); 
		break;
	}

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

	if(EventManager::Instance().KeyPressed(SDL_SCANCODE_K))
	{
		m_pStarShip->TakeDamage(25); // StarShip takes damage
		m_pStarShip->GetTree()->GetEnemyHitNode()->SetIsHit(true);
		std::cout << "StarShip at: " << m_pStarShip->GetHealth() << "%. " << std::endl;
	}

	if (EventManager::Instance().KeyPressed(SDL_SCANCODE_R))
	{
		m_pStarShip->SetHealth(100); // Reset health
		m_pStarShip->GetTree()->GetEnemyHitNode()->SetIsHit(false);
		m_pStarShip->GetTree()->GetPlayerDetectedNode()->SetDetected(false);

		std::cout << "Conditions have been Reset" << std::endl;
	}
}

void PlayScene::BuildObstaclePool()
{
	std::ifstream inFile("../Assets/data/obstacles.txt");

	while(!inFile.eof())
	{
		std::cout << "Obstacle" << std::endl;
		auto obstacle = new Obstacle();
		float x, y, w, h; // same way the file is organized
		inFile >> x >> y >> w >> h; // read data from the file line by line
		obstacle->GetTransform()->position = glm::vec2(x, y);
		obstacle->SetWidth(w);
		obstacle->SetHeight(h);
		AddChild(obstacle, 2);
		m_pObstacles.push_back(obstacle);
	}
	inFile.close();
}

void PlayScene::m_buildGrid()
{
	const auto tile_size = Config::TILE_SIZE;
	auto offset = glm::vec2(Config::TILE_SIZE * 0.5f, Config::TILE_SIZE * 0.5f);

	m_clearNodes(); // we will need to clear nodes every time we move an obstacle

	// lay out a grid of path_nodes
	for (int row = 0; row < Config::ROW_NUM; ++row)
	{
		for (int col = 0; col < Config::COL_NUM; ++col)
		{
			PathNode* path_node = new PathNode();
			path_node->GetTransform()->position = 
				glm::vec2(static_cast<float>(col) * tile_size + offset.x, static_cast<float>(row) * tile_size + offset.y);

			bool keep_node = true;
			for (auto obstacle : m_pObstacles)
			{
				// determine which path_nodes are inside the obstacles
				if(CollisionManager::AABBCheck(path_node, obstacle))
				{
					keep_node = false;
				}
			}

			if (keep_node)
			{
				AddChild(path_node, 1);
				m_pGrid.push_back(path_node);
			}
			else
			{
				delete path_node;
			}
		}
	}

	// only display the Grid if it is toggled on
	m_toggleGrid(m_isGridEnabled);
}

void PlayScene::m_toggleGrid(const bool state) const
{
	for (auto path_node : m_pGrid)
	{
		path_node->SetVisible(state);
	}
}

bool PlayScene::m_checkAgentLOS(Agent* agent, DisplayObject* target_object) const
{
	bool has_LOS = false; // default - no LOS
	agent->SetHasLOS(has_LOS);

	// if ship to target distance is less than or equal to the LOS Distance (Range)
	const auto agent_to_range = Util::GetClosestEdge(agent->GetTransform()->position, target_object);
	if (agent_to_range <= agent->GetLOSDistance())
	{
		// we are in range
		std::vector<DisplayObject*> contact_list;
		for (auto display_object : GetDisplayList())
		{
			if (display_object->GetType() == GameObjectType::NONE) { continue;  }

			const auto agent_to_object_distance = Util::GetClosestEdge(agent->GetTransform()->position, display_object);
			if (agent_to_object_distance > agent_to_range) { continue; } // target is out of range
			if((display_object->GetType() != GameObjectType::AGENT) && (display_object->GetType() != GameObjectType::PATH_NODE) && (display_object->GetType() != GameObjectType::TARGET))
			{
				contact_list.push_back(display_object);
			}
		}

		const glm::vec2 agent_LOS_end_point = agent->GetTransform()->position + agent->GetCurrentDirection() * agent->GetLOSDistance();
		has_LOS = CollisionManager::LOSCheck(agent, agent_LOS_end_point, contact_list, target_object);

	}
	agent->SetHasLOS(has_LOS);

	return has_LOS;
}

bool PlayScene::m_checkPathNodeLOS(PathNode* path_node, DisplayObject* target_object) const
{
	// check angle to the target so we can still use LOS distance for path_nodes
	const auto target_direction = target_object->GetTransform()->position - path_node->GetTransform()->position;
	const auto normalized_direction = Util::Normalize(target_direction); // changes direction to a unit vector (length of 1)
	path_node->SetCurrentDirection(normalized_direction);
	return m_checkAgentLOS(path_node, target_object);
}

void PlayScene::m_checkAllNodesWithTarget(DisplayObject* target_object) const
{
	for (const auto path_node : m_pGrid)
	{
		m_checkPathNodeLOS(path_node, target_object);
	}
}

void PlayScene::m_checkAllNodesWithBoth() const
{
	for (const auto path_node : m_pGrid)
	{
		const bool LOSWithStarShip = m_checkPathNodeLOS(path_node, m_pStarShip);
		const bool LOSWithTarget = m_checkPathNodeLOS(path_node, m_pTarget);
		path_node->SetHasLOS(LOSWithStarShip && LOSWithTarget, glm::vec4(0, 1, 1, 1));
	}
}

void PlayScene::m_setPathNodeLOSDistance(const int dist) const
{
	for (const auto path_node : m_pGrid)
	{
		path_node->SetLOSDistance(static_cast<float>(dist));
	}
}

void PlayScene::m_clearNodes()
{
	m_pGrid.clear();
	for (const auto display_object : GetDisplayList())
	{
		if(display_object->GetType() == GameObjectType::PATH_NODE)
		{
			RemoveChild(display_object);
		}
	}
}

void PlayScene::Start()
{
	// Set GUI Title
	m_guiTitle = "Lab 7 - Part 3";

	// Setup a few more fields
	m_LOSMode = LOSMode::TARGET;
	m_pathNodeLOSDistance = 1000; // 1000px distance
	m_setPathNodeLOSDistance(m_pathNodeLOSDistance);

	// Set Input Type
	m_pCurrentInputType = static_cast<int>(InputType::KEYBOARD_MOUSE);

	// Add Game Objects
	m_pBackground = new Background();
	AddChild(m_pBackground, 0);

	m_pTarget = new Target();
	m_pTarget->GetTransform()->position = glm::vec2(500.0f, 300.0f);
	AddChild(m_pTarget, 3);

	//m_pStarShip = new CloseCombatEnemy();
	m_pStarShip = new RangedCombatEnemy();
	m_pStarShip->GetTransform()->position = glm::vec2(400.0f, 40.0f);
	AddChild(m_pStarShip, 4);

	// Add Obstacles
	BuildObstaclePool();

	// Setup the Grid
	m_isGridEnabled = false;
	m_buildGrid();
	m_toggleGrid(m_isGridEnabled);

	// Pre-load sounds
	SoundManager::Instance().Load("../Assets/audio/yay.ogg", "yay", SoundType::SOUND_SFX);
	SoundManager::Instance().Load("../Assets/audio/thunder.ogg", "thunder", SoundType::SOUND_SFX);

	// Pre-load Music
	SoundManager::Instance().Load("../Assets/audio/Klingon.mp3", "klingon", SoundType::SOUND_MUSIC);
	SoundManager::Instance().SetMusicVolume(16);

	// Play Music
	SoundManager::Instance().PlayMusic("klingon");

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

	static int LOS_mode = static_cast<int>(m_LOSMode);
	ImGui::Text("Path Node LOS");
	ImGui::RadioButton("Target", &LOS_mode, static_cast<int>(LOSMode::TARGET)); ImGui::SameLine();
	ImGui::RadioButton("StarShip", &LOS_mode, static_cast<int>(LOSMode::SHIP)); ImGui::SameLine();
	ImGui::RadioButton("Both Target & StarShip", &LOS_mode, static_cast<int>(LOSMode::BOTH)); 
	
	m_LOSMode = static_cast<LOSMode>(LOS_mode);

	ImGui::Separator();

	if(ImGui::SliderInt("Path Node LOS Distance", &m_pathNodeLOSDistance, 0, 1000))
	{
		m_setPathNodeLOSDistance(m_pathNodeLOSDistance);
	}

	ImGui::Separator();

	// spaceship properties

	static int shipPosition[] = { static_cast<int>(m_pStarShip->GetTransform()->position.x), static_cast<int>(m_pStarShip->GetTransform()->position.y)};
	if (ImGui::SliderInt2("Ship Position", shipPosition, 0, 800))
	{
		m_pStarShip->GetTransform()->position.x = static_cast<float>(shipPosition[0]);
		m_pStarShip->GetTransform()->position.y = static_cast<float>(shipPosition[1]);
	}

	// allow the ship to rotate
	static int angle;
	if (ImGui::SliderInt("Ship Direction", &angle, -360, 360))
	{
		m_pStarShip->SetCurrentHeading(static_cast<float>(angle));
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