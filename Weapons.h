#pragma once

#include <map>
#include <string>

enum class Weapon
{
    Handgun,
    SubmachineGun,
    Rifle,
    Shotgun,
    AssaultRifle
};

const std::map<Weapon, int> WEAPON_DAMAGE = {
    { Weapon::Handgun, 8 },
    { Weapon::SubmachineGun, 10 },
    { Weapon::Rifle, 50 },
    { Weapon::Shotgun, 15 },
    { Weapon::AssaultRifle, 20 }
};

const std::map<Weapon, int> WEAPON_SPREAD = {
    { Weapon::Handgun, 2 },
    { Weapon::SubmachineGun, 3 },
    { Weapon::Rifle, 0 },
    { Weapon::Shotgun, 10 },
    { Weapon::AssaultRifle, 1 }
};

const std::map<Weapon, int> WEAPON_SPEED = {
    { Weapon::Handgun, 800 },
    { Weapon::SubmachineGun, 1000 },
    { Weapon::Rifle, 1500 },
    { Weapon::Shotgun, 800 },
    { Weapon::AssaultRifle, 1300 }
};

const std::map<Weapon, float> WEAPON_DELAYS = {
    { Weapon::Handgun, 0.4 },
    { Weapon::SubmachineGun, 0.1 },
    { Weapon::Rifle, 0.7 },
    { Weapon::Shotgun, 1.2 },
    { Weapon::AssaultRifle, 0.1 }
};

const std::map<Weapon, int> MAX_AMMO = {
    { Weapon::Handgun, 100 },
    { Weapon::SubmachineGun, 200 },
    { Weapon::Rifle, 80 },
    { Weapon::Shotgun, 50 },
    { Weapon::AssaultRifle, 200 }
};

const std::map<Weapon, int> WEAPON_PICKUP_AMOUNT = {
    { Weapon::Handgun, 5 },
    { Weapon::SubmachineGun, 10 },
    { Weapon::Rifle, 2 },
    { Weapon::Shotgun, 4 },
    { Weapon::AssaultRifle, 10 }
};

const std::map<Weapon, int> AMMO_PICKUP_AMOUNT = {
    { Weapon::Handgun, 10 },
    { Weapon::SubmachineGun, 20 },
    { Weapon::Rifle, 5 },
    { Weapon::Shotgun, 10 },
    { Weapon::AssaultRifle, 20 }
};

const std::map<Weapon, std::string> WEAPON_TEXTURES = {
    { Weapon::Handgun, "Resources/Images/Handgun.png" },
    { Weapon::SubmachineGun, "Resources/Images/SubmachineGun.png" },
    { Weapon::Rifle, "Resources/Images/Rifle.png" },
    { Weapon::Shotgun, "Resources/Images/Shotgun.png" },
    { Weapon::AssaultRifle, "Resources/Images/AssaultRifle.png" }
};

const std::map<Weapon, std::string> PROJECTILE_TEXTURES = {
    { Weapon::Handgun, "Resources/Images/HandgunBullet.png" },
    { Weapon::SubmachineGun, "Resources/Images/SubmachineGunBullet.png" },
    { Weapon::Rifle, "Resources/Images/RifleBullet.png" },
    { Weapon::Shotgun, "Resources/Images/ShotgunBullet.png" },
    { Weapon::AssaultRifle, "Resources/Images/AssaultRifleBullet.png" }
};

const std::map<Weapon, std::string> AMMO_TEXTURES = {
    { Weapon::Handgun, "Resources/Images/HandgunAmmo.png" },
    { Weapon::SubmachineGun, "Resources/Images/SubmachineGunAmmo.png" },
    { Weapon::Rifle, "Resources/Images/RifleAmmo.png" },
    { Weapon::Shotgun, "Resources/Images/ShotgunAmmo.png" },
    { Weapon::AssaultRifle, "Resources/Images/AssaultRifleAmmo.png" }
};

const std::map<Weapon, std::string> GUN_SOUNDS = {
    { Weapon::Handgun, "Resources/Sounds/Handgun.wav" },
    { Weapon::SubmachineGun, "Resources/Sounds/SubmachineGun.wav" },
    { Weapon::Rifle, "Resources/Sounds/Rifle.wav" },
    { Weapon::Shotgun, "Resources/Sounds/Shotgun.wav" },
    { Weapon::AssaultRifle, "Resources/Sounds/AssaultRifle.wav" }
};

const std::map<Weapon, std::string> PLAYER_TEXTURES = {
    { Weapon::Handgun, "Resources/Images/Player.png" },
    { Weapon::SubmachineGun, "Resources/Images/PlayerSubmachineGun.png" },
    { Weapon::Rifle, "Resources/Images/PlayerRifle.png" },
    { Weapon::Shotgun, "Resources/Images/PlayerShotgun.png" },
    { Weapon::AssaultRifle, "Resources/Images/PlayerAssaultRifle.png" }
};