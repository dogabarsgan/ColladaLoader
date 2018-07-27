#include <ColladaLoader.h>

ColladaLoader::
ColladaLoader(){

  indexNum = 0;
  numOfInput = 0;
  
}

void
ColladaLoader::
parseSpecNode(XMLNode& _node){

    glm::vec4 vecToAdd;
    float floatToAdd;

    XMLNode infoNode(_node.getFirstChild());

    if(infoNode.name() != "texture"){
      
      string nodeContent = infoNode.getString();

      string specName = _node.name();
        
      // tokenizing the string
      istringstream iss(nodeContent);

      vector<float> tokens;

      copy(istream_iterator<float>(iss),
        istream_iterator<float>(),
        back_inserter(tokens));

      if(specName == "shininess" || specName == "reflectivity" || specName == "transparency" ){

        floatToAdd = tokens[0];

      } else{

        vecToAdd[0] = tokens[0];
        vecToAdd[1] = tokens[1];
        vecToAdd[2] = tokens[2];
        vecToAdd[3] = tokens[3];

      }
          
      if(specName == "shininess"){

        material.shininess = floatToAdd;

      } else if(specName == "reflectivity"){

        material.reflectivity = floatToAdd;

      } else if(specName == "transparency"){

        material.transparency = floatToAdd;

      } else if(specName == "index_of_refraction"){

        material.refractionIndex = floatToAdd;

      }

      if(specName == "emission"){

        material.emission = vecToAdd;

      } else if(specName == "ambient"){

        material.ambient = vecToAdd;

      } else if(specName == "diffuse"){

        material.diffuse = vecToAdd;

      } else if(specName == "specular"){

        material.specular = vecToAdd;

      } else if(specName == "reflective"){

        material.reflective = vecToAdd;

      } else if(specName == "transparent"){

        material.transparent = vecToAdd;  

      }
    
  }
}

void 
ColladaLoader::
parseMaterials(XMLNode& _node){

  XMLNode* effectNode = nullptr;
  XMLNode* profileCommonNode = nullptr;
  XMLNode* techniqueNode = nullptr;
  XMLNode* specNode = nullptr;

  // reaching the effects node
  for (auto& child : _node){

    if(child.name() == "effect"){

      effectNode = &child;
      break;

    }

  }

  // reaching the profileCommonNode
  for (auto& child : *effectNode){

    if(child.name() == "profile_COMMON"){

      profileCommonNode = &child;
      break;

    }

  }

  // reaching the techniqueNode
   for (auto& child : *profileCommonNode){

    if(child.name() == "technique"){

      techniqueNode = &child;
      break;

    }

  }


  // reaching the specNode
   for (auto& child : *techniqueNode){

    if(child.name() == "phong" || child.name() == "blinn" || child.name() == "lambert"){

      specNode = &child;
      break;

    }

  }

  // since there are more than one property stored
  for (auto& child : *specNode){

      parseSpecNode(child);

      // add to the material vector
      materialVector.push_back(material);  

  }


}

void
ColladaLoader::
fillPolylistVector(){

  for(int i=0; i<faceVector.size(); i++){ // amount of polylists

    Polylist polylistVectorToAdd;

    for(int j=0; j< faceVector[i].size(); j++){ // filling the polylist

      Vertex vertexToAdd;

      // filling in the vertices
      vertexToAdd.position = arrayVector[0][faceVector[i][j].x];
      vertexToAdd.normal = arrayVector[1][faceVector[i][j].y];
      vertexToAdd.texture = arrayVector[2][faceVector[i][j].z];

      // adding vertices to a polylist
      polylistVectorToAdd.vertexCollection.push_back(vertexToAdd);

    } // a polylist is now filled with vertices

    // add the filled polylist to the list of polylists
    polylistVector.push_back(polylistVectorToAdd);

    // clearing the vertex collection
    polylistVectorToAdd.vertexCollection.clear();

  }

}

void
ColladaLoader::
parsePolylistNode(XMLNode& _node){

  vector<glm::vec3> vectors;

  // vector<string> idVector;

  XMLNode* pNode = nullptr;
  XMLNode* vcount = nullptr;
  XMLNode* inputNode = nullptr; 

  for (auto& child : _node) {

    // reaching the input node
    if(child.name() == "input"){

      // idVector.push_back( child.read("source", true, "", "SOURCE"));

      // incrementing this allows us to see how many parameters per vertex
      numOfInput++;

    }

  }

  // reaching the input node
  for (auto& child : _node) {

    if(child.name() == "input"){

      inputNode = &child;
      break;

    }

  }

  // reaching p node
  for (auto& child : _node) {

     if (child.name() == "p"){

      pNode = &child;
      break;

    }

  }
 
  // reaching vcount node
  for (auto& child : _node) {

    if (child.name() == "vcount"){

      vcount = &child;
      break;

    }

  }

  // turning the entire concent into a big string
  string nodeContent = pNode->getString();

  // tokenizing the string
  istringstream iss(nodeContent);

  vector<float> tokens;

  // seperating each space delimitered string, converting
  // them into floats
  copy( istream_iterator<float>(iss),
        istream_iterator<float>(),
         back_inserter(tokens)  );

  // adding vectors of float to the position vectors
  glm::vec3 verticesToBeAdded;
 
    for(int i=0; i<tokens.size(); i++){

      if(i % numOfInput == 0){

        verticesToBeAdded.x = (tokens[i]);

      } else if(i % numOfInput == 1){

        verticesToBeAdded.y = (tokens[i]);

      } else if(i % numOfInput == numOfInput-1){

        verticesToBeAdded.z = (tokens[i]);
        
      }

      // faceVectorsToBeAdded.id += idVector[ i % numOfInput] + " ";

      // when at the end of a block, push it 
      // to the vectors 
      if (i % numOfInput == numOfInput-1){

        vectors.push_back(verticesToBeAdded);
        
        // faceVectorsToBeAdded.id = "";

      }
     
    }

    faceVector.push_back(vectors); // adds face vectors to the collection

}

void
ColladaLoader::
parseSourceNode(XMLNode& _node){

  vector<glm::vec3> vectors;

  // reaching the child node with the relevant info
  XMLNode arrayNode(_node.getFirstChild());

  // turning the entire concent into a big string
  string nodeContent = arrayNode.getString();

  // // reading the id of the child node
  // string id = arrayNode.read("id", true, "", "ID");

  // tokenizing the string
  istringstream iss(nodeContent);

  vector<float> tokens;

  // seperating each space delimitered string, converting
  // them into floats
  copy( istream_iterator<float>(iss),
        istream_iterator<float>(),
        back_inserter(tokens) );

  // tokens.id = arrayNode.read("id", true, "", "ID");

  XMLNode* techCommonNode = nullptr;
  XMLNode* accessorNode = nullptr;

  // reaching the technique_common node
  for (auto& child : _node) {

    if (child.name() == "technique_common"){

       techCommonNode = &child;
       break;

    }
     
  }

  // reaching the accessor node
   for (auto& child : *techCommonNode) {

    if (child.name() == "accessor"){

       accessorNode = &child;
       break;

    }
 
  }

  // reading accessor node attributes
  int stride = stoi(accessorNode->read("stride", true, "", "Stride"));
  int count = stoi(accessorNode->read("count", true, "", "Count"));

  // adding vectors of float to the position vectors
  glm::vec3 vectorsToBeAdded;
 
    for(int i=0; i<count*stride; i++){

      // if stride is 3, group things in 3s
      // if stride is 2, group things in 2s
      if(stride == 3){

          if(i % stride == 0) {

          vectorsToBeAdded.x = (tokens[i]);

          } else if(i % stride == 1){

          vectorsToBeAdded.y = (tokens[i]);

          } else if(i % stride == stride-1){

          vectorsToBeAdded.z = (tokens[i]);

          }

        // vectorsToBeAdded.id = tokens.id;

        // when at the end of a block, push it 
        // to the vectors 
        if (i % stride == stride-1){

          vectors.push_back(vectorsToBeAdded);

        }
     
      } else if(stride == 2){

        if(i % stride == 0){
          
        vectorsToBeAdded.x = (tokens[i]);

        } else if(i % stride == 1){

        vectorsToBeAdded.y = (tokens[i]);
        
        }

        // vectorsToBeAdded.id = tokens.id;

        if (i % stride == stride-1){

          vectors.push_back(vectorsToBeAdded);

        }

      }

    }

  arrayVector.push_back(vectors);

}

/*
  void
  ColladaLoader::
  parseArrayIDs(XMLNode& _node){

    for (auto& child : _node) {

      if(child.name() == "float_array"){

        idLookup.insert(   {child.read("id", true, "", "ID"), indexNum }   );
        indexNum++;

      }
      
    }
    
  }

  // TODO
  void
  ColladaLoader::
  parseSourceIDs(XMLNode& _node){}



*/

void
ColladaLoader::
parseGeometries(XMLNode& _node){

  XMLNode* geoNode = nullptr;
  XMLNode* meshNode = nullptr;

  // there can be more than 2 source nodes! 
  XMLNode* sourceNode = nullptr;

  // there can be more than 2 polylist nodes!
  XMLNode* polylistNode = nullptr;

  // reaching the geometry node
  for (auto& child : _node) {

    if (child.name() == "geometry"){

      // geometry instance
      Geometry geometryToAdd;

      geoNode = &child;
        
        // reaching the mesh node
      for (auto& child : *geoNode) {

        if (child.name() == "mesh"){

          meshNode = &child;
          break;

        }
      
      }

      // reaching the source node. There can be more than 2!
      for (auto& child : *meshNode) {

        if (child.name() == "source"){

          sourceNode = &child;
          
          // parseArrayIDs(*sourceNode);
          parseSourceNode(*sourceNode);

        }
      
      }

      // reaching the polylist node. There can be more than 2!
      for (auto& child : *meshNode) {

        if (child.name() == "polylist" || child.name() == "triangles"  ){

          polylistNode = &child;

          // parseSourceIDs(*polylistNode);
          parsePolylistNode(*polylistNode);

        }
      
      }

      fillPolylistVector();

      // filling in the geometryToAdd information with the updated polylistVector
      geometryToAdd.polylistCollection = polylistVector;

      // adding the geometry to the vector
      geometryVector.push_back(geometryToAdd);

      
      //clearing instance variables
      geometryToAdd.polylistCollection.clear();
      arrayVector.clear();
      faceVector.clear();
      polylistVector.clear();

    }
  
  }
  
}

void
ColladaLoader::
parseCollada(const string& _filename, const string& _desiredNode){

  // getting the root node of the tree
  XMLNode rootNode(_filename, "COLLADA");

  // Find the 'library_geometries' and 'library_effects nodes
  XMLNode* libGeoNode = nullptr;
  XMLNode* libEffNode = nullptr;

  for (auto& child : rootNode) {

    if (child.name() == "library_geometries" && !libGeoNode){

       libGeoNode = &child;

    } else if(child.name() == "library_effects" && !libEffNode){

        libEffNode = &child;

    }
     
  }

  parseGeometries(*libGeoNode);

  parseMaterials(*libEffNode);

}