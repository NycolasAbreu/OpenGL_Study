#include "IndexBuffer.h"

#include <glad/glad.h>

//--------------------------------------------------------------------------------------------

IndexBuffer::IndexBuffer(
	unsigned int size,
	const void* data
) 
{
	glGenBuffers(1, &id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

//--------------------------------------------------------------------------------------------

IndexBuffer::~IndexBuffer() 
{
	glDeleteBuffers(1, &id);
}

//--------------------------------------------------------------------------------------------

void IndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
}

//--------------------------------------------------------------------------------------------

void IndexBuffer::Unbind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

//--------------------------------------------------------------------------------------------
