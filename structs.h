#ifndef __STRUCTS_H__	//guard against cyclic dependancy
#define __STRUCTS_H__

typedef struct{
	float x, y, z;
}Vector3D;

typedef struct{
	float x, y, z, w;
}Vector4D;

struct Obsidian {
	float m_amb[3] = {0.05375, 0.05, 0.06625}; 
	float m_diff[3] = {0.18275, 0.17, 0.22525};
	float m_spec[3] = {0.332741, 0.328634, 0.346435}; 
	float shiny = 0.3f; 
};

struct Emerald
{
	float m_amb[3] = {0.0215, 0.1745, 0.0215}; 
	float m_diff[3] = {0.07568, 0.61424, 0.07568};
	float m_spec[3] = {0.633, 0.727811, 0.633}; 
	float shiny = 0.6f; 
};

struct Gold
{
	float m_amb[3] = {0.24725, 0.1995, 0.0745}; 
	float m_diff[3] = {0.75164, 0.60648, 0.22648};
	float m_spec[3] = {0.628281, 0.555802, 0.366065}; 
	float shiny = 0.4f; 
};

struct Ruby
{
	float m_amb[3] = {0.1745, 0.01175, 0.01175}; 
	float m_diff[3] = {0.61424, 0.04136, 0.04136};
	float m_spec[3] = {0.727811, 0.626959, 0.626959}; 
	float shiny = 0.6f; 
};


struct BlackRubber
{
	float m_amb[3] = {0.02, 0.02, 0.02}; 
	float m_diff[3] = {0.01, 0.01, 0.01};
	float m_spec[3] = {0.4, 0.4, 0.4}; 
	float shiny = 0.078125f; 
};



#endif