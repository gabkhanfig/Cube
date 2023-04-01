#include "PersistentMappedTripleIndirect.h"

PersistentMappedTripleIndirect::PersistentMappedTripleIndirect()
	: capacity(0), boundId(0), modifyId(1)
{}

PersistentMappedTripleIndirect::~PersistentMappedTripleIndirect()
{
	DeleteIndirectBuffers();
}

void PersistentMappedTripleIndirect::Reserve(uint32 count)
{
	DeleteIndirectBuffers();
	capacity = count;
	boundId = 0;
	modifyId = 1;

	for (int i = 0; i < 3; i++) {
		MappedIndirect mapped;
		mapped.dibo = DrawIndirectBufferObject::CreatePersistentMapped(capacity, &mapped.data);
		dibos[i] = mapped;
	}
}

void PersistentMappedTripleIndirect::SwapNextBuffer()
{
	boundId = (boundId + 1) % 3;
	modifyId = (modifyId + 1) % 3;
}

void PersistentMappedTripleIndirect::DeleteIndirectBuffers()
{
	for (int i = 0; i < 3; i++) {
		DrawIndirectBufferObject* dibo = dibos[i].dibo;
		if (dibo) {
			delete dibo;
		}
		dibos[i] = MappedIndirect();
	}
}
