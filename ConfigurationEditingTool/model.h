#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <vector>

class Model
{
public:
   Model();

   bool LoadConfiguration(const std::string& file,
                          std::vector<std::string>& errors);
   void SaveConfiguration(const std::string& file);
};

#endif // MODEL_H
