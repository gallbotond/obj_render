#ifndef OBJECT_H
#define OBJECT_H

#include <vector.h>

using namespace shl;

struct vec3
{
	union { float x;float r; };
	union { float y;float g; };
	union { float z;float b; };
};

struct face
{
	char* mat_name = __nullptr;
	vector<vector<int>> n;
};

struct material
{
	char* mat_name = nullptr;
	float Ns = 0;
	vec3 Ka = vec3(), Kd = vec3(), Ks = vec3(), Ke = vec3();
	float Ni = 0;
	float d = 0;
};

struct wavefront
{
	vector<vec3> v;
	vector<vec3> vt;
	vector<vec3> vn;
	vector<face> f;
	vector<material> m;
};

class object
{
private:
	unsigned long obj_count = 0;
	void loadmtl(char*);
	char* mtllib;
	template <typename T>
	void log(T);

public:
	void addObject(const char*);
	unsigned long count();
	vector<wavefront> obj;

	object();
	~object();

};

#endif // !OBJECT_H