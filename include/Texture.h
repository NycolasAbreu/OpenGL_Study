#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

//--------------------------------------------------------------------------------------------

enum ImageExtension {
	UNKNOW = 0,
	JPG,
	PNG
};

//--------------------------------------------------------------------------------------------

class Texture
{
	public:
		explicit Texture(
			const std::string& path,
			ImageExtension imageExt,
			bool shouldFlip = false
		);

		Texture() = delete;
		~Texture() = default;

		const Texture(const Texture&) = delete;

		void Use() const;
		void UseUnit(
			unsigned int textureUnitNum
		) const;

	private:
		unsigned int id = -1;
};

#endif