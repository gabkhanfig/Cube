#include "ChunkRenderer.h"
#include "../../Graphics/OpenGL/Shader/Shader.h"
#include "../../Graphics/OpenGL/Buffers/VertexBufferObject.h"
#include "../../Graphics/OpenGL/Buffers/IndexBufferObject.h"
#include "../../Graphics/OpenGL/Buffers/VertexArrayObject.h"
#include "../../Graphics/OpenGL/Buffers/ShaderBufferObject.h"
#include "../../../Resources/GeneratedFiles.h"
#include "../../Graphics/Geometry/BlockGeometry.h"
#include "../WorldTransform.h"
#include "../World.h"
#include "../../Player/Player.h"
#include "../Chunk/Chunk.h"
#include "../../Graphics/OpenGL/Render/Renderer.h"
#include "../Chunk/ChunkRenderComponent.h"
#include "../../Graphics/OpenGL/Camera/Camera.h"
#include <glad/glad.h>

ChunkRenderer::ChunkRenderer()
  : chunkOffsetUniform("u_chunkOffset"), cameraMvpUniform("u_cameraMVP"), boundDrawCallId(0), modifyDrawCallId(1)
{
  shader = new Shader(generated_Chunk_vert, generated_Chunk_frag);
  multidrawShader = new Shader(generated_ChunkMultidraw_vert, generated_ChunkMultidraw_frag);
  vao = new VertexArrayObject();
  vao->SetFormatLayout(BlockQuad::GetQuadsVertexBufferLayout());
  multidrawVbos = new PersistentMappedTripleVbo<BlockQuad>();
  multidrawIbos = new PersistentMappedTripleIbo();
  multidrawOffsets = new PersistentMappedTripleVbo<glm::vec3>();
  multidrawIndirectBuffers = new PersistentMappedTripleIndirect();
}

glm::vec3 ChunkRenderer::GetOffsetForChunkDraw(const Chunk* chunk) const
{
  const glm::dvec3 playerLocation = GetWorld()->GetPlayer()->GetLocation();
  const ChunkPosition _cPos = chunk->GetPosition();
  const glm::dvec3 chunkLocation = glm::dvec3(_cPos.x * CHUNK_LENGTH, _cPos.y * CHUNK_LENGTH, _cPos.z * CHUNK_LENGTH);
  return glm::vec3(double(chunkLocation.x - playerLocation.x), double(chunkLocation.y - playerLocation.y), double(chunkLocation.z - playerLocation.z));
}

void ChunkRenderer::SetShaderChunkOffset(glm::vec3 chunkOffset)
{
  shader->SetUniform3f(chunkOffsetUniform, chunkOffset);
}

void ChunkRenderer::SetShaderCameraMVP(const glm::mat4& cameraMVP)
{
  shader->SetUniformMat4f(cameraMvpUniform, cameraMVP);
}

void ChunkRenderer::Bind()
{
  shader->Bind();
  vao->Bind();
}

void ChunkRenderer::BindBlocksVertexBufferObject(VertexBufferObject* vbo)
{
  vao->BindVertexBufferObject(vbo, sizeof(BlockVertex));
}

void ChunkRenderer::Draw(VertexBufferObject* vbo, IndexBufferObject* ibo)
{
  Renderer::DrawVboTriangles(vbo, ibo);
}

void ChunkRenderer::ReserveVbosAndIbosForChunkQuantity(uint32 chunksNum)
{
  check(chunksNum > 0);
  std::cout << chunksNum << std::endl;
  const uint32 vbosCapacity = chunksNum * ChunkRenderComponent::GetMaximumQuadsPerChunkMesh();
  const uint32 ibosCapacity = chunksNum * ChunkRenderComponent::GetMaximumIndicesPerChunkMesh();
  const uint32 indirectCapacity = chunksNum;

  std::cout << vbosCapacity << std::endl;

  multidrawVbos->Reserve(vbosCapacity);
  multidrawIbos->Reserve(ibosCapacity);
  multidrawOffsets->Reserve(chunksNum);
  multidrawIndirectBuffers->Reserve(indirectCapacity);
}

void ChunkRenderer::SwapNextBuffers()
{
  multidrawVbos->SwapNextBuffer();
  multidrawIbos->SwapNextBuffer();
  multidrawOffsets->SwapNextBuffer();
  multidrawIndirectBuffers->SwapNextBuffer();
}

void ChunkRenderer::DrawAllChunks(const HashMap<ChunkPosition, Chunk*>& chunks)
{
  shader->Bind();
  Camera* cam = Camera::GetActiveCamera();
  SetShaderCameraMVP(cam->GetMvpMatrix());
  for (auto& chunkPair : chunks) {
    Chunk* chunk = chunkPair.second;
    ChunkRenderComponent* renderComponent = chunk->GetRenderComponent();
    renderComponent->Draw(this);
  }
}

void ChunkRenderer::MultidrawIndirectAllChunks(const HashMap<ChunkPosition, Chunk*>& chunks)
{
  //https://github.com/litasa/Advanced-OpenGL-Examples/blob/master/src/MultidrawIndirect/MultidrawIndirect.cpp

  //Benchmark multidraw{ "multidraw indirect all chunks pre-draw" };

  multidrawShader->Bind(); // Use the multidraw shader
  Camera* cam = Camera::GetActiveCamera(); 
  multidrawShader->SetUniformMat4f(cameraMvpUniform, cam->GetMvpMatrix()); // Set the u_cameraMVP matrix in ChunkMultidraw.vert

  darray<const Chunk*> drawChunks;
  drawChunks.Reserve(chunks.size());

  uint32 requiredVboCapacity = 0;
  uint32 requiredIboCapacity = 0;
  uint32 requiredOffsetCount = 0;
  uint32 requiredIndirectCapacity = 0;

  // Check which chunks should be drawn, and how much buffer capacity is required.
  for (const auto& chunkPair : chunks) {
    const Chunk* chunk = chunkPair.second;
    ChunkRenderComponent* renderComponent = chunk->GetRenderComponent();
    if (renderComponent->IsMeshEmpty()) continue;

    const ChunkMesh* mesh = renderComponent->GetMesh();

    drawChunks.Add(chunk);
    requiredVboCapacity += mesh->GetQuadCount();
    requiredIboCapacity += mesh->GetIndexCount();
    requiredOffsetCount++;
    requiredIndirectCapacity++;
  }

  //std::cout << "quad count: " << requiredVboCapacity << std::endl;
  //std::cout << "index count: " << requiredIboCapacity << std::endl;

  if (multidrawVbos->GetCapacity() < requiredVboCapacity) { // Reserve more block quad vbo capacity if necessary
    multidrawVbos->Reserve(requiredVboCapacity);
  }
  if (multidrawIbos->GetCapacity() < requiredIboCapacity) { // Reserve more ibo capacity if necessary
    multidrawIbos->Reserve(requiredIboCapacity);
  }
  if (multidrawOffsets->GetCapacity() < requiredOffsetCount) { // Reserve more offset vbo capacity if necessary
    multidrawOffsets->Reserve(requiredOffsetCount);
  }
  if (multidrawIndirectBuffers->GetCapacity() < requiredIndirectCapacity) { // Reserve more indirect buffer capacity if necessary
    multidrawIndirectBuffers->Reserve(requiredIndirectCapacity);
  }

  uint32 chunkIndex = 0;
  uint32 baseQuad = 0;
  uint32 indexOffset = 0;

  darray<DrawElementsIndirectCommand> indirectCommands;
  darray<glm::vec3> chunkOffsets;
  darray<BlockQuad> quads;
  darray<uint32> indices;

  for (const Chunk* chunk : drawChunks) {
    if (chunk->GetPosition().x > 3) continue;
    //if (chunkIndex == 300) break;
    //std::cout << chunkIndex << std::endl;
    // TODO add chunk offsets as a mat4 for vertex shader

    //const Chunk* chunk = chunkPair.second;
    ChunkRenderComponent* renderComponent = chunk->GetRenderComponent();

    indirectCommands.Add(renderComponent->GenerateDrawElementsIndirectCommand(
      baseQuad, // Starting from this vertex as an offset into the vbo (4 vertices per quad)
      chunkIndex)); // gl_InstanceID
    chunkOffsets.Add(GetOffsetForChunkDraw(chunk));

    //PersistentMappedTripleVbo<BlockQuad>::MappedVbo& mappedVbo = multidrawVbos->GetModifyMappedVbo();
    //renderComponent->CopyMeshQuadsToVboOffset(mappedVbo, baseQuad); // Copy the mesh data into the modifiable vbo starting at baseQuad offset into the memory

    //PersistentMappedTripleIbo::MappedIbo& mappedIbo = multidrawIbos->GetModifyMappedIbo();
    //renderComponent->CopyMeshIndicesToIboOffset(mappedIbo, indexOffset); // Copy the indices into the modifiable ibo starting at indexOffset into the memory

    const ChunkMesh* mesh = renderComponent->GetMesh();
    quads.Append(mesh->GetQuads());
    indices.Append(mesh->GetIndices());

    chunkIndex++; // Increment for the next chunk
    baseQuad += mesh->GetQuadCount() * 4; // Increment the quad memory offset for the next chunk
    indexOffset += mesh->GetIndexCount(); // Increment the index memory offset for the next chunk
    //std::cout << "baseQuad: " << baseQuad << std::endl;
    //std::cout << "indexOffset: " << indexOffset << std::endl;
  }
  //std::cout << "\n\n";

  std::cout << quads.Size() << std::endl;
  std::cout << indices.Size() << std::endl;

  VertexBufferObject* quadsVbo = VertexBufferObject::Create<BlockQuad>(quads.Data(), quads.Size());
  IndexBufferObject* quadsIbo = new IndexBufferObject(indices.Data(), indices.Size());
  BindBlocksVertexBufferObject(quadsVbo);

  VertexBufferObject* offsetsVbo = VertexBufferObject::Create<glm::vec3>(chunkOffsets.Data(), chunkOffsets.Size());

  //PersistentMappedTripleIndirect::MappedIndirect& mappedIndirect = multidrawIndirectBuffers->GetModifyMappedDibo();
  //memcpy(mappedIndirect.data, indirectCommands.Data(), sizeof(DrawElementsIndirectCommand) * indirectCommands.Size()); // Copy the draw indirect commands into the modifiable buffer object

  //VertexBufferObject* boundVbo = multidrawVbos->GetBoundVbo();
  //IndexBufferObject* boundIbo = multidrawIbos->GetBoundIbo();
  //DrawIndirectBufferObject* boundIndirect = multidrawIndirectBuffers->GetBoundBufferObject();
  //BindBlocksVertexBufferObject(boundVbo); // Bind the vbo to the vao's format

  //PersistentMappedTripleVbo<glm::vec3>::MappedVbo& mappedOffset = multidrawOffsets->GetModifyMappedVbo();
  //memcpy(mappedOffset.data, chunkOffsets.Data(), chunkOffsets.Size() * sizeof(glm::vec3));

  //multidrawOffsets->GetBoundVbo()->Bind();

  // Use attribute location 4.
  glEnableVertexAttribArray(4);
  // Use vec3 for offset.
  glVertexAttribPointer(4, 
    3, // 3 of...
    GL_FLOAT, // floats
    GL_FALSE, // that arent normalized
    sizeof(glm::vec3), // with a size of a vec3 (tightly packed)
    (void*)0); // with no offset between generic attributes.
  // Only apply one attrib per instance (chunk)
  glVertexAttribDivisor(4, 1);

  //boundVbo->Bind();
  //boundIbo->Bind(); // Bind the ibo
  //boundIndirect->Bind(); // Bind the draw elements indirect commands

  DrawIndirectBufferObject* dbo = DrawIndirectBufferObject::Create(indirectCommands.Data(), indirectCommands.Size());

  glMultiDrawElementsIndirect(
    GL_TRIANGLES,     //type
    GL_UNSIGNED_INT,  //indices represented as unsigned ints
    (void*)0,         //start with the first draw command, relies on previous bind to indirect buffer
    chunkIndex,       //draw n chunks
    0);               //no stride, the draw commands are tightly packed

  SwapNextBuffers();

  //glDeleteBuffers(1, &offsetsVbo);
  delete quadsVbo;
  delete quadsIbo;
  delete offsetsVbo;
  delete dbo;
}

void ChunkRenderer::StoreModifyDrawCallData()
{
  DrawCallData& modifyDrawData = drawCalls[modifyDrawCallId];
  modifyDrawData.playerPos = GetWorld()->GetPlayer()->GetLocation();
  modifyDrawData.cameraMVP = Camera::GetActiveCamera()->GetMvpMatrix();


}

ChunkMesh* ChunkRenderer::GetChunkMesh(Chunk* chunk) const
{
  auto found = meshes.find(chunk);
  check(found != meshes.end());
  return found->second;
}

void ChunkRenderer::AllocateMeshesForChunks(const HashMap<ChunkPosition, Chunk*>& chunks)
{
  for (const auto& chunkPair : chunks) {
    if (meshes.contains(chunkPair.second)) continue;
    meshes.insert({ chunkPair.second, new ChunkMesh() });
  }
}
