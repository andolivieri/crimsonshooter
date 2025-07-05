# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build System and Commands

### Building the Project
**IMPORTANT: This is a Windows-only project that requires Visual Studio. Do not attempt to compile from WSL/Linux environment - it will fail.**

```bash
# Standard CMake build (Windows with Visual Studio >= 2017, or Linux/Mac with pkg-config)
cd build
cmake ..
cmake --build .

# Quick build script
./build.sh

# Debug build specifically
cmake --build . --config Debug
```

**Note for Claude Code**: Do not attempt to run compilation commands as this project is Windows-specific and will fail in WSL/Linux environments.

### Running the Game
```bash
# From build directory
./dist/Debug/crimsonshooter.exe    # Windows
./dist/Debug/crimsonshooter        # Linux/Mac
```

### Development Environment
- Requires Visual Studio >= 2017 on Windows
- CMake is required for all platforms
- Assets are automatically copied to build output directory
- DLLs are automatically copied on Windows builds

## Core Architecture

### Entity-Component-System (ECS)
This is a custom ECS implementation with the following key concepts:

- **Entity**: Game objects identified by unique IDs, can have multiple components
- **Component**: Data containers (TransformComponent, SpriteComponent, etc.)
- **ComponentManager**: Handles component lifecycle and relationships
- **Groups**: Entities are organized into rendering/update groups (`groupPlayers`, `groupEnemies`, `groupProjectiles`, etc.)

The ECS limits are defined as:
- `maxComponents = 32`
- `maxGroups = 32`
- Components use bitsets for efficient querying

### Component Categories
Components are organized into logical directories:
- **core/**: Basic functionality (Transform, Sprite, Collider, Timer, etc.)
- **ai/**: AI behaviors (AIComponent, DumbComponent)
- **weapons/**: Weapon systems (WeaponComponent, ProjectileComponent, GrenadeComponent)
- **gameplay/**: Game mechanics (DamageModel, Stamina, ScoreCollector)
- **ui/**: User interface (OverlayComponent, TextComponent)
- **spawning/**: Entity spawning (FoeSpawner, PerkSpawner)
- **effects/**: Visual/audio effects (SoundComponent, BloodPatch, Shockwave)
- **input/**: Input handling (InputComponent, ActorController)

### Rendering Pipeline
The game uses a hybrid rendering approach:

1. **TileRenderer**: Direct SDL rendering for static map tiles (performance optimization)
   - Stores tiles as `TileData` structs in a static vector
   - Renders directly via `SDL_RenderCopyEx()` with frustum culling
   - Bypasses ECS for tile rendering to avoid 2400+ entities in update loop

2. **Entity Rendering**: Traditional ECS rendering for dynamic objects
   - Entities are drawn in group order (groupMap, groupColliders, groupBloodPatches, etc.)
   - Each entity calls its `draw()` method

### Map System
- **GameMap**: Loads Tiled JSON format maps with multiple layer support
- **Assets**: Tilesets in `assets/maptiles.png`, maps in `assets/themap.json`
- **Optimization**: Use `TileRenderer` for static tiles, ECS entities for interactive elements

### Perk System
Dual-category perk system with intelligent selection:
- **Weapon Perks**: pistol, uzi, shotgun, chainsaw
- **Other Perks**: health (restores 50% max health), grenade (+3 grenades)
- **Selection Rules**: First perk must be weapon, then alternates to other perks, except every 5th perk forces weapon selection

### Audio System
- SDL2_mixer integration with OGG Vorbis support
- Sound files in `assets/sounds/` directory
- SoundComponent handles audio playback with channel management

## Code Style Guidelines

### TODO Comments
Use this format for future improvements:
```cpp
// todo andoli: description of what needs to be done
// keep the comments to a very bare minimum
```

### Performance Considerations
- **Entity Count Awareness**: Monitor entity count with `Game::dumpStats()` (press 'M' key)
- **Static vs Dynamic**: Use TileRenderer for static elements, ECS for interactive objects
- **Memory Management**: Prefer direct SDL rendering over entity creation for performance-critical rendering

### Component Design
- Components should be lightweight data containers
- Logic should be in component update methods
- Use composition over inheritance
- Components communicate via entity relationships or direct component access

### Asset Management
- Textures managed via TextureManager **singleton**
- Assets automatically copied to build directory
- Use relative paths from executable location: `assets/filename.ext`

## Game-Specific Systems

### Weapon System
- **WeaponBayComponent**: Manages player's equipped weapons and grenade count
- **WeaponComponent**: Individual weapon behavior and stats
- **ProjectileComponent**: Handles bullets, grenades have separate GrenadeProjectileComponent

### AI System
- **AIComponent**: Enemy AI with multiple behavior modes
- **FoeSpawnerComponent**: Wave-based enemy spawning with configurable parameters

### Input System
- **InputComponent**: Raw input capture and event generation
- **ActorControllerComponent**: Translates input events to game actions
- Key mappings: WASD movement, Space for grenades, Mouse for aiming/shooting

### Camera System
- Global camera managed in Game class
- Follows player with boundary constraints
- Camera-to-world and world-to-camera coordinate conversion utilities

## Development Notes

### Debugging
- Press 'P' to pause/unpause game
- Press 'M' to dump entity statistics to console
- Console logging available throughout the codebase

### Asset Pipeline
- Sprites created in PixelOver (.pxo files in sprites/ directory)
- Maps created in Tiled Map Editor (.tmx files, exported to JSON)
- Audio in OGG Vorbis format for cross-platform compatibility