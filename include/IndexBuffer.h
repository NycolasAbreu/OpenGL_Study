#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H

class IndexBuffer
{
	public:
		explicit IndexBuffer(
			unsigned int size,
			const void* data
		);

		IndexBuffer() = delete;
		
		virtual ~IndexBuffer();

		void Bind() const;
		void Unbind() const;

	private:
		unsigned int id = -1;
};

#endif
