
# ASI MK11
ASI MK11 is the name I'm giving to the ASI Loader mods. Basically they're similar to Cheat Engine mods but they're applied to the game directly, this means you don't have to manually open Cheat Engine everytime you want to apply some mods, and you don't have to patch the exe to apply anti cheats.
What is it used for? It's used to modify game functions to make them do whatever you want them to, making the game act like you coded it.

### Current Features:
- Anti Cheat Engine Patch. Allows you to use Cheat Engine while game is running.
- Anti CVD Mod Patch. Allows you to swap files.
- Anti CVD 2 Patch. Allows you to modify files.
- Mod Loader. Allows you to make a mods folder to keep all your mods in 1 place, so that you don't have to overwrite any file.
- Fun Cheats. Some fun cheats that I use in my videos.

### How to use:
Download the Latest MK11 hook version, and select the mods you want in the provided ini file. When a game updates, you need to change the addresses in the `[Addresses]` section of the ini. Addresses will be provided for you to add. **Update July 2021: Addresses have been replaced with Patterns or Proc Names.**

To use file swaps (like moveset swap), create a folder called `MKSwap` in the root folder (Mortal Kombat 11 Folder), and inside it create a new folder similar to the file that you are copying. Example, to swap Rain's Friendship with Subzero, copy `Asset/RAIN_A_FRIENDSHIP.xxx` into `/MKSwap/Asset/SUBZERO_A_FRIENDSHIP.xxx`. Console will say `"Loaded Asset/RAIN_A_FRIENDSHIP.xxx from MKSwap"`.

In order to use the cheats, make sure to enable the usage of cheats in the ini and set a hotkey. By default the hotkey is `F12`. In order to use cheats, you need to hold the hotkey (`F12` for example) and then press the number that identifies the cheat you want to enable/disable. Currently available cheats are:

 1. No Mercy Requirements: Perform Mercy any time.
 2. No Ground Requirement: You can perform ground moves (like slide) in the air.
 3. No Brutality Requirements: You can do any brutality at any time.
 4. Spawn Meteor: Towers of Time Meteor Always Spawns, regardless of
    time.

### All Ini Settings

    [Settings]
    bEnableConsoleWindow = true/false ; Shows the console window for logging
    bPauseOnStart = true/false ; Pauses the game before it launches to give you time to attach a debugger. Useful when your PC loads faster than you can attach a debugger.
    bPatchUnlocker = true/false ; Enables the PIPE between Unlocker and MK11. Unlocker not working yet and causes bans. [color=#FF0000]DO NOT USE.[/color]
    bModLoader = true/false ; Enables the Mod Loader file
    bEnableCameraMod = true/false ; Enables the use of Camera Mod
    bEnableCheats = true ; Enables the usage of cheats
    hotkey_cheats = F12 ; Hotkey to use to enable cheats
    
    [AntiCheat]
    bDisableAntiCheatEngine = true ; Hooks user32.dll to Disable Cheat Engine check. Should not break on updates.
    bDisableAntiCVD1 = true ; Modifies game code to skip CVD1 check.
    bDisableAntiCVD2 = true ; Modifies game code to skip CVD2 check.
    
    [Addresses]
    iCVD1 = 1ABA62B ; Requires Update with every game update.
    iCVD2 = 1A65995 ; Requires Update with every game update.
    iModLoader = 14248d3e0 ; Requires Update with every game update.
    iUnlocker = 15F6A2D6 ; Requires Update with every game update. [color=#FF0000]DO NOT USE[/color]
    iCamOffset = 0x6BC ; Requires Update only when Camera Mod stops working
    
    [Patterns]
    pCam = 0F 11 44 24 30 F2 0F 11 4C 24 40 8B ; Requires Update only when Camera Mod Stops working
    
    [Patterns.Cheats]
    pMercyAnyTime = 40 53 48 83 EC 20 48 8B D9 48 85 C9 0F 84 98 ; Perform Mercy at any time
    pNoGroundReq = 30 5B C3 CC CC CC CC CC CC 40 53 48 83 EC 20 8B ; Perform Ground moves in the air (such as Subzero slide)
    pNoBrutalityReq = 48 89 6C 24 18 56 57 41 56 48 83 EC 50 48 89 ; Perform any Brutality any time
    pBrutalityAlwaysCorrect = 40 53 48 83 EC 20 48 8B D9 48 85 C9 74 7C ; Brutalities don't require holding specific buttons
    pMeteorAlwaysSpawns = 75 1C 48 8B 03 48 8B D7 ; ToT Meteor always spawns
    
    [Camera.Config]
    fSpeed = 50 ; Camera movement speed
    fCamHold = 50 ; Milliseconds to wait while holding camera. Smaller = Faster.
    
    [Camera.Keys]
    hotkey_toggle = F2 ; To Toggle Camera mod on/off
    hotkey_x+ = NUMPAD8 ; Move X towards the positive axis
    hotkey_x- = NUMPAD2 ; Move X towards the negative axis
    hotkey_y+ = RIGHT ; The rest are the same as above 2
    hotkey_y- = LEFT
    hotkey_z+ = UP
    hotkey_z- = DOWN
    hotkey_fov+ = NUMPAD-
    hotkey_fov- = NUMPAD+
    hotkey_rot+ = NUMPAD9
    hotkey_rot- = NUMPAD7
    hotkey_pitch+ = NUMPAD3
    hotkey_pitch- = NUMPAD1
    hotkey_yaw+ = NUMPAD6
    hotkey_yaw- = NUMPAD4
    hotkey_timestop = NUMPAD0 ; Time Stop to freely wander around (not yet implemented)

Download from [MKSecrets All-In-One Thread](https://www.mksecrets.net/forums/eng/viewtopic.php?f=120&t=47702&p=1398900#p139900).
