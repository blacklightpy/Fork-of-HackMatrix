#pragma once
#include "mesh.h"
#include "persister.h"
#include "shader.h"
#include <assimp/mesh.h>
#include <assimp/scene.h>
#include "entity.h"

unsigned int TextureFromFile(const char *path, const string &directory,
                             bool gamma = false);

struct Light {
  glm::vec3 color;
};

class LightPersister: public SQLPersisterImpl {
public:
  LightPersister(std::shared_ptr<EntityRegistry> registry)
      : SQLPersisterImpl(registry){};
  void createTablesIfNeeded() override;
  void saveAll() override;
  void save(entt::entity) override;
  void loadAll() override;
  void load(entt::entity) override;
};

struct Positionable {
  Positionable(glm::vec3 pos, float scale);
  glm::vec3 pos;
  float scale;
  glm::mat4 modelMatrix;
  glm::mat3 normalMatrix;
};

class PositionablePersister : public SQLPersisterImpl {
public:
  PositionablePersister(std::shared_ptr<EntityRegistry> registry)
      : SQLPersisterImpl(registry){};
  void createTablesIfNeeded() override;
  void saveAll() override;
  void save(entt::entity) override;
  void loadAll() override;
  void load(entt::entity) override;
};

class Model {
public:
  string path;
  Model(string path);
  void Draw(Shader &shader);

private:
  // model data
  vector<Mesh> meshes;
  vector<MeshTexture> textures_loaded;
  string directory;

  void loadModel(string path);
  void processNode(aiNode *node, const aiScene *scene);
  Mesh processMesh(aiMesh *mesh, const aiScene *scene);
  vector<MeshTexture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                       string typeName);
};

class ModelPersister : public SQLPersisterImpl {
public:
  ModelPersister(std::shared_ptr<EntityRegistry> registry):
    SQLPersisterImpl(registry){};
  void createTablesIfNeeded() override;
  void saveAll() override;
  void save(entt::entity) override;
  void loadAll() override;
  void load(entt::entity) override;
};