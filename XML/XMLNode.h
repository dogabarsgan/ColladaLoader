#ifndef _XML_NODE_H_
#define _XML_NODE_H_

// STL
#include <unordered_set>
#include <vector>

// tinyxml
#ifndef TIXML_USE_STL
#define TIXML_USE_STL
#endif
#include "tinyxml/tinyxml.h"

// mathtool
#include <Vector.h>

// Exceptions
#include <Exceptions.h>

////////////////////////////////////////////////////////////////////////////////
/// @ingroup IOUtils
/// @brief Wrapper class for XML handeling
///
/// This is a wrapper class for XML handeling.  It is READONLY and supports only
/// trivial XML parsing.  Wrapping up TinyXML, it only supports TiXmlElements;
/// this is because this is our only need for intput XML files - for now.
////////////////////////////////////////////////////////////////////////////////
class XMLNode {
  public:

    std::string getString();

    TiXmlNode* getNextSibling();

    TiXmlNode* getFirstChild();
    
    typedef std::vector<XMLNode>::iterator iterator; ///< Child iterator

    ////////////////////////////////////////////////////////////////////////////
    /// @param _filename XML Filename
    /// @param _desiredNode Desired XML Node to make root of tree
    ///
    /// Will throw ParseException when \p _desiredNode cannot be found of
    /// \p _filename is poorly formed input
    explicit XMLNode(const std::string& _filename, const std::string& _desiredNode);

    XMLNode(TiXmlNode* _node);

    ////////////////////////////////////////////////////////////////////////////
    /// @return Name of XMLNode
    const std::string& name() const {return m_node->ValueStr();}
    ////////////////////////////////////////////////////////////////////////////
    /// @return Name of XML file
    const std::string& filename() const {return m_filename;}

    ////////////////////////////////////////////////////////////////////////////
    /// @return Iterator to first child
    iterator begin();
    ////////////////////////////////////////////////////////////////////////////
    /// @return Iterator to end of children
    iterator end();

    ////////////////////////////////////////////////////////////////////////////
    /// @brief Verify XML node name and consider it accessed
    void verify(const std::string& _name);

    ////////////////////////////////////////////////////////////////////////////
    /// @brief Read XML attribute
    /// @tparam T Type of attribute
    /// @param _name Name of attribute
    /// @param _req Is attribute required
    /// @param _default Default value of attribute
    /// @param _min Minimum valud of attribute
    /// @param _max Maximum valud of attribute
    /// @param _desc Description of attribute
    /// @return Value of attribute
    ///
    /// Reads XML attribute value with \p _name. If _req is specified and no
    /// attribute is given, \p _default is returned, otherwise input value is
    /// required to be in the range [\p _min, \p _max]. Otherwise, an error is
    /// reported and \p _desc is shown to the user.
    template<typename T>
      T read(const std::string& _name,
             bool _req,
             const T& _default,
             const T& _min,
             const T& _max,
             const std::string& _desc);

    ////////////////////////////////////////////////////////////////////////////
    /// @brief Read XML vector attribute
    /// @tparam T Type of vector components
    /// @tparam T Number of vector components
    /// @param _name Name of attribute
    /// @param _req Is attribute required
    /// @param _default Default value of attribute
    /// @param _desc Description of attribute
    /// @return Value of attribute
    ///
    /// Reads XML attribute value with \p _name. If _req is specified and no
    /// attribute is given, \p _default is returned. Otherwise, an error is
    /// reported and \p _desc is shown to the user.
    template<typename T, size_t D>
      mathtool::Vector<T, D> read(const std::string& _name,
                                  bool _req,
                                  const mathtool::Vector<T, D>& _default,
                                  const std::string& _desc);

    ////////////////////////////////////////////////////////////////////////////
    /// @brief Read XML boolean attribute
    /// @param _name Name of attribute
    /// @param _req Is attribute required
    /// @param _default Default value of attribute
    /// @param _desc Description of attribute
    /// @return Value of attribute
    ///
    /// Reads XML attribute value with \p _name. If _req is specified and no
    /// attribute is given, \p _default is returned. Otherwise, an error is
    /// reported and \p _desc is shown to the user.
    bool read(const std::string& _name,
              bool _req,
              bool _default,
              const std::string& _desc);

    ////////////////////////////////////////////////////////////////////////////
    /// @brief Read XML string attribute
    /// @return Value of attribute
    ///
    /// Calls string version of function to avoid confusion with bool -> const
    /// char* conversion in compile.
    std::string read(const std::string& _name,
                     bool _req,
                     const char* _default,
                     const std::string& _desc) {
      return read(_name, _req, std::string(_default), _desc);
    }

    ////////////////////////////////////////////////////////////////////////////
    /// @brief Read XML string attribute
    /// @param _name Name of attribute
    /// @param _req Is attribute required
    /// @param _default Default value of attribute
    /// @param _desc Description of attribute
    /// @return Value of attribute
    ///
    /// Reads XML attribute value with \p _name. If _req is specified and no
    /// attribute is given, \p _default is returned. Otherwise, an error is
    /// reported and \p _desc is shown to the user.
    std::string read(const std::string& _name,
                     bool _req,
                     const std::string& _default,
                     const std::string& _desc);

    ////////////////////////////////////////////////////////////////////////////
    /// @brief Report warnings for XML tree rooted at this node
    /// @param _warningsAsErrors True will throw exceptions for warnings
    ///
    /// To be called after parsing phase. This will report warnings throughout
    /// entire XML document. Should only be called on root XML node. Warnings to
    /// be reported:
    ///   - unknown/unparsed nodes
    ///   - unrequested attribues
    void warnAll(bool _warningsAsErrors = false);

    ////////////////////////////////////////////////////////////////////////////
    /// @brief Generate string describing where the node is
    /// @return String representing where node is
    ///
    /// To be used with PMPLExceptions, specifically ParseException. Gives
    /// string with filename, row (line number), and column of XMLNode.
    std::string where() const;

  private:
    ////////////////////////////////////////////////////////////////////////////
    /// @brief Generate XMLNodes for all children
    ///
    /// Builds the internal vector of children, used when iterating over
    /// the children. This vector is only built with ELEMENT type nodes.
    void buildChildVector();

    ////////////////////////////////////////////////////////////////////////////
    /// @brief Private constructor for use within BuildChildVector
    /// @param _node New TiXMLNode
    /// @param _filename XML filename
    /// @param _doc TiXmlDocument from tree's root node
    explicit XMLNode(TiXmlNode* _node, const std::string& _filename,
                     TiXmlDocument* _doc);

    ////////////////////////////////////////////////////////////////////////////
    /// @brief Return error report for attribute being the wrong type
    /// @param _name Name of attribute
    /// @param _desc Description of attribute
    /// @return Error report
    static std::string attrWrongType(const std::string& _name,
                                     const std::string& _desc);

    ////////////////////////////////////////////////////////////////////////////
    /// @brief Return error report for missing attribute
    /// @param _name Name of attribute
    /// @param _desc Description of attribute
    /// @return Error report
    static std::string attrMissing(const std::string& _name,
                                   const std::string& _desc);

    ////////////////////////////////////////////////////////////////////////////
    /// @brief Return error report for attribute being in an invalid range
    /// @tparam T Type of attribute
    /// @param _name Name of attribute
    /// @param _desc Description of attribute
    /// @param _min Minimum value of attribute
    /// @param _max Maximum value of attribute
    /// @return Error report
    template<typename T>
      static std::string attrInvalidBounds(const std::string& _name,
                                           const std::string& _desc,
                                           const T& _min, const T& _max);

    ////////////////////////////////////////////////////////////////////////////
    /// @brief Recursive function computing whether nodes have been accessed
    void computeAccessed();

    ////////////////////////////////////////////////////////////////////////////
    /// @brief Recursive function reporting all unknown/unparsed nodes and
    ///        unrequested attributes
    /// @param[out] _anyWarnings Initially should be false, and stores whether
    ///                          any warnings have been reported
    void warnAllRec(bool& _anyWarnings);

    ////////////////////////////////////////////////////////////////////////////
    /// @brief Report unknown node warning to cerr
    void warnUnknownNode();

    ////////////////////////////////////////////////////////////////////////////
    /// @brief Report unrequested attributes to cerr
    bool warnUnrequestedAttributes();

    ////////////////////////////////////////////////////////////////////////////
    /// @brief Generate string describing where the node is
    /// @param _f Filename
    /// @param _l Line number
    /// @param _c Column number
    /// @param _name Report name of node
    /// @return String representing where node is
    ///
    /// To be used with PMPLExceptions, specifically ParseException. Gives
    /// string with filename, name, row (line number), and column of XMLNode.
    std::string where(const std::string& _f,
                      int _l, int _c, bool _name = true) const;

    TiXmlNode* m_node;          ///< TiXmlNode
    bool m_childBuilt{false};   ///< Have children been parsed into nodes?
    bool m_accessed{false};     ///< Has this node been accessed or not?
    std::vector<XMLNode> m_children; ///< Children of node
    std::unordered_set<std::string>
      m_reqAttributes;          ///< Attributes which have been requested
    std::string m_filename;          ///< XML Filename
    TiXmlDocument* m_doc;       ///< Overall TiXmlDocument
};

template<typename T>
T
XMLNode::
read(const std::string& _name,
     bool _req,
     const T& _default,
     const T& _min,
     const T& _max,
     const std::string& _desc) {
  m_accessed = true;
  m_reqAttributes.insert(_name);
  T toReturn;
  int qr = m_node->ToElement()->QueryValueAttribute(_name, &toReturn);
  switch(qr) {
    case TIXML_WRONG_TYPE:
      throw ParseException(where(), attrWrongType(_name, _desc));
      break;
    case TIXML_NO_ATTRIBUTE:
      {
        if(_req)
          throw ParseException(where(), attrMissing(_name, _desc));
        else
          toReturn = _default;
        break;
      }
    case TIXML_SUCCESS:
      {
        if(toReturn < _min || toReturn > _max)
          throw ParseException(where(),
                               attrInvalidBounds(_name, _desc, _min, _max));
        break;
      }
    default:
      throw RunTimeException(WHERE, "Logic shouldn't be able to reach this.");
  }

  return toReturn;
}

template<typename T, size_t D>
mathtool::Vector<T, D>
XMLNode::
read(const std::string& _name, bool _req,
     const mathtool::Vector<T, D>& _default, const std::string& _desc) {
  m_accessed = true;
  m_reqAttributes.insert(_name);
  mathtool::Vector<T, D> toReturn;
  const char* attrVal =  m_node->ToElement()->Attribute(_name.c_str());

  if(attrVal == nullptr) {
    if(_req)
      throw ParseException(where(), attrMissing(_name, _desc));
    else
      return _default;
  }

  std::istringstream iss(attrVal);
  if(!(iss >> toReturn))
    throw ParseException(where(), attrWrongType(_name, _desc));
  return toReturn;
  return toReturn;
}

template<typename T>
std::string
XMLNode::
attrInvalidBounds(const std::string& _name, const std::string& _desc,
                  const T& _min, const T& _max) {
  std::ostringstream oss;
  oss << "Invalid attribute range on '" << _name << "'." << std::endl;
  oss << "\tAttribute description: " << _desc << "." << std::endl;
  oss << "\tValid range: ["<< _min << ", " << _max << "].";
  return oss.str();
}

#endif
