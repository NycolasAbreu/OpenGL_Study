#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

class VertexBuffer
{
	public:
		explicit VertexBuffer(
			unsigned int size,
			const void* data
		);

		VertexBuffer() = delete;
		
		virtual ~VertexBuffer();

		void Bind() const;
		void Unbind() const;

	private:
		unsigned int id = -1;
};

#endif // !VERTEXBUFFER_H
