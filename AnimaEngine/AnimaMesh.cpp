//
//  AnimaMesh.cpp
//  Anima
//
//  Created by Marco Zille on 25/11/14.
//
//

#include <stdio.h>
#include <string.h>
#include "AnimaMesh.h"

BEGIN_ANIMA_ENGINE_NAMESPACE

AnimaMesh::AnimaMesh(AnimaEngine* engine)
: _meshName(engine)
{
	ANIMA_ASSERT(engine != nullptr)
	_engine = engine;
	
	_vertices = nullptr;
	_normals = nullptr;
	_textureCoords = nullptr;
	_faces = nullptr;

	_indexesBufferObject = 0;
	_verticesBufferObject = 0;
	_colorsBufferObject = 0;
	_vertexArrayObject = 0;
	_needsBuffersUpdate = true;

	_verticesNumber = 0;
	_normalsNumber = 0;
	_textureCoordsNumber = 0;
	_facesNumber = 0;
	_meshName = "_mesh_";
}

AnimaMesh::AnimaMesh(const AnimaMesh& src)
	: _meshName(src._meshName)
{
	_engine = src._engine;

	_indexesBufferObject = src._indexesBufferObject;
	_verticesBufferObject = src._verticesBufferObject;
	_colorsBufferObject = src._colorsBufferObject;
	_vertexArrayObject = src._vertexArrayObject;
	_needsBuffersUpdate = src._needsBuffersUpdate;
	
	_vertices = nullptr;
	_normals = nullptr;
	_textureCoords = nullptr;
	_faces = nullptr;
	
	_verticesNumber = 0;
	_normalsNumber = 0;
	_textureCoordsNumber = 0;
	_facesNumber = 0;
	
	SetVertices(src._vertices, src._verticesNumber);
	SetNormals(src._normals, src._normalsNumber);
	SetTextureCoords(src._textureCoords, src._textureCoordsNumber);
	SetFaces(src._faces, src._facesNumber);
}

AnimaMesh::AnimaMesh(AnimaMesh&& src)
: _vertices(src._vertices)
, _verticesNumber(src._verticesNumber)
, _normals(src._normals)
, _normalsNumber(src._normalsNumber)
, _textureCoords(src._textureCoords)
, _textureCoordsNumber(src._textureCoordsNumber)
, _faces(src._faces)
, _facesNumber(src._facesNumber)
, _engine(src._engine)
, _meshName(src._meshName)
, _indexesBufferObject(src._indexesBufferObject)
, _verticesBufferObject(src._verticesBufferObject)
, _needsBuffersUpdate(src._needsBuffersUpdate)
, _vertexArrayObject(src._vertexArrayObject)
, _colorsBufferObject(src._colorsBufferObject)
{
	src._vertices = nullptr;
	src._normals = nullptr;
	src._textureCoords = nullptr;
	src._faces = nullptr;
	
	src._verticesNumber = 0;
	src._normalsNumber = 0;
	src._textureCoordsNumber = 0;
	src._facesNumber = 0;
}

AnimaMesh::~AnimaMesh()
{
	ANIMA_ASSERT(_engine != nullptr);
	
	ClearVertices();
	ClearNormals();
	ClearTextureCoords();
	ClearFaces();
}

AnimaMesh& AnimaMesh::operator=(const AnimaMesh& src)
{
	if (this != &src)
	{
		_engine = src._engine;
		_indexesBufferObject = src._indexesBufferObject;
		_verticesBufferObject = src._verticesBufferObject;
		_colorsBufferObject = src._colorsBufferObject;
		_needsBuffersUpdate = src._needsBuffersUpdate;
		_vertexArrayObject = src._vertexArrayObject;
		
		SetMeshName(src._meshName);
		SetVertices(src._vertices, src._verticesNumber);
		SetNormals(src._normals, src._normalsNumber);
		SetTextureCoords(src._textureCoords, src._textureCoordsNumber);
		SetFaces(src._faces, src._facesNumber);
	}
	
	return *this;
}

AnimaMesh& AnimaMesh::operator=(AnimaMesh&& src)
{
	if (this != &src)
	{
		_engine = src._engine;
		
		_vertices = src._vertices;
		_normals = src._normals;
		_textureCoords = src._textureCoords;
		_faces = src._faces;
		
		_verticesNumber = src._verticesNumber;
		_normalsNumber = src._normalsNumber;
		_textureCoordsNumber = src._textureCoordsNumber;
		_facesNumber = src._facesNumber;

		_indexesBufferObject = src._indexesBufferObject;
		_verticesBufferObject = src._verticesBufferObject;
		_colorsBufferObject = src._colorsBufferObject;
		_vertexArrayObject = src._vertexArrayObject;
		_needsBuffersUpdate = src._needsBuffersUpdate;

		_meshName = src._meshName;
		
		src._vertices = nullptr;
		src._normals = nullptr;
		src._textureCoords = nullptr;
		src._faces = nullptr;
		
		src._verticesNumber = 0;
		src._normalsNumber = 0;
		src._textureCoordsNumber = 0;
		src._facesNumber = 0;
	}
	
	return *this;
}

void AnimaMesh::ClearVertices()
{
	if(_vertices != nullptr && _verticesNumber > 0)
	{
		AnimaAllocatorNamespace::DeallocateArray(*(_engine->GetModelDataAllocator()), _vertices);
		_vertices = nullptr;
		_verticesNumber = 0;
	}
}

void AnimaMesh::ClearNormals()
{
	if(_normals != nullptr && _normalsNumber > 0)
	{
		AnimaAllocatorNamespace::DeallocateArray(*(_engine->GetModelDataAllocator()), _normals);
		_normals = nullptr;
		_normalsNumber = 0;
	}
}

void AnimaMesh::ClearTextureCoords()
{
	if(_textureCoords != nullptr && _textureCoordsNumber > 0)
	{
		AnimaAllocatorNamespace::DeallocateArray(*(_engine->GetModelDataAllocator()), _textureCoords);
		_textureCoords = nullptr;
		_textureCoordsNumber = 0;
	}
}

void AnimaMesh::ClearFaces()
{
	if(_faces != nullptr && _facesNumber > 0)
	{
		AnimaAllocatorNamespace::DeallocateArray(*(_engine->GetModelDataAllocator()), _faces);
		_faces = nullptr;
		_facesNumber = 0;
	}
}

//void AnimaMesh::SetVertices(AnimaVertex4f* v, ASizeT n)
//{
//	ANIMA_ASSERT(_engine != nullptr)
//	ClearVertices();
//	
//	if(v != nullptr && n > 0)
//	{
//		_verticesNumber = n;
//		_vertices = AnimaAllocatorNamespace::AllocateArray<AnimaVertex4f>(*(_engine->GetModelDataAllocator()), n, _engine);
//	
//		for (int i = 0; i < _verticesNumber; i++)
//			_vertices[i] = v[i];
//	}
//	//memcpy(_vertices, v, sizeof(AnimaVertex4f) * _verticesNumber);
//}

void AnimaMesh::SetVertices(AnimaVertex3f* v, ASizeT n)
{
	ANIMA_ASSERT(_engine != nullptr)
	ClearVertices();
	
	if(v != nullptr && n > 0)
	{
		_verticesNumber = n;
		_vertices = AnimaAllocatorNamespace::AllocateArray<AnimaVertex3f>(*(_engine->GetModelDataAllocator()), n, _engine);
		
		for(int i = 0; i < _verticesNumber; i++)
		{
			_vertices[i][0] = v[i][0];
			_vertices[i][1] = v[i][1];
			_vertices[i][2] = v[i][2];
			//_vertices[i][0] = 1.0f;
		}
	}
}

//void AnimaMesh::AddVertex(const AnimaVertex4f& v)
//{
//	ANIMA_ASSERT(_engine != nullptr);
//	if(_verticesNumber > 0)
//	{
//		AnimaVertex4f* tmpOldVertices = AnimaAllocatorNamespace::AllocateArray<AnimaVertex4f>(*(_engine->GetModelDataAllocator()), _verticesNumber, _engine);
//	
//		//memcpy(tmpOldVertices, _vertices, sizeof(AnimaVertex4f) * _verticesNumber);
//		for (int i = 0; i < _verticesNumber; i++)
//			tmpOldVertices[i] = _vertices[i];
//	
//		//_allocator->Deallocate(_vertices);
//		AnimaAllocatorNamespace::DeallocateArray(*(_engine->GetModelDataAllocator()), _vertices);
//	
//		_verticesNumber++;
//		_vertices = AnimaAllocatorNamespace::AllocateArray<AnimaVertex4f>(*(_engine->GetModelDataAllocator()), _verticesNumber, _engine);
//	
//		//memcpy(_vertices, tmpOldVertices, sizeof(AnimaVertex4f) * (_verticesNumber - 1));
//		for (int i = 0; i < _verticesNumber - 1; i++)
//			_vertices[i] = tmpOldVertices[i];
//	
//		_vertices[_verticesNumber - 1] = v;
//	
//		AnimaAllocatorNamespace::DeallocateArray(*(_engine->GetModelDataAllocator()), tmpOldVertices);
//	}
//	else
//	{
//		_verticesNumber++;
//		_vertices = AnimaAllocatorNamespace::AllocateArray<AnimaVertex4f>(*(_engine->GetModelDataAllocator()), _verticesNumber, _engine);
//		
//		_vertices[_verticesNumber - 1] = v;
//	}
//}

void AnimaMesh::AddVertex(const AnimaVertex3f& v)
{
	ANIMA_ASSERT(_engine != nullptr);
	if(_verticesNumber > 0)
	{
		AnimaVertex3f* tmpOldVertices = AnimaAllocatorNamespace::AllocateArray<AnimaVertex3f>(*(_engine->GetModelDataAllocator()), _verticesNumber, _engine);
	
		//memcpy(tmpOldVertices, _vertices, sizeof(AnimaVertex4f) * _verticesNumber);
		for (int i = 0; i < _verticesNumber; i++)
			tmpOldVertices[i] = _vertices[i];
	
		//_allocator->Deallocate(_vertices);
		AnimaAllocatorNamespace::DeallocateArray(*(_engine->GetModelDataAllocator()), _vertices);
	
		_verticesNumber++;
		_vertices = AnimaAllocatorNamespace::AllocateArray<AnimaVertex3f>(*(_engine->GetModelDataAllocator()), _verticesNumber, _engine);
	
		//memcpy(_vertices, tmpOldVertices, sizeof(AnimaVertex4f) * (_verticesNumber - 1));
		for (int i = 0; i < _verticesNumber - 1; i++)
			_vertices[i] = tmpOldVertices[i];
	
		_vertices[_verticesNumber - 1][0] = v[0];
		_vertices[_verticesNumber - 1][1] = v[1];
		_vertices[_verticesNumber - 1][2] = v[2];
		//_vertices[_verticesNumber - 1][3] = 1.0f;
	
		AnimaAllocatorNamespace::DeallocateArray(*(_engine->GetModelDataAllocator()), tmpOldVertices);
	}
	else
	{
		_verticesNumber++;
		_vertices = AnimaAllocatorNamespace::AllocateArray<AnimaVertex3f>(*(_engine->GetModelDataAllocator()), _verticesNumber, _engine);
		
		_vertices[_verticesNumber - 1][0] = v[0];
		_vertices[_verticesNumber - 1][1] = v[1];
		_vertices[_verticesNumber - 1][2] = v[2];
		//_vertices[_verticesNumber - 1][3] = 1.0f;
	}
}

//void AnimaMesh::SetNormals(AnimaVertex4f* v, ASizeT n)
//{
//	ANIMA_ASSERT(_engine != nullptr)
//	ClearNormals();
//		
//	if(v != nullptr && n > 0)
//	{
//		_normalsNumber = n;
//		_normals = AnimaAllocatorNamespace::AllocateArray<AnimaVertex4f>(*(_engine->GetModelDataAllocator()), _normalsNumber, _engine);
//	
//		//memcpy(_normals, v, sizeof(AnimaVertex4f) * _normalsNumber);
//		for (int i = 0; i < _normalsNumber; i++)
//			_normals[i] = v[i];
//	}
//}

void AnimaMesh::SetNormals(AnimaVertex3f* v, ASizeT n)
{
	ANIMA_ASSERT(_engine != nullptr)
	ClearNormals();
	
	if(v != nullptr && n > 0)
	{
		_normalsNumber = n;
		_normals = AnimaAllocatorNamespace::AllocateArray<AnimaVertex3f>(*(_engine->GetModelDataAllocator()), _normalsNumber, _engine);
	
		for (int i = 0; i < _normalsNumber; i++)
		{
			_normals[i][0] = v[i][0];
			_normals[i][1] = v[i][1];
			_normals[i][2] = v[i][2];
			//_normals[i][0] = 1.0f;
		}
	}
}

//void AnimaMesh::AddNormal(const AnimaVertex4f& v)
//{
//	ANIMA_ASSERT(_engine != nullptr);
//	if(_normalsNumber > 0)
//	{
//		AnimaVertex4f* tmpOldNormals = AnimaAllocatorNamespace::AllocateArray<AnimaVertex4f>(*(_engine->GetModelDataAllocator()), _normalsNumber, _engine);
//	
//		//memcpy(tmpOldVertices, _vertices, sizeof(AnimaVertex4f) * _verticesNumber);
//		for (int i = 0; i < _normalsNumber; i++)
//			tmpOldNormals[i] = _normals[i];
//	
//		//_allocator->Deallocate(_normals);
//		AnimaAllocatorNamespace::DeallocateArray(*(_engine->GetModelDataAllocator()), _normals);
//	
//		_normalsNumber++;
//		_normals = AnimaAllocatorNamespace::AllocateArray<AnimaVertex4f>(*(_engine->GetModelDataAllocator()), _normalsNumber, _engine);
//	
//		//memcpy(_vertices, tmpOldVertices, sizeof(AnimaVertex4f) * (_verticesNumber - 1));
//		for (int i = 0; i < _normalsNumber - 1; i++)
//			_normals[i] = tmpOldNormals[i];
//	
//		_normals[_normalsNumber - 1] = v;
//	
//		AnimaAllocatorNamespace::DeallocateArray(*(_engine->GetModelDataAllocator()), tmpOldNormals);
//	}
//	else
//	{
//		_normalsNumber++;
//		_normals = AnimaAllocatorNamespace::AllocateArray<AnimaVertex4f>(*(_engine->GetModelDataAllocator()), _normalsNumber, _engine);
//		
//		_normals[_normalsNumber - 1] = v;
//	}
//}

void AnimaMesh::AddNormal(const AnimaVertex3f& v)
{
	ANIMA_ASSERT(_engine != nullptr);
	if(_normalsNumber > 0)
	{
		AnimaVertex3f* tmpOldNormals = AnimaAllocatorNamespace::AllocateArray<AnimaVertex3f>(*(_engine->GetModelDataAllocator()), _normalsNumber, _engine);
	
		//memcpy(tmpOldVertices, _vertices, sizeof(AnimaVertex4f) * _verticesNumber);
		for (int i = 0; i < _normalsNumber; i++)
			tmpOldNormals[i] = _normals[i];
		
		//_allocator->Deallocate(_normals);
		AnimaAllocatorNamespace::DeallocateArray(*(_engine->GetModelDataAllocator()), _normals);
	
		_normalsNumber++;
		_normals = AnimaAllocatorNamespace::AllocateArray<AnimaVertex3f>(*(_engine->GetModelDataAllocator()), _normalsNumber, _engine);
	
		//memcpy(_vertices, tmpOldVertices, sizeof(AnimaVertex4f) * (_verticesNumber - 1));
		for (int i = 0; i < _normalsNumber - 1; i++)
			_normals[i] = tmpOldNormals[i];
	
		_normals[_normalsNumber - 1][0] = v[0];
		_normals[_normalsNumber - 1][1] = v[1];
		_normals[_normalsNumber - 1][2] = v[2];
		//_normals[_normalsNumber - 1][3] = 1.0;
	
		AnimaAllocatorNamespace::DeallocateArray(*(_engine->GetModelDataAllocator()), tmpOldNormals);
	}
	else
	{
		_normalsNumber++;
		_normals = AnimaAllocatorNamespace::AllocateArray<AnimaVertex3f>(*(_engine->GetModelDataAllocator()), _normalsNumber, _engine);
		
		_normals[_normalsNumber - 1][0] = v[0];
		_normals[_normalsNumber - 1][1] = v[1];
		_normals[_normalsNumber - 1][2] = v[2];
		//_normals[_normalsNumber - 1][3] = 1.0;
	}
}

void AnimaMesh::SetTextureCoords(AnimaVertex2f* v, ASizeT n)
{
	ANIMA_ASSERT(_engine != nullptr);
	ClearTextureCoords();
	
	if(v != nullptr && n > 0)
	{
		_textureCoordsNumber = n;
		_textureCoords = AnimaAllocatorNamespace::AllocateArray<AnimaVertex2f>(*(_engine->GetModelDataAllocator()), _textureCoordsNumber, _engine);
	
		//memcpy(_textureCoords, v, sizeof(AnimaVertex2f) * _textureCoordsNumber);
		for (int i = 0; i < _textureCoordsNumber; i++)
			_textureCoords[i] = v[i];
	}
}

void AnimaMesh::AddTextureCoord(const AnimaVertex2f& v)
{
	ANIMA_ASSERT(_engine != nullptr);
	if(_textureCoordsNumber > 0)
	{
		AnimaVertex2f* tmpOldTextureCoords = AnimaAllocatorNamespace::AllocateArray<AnimaVertex2f>(*(_engine->GetModelDataAllocator()), _textureCoordsNumber, _engine);
		
		//memcpy(tmpOldVertices, _vertices, sizeof(AnimaVertex4f) * _verticesNumber);
		for (int i = 0; i < _textureCoordsNumber; i++)
			tmpOldTextureCoords[i] = _textureCoords[i];
	
		//_allocator->Deallocate(_normals);
		AnimaAllocatorNamespace::DeallocateArray(*(_engine->GetModelDataAllocator()), _textureCoords);
	
		_textureCoordsNumber++;
		_textureCoords = AnimaAllocatorNamespace::AllocateArray<AnimaVertex2f>(*(_engine->GetModelDataAllocator()), _textureCoordsNumber, _engine);
	
		//memcpy(_vertices, tmpOldVertices, sizeof(AnimaVertex4f) * (_verticesNumber - 1));
		for (int i = 0; i < _textureCoordsNumber - 1; i++)
			_textureCoords[i] = tmpOldTextureCoords[i];
	
		_textureCoords[_textureCoordsNumber - 1] = v;
	
		AnimaAllocatorNamespace::DeallocateArray(*(_engine->GetModelDataAllocator()), tmpOldTextureCoords);
	}
	else
	{
		_textureCoordsNumber++;
		_textureCoords = AnimaAllocatorNamespace::AllocateArray<AnimaVertex2f>(*(_engine->GetModelDataAllocator()), _textureCoordsNumber, _engine);
		
		_textureCoords[_textureCoordsNumber - 1] = v;
	}
}

void AnimaMesh::SetFaces(AnimaFace* faces, ASizeT n)
{
	ANIMA_ASSERT(_engine != nullptr);
	ClearFaces();
	
	if(faces != nullptr && n > 0)
	{
		_facesNumber = n;
		_faces = AnimaAllocatorNamespace::AllocateArray<AnimaFace>(*(_engine->GetModelDataAllocator()), _facesNumber, _engine);
	
		//memcpy(_textureCoords, v, sizeof(AnimaVertex2f) * _textureCoordsNumber);
		for (int i = 0; i < _facesNumber; i++)
			_faces[i] = faces[i];
	}
}

void AnimaMesh::AddFace(const AnimaFace& index)
{
	ANIMA_ASSERT(_engine != nullptr);
	if(_facesNumber > 0)
	{
		AnimaFace* tmpOldFaces = AnimaAllocatorNamespace::AllocateArray<AnimaFace>(*(_engine->GetModelDataAllocator()), _facesNumber, _engine);
	
		//memcpy(tmpOldVertices, _vertices, sizeof(AnimaVertex4f) * _verticesNumber);
		for (int i = 0; i < _facesNumber; i++)
			tmpOldFaces[i] = _faces[i];
		
		//_allocator->Deallocate(_normals);
		AnimaAllocatorNamespace::DeallocateArray(*(_engine->GetModelDataAllocator()), _textureCoords);
	
		_facesNumber++;
		_faces = AnimaAllocatorNamespace::AllocateArray<AnimaFace>(*(_engine->GetModelDataAllocator()), _facesNumber, _engine);
	
		//memcpy(_vertices, tmpOldVertices, sizeof(AnimaVertex4f) * (_verticesNumber - 1));
		for (int i = 0; i < _facesNumber - 1; i++)
			_faces[i] = tmpOldFaces[i];
	
		_faces[_facesNumber - 1] = index;
	
		AnimaAllocatorNamespace::DeallocateArray(*(_engine->GetModelDataAllocator()), tmpOldFaces);
	}
	else
	{
		_facesNumber++;
		_faces = AnimaAllocatorNamespace::AllocateArray<AnimaFace>(*(_engine->GetModelDataAllocator()), _facesNumber, _engine);
		
		_faces[_facesNumber - 1] = index;
	}
}

ASizeT AnimaMesh::GetVerticesNumber()
{
	return _verticesNumber;
}

AnimaVertex3f AnimaMesh::GetVertex(ASizeT index)
{
	ANIMA_ASSERT(index >= 0 && index < _verticesNumber);
	return _vertices[index];
}

AnimaVertex3f* AnimaMesh::GetPVertex(ASizeT index)
{
	ANIMA_ASSERT(index >= 0 && index < _verticesNumber);
	return &_vertices[index];
}

AnimaVertex3f* AnimaMesh::GetVertices()
{
	return _vertices;
}

ASizeT AnimaMesh::GetNormalsNumber()
{
	return _normalsNumber;
}

AnimaVertex3f AnimaMesh::GetNormal(ASizeT index)
{
	ANIMA_ASSERT(index >= 0 && index < _normalsNumber);
	return _normals[index];
}

AnimaVertex3f* AnimaMesh::GetPNormal(ASizeT index)
{
	ANIMA_ASSERT(index >= 0 && index < _normalsNumber);
	return &_normals[index];
}

AnimaVertex3f* AnimaMesh::GetNormals()
{
	return _normals;
}

ASizeT AnimaMesh::GetTextureCoordsNumber()
{
	return _textureCoordsNumber;
}

AnimaVertex2f AnimaMesh::GetTextureCoord(ASizeT index)
{
	ANIMA_ASSERT(index >= 0 && index < _textureCoordsNumber);
	return _textureCoords[index];
}

AnimaVertex2f* AnimaMesh::GetPTextureCoord(ASizeT index)
{
	ANIMA_ASSERT(index >= 0 && index < _textureCoordsNumber);
	return &_textureCoords[index];
}

AnimaVertex2f* AnimaMesh::GetTextureCoords()
{
	return _textureCoords;
}

ASizeT AnimaMesh::GetFacesNumber()
{
	return _facesNumber;
}

AnimaFace AnimaMesh::GetFace(ASizeT index)
{
	ANIMA_ASSERT(index >= 0 && index < _facesNumber);
	return _faces[index];
}

AnimaFace* AnimaMesh::GetPFace(ASizeT index)
{
	ANIMA_ASSERT(index >= 0 && index < _facesNumber);
	return &_faces[index];
}

AnimaFace* AnimaMesh::GetFaces()
{
	return _faces;
}

void AnimaMesh::SetMeshName(const AnimaString& name)
{
	_meshName = name;
}

void AnimaMesh::SetMeshName(const char* name)
{
	_meshName = name;
}

AnimaString AnimaMesh::GetAnimaMeshName()
{
	return _meshName;
}

const char* AnimaMesh::GetMeshName()
{
	return _meshName.GetConstBuffer();
}

void AnimaMesh::Draw(AnimaMatrix transformMatrix)
{
	if (NeedsBuffersUpdate())
		UpdateBuffers();

	GLenum error = glGetError();
	
	glBindVertexArray(_vertexArrayObject);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexesBufferObject);
	
	glBindBuffer(GL_ARRAY_BUFFER, _verticesBufferObject);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	glBindBuffer(GL_ARRAY_BUFFER, _colorsBufferObject);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawElements(GL_TRIANGLES, (AUint)36, GL_UNSIGNED_INT, 0);
//	glDrawArrays(GL_TRIANGLES, 0, 9);
	
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
}

bool AnimaMesh::CreateBuffers()
{
	if (!CreateIndicesBuffer())
		return false;

	CreateVerticesBuffer();
	return true;
}

void AnimaMesh::UpdateBuffers()
{
	if (!AreBuffersCreated())
	{
		ANIMA_ASSERT(CreateBuffers());
	}
	else
	{
		glInvalidateBufferData(_verticesBufferObject);
		glInvalidateBufferData(_indexesBufferObject);
	}
	
	glBindVertexArray(_vertexArrayObject);

	ASizeT* indexes = GetFacesIndexes();
	ANIMA_ASSERT(indexes != nullptr);
	
	GLenum error = glGetError();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexesBufferObject);
	error = glGetError();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ASizeT) * 36, indexes, GL_STATIC_DRAW);
	error = glGetError();
	AnimaAllocatorNamespace::DeallocateArray(*(_engine->GetGenericAllocator()), indexes);
	indexes = nullptr;

	float* vertices = GetVerticesInternal();
	ANIMA_ASSERT(vertices != nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, _verticesBufferObject);
	error = glGetError();
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, vertices, GL_STATIC_DRAW);
	error = glGetError();
	AnimaAllocatorNamespace::DeallocateArray(*(_engine->GetGenericAllocator()), vertices);
	vertices = nullptr;
	
	float colors[24];
	
	colors[0] = 0.0;	colors[1] = 0.0;	colors[2] = 1.0;
	colors[3] = 0.0;	colors[4] = 1.0;	colors[5] = 1.0;
	colors[6] = 1.0;	colors[5] = 1.0;	colors[8] = 1.0;
	colors[9] = 1.0;	colors[10] = 0.0;	colors[11] = 1.0;
	colors[12] = 0.0;	colors[13] = 0.0;	colors[14] = 0.0;
	colors[15] = 0.0;	colors[16] = 1.0;	colors[17] = 0.0;
	colors[18] = 1.0;	colors[19] = 1.0;	colors[20] = 0.0;
	colors[21] = 1.0;	colors[22] = 0.0;	colors[23] = 0.0;
	
	glBindBuffer(GL_ARRAY_BUFFER, _colorsBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, colors, GL_STATIC_DRAW);

	_needsBuffersUpdate = false;
}

bool AnimaMesh::AreBuffersCreated()
{
	return IsIndicesBufferCreated() && IsVerticesBufferCreated();
}

bool AnimaMesh::IsIndicesBufferCreated()
{
	return _indexesBufferObject > 0;
}

bool AnimaMesh::IsVerticesBufferCreated()
{
	return _verticesBufferObject > 0;
}

bool AnimaMesh::CreateIndicesBuffer()
{
	if (IsIndicesBufferCreated())
		return true;

	glGenBuffers(1, &_indexesBufferObject);

	if (_indexesBufferObject <= 0 || glGetError() != GL_NO_ERROR)
		return false;
	return true;
}

bool AnimaMesh::CreateVerticesBuffer()
{
	if (IsVerticesBufferCreated())
		return true;

	glGenVertexArrays(1, &_vertexArrayObject);

	if (_vertexArrayObject <= 0 || glGetError() != GL_NO_ERROR)
		return false;

	glGenBuffers(1, &_verticesBufferObject);

	if (_verticesBufferObject <= 0 || glGetError() != GL_NO_ERROR)
		return false;
	
	glGenBuffers(1, &_colorsBufferObject);
	
	if (_colorsBufferObject <= 0 || glGetError() != GL_NO_ERROR)
		return false;
	
	return true;
}

void AnimaMesh::SetUpdateBuffers(bool bUpdate)
{
	_needsBuffersUpdate = bUpdate;
}

bool AnimaMesh::NeedsBuffersUpdate()
{
	return _needsBuffersUpdate;
}

AUint AnimaMesh::GetTotalIndexesCount()
{
	AUint count = 0;
	for (int i = 0; i < _facesNumber; i++)
		count += _faces[i].GetIndexesCount();
	return count;
}

ASizeT* AnimaMesh::GetFacesIndexes()
{
	ASizeT* indexes = nullptr;
	ASizeT count = 36;//GetTotalIndexesCount();
	ASizeT copied = 0;
	ASizeT offset = 0;

	if (count > 0)
	{
		indexes = AnimaAllocatorNamespace::AllocateArray<ASizeT>(*(_engine->GetGenericAllocator()), count);

		indexes[0] = 0;		indexes[1] = 1;		indexes[2] = 3;
		indexes[3] = 3;		indexes[4] = 1;		indexes[5] = 2;
		indexes[6] = 4;		indexes[5] = 5;		indexes[8] = 1;
		indexes[9] = 4;		indexes[10] = 1;	indexes[11] = 0;
		indexes[12] = 7;	indexes[13] = 4;	indexes[14] = 5;
		indexes[15] = 7;	indexes[16] = 5;	indexes[17] = 6;
		indexes[18] = 3;	indexes[19] = 6;	indexes[20] = 7;
		indexes[21] = 3;	indexes[22] = 2;	indexes[23] = 6;
		indexes[24] = 4;	indexes[25] = 0;	indexes[26] = 3;
		indexes[27] = 4;	indexes[28] = 3;	indexes[29] = 7;
		indexes[30] = 1;	indexes[31] = 5;	indexes[32] = 2;
		indexes[33] = 2;	indexes[34] = 5;	indexes[35] = 6;

//		for (int i = 0; i < _facesNumber; i++)
//		{
//			copied = _faces[i].GetIndexesCount();
//			_faces[i].GetConstIndexes(indexes + offset, copied);
//			int tmp = indexes[offset];
//			indexes[offset] = indexes[offset + 2];
//			indexes[offset + 2] = tmp;
//			offset += copied;
//		}
	}

	return indexes;
}

AUint AnimaMesh::GetVerticesCountInternal()
{
	return _verticesNumber * 3;
}

float* AnimaMesh::GetVerticesInternal()
{
	float* vertices = nullptr;
	ASizeT count = 24;//GetVerticesCountInternal();
	ASizeT offset = 0;

	if (count > 0)
	{
		vertices = AnimaAllocatorNamespace::AllocateArray<float>(*(_engine->GetGenericAllocator()), count);
		
		vertices[0] = -1.0;		vertices[1] = -1.0;		vertices[2] = 1.0;
		vertices[3] = -1.0;		vertices[4] = 1.0;		vertices[5] = 1.0;
		vertices[6] = 1.0;		vertices[5] = 1.0;		vertices[8] = 1.0;
		vertices[9] = 1.0;		vertices[10] = -1.0;	vertices[11] = 1.0;
		vertices[12] = -1.0;	vertices[13] = -1.0;	vertices[14] = -1.0;
		vertices[15] = -1.0;	vertices[16] = 1.0;		vertices[17] = -1.0;
		vertices[18] = 1.0;		vertices[19] = 1.0;		vertices[20] = -1.0;
		vertices[21] = 1.0;		vertices[22] = -1.0;	vertices[23] = -1.0;

		for (int i = 0; i < _verticesNumber; i++)
		{
			_vertices[i].CopyData(vertices + offset);
			offset += 3;
		}
	}

	return vertices;
}

END_ANIMA_ENGINE_NAMESPACE
