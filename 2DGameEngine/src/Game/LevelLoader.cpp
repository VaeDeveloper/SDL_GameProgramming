#include "LevelLoader.h"
#include <glm/glm.hpp>
#include <fstream>
#include "../Components/TransformComponent.h"
#include "../Components/AnimationComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/CameraFollowComponent.h"
#include "../Components/HealthComponent.h"
#include "../Components/KeyboardControlledComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/ProjectileEmitterComponent.h"
#include "../Components/TextRenderComponent.h"
#include "../Logger/Logger.h"
#include "./Game.h"


LevelLoader::LevelLoader()
{
    Logger::Log("Level Loader constructor");
}

LevelLoader::~LevelLoader()
{
    Logger::Log("Level Loader destructor");
}

void LevelLoader::LoadLevel(sol::state& lua, SDL_Renderer *renderer, const std::unique_ptr<Registry> &registry, const std::unique_ptr<AssetManager>& assetManager, int levelNumber)
{
	sol::load_result script = lua.load_file("./assets/scripts/Level" + std::to_string(levelNumber) + ".lua");
    if (!script.valid()) {
        sol::error err = script;
        std::string errorMessage = err.what();
        Logger::Err("Error loading the lua script: " + errorMessage);
        return;
    }

    // Executes the script using the Sol state
    lua.script_file("./assets/scripts/Level" + std::to_string(levelNumber) + ".lua");

    // Read the big table for the current level
    sol::table level = lua["Level"];

    sol::table assets = level["assets"];

    int i = 0;
    while(true)
    {
        sol::optional<sol::table> hasAsset = assets[i];
        if (hasAsset == sol::nullopt)
        {
            Logger::Warn("has asset == null optional");
            break;
        }

        sol::table asset = assets[i];
        std::string assetType = asset["type"];
        std::string assetId = asset["id"];

        if (assetType == "texture")
        {
            assetManager->AddTexture(renderer, assetId, asset["file"]);
            Logger::Log("a new texutre asset was added to the asset store, id: " + assetId);
        }
        if (assetType == "font")
        {
            assetManager->AddFont(assetId, asset["file"], asset["font-size"]);
            Logger::Log("a new font asset was added to the asset store, id: " + assetId);
        }
        i++;

    }

	/**
	 * read level tile map information 
	 */
	sol::table map = level["tilemap"];
	std::string mapFilePath = map["map-file"];
	std::string mapTextureAssetId = map["texture_asset_id"];
	int mapNumRows = map["num_rows"];
	int mapNumCols = map["num_cols"];
	int tileSize = map["tile_size"];
	double mapScale = map["scale"];
	std::fstream mapFile;
	mapFile.open(mapFilePath);
	for (int y = 0; y < mapNumRows; y++)
	{
		for (int x = 0; x < mapNumCols; x++)
		{
			char ch;
			mapFile.get(ch);
			int srcRectX = std::atoi(&ch) * tileSize;
			mapFile.get(ch);
			int srcRectY = std::atoi(&ch) * tileSize;
			mapFile.ignore();

			Entity tile = registry->CreateEntity();
			tile.AddComponent<TransformComponent>(glm::vec2(x * (mapScale * tileSize), y * (mapScale * tileSize)),glm::vec2(mapScale, mapScale), 0);
			tile.AddComponent<SpriteComponent>(mapTextureAssetId, tileSize, tileSize, 0, false, srcRectX, srcRectY);
		}
	}
	mapFile.close();
	Game::MapWidth = mapNumCols * tileSize * mapScale;
	Game::MapHeight = mapNumRows * tileSize * mapScale;


	/**
	 * Read the level entities and their components 
	 */
	sol::table entities = level["entities"];
	i = 0;
	while(true)
	{
		sol::optional<sol::table> hasEntity = entities[i];
		if (hasEntity == sol::nullopt)
		{
			Logger::Err("has entity is nullptr");
			break;
		}

		sol::table entity = entities[i];
		Entity newEntity = registry->CreateEntity();
		
		/** Tag */
		sol::optional<std::string> tag = entity["tag"];
		if (tag != sol::nullopt)
		{
			newEntity.Tag(entity["tag"]);
		}

		/** Group  */
		sol::optional<std::string> group = entity["group"];
		if (group != sol::nullopt)
		{
			newEntity.Group(entity["group"]);
		}

		/** Components */
		sol::optional<sol::table> hasComponent = entity["components"];
		if (hasComponent != sol::nullopt)
		{
			/** Transform  */
			sol::optional<sol::table> transform = entity["commponent"]["transform"];
			if (transform != sol::nullopt)
			{
				newEntity.AddComponent<TransformComponent>
				(
					glm::vec2
					(
						entity["components"]["transform"]["position"]["x"],
						entity["components"]["transform"]["position"]["y"]
					),
					glm::vec2
					(
						entity["components"]["transform"]["scale"]["x"],
						entity["components"]["transform"]["scale"]["y"]
					),
					entity["components"]["transform"]["rotation"].get_or(0.0)
				);
			}

			/** RigidBody  */
			
		}

		

	}


    /*
    assetManager->AddTexture(renderer, "tank-image", "./assets/images/tank-panther-right.png");
	assetManager->AddTexture(renderer, "truck-image", "./assets/images/truck-ford-right.png");
	assetManager->AddTexture(renderer, "chopper-image", "./assets/images/chopper-spritesheet.png");
	assetManager->AddTexture(renderer, "radar-image", "./assets/images/radar.png");
	assetManager->AddTexture(renderer, "tile-image", "./assets/tilemaps/jungle.png");
	assetManager->AddTexture(renderer, "bullet-image", "./assets/images/bullet.png");
	assetManager->AddTexture(renderer, "tree-image", "./assets/images/tree.png");

	assetManager->AddFont("charriot-font", "./assets/fonts/charriot.ttf", 14);
	assetManager->AddFont("pico8-font-5", "./assets/fonts/pico8.ttf", 5);
	assetManager->AddFont("pico8-font-10", "./assets/fonts/pico8.ttf", 10);

    int tileSize = 32;	
	double tileScale = 2.0;
	int mapNumCols = 60;
	int mapNumRows = 40;
	std::fstream mapFile;
	mapFile.open("./assets/tilemaps/jungle.map");

	for (int y = 0; y < mapNumRows; y++)
	{
		for (int x = 0; x < mapNumCols; x++)
		{
			char ch;
			mapFile.get(ch);
			int srcRectY = std::atoi(&ch) * tileSize;
			mapFile.get(ch);
			int srcRectX = std::atoi(&ch) * tileSize;
			mapFile.ignore();

			Entity tile = registry->CreateEntity();
			tile.Group("tiles");
			tile.AddComponent<TransformComponent>(glm::vec2(x * (tileScale * tileSize),
															y * (tileScale * tileSize)),
															glm::vec2(tileScale, tileScale),
															0.0);

			tile.AddComponent<SpriteComponent>("tile-image", tileSize, tileSize, 0, false, srcRectX, srcRectY);
		}
	}
	mapFile.close();
	Game::MapWidth = mapNumCols * tileSize * tileScale;
	Game::MapHeight = mapNumRows * tileSize * tileScale;

	Entity chopper = registry->CreateEntity();
	chopper.Tag("player");
	chopper.SetName("Player chopper");
	chopper.AddComponent<TransformComponent>(glm::vec2(237.0, 117.0), glm::vec2(1.0, 1.0), 0.0);
	chopper.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
	chopper.AddComponent<SpriteComponent>("chopper-image", 32, 32, 1);
	chopper.AddComponent<AnimationComponent>(2, 15, true);
	chopper.AddComponent<ProjectileEmitterComponent>(glm::vec2(150.0, 150.0), 0, 10000, 10, true);
	chopper.AddComponent<BoxCollisionComponent>(32, 25);
	chopper.AddComponent<KeyboardControlledComponent>
	(
		glm::vec2(0, -50), 
		glm::vec2(50, 0), 
		glm::vec2(0, 50), 
		glm::vec2(-50, 0)
	);
	chopper.AddComponent<CameraFollowComponent>();
	chopper.AddComponent<HealthComponent>(100);

	Entity radar = registry->CreateEntity();
	radar.AddComponent<TransformComponent>(glm::vec2(Game::WindowWidth - 75.0, 10.0), glm::vec2(1.0, 1.0), 0.0);
	radar.AddComponent<SpriteComponent>("radar-image", 64, 64, 1, true);
	radar.AddComponent<AnimationComponent>(8, 8, true);
	
    Entity tank = registry->CreateEntity();
	tank.Group("enemies");
    tank.AddComponent<TransformComponent>(glm::vec2(500.0,500.0), glm::vec2(1.0, 1.0), 0.0);
    tank.AddComponent<RigidBodyComponent>(glm::vec2(20.0, 0.0));
    tank.AddComponent<SpriteComponent>("tank-image", 32, 32, 1);
    tank.AddComponent<BoxCollisionComponent>(32, 32);
    tank.AddComponent<ProjectileEmitterComponent>(glm::vec2(100.0, 0.0), 5000, 3000, 10, false);
    tank.AddComponent<HealthComponent>(100);

    Entity truck = registry->CreateEntity();
	truck.Group("enemies");
    truck.AddComponent<TransformComponent>(glm::vec2(110.0, 500.0), glm::vec2(1.0, 1.0), 0.0);
    truck.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
    truck.AddComponent<SpriteComponent>("truck-image", 32, 32, 2);
    truck.AddComponent<BoxCollisionComponent>(32, 32);
    truck.AddComponent<ProjectileEmitterComponent>(glm::vec2(0.0, -200.0), 2000, 5000, 10, false);
    truck.AddComponent<HealthComponent>(100);

	Entity treeA = registry->CreateEntity();
	treeA.Group("obstacles");
    treeA.AddComponent<TransformComponent>(glm::vec2(600.0,495.0), glm::vec2(1.0, 1.0), 0.0);
	treeA.AddComponent<SpriteComponent>("tree-image", 16, 32, 2);
	treeA.AddComponent<BoxCollisionComponent>(16, 32);

	Entity treeB = registry->CreateEntity();
	treeB.Group("obstacles");
    treeB.AddComponent<TransformComponent>(glm::vec2(400.0,495.0), glm::vec2(1.0, 1.0), 0.0);
	treeB.AddComponent<SpriteComponent>("tree-image", 16, 32, 2);
	treeB.AddComponent<BoxCollisionComponent>(16, 32);


	Entity label = registry->CreateEntity();
	SDL_Color color {0, 255, 0, 255};
	label.AddComponent<TextRenderComponent>(glm::vec2(Game::WindowWidth / 2 - 40, 10), "CHOPPER 1.0", "charriot-font", color, true);
    */
}




