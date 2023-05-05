#pragma once

// https://github.com/Enhanced-Source-Project/Enhanced-Source/blob/master/src/public/bspfile.h
// https://developer.valvesoftware.com/wiki/Source_BSP_File_Format
// I can just copy from there right? Reverse-engineering is against the EULA...

// https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/public/Color.h
typedef unsigned char color32[4];
typedef float Vector[3];
typedef float QAngle[3];

struct StaticPropLump9_t {
	Vector m_Origin;
	QAngle m_Angles;
	unsigned short m_PropType;
	unsigned short m_FirstLeaf;
	unsigned short m_LeafCount;
	unsigned char m_Solid;
	unsigned char m_Flags;
	int m_Skin;
	float m_FadeMinDist;
	float m_FadeMaxDist;
	Vector m_LightingOrigin;
	float m_flForcedFadeScale;
	unsigned char m_nMinCPULevel;
	unsigned char m_nMaxCPULevel;
	unsigned char m_nMinGPULevel;
	unsigned char m_nMaxGPULevel;
//	int m_Lighting;            // index into the GAMELUMP_STATIC_PROP_LIGHTING lump
	color32 m_DiffuseModulation;    // per instance color and alpha modulation
	bool m_bDisableX360;
};

struct StaticPropLump11_t {
	Vector m_Origin;
	QAngle m_Angles;
	unsigned short m_PropType;
	unsigned short m_FirstLeaf;
	unsigned short m_LeafCount;
	unsigned char m_Solid;
	unsigned char m_Flags;
	int m_Skin;
	float m_FadeMinDist;
	float m_FadeMaxDist;
	Vector m_LightingOrigin;
	float m_flForcedFadeScale;
	unsigned char m_nMinCPULevel;
	unsigned char m_nMaxCPULevel;
	unsigned char m_nMinGPULevel;
	unsigned char m_nMaxGPULevel;
//	int m_Lighting;            // index into the GAMELUMP_STATIC_PROP_LIGHTING lump
	color32 m_DiffuseModulation;    // per instance color and alpha modulation
	bool m_bDisableX360;
	unsigned int FlagsEx;           // Further bitflags.
	// since v11
	float UniformScale;      // Prop scale
};

struct GameLump_t {
//	int id;        // gamelump ID
	char id[4];        // gamelump ID
	unsigned short flags;     // flags
	unsigned short version;   // gamelump version
	int fileofs;   // offset to this gamelump
	int filelen;   // length
};

struct Lump_t {
	int fileofs;      // offset into file (bytes)
	int filelen;      // length of lump (bytes)
	int version;      // lump format version
	char fourCC[4];    // lump ident code
};

// https://developer.valvesoftware.com/wiki/Source_BSP_File_Format#Lump_types
enum HeaderLumpIndex : uint8_t {
	LUMP_PROPCOLLISION = 22,    // static props convex hull lists
	LUMP_PROPHULLS = 23,    // static prop convex hulls
	LUMP_PROPHULLVERTS = 24,    // static prop collision verts
	LUMP_PROPTRIS = 25,    // static prop per hull triangle index start/count

	LUMP_GAME_LUMP = 35, // Game-specific data lump
};

#define HEADER_LUMP_COUNT 64

struct BSPHeader_t {
	int ident;                  // BSP file identifier
	int version;                // BSP file version
	Lump_t lumps[HEADER_LUMP_COUNT];    // lump directory array
	int mapRevision;            // the map's revision (iteration, version) number
};