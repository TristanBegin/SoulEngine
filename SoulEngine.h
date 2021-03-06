#pragma once
#include "AEEngine.h"
#include "fmod.h"
#define FIRST 0
#define LAST -1
#define ERR_SURPASSED_BUFFER_SIZE -1
#define deltaTime AEFrameRateControllerGetFrameTime()
typedef struct VECTOR VECTOR;
typedef struct TRANSFORM TRANSFORM;
typedef struct ARCHETYPE ARCHETYPE;
typedef struct COMPONENT COMPONENT;
typedef struct PHYSICS PHYSICS;
typedef struct COLLIDER COLLIDER;
typedef struct SOUND SOUND;
typedef struct SPRITE SPRITE;
typedef struct MESH MESH;
typedef struct GAMESTATS GAMESTATS;
typedef struct GAME GAME;
typedef struct UNIT UNIT;
typedef struct LEVEL LEVEL;
typedef struct CAMERA CAMERA;
typedef struct BEHAVIOR BEHAVIOR;
typedef struct VAR VAR;
typedef struct COLLISIONINFO COLLISIONINFO;
//typedef struct COLOR COLOR;
typedef struct MATRIX MATRIX;
typedef struct IMAGE IMAGE;
typedef enum COMPONENTTYPE COMPONENTTYPE;
typedef enum BOOL BOOL;
typedef enum VARTYPE VTYPE;
typedef enum GAMESTATE GAMESTATE;
typedef enum TAG TAG;

typedef struct COLOR
{
  float r;
  float g;
  float b;
  float a;
}COLOR;

// Vector, pretty self explanatory.
typedef struct VECTOR
{
  float x;
  float y;
}VECTOR;

/* The GAME struct branches down to literally everything in the game.
At no point should there be something that the Game cannot access.
This is so that the entire game can be freed simply by referencing
the Game and we won't have to worry as much about memory leaks.

This is all done through linked lists. The general structure of the
engine is as follows (more information below):

      U_Archetype  U_Archetype  U_Archetype
             |        |        |
  Level---->Unit---->Unit---->Unit
  |
  Level...
  |
GAME
  |
  G_Archetype...
    |
  G_Archetype...
    |
  G_Archetype->Component->Component->Component
                   |          |          |
                Sprite    Behavior   SquareMesh

*/
typedef struct GAME
{
  char * Name;               // The name given to the game ("Taika", "Jerry&Jeffrey", ext).
  GAMESTATS *pGameStats;     // Stats of the game, including the default components, ext.
  ARCHETYPE *nextArchetype;  // The first archetype in a linked list of archetypes.
  LEVEL *nextLevel;          // The first level in a linked list of archetypes.
}GAME;


/* Similar to the Archetypes in Zero. These hold information for an object.

  Each archetype contains a linked list of COMPONENTS, which hold the information
  about an object. (See below for mor information on COMPONENTS).

  There are game Archetypes and unit archetypes:

  Game (Init) Archetypes: Part of a linked list of archetypes on the GAME. These are 
                   used as a reference to when a unit is being initialized. THESE SHOULD NOT BE 
                   ACCESSED ON RUNTIME. They are referenced on initialization to create unit archetypes.
                   Uou'll know it's a Game Archetype if pUnit is NULL.

  Unit Archetypes: These are instances of Game Archetypes that are attatched to units. 
                   They CAN be changed in runtime and are simply copies of Game Archetypes. These are
                   referenced on update to describe how an object should be displayed, what it should do, ext.
*/
typedef struct ARCHETYPE
{
  char *Name;               //The name of the archetype.
  TAG Tag;                  //The Tag for the Archetype.
  GAME * pGame;             //The Game object that owns this archetype.
  UNIT * pUnit;             //Will be null if this is original archetype (aka not an instance attatched to a unit).
  COMPONENT *nextComponent; //Pointer to the first component in the archetype's list of components.
  ARCHETYPE *nextArchetype; //The next Archetype in a linked list that starts at pGame.
} ARCHETYPE;


/* The COMPONENT scruct is a bridge to structs such as SPRITE and SQUAREMESH.
  Archetypes hold a linked list of components, and each component has a pointer to a struct.

  Archetype->Component->Component->Component
                 ^          ^          ^
              Sprite    Behavior   SquareMesh

  The current structs that can be linked to a component are:
  SPRITE
  SQUAREMESH
  BEHAVIOR
*/
typedef struct COMPONENT
{
  void *pStruct;            //The "Meat" of the component, (the SPRITE, BEHAVIOR, ext. that is the actual "Component")
  COMPONENTTYPE Type;       //The type that pStruct points to (Sprite, ext).
  ARCHETYPE *pArchetype;    //The Archetpye that owns this component.
  COMPONENT *nextComponent; //The next Component in a linked list of components.
}COMPONENT;


// COMPONENT STRUCT
// Adds physics functionality to an object.
typedef struct PHYSICS
{
  COMPONENT *pComponent;  //The component that holds this Physics.
  ARCHETYPE *pArchetype;  //The original archetype this came from.
  VECTOR Velocity;
  VECTOR Acceleration;
  float Gravity;
  float Friction;
  float MaxSpeed;

}PHYSICS;



//COMPONENT STRUCT
//Adds collision functionality to an object.
typedef struct COLLIDER
{
  COMPONENT *pComponent;  //The component that holds this Collider.
  ARCHETYPE *pArchetype;  //The original archetype this came from.
  BOOL Enabled;
  VECTOR Offset;
  float Height;
  float Width;
  BOOL IsGhosted;
  float Grounded;
  float TopBlocked;
  float LeftBlocked;
  float RightBlocked;
  BOOL RightGrounded;
  BOOL LeftGrounded;
  BOOL GhostEnter;
  BOOL GhostStay;
  BOOL GhostExit;
  COLLIDER * pCollidedWithGhost;

}COLLIDER;


// COMPONENT STRUCT
// Adds sound functionality to an object.
typedef struct KSOUND
{
  COMPONENT *pComponent;  //The component that holds this Sound.
  ARCHETYPE *pArchetype;  //The original archetype this came from.
  float Volume;
  BOOL Positional;
  BOOL Loop;
  BOOL StreamIt; // Set TRUE for long files, FALSE for short single hits.
  float MaxReach;
  char * SoundFile;
  float Radius;
  FMOD_CHANNEL * Channel;
  BOOL PlayOnStart;
}KSOUND;

// COMPONENT STRUCT
// Holds information to display a texture.
typedef struct SPRITE
{
  COMPONENT *pComponent;  //The component that holds this Sprite.
  ARCHETYPE *pArchetype;  //The original archetype this came from.
  BOOL Visible;
  IMAGE *pImage;
  BOOL Animated;          //The Boolean that tells if it is animated.
  VECTOR RowCol;          //The number of rows and columns in the sprite sheet. 
  VECTOR Offset;          //The offset of the texture on the object.
  float AnimationSpeed;   //The speed of the animation.
  float TimeSinceLastFrame;   //The speed of the animation.
  char *CurrentAnimation;  // The current animation.
}SPRITE;


typedef struct IMAGE
{
  AEGfxTexture *pTexture; //The texture to display.
  char * TextureFile;
  IMAGE *pNextImage;
}IMAGE;


// COMPONENT STRUCT
// Holds information to render a mesh.
typedef struct MESH
{
  COMPONENT *pComponent;  //The component that holds this Sprite.
  ARCHETYPE *pArchetype;  //The original archetype this came from.
  VECTOR Size;            //The scale of the mesh (read only).
  AEGfxVertexList *pMeshLit; //The actual mesh.
  COLOR Color;
  float Opacity;
}MESH;


// COMPONENT STRUCT
// Holds pointer to a function that defines what an object should do.
// Will be passed "Triggers" such as "Update" or "Start" to tell
// the funtion what has triggered it.
typedef struct BEHAVIOR
{
  //Pointer to the funtion that defines the behavior.
  void(*BehaviorScript)(BEHAVIOR * Owner, char * Trigger, void * Data);

  VAR * nextVar; //Linked list of variables for the function to reference.

  COMPONENT * pComponent; //The component that holds this Behavior.
  ARCHETYPE * pArchetype; //The original archetype this came from.

}BEHAVIOR;


/* NOT A COMPONENT STRUCT 
  Similar to the Transform in Zero Engine. 
  Currently only the position value is being used.
  TRANSFORMS CANNOT BE COMPONENTS. Each unit has a Transform pointer,
  so there is no reason to put one on an archetype.
*/
typedef struct TRANSFORM
{
  VECTOR Position;
  float Rotation;
  VECTOR Scale;
}TRANSFORM;

typedef struct COLLISIONINFO
{
  COLLIDER * pCollider;
  UNIT * pUnit;
  TAG Tag;
};


//The stats for the game object.
typedef struct GAMESTATS
{
  float Health;       //(Placeholder)
  VECTOR SpawnPoint;  //(Placeholder)
  float Points;       //(Placeholder)

  float GridSize; //Multiplier for position * pixels.

  unsigned int currentLevel;  //GSM current level state
  unsigned int nextLevel;     //GSM next level state
  unsigned int previousLevel; //GSM previous level state

  LEVEL * pRunningLevel; //Level currently running.

  MESH *pDefaultMesh; //Default square mesh. (32x32 square).
  SPRITE *pDefaultSprite;         //Default sprite.   (White square)
  TRANSFORM *pDefaultTransform;   //Default Transform. (P:(0,0) R:(0) S:(1,1))
  BEHAVIOR *pDefaultBehavior;     //Default Behavior (currently NULL)

}GAMESTATS;


/* Each level has a linked list of units that are essentially the objects in the level.
They have initial information (pInitTransform, pInitArchetype) and current information
(pTransform, pArchetype). 

When a level is initialized, every unit is initialized by copying the information from 
pInitTransform and pInitArchetype into pTransform and pArchetype.

Note that pInitArchetype is a Game Archetype and should not be changed in runtime, while
pArchetype is a Unit Archetype and is an instance of the game archetype.
*/
typedef struct UNIT
{
  char *Name;     //Name of this Unit.
  TAG Tag; //Tag of this unit (Used to override archetype tag).
  TRANSFORM *pInitTransform; //Initial Transform information.
  ARCHETYPE *pInitArchetype; //Initial (Game) Archetype. Copied into pArchetype on initialize.
  VAR *nextVar; //Override variables. Name the same as variables on behaviors to override initial values.

  TRANSFORM *pTransform; //Instanced Transform, generated on initialize.
  ARCHETYPE * pArchetype; //Instanced (Unit) Archeytpe, generated on initialize.

  LEVEL * pLevel; //Level that owns this unit.
  UNIT * nextUnit; //next unit in the linked list of units that starts at pLevel.
}UNIT;


//Information used to start and update a level.
typedef struct LEVEL
{
  char * Name; //The name of the level.
  int Order; //When the level should be loaded, (0 is the first level, 1 is the next, ext).
  UNIT *nextUnit; //Linked list of Units, all of the objects that are in the level.
  CAMERA *pCamera; //(Placeholder)
  LEVEL *nextLevel; //Next level in the linked list of Levels that starts at pGame.
  GAME * pGame; //The game that owns this level.
}LEVEL;

//Placeholder
typedef struct CAMERA
{
  VECTOR Position;
  float Zoom;
}CAMERA;



typedef struct MATRIX
{
  float m[3][3];
}MATRIX;

//Used to have custom variables.
typedef struct VAR
{
  char * Name;
  void * Data;
  VTYPE Type;
  VAR * nextVar;
}VAR;

enum VARTYPE
{
  Int,
  Float,
  Vector,
  String,
  Color,
  Bool,
  Char,
  Matrix
};

typedef enum COMPONENTTYPE
{
  Sprite,
  Mesh,
  Behavior,
  Physics,
  Collider,
  KSound
}COMPONENTTYPE;

typedef enum TAG
{
  DEFAULT,
  PLAYER,
  BAD,
  ENEMY,
  WALL,
  WEAPON
}TAG;

TAG GetTagFromString(char * String);

typedef enum BOOL
{
  False,
  True
}BOOL;

typedef enum DIRECTION
{
	Bottom,
	Top,
	Left,
	Right
}DIRECTION;

typedef enum GAMESTATE
{
  Level0,
  Level1,

  LevelNum,

  Restart,
  Quit
}GAMESTATE;

void * AddVar(VTYPE Type, char * Name, BEHAVIOR * Owner);

VAR * AddUnitVar(VTYPE Type, char * Name, UNIT * Owner);

void * GetVar(char * Name, BEHAVIOR * Owner);

VAR * GetActualVar(char * Name, BEHAVIOR * Owner);


// Creates a new game with the given name and sets it's defaults.
GAME * InitializeGame(char * Name);

//Sets the defaults game stats for a given game. Called by InitializeGame.
GAMESTATS * SetDefaults(GAME * pGame);

//Creates an empty Archetype and adds it to the Game.
ARCHETYPE * CreateArchetype(GAME * pGame, char *Name);

//Frees all aspects of the game (UNFINISHED, DO NOT USE)
void FreeGame(GAME * pGame);

//Adds a default component of the given type to a given Archetype and return the new Component.
COMPONENT * AddComponent(ARCHETYPE *pArchetype, COMPONENTTYPE DesiredType);

//Adds a Behavior component with the given function as it's script. (Use AddComponent to simply add default Behavior).
COMPONENT * AddBehaviorComponent(ARCHETYPE *pArchetype, void(*BehaviorScript)(BEHAVIOR * Owner, char * Trigger, void * Data));

//Finds the first component of a given type on an archetype and returns the component.
COMPONENT * FindComponent(ARCHETYPE * pArchetype, COMPONENTTYPE DesiredType);

//Same as FindComponent, but returns the void pointer to the struct (SPRITE, SQUAREMESH, ext).
void * FindComponentStruct(ARCHETYPE * pArchetype, COMPONENTTYPE DesiredType);

//Finds an Archetype in the given Game of the given name.
ARCHETYPE * FindArchetypeByName(GAME *pGame, char *Name);

//Creates an empty Level.
LEVEL * AddLevel(GAME * pGame, char *Name, int Order);

//Adds a Unit of the given Archetype to the given level with the default Transform.
//DO NOT USE AT RUNTIME (Use InstantiateUnit)
UNIT * AddUnit(LEVEL *pLevel, ARCHETYPE *pArchetype, char *Name);

//Adds a new Unit AT RUNTIME (do not use on initialize, do not use AddUnit at runtime).
UNIT * InstantiateUnit(LEVEL *pLevel, char * ArchetypeName, VECTOR position);

//Finds all Units in a level with the given Archetype and stores them in given array. 
//Returns amount of Units stored, -1 (ERR_SURPASSED_BUFFER_SIZE) if surpassed the buffer size.
//(UNFINISHED DO NOT USE)
int FindAllUnitsByArchetype(LEVEL *pLevel, ARCHETYPE *pArchetype, UNIT * pUnitArray[], int BufferSize);

//Finds a Unit by name (Units in the same level cannot have the same name). 
//UNFINISHED DO NOT USE
UNIT * FindUnitByName(LEVEL *pLevel, char *Name);

//Finds a level in a given game that has the given name.
LEVEL * FindLevelByName(GAME *pGame, char * Name);

//Finds a level in a given game with the given order (0, 1, 2, ext).
LEVEL * FindLevelByOrder(GAME *pGame, int Order);

//Initializes a unit in a level, 
//creates an instance of it's archetype and transform and initializes
//values. Also calls "Start" trigger on all behaviors.
void InitializeUnit(UNIT * pUnit);

ARCHETYPE * CreateInstanceOfArchetype(ARCHETYPE * pArchetype, UNIT * pUnit);

//Returns a new color with the given rgba values.
COLOR NewColor(float r, float g, float b, float a);

//Returns a new vector with given x and y.
VECTOR NewVector(float x, float y);

//Destroys a given unit (UNFINISHED DO NOT USE)
void DestroyUnit(UNIT *pUnit);

//Finds a Unit by a given name in a level and destroys it.
//(UNFINISHED DO NOT USE)
void DestroyUnitByName(LEVEL *pLevel, char *Name);

//Finds all units of a given archetype in a given level and destroys them.
//(UNFINISHED DO NOT USE).
void DestroyAllUnitsByArchetype(LEVEL *pLevel, ARCHETYPE *pArchetype);