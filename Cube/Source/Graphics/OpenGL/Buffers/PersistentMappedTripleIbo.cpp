#include "PersistentMappedTripleIbo.h"

PersistentMappedTripleIbo::PersistentMappedTripleIbo()
	: capacity(0), boundId(0), modifyId(2)
{}

PersistentMappedTripleIbo::~PersistentMappedTripleIbo()
{
	DeleteIbos();
}

void PersistentMappedTripleIbo::Reserve(uint32 count)
{
	DeleteIbos();
	capacity = count;
	boundId = 0;
	modifyId = 2;

	for (int i = 0; i < 3; i++) {
		MappedIbo mapped;
		mapped.ibo = IndexBufferObject::CreatePersistentMappedIbo(capacity, &mapped.data);
		ibos[i] = mapped;
	}
}

void PersistentMappedTripleIbo::SwapNextBuffer()
{
	boundId = (boundId + 1) % 3;
	modifyId = (modifyId + 1) % 3;
}

void PersistentMappedTripleIbo::DeleteIbos()
{
	for (int i = 0; i < 3; i++) {
		IndexBufferObject* ibo = ibos[i].ibo;
		if (ibo) {
			delete ibo;
		}
		ibos[i] = MappedIbo();
	}
}
