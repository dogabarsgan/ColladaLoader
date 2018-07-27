#include "XMLNode.h"

// STL
#include <algorithm>
using namespace std;

XMLNode::
XMLNode(TiXmlNode* _node){

  m_node = _node;

}

XMLNode::
XMLNode(const string& _filename, const string& _desiredNode) :
    m_filename(_filename) {
  m_doc = new TiXmlDocument(_filename);

  if(!m_doc->LoadFile())
    throw ParseException(
        where(_filename, m_doc->ErrorRow(), m_doc->ErrorCol(), false),
        m_doc->ErrorDesc());

  m_node = m_doc->FirstChild(_desiredNode.c_str());

  if(!m_node)
    throw ParseException(_filename,
        "Unable to find XML node '" + _desiredNode + "'.");
}

XMLNode::iterator
XMLNode::
begin() {
  buildChildVector();
  return m_children.begin();
}

XMLNode::iterator
XMLNode::
end() {
  buildChildVector();
  return m_children.end();
}

void
XMLNode::
verify(const std::string& _name) {
  if(_name == name())
    m_accessed = true;
  else
    throw ParseException(where(), "Invalid request of node '" + name() + "'.");
}

bool
XMLNode::
read(const string& _name,
    bool _req,
    bool _default,
    const string& _desc) {
  m_accessed = true;
  m_reqAttributes.insert(_name);
  const char* attrVal =  m_node->ToElement()->Attribute(_name.c_str());
  string toReturn;
  if(attrVal == nullptr) {
    if(_req)
      throw ParseException(where(), attrMissing(_name, _desc));
    else
      return _default;
  }
  else
    toReturn = attrVal;

  transform(toReturn.begin(), toReturn.end(), toReturn.begin(), ::toupper);
  if(toReturn == "TRUE")
    return true;
  else if(toReturn == "FALSE")
    return false;
  else
    throw ParseException(where(), attrWrongType(_name, _desc));
}

string
XMLNode::
read(const string& _name,
    bool _req,
    const string& _default,
    const string& _desc) {
  m_accessed = true;
  m_reqAttributes.insert(_name);
  const char* attrVal =  m_node->ToElement()->Attribute(_name.c_str());
  string toReturn;
  if(attrVal == NULL) {
    if(_req == true)
      throw ParseException(where(), attrMissing(_name, _desc));
    else
      toReturn = _default;
  }
  else
    toReturn = attrVal;
  return toReturn;
}

void
XMLNode::
warnAll(bool _warningsAsErrors) {
  computeAccessed();
  bool anyWarnings = false;
  warnAllRec(anyWarnings);
  if(anyWarnings && _warningsAsErrors)
    throw ParseException(m_filename, "Reported Warnings are errors.");
}

string
XMLNode::
where() const {
  return where(m_filename, m_node->Row(), m_node->Column());
}

void
XMLNode::
buildChildVector() {
  if(!m_childBuilt) {
    TiXmlNode* child = m_node->FirstChild();
    while(child != nullptr) {
      if(child->Type() == TiXmlNode::ELEMENT)
        m_children.push_back(XMLNode(child, m_filename, m_doc));
      else if(child->Type() != TiXmlNode::COMMENT)
        throw ParseException(where(m_filename, child->Row(), child->Column()),
            "Invalid XML element.");
      child = child->NextSibling();
    }

    m_childBuilt = true;
  }
}

XMLNode::
XMLNode(TiXmlNode* _node, const string& _filename, TiXmlDocument* _doc) :
  m_node(_node), m_filename(_filename), m_doc(_doc) {
  }

string
XMLNode::
attrWrongType(const string& _name, const string& _desc) {
  ostringstream oss;
  oss << "Wrong attribute type requested on '" << _name << "'." << endl;
  oss << "\tAttribute description: " << _desc << ".";
  return oss.str();
}

string
XMLNode::
attrMissing(const string& _name, const string& _desc) {
  ostringstream oss;
  oss << "Missing required attribute '" << _name << "'." << endl;
  oss << "\tAttribute description: " << _desc << ".";
  return oss.str();
}

void
XMLNode::
computeAccessed() {
  for(auto& child : *this) {
    child.computeAccessed();
    m_accessed = m_accessed || child.m_accessed;
  }
}

void
XMLNode::
warnAllRec(bool& _anyWarnings) {
  if(m_accessed) {
    for(auto& child : *this)
      child.warnAllRec(_anyWarnings);
    if(warnUnrequestedAttributes())
      _anyWarnings = true;
  }
  else {
    warnUnknownNode();
    _anyWarnings = true;
  }
}

void
XMLNode::
warnUnknownNode() {
  cerr << "*************************************************************" << endl;
  cerr << "XML Warning:: Unknown or Unrequested Node" << endl;
  cerr << "File:: " << m_filename << endl;
  cerr << "Node: " << name() << endl;
  cerr << "Line: " << m_node->Row() << endl;
  cerr << "Col: " << m_node->Column() << endl;
  cerr << "*************************************************************" << endl;
}

bool
XMLNode::
warnUnrequestedAttributes() {
  vector<string> unreqAttr;
  const TiXmlAttribute* attr = m_node->ToElement()->FirstAttribute();
  while(attr != NULL) {
    if(m_reqAttributes.count(attr->Name()) == 0)
      unreqAttr.push_back(attr->Name());
    attr = attr->Next();
  }
  if(unreqAttr.size() > 0) {
    cerr << "*************************************************************" << endl;
    cerr << "XML Warning:: Unrequested Attributes Exist" << endl;
    cerr << "File:: " << m_filename << endl;
    cerr << "Node: " << name() << endl;
    cerr << "Line: " << m_node->Row() << endl;
    cerr << "Col: " << m_node->Column() << endl;
    cerr << "Unrequested Attributes::" << endl;
    for(auto& a : unreqAttr)
      cerr << "\t" << a << endl;
    cerr << "*************************************************************" << endl;
    return true;
  }
  return false;
}

string
XMLNode::
where(const string& _f, int _l, int _c, bool _name) const {
  ostringstream oss;
  oss << "File: " << _f;
  if(_name)
    oss << "\n\tNode: " << name();
  oss << "\n\tLine: " << _l;
  oss << "\n\tCol: " << _c;
  return oss.str();
}


string
XMLNode::
getString(){

  string str = m_node->ToElement()->GetText();

return str;
  
}

TiXmlNode*
XMLNode::
getNextSibling(){

  return m_node->NextSibling();

}

TiXmlNode*
XMLNode::
getFirstChild(){

  return m_node->FirstChild();

}




