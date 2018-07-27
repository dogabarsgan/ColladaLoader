COLLADALOADER LIBRARY v1.0 | Doga Barsgan, Summer 2018
================================================================

////////////////////////////////////////////////////////////////
  (1)  Notes about Collada File Format (.dae)
////////////////////////////////////////////////////////////////

  - <library_geometries> : where the geometry info is stored
    ~ There can be more than 1 <geometry> node in a file,
      which means the object has different parts to it.
    ~ Each <geometry> node can have more than 1 <polylist>
      node. However, these polylists all use the same arrays
      of information. They just create different faces of the
      same object

- <library_effects> : where the material properties are 
                        stored

////////////////////////////////////////////////////////////////
  (2)  Notes about the ColladaLoader
////////////////////////////////////////////////////////////////

  - There are 4 main structs: Vertex, Polylist, 
                              Geometry, Material

  - Geometry encapsulates materials and polylists
                          ( there can be more than 1 of them 
                            per geometry )

  - Polylist encapsulates vertices. Polylist is basically the
    collection of all the vertices to draw

////////////////////////////////////////////////////////////////
  (3)  Integrating the library into your code
////////////////////////////////////////////////////////////////

  - #include <ColladaLoader.h>
  - Set the standard linking and include
    statements to your makefile

////////////////////////////////////////////////////////////////
  (4)  Example use of the library in your code
////////////////////////////////////////////////////////////////

  #include <ColladaLoader.h>

  int main(int _argc, char** _argv) {

    ColladaLoader instance;
    ColladaLoader* ptr = &instance;
  
    //  This function triggers the execution of the library 
        where relevant data structures are filled with the
        information from the collada file, therefore it is
        a must to call before all
    ptr->parseCollada("jepson.dae", "COLLADA"); 

    Material firstMaterial = ptr->materialVector[0];
    Geometry firstGeometry = ptr->geometryVector[0];
    Polylist firstPolylist = firstGeometry.polylistCollection[0];
    Vertex firstVertex = firstPolylist.vertexCollection[0];

    float posX = firstVertex.position.x;
    float posY = firstVertex.position.y;
    float posZ = firstVertex.position.z;

    float normalX = firstVertex.normal.x;
    float normalY = firstVertex.normal.y;
    float normalZ = firstVertex.normal.z;

    float texX = firstVertex.texture.x;
    float texY = firstVertex.texture.y;

  }  

////////////////////////////////////////////////////////////////
  (5)  Working functionality
////////////////////////////////////////////////////////////////

  - Works with basic .dae files that have single polylists
  - Material properties work (no Kd etc. values are present
                              in a collada file, only color)
  - Best works with models that are converted from .obj files
    using AutoDesk Maya after triangulation.

////////////////////////////////////////////////////////////////
  (6)  Known Bugs/Unfinished Features
////////////////////////////////////////////////////////////////

  - In the case where there are more than 1 geometry per collada
    file, ColladaLoader succesfully stores them under geometry 
    vector but when we try to draw them, it causes an exception.
    Probably have to do with the way parser adds to the faceVector.
  - Automate the parsing with the use of id's to refer to structures 
    (some comments to achieve this are commented out throughout 
    the code)

////////////////////////////////////////////////////////////////
  (7)  Further Possible Improvements
////////////////////////////////////////////////////////////////

  - Read in animation information from the collada file
  - Currently, the loader does not use <vcount> information
    while creating the faces. This is not really a problem
    since a big majority of collada files uses 3 paramaters.
  - Compilation of the library can be a little faster