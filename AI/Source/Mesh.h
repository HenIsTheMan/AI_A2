#ifndef MESH_H
#define MESH_H

#include <string>
#include "Material.h"

class Mesh{
public:
	enum struct DRAW_MODE: int{
		DRAW_TRIANGLES,
		DRAW_TRIANGLE_STRIP,
		DRAW_LINES,
		DRAW_MODE_LAST,
		Amt
	};

	Mesh(const std::string &meshName);
	virtual ~Mesh();
	virtual void Render() const;
	void Render(unsigned offset, unsigned count) const;

	const std::string name;
	DRAW_MODE mode;
	unsigned vertexBuffer;
	unsigned indexBuffer;
	unsigned indexSize;

	Material material;
	unsigned textureID;
};

#endif