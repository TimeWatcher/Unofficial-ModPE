#include <jni.h>
#include <dlfcn.h>
#include "modpe.h"

bool isInited;
bool atkhooked;
void (*atkhook)(Player&, Entity&);

ModPE* ModPE::m_instance;

void ModPE::setLevel_(Level* level)
{
	this->level = level;
}

void ModPE::setGui(Gui* gui)
{
	this->gui = gui;
}

void ModPE::setInventoryMenu(InventoryMenu* inventorymenu)
{
	this->inventorymenu = inventorymenu;
}

void ModPE::setMinecraftClient(MinecraftClient* minecraftclient)
{
	this->minecraftclient = minecraftclient;
}

void ModPE::setLocalPlayer(LocalPlayer* localplayer)
{
	this->localplayer = localplayer;
}

void ModPE::setBlockSource(BlockSource* blocksource)
{
	this->blocksource = blocksource;
}

void ModPE::setInventory(Inventory* inventory)
{
	this->inventory = inventory;
}

Level* ModPE::getLevel_()
{
	if(!isInited)
		return NULL;
	if(this->level)
		return this->level;
}

LocalPlayer* ModPE::getLocalPlayer()
{
	if(!isInited)
		return NULL;
	if(this->localplayer)
		return this->localplayer;
}

Gui* ModPE::getGui()
{
	if(!isInited)
		return NULL;
	if(this->gui)
		return this->gui;
}

InventoryMenu* ModPE::getInventoryMenu()
{
	if(!isInited)
		return NULL;
	if(this->inventorymenu)
		return this->inventorymenu;
}

MinecraftClient* ModPE::getMinecraftClient()
{
	if(!isInited)
		return NULL;
	if(this->minecraftclient)
		return this->minecraftclient;
}

BlockSource* ModPE::getBlockSource()
{
	if(!isInited)
		return NULL;
	if(this->blocksource)
		return this->blocksource;
}

Inventory* ModPE::getInventory()
{
	if(!isInited)
		return NULL;
	if(this->inventory)
		return this->inventory;
}

static void (*_Inventory$tick)(Inventory*);
static void Inventory$tick(Inventory* inv)
{
	_Inventory$tick(inv);
	ModPE::GetInstance()->setInventory(inv);
	std::stringstream ss;
	std::string s;
	ss << ModPE::GetInstance()->getLevel();
	ss >> s;
	s = "§aPlayer Current Level: " + s;
	ModPE::GetInstance()->getGui()->showTipMessage(s);
}

static void (*_Level$tick)(Level*);
static void Level$tick(Level* l)
{
	_Level$tick(l);
	ModPE::GetInstance()->setLevel_(l);
	ModPE::GetInstance()->setLocalPlayer(l->getLocalPlayer());
	ModPE::GetInstance()->setMinecraftClient(l->getLocalPlayer()->getMinecraftClient());
	ModPE::GetInstance()->setGui(l->getLocalPlayer()->getMinecraftClient()->getGui());
	ModPE::GetInstance()->setBlockSource(&l->getLocalPlayer()->getRegion());
	ModPE::GetInstance()->setInventoryMenu(l->getLocalPlayer()->getInventoryMenu());
}

static void (*_GameMode$attack)(GameMode*, Player&, Entity&);
static void GameMode$attack(GameMode* gm, Player& p, Entity& e)
{
	_GameMode$attack(gm, p, e);
	if(atkhooked)
		atkhook(p, e);
}

void ModPE::initModPE()
{
	HOOK(Level::tick, Level$tick, _Level$tick);
	HOOK(Inventory::tick, Inventory$tick, _Inventory$tick);
	HOOK(GameMode::attack, GameMode$attack, _GameMode$attack);
	isInited = true;
}

void ModPE::attackHook(void (*func)(Player& attacker, Entity& victim))
{
	atkhooked = true;
	atkhook = func;
}

void ModPE::addItemInventory(int id, int amount, int damage)
{
	ItemInstance itemIns(id, amount, damage);
	ModPE::GetInstance()->getInventory()->add(itemIns, false);
}

void ModPE::clientMessage(std::string const& str)
{
	ModPE::GetInstance()->getGui()->displayClientMessage(str);
}

void ModPE::explode(float x, float y, float z, float r, bool withFire)
{
	BlockSource& region = *(ModPE::GetInstance()->getBlockSource());
	ModPE::GetInstance()->getLevel_()->explode(region, NULL,{x, y, z}, r, withFire);
}

int ModPE::getCarriedItem()
{
	return ModPE::GetInstance()->getLocalPlayer()->getSelectedItem();
}

int ModPE::getPlayerEnt()
{
	return ModPE::GetInstance()->getLocalPlayer()->getUniqueID();
}

float ModPE::getPlayerX()
{
	return ModPE::GetInstance()->getLocalPlayer()->pos.x;
}

float ModPE::getPlayerY()
{
	return ModPE::GetInstance()->getLocalPlayer()->pos.y;
}

float ModPE::getPlayerZ()
{
	return ModPE::GetInstance()->getLocalPlayer()->pos.z;
}

int ModPE::getLevel()
{
	return ModPE::GetInstance()->getLocalPlayer()->getPlayerLevel();
}

void ModPE::rideAnimal(Entity& rider, Entity& animal)
{
	animal.addRider(rider);
}

void ModPE::setNightMode(bool isNightMode)
{
	ModPE::GetInstance()->getLevel_()->setNightMode(isNightMode);
}

void ModPE::setPosition(Entity& e, float x, float y, float z)
{
	e.setPos({x, y, z});
}

void ModPE::setRot(Entity& e, float x, float y)
{
	e.setRot({x, y});
}

void ModPE::Level_setTime(int time)
{
	ModPE::GetInstance()->getLevel_()->setTime(time);
}

void ModPE::Player_setHealth(int health)
{
	ModPE::GetInstance()->getLocalPlayer()->serializationSetHealth(health);
}
