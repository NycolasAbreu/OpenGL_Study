#ifndef UTILS_H
#define UTILS_H

#include <fstream>
#include <sstream>

//--------------------------------------------------------------------------------------------

void ExtractTextFromFile(
  const std::string& filePath,
  std::string& text
)
{
  std::ifstream fileStream;

  // ensure ifstream objects can throw exceptions:
  fileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  // open file
  fileStream.open(filePath);

  // read file buffer contents into stream
  std::stringstream streamBuffer;
  streamBuffer << fileStream.rdbuf();

  // return code from stream
  text = streamBuffer.str();

  // close file handler
  fileStream.close();
}

//--------------------------------------------------------------------------------------------

#endif
