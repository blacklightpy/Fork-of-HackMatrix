#include "chunk.h"
#include <cassert>
#include <memory>
#include <vector>

Chunk::Chunk(int x, int y, int z)
  : posX(x)
  , posY(y)
  , posZ(z)
{
  mesher = make_shared<Mesher>(this, x, z);
  data = vector<shared_ptr<Cube>>(size[0] * size[1] * size[2]);
}

const vector<int> Chunk::size = { 32, 384, 32 };

Chunk::Chunk()
{
  posX = 0;
  posY = 0;
  posZ = 0;
  mesher = make_shared<Mesher>(this, posX, posZ);
  data = vector<shared_ptr<Cube>>(size[0] * size[1] * size[2]);
}

Chunk::Chunk(const Chunk& other)
{
  posX = other.posX;
  posY = other.posY;
  posZ = other.posZ;
  data = vector<shared_ptr<Cube>>(size[0] * size[1] * size[2]);
  for (int i = 0; i < size[0] * size[1] * size[2]; i++) {
    data[i] = other.data[i];
  }
  mesher = other.mesher;
}

shared_ptr<Cube>
Chunk::getCube(int x, int y, int z)
{
  auto rv = data[index(x, y, z)];
  if (rv == NULL) {
    return null;
  }
  return rv;
}

shared_ptr<Cube>
Chunk::getCube_(int x, int y, int z)
{
  if (x >= 0 && x < size[0] && y >= 0 && y < size[1] && z >= 0 && z < size[2]) {
    return data[index(x, y, z)];
  }
  return NULL;
}

void
Chunk::removeCube(int x, int y, int z)
{
  array<int, 3> pos = { x, y, z };
  mesher->meshDamaged(pos);
  data[index(x, y, z)].reset();
  count--;
}
void
Chunk::addCube(Cube c, int x, int y, int z)
{
  array<int, 3> pos = { x, y, z };
  mesher->meshDamaged(pos);
  data[index(x, y, z)] = make_shared<Cube>(c);
  count++;
}

ChunkMesh
Chunk::meshedFaceFromPosition(Position position)
{
  return mesher->meshedFaceFromPosition(position);
}

shared_ptr<ChunkMesh> getEmptyChunkMesh() {
  static std::vector<glm::vec3> positions;
  static std::vector<glm::vec2> texCoords;
  static std::vector<int> blockTypes;
  static std::vector<int> selects;
  static shared_ptr<ChunkMesh> rv = std::make_shared<ChunkMesh>(
    SIMPLE, positions, texCoords, blockTypes, selects);

  return rv;
}

shared_ptr<ChunkMesh>
Chunk::mesh()
{
  if(count > 0) {
    return mesher->mesh();
  } else {
    return getEmptyChunkMesh();
  }
}

void
Chunk::meshAsync()
{
  mesher->meshAsync();
}

int
Chunk::index(int x, int y, int z)
{
  return x * size[1] * size[2] + y * size[2] + z;
}

ChunkCoords
Chunk::getCoords(int index)
{
  ChunkCoords rv;
  rv.z = index % size[2];
  index /= size[2];

  rv.y = index % size[1];
  index /= size[1];

  rv.x = index;
  return rv;
}

const vector<int>
Chunk::getSize()
{
  return size;
}

ChunkPosition
Chunk::getPosition()
{
  return ChunkPosition{ posX, posY, posZ };
}
