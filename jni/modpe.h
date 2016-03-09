#pragma once

#include <dlfcn.h>
#include <string>
#include <sstream>
#include "Substrate.h"
#include "headers/Vec2.h"
#include "headers/Vec3.h"
#include "headers/AABB.h"
#include "headers/SynchedEntityData.h"

#define HOOK(function_default,function_replace,function_ptr); MSHookFunction((void*)&function_default,(void*)&function_replace,(void**)&function_ptr);
#define MHOOK(dlfcn_string,function_replace,function_ptr); MSHookFunction(dlsym(dlfcn("libminecraftpe.so",RTLD_LAZY),dlfcn_string),(void*)&function_replace,(void**)&function_ptr);

class EntityUniqueID;

class Gui
{
	public:
	void displaySystemMessage(std::string const&);
	void displayClientMessage(std::string const&);
	void displayChatMessage(std::string const&, std::string const&);
	void showTipMessage(std::string const&);
};
class BlockSource;
class MinecraftClient
{
	public:
	Gui* getGui() const;
};
class InventoryMenu;
class ItemInstance
{
	public:
	short count, aux;
	void* userData;
	bool valid;
	void* item;
	void* block;

	ItemInstance();
	ItemInstance(int, int, int);
	ItemInstance(const void*, int);
	ItemInstance(const void*, int, int);
	ItemInstance(const ItemInstance&);
	
	const ItemInstance& operator=(const ItemInstance&);
	
	bool isLiquidClipItem();
	void hurtAndBreak(int, void*);
};
class Entity
{
	public:
	void* synchedData; // 4
	Vec3 pos; // 8
	Vec3 oldPos; // 20
	Vec3 chunkPos; // 32
	Vec3 velocity; // 44
	Vec2 rotation; // 56
	Vec2 idk; // 64
	Vec2 idk1; // 72
	SynchedEntityData entityData; // 80
	char filler[28]; // 96
	//DimensionId dimensionId; // 108
	char filler1[8]; // 112
	//Level& level; // 116
	char idec[8]; // 120
	//Color lastLightColor; // 124
	AABB boundingBox; // 140
	float someFloat; // 168;
	float headHeight; // 172
	char filler2[12]; // 176
	float ridingHeight; // 188
	Vec2 idk2; // 192
	char filler3[16]; // 200
	int airSupply; // 216
	int fireTicks; // 220
	char filler4[16]; // 224
	//BlockID block; // 232
	int noclue; // 236  **ALWAYS 255**
	char filler5[8]; // 240
	//EntityRendererId rendererId; // 244
	char fillero[8]; // 248
	//EntityList riders; // 248
	Entity& rider; // 252
	Entity& riding; // 256
	bool isRiding; // 260
	int idk5; // 264
	char filler6[12]; // 268
	bool what1; // 280
	bool what2; // 281
	bool what3; // 282
	bool what4; // 283
	bool what5; // 284
	bool what6; // 285
	bool hurtMarked; // 286
	bool noPhysics; // 287
	bool shouldRender; // 288
	bool isInvincible; // 289
	bool idk6; // 290
	bool notaclue; // 291
	bool huh; // 292
	bool noidea; // 293
	char uniqueId[8]; //  296
	int autoSend; // 304
	Vec3 someVec; // 308
	char filler7[8]; // 320
	bool what7; // 328
	bool isStuckInWeb; // 329
	bool inWater; // 330
	bool immobile; // 331
	bool changed; // 332
	char filler__[4]; // 336
	//ExplodeComponent* exploder; // 336
	int idk4; // 340
	bool isRemoved; // 344
	bool isGlobal; // 345
	bool isRegistered; // 346
	BlockSource& region; // 348
	static int mEntityCounter;
	
	Entity(BlockSource&);
	virtual ~Entity();
	
	int getUniqueID() const;
	BlockSource& getRegion() const;
	void addRider(Entity&);
	void setPos(Vec3 const&);
	void setRot(Vec2 const&);
};
class Mob : public Entity
{
	public:
	void serializationSetHealth(int);
};
class Player : public Mob
{
	public:
	int getSelectedItem() const;
	int getPlayerLevel() const;
};
class LocalPlayer : public Player
{
	public:
	MinecraftClient* getMinecraftClient() const;
	InventoryMenu* getInventoryMenu();
};
class Level
{
	public:
	virtual void tick();
	LocalPlayer* getLocalPlayer() const;
	void explode(BlockSource&,Entity*,Vec3 const&,float,bool);
	void setNightMode(bool);
	void setTime(int);
	Entity* getEntity(EntityUniqueID, bool) const;
};
class Inventory
{
	public:
	virtual void tick();
	void add(ItemInstance&, bool);
};
class GameMode
{
	public:
	virtual void attack(Player &, Entity &);
};

class ModPE
{
	ModPE(){};
	static ModPE* m_instance;
	Level* level;
	Gui* gui;
	InventoryMenu* inventorymenu;
	MinecraftClient* minecraftclient;
	LocalPlayer* localplayer;
	BlockSource* blocksource;
	Inventory* inventory;
	
	class Del
	{
	public:
		~Del()
		{
			if(ModPE::m_instance)
				delete m_instance;
		}
	};
public:

	static ModPE* GetInstance()
	{
		if(m_instance == NULL)
			m_instance = new ModPE();
		return m_instance;
	}
	void initModPE();
	
	void setLevel_(Level*);
	void setGui(Gui*);
	void setInventoryMenu(InventoryMenu*);
	void setMinecraftClient(MinecraftClient*);
	void setLocalPlayer(LocalPlayer*);
	void setBlockSource(BlockSource*);
	void setInventory(Inventory*);
	
	Level* getLevel_();
	Gui* getGui();
	InventoryMenu* getInventoryMenu();
	MinecraftClient* getMinecraftClient();
	LocalPlayer* getLocalPlayer();
	BlockSource* getBlockSource();
	Inventory* getInventory();
	
	void attackHook(void (*func)(Player&, Entity&));
	void addItemInventory(int, int, int);
	void clientMessage(std::string const&);
	void explode(float, float, float, float, bool);
	int getCarriedItem();
	int getPlayerEnt();
	float getPlayerX();
	float getPlayerY();
	float getPlayerZ();
	int getTile(float, float, float);
	int getLevel();
	void rideAnimal(Entity&, Entity&);
	void setNightMode(bool);
	void setPosition(Entity&, float, float, float);
	void setRot(Entity&, float, float);
	void setTile(int, int, int, int, int);
	void setVelX(Entity&, float);
	void setVelY(Entity&, float);
	void setVelZ(Entity&, float);
	void Level_setTime(int);
	void Player_setHealth(int);
};
