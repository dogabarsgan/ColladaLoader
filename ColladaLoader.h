#ifndef _COLLADA_LOADER_H_
#define _COLLADA_LOADER_H_

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>

#include <XMLNode.h>
#include <glm/vec2.hpp> 
#include <glm/vec3.hpp> 
#include <glm/vec4.hpp> 

class XMLNode;

using namespace std;

class ColladaLoader{

  public:

    struct Vertex {

      glm::vec3 position;
      glm::vec3 normal;
      glm::vec2 texture;

    };  

    struct Material {

      glm::vec4 defaultVector = {0, 0, 0, 0};
      float defaultFloat = 0;

      // RGBA
      glm::vec4 emission = defaultVector;
      glm::vec4 ambient = defaultVector;
      glm::vec4 diffuse = defaultVector;
      glm::vec4 specular = defaultVector;
      glm::vec4 reflective = defaultVector;
      glm::vec4 transparent = defaultVector;

      float shininess = defaultFloat;
      float refractionIndex = defaultFloat;
      float reflectivity = defaultFloat;
      float transparency = defaultFloat;

    };  

    struct Polylist {

      vector < Vertex > vertexCollection;
      
    };

    struct Geometry {

      vector < Polylist > polylistCollection;
      vector < Material > materialCollection;

    };

    ColladaLoader();

    // void parseArrayIDs(XMLNode& _node);
    // void parseSourceIDs(XMLNode& _node);

    // map<string, int> idLookup;
    // map<string, int> sourceLookup;

    void parseSourceNode(XMLNode& _node);
    void parsePolylistNode(XMLNode& _node);
    void parseCollada(const string& _filename, const string& _desiredNode);
    
    void parseGeometries(XMLNode& _node);
    void parseMaterials(XMLNode& _node);

    void parseSpecNode(XMLNode& _node);

    void fillPolylistVector();

    int indexNum;
    
    int numOfInput;
    
    vector<float> arrayBuffer;
    vector<int> elementArrayBuffer;

    vector < vector<glm::vec3> > arrayVector;

    vector < vector<glm::vec3> > faceVector;

    Material material;

    vector < Polylist > polylistVector;
    vector < Material > materialVector;
    vector < Geometry > geometryVector;

  private:
  
};

#endif