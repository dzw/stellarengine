//  //
// #   #  ###  #   #              -= Nuclex Library =-                   //
// ##  # #   # ## ## Node.cpp - Nuclex tree node                         //
// ### # #      ###                                                      //
// # ### #      ###  Base class for objects which can be integrated      //
// #  ## #   # ## ## into nuclex' hierarchical object tree               //
// #   #  ###  #   # R1             (C)2002 Markus Ewald -> License.txt  //
//  //
#include "Nuclex/Script/Node.h"

using namespace Nuclex;

namespace {

//  //
//  Nuclex::NodeEnumerator                                                  //
//  //
/// Directory stream info iterator
/** Returned by CDirStorage::Iterator(), this class iterates over
    files in a directory which can by opened by CDirStorage using
    a CFileStream.
*/
class NodeEnumerator :
  public Node::NodeEnumerator {
  public:
    /// Constructor
    /** Initializes an instance of CNodeNumerator

        @param  Childs  Child list to enumerate
    */
    NodeEnumerator(const std::map<string, shared_ptr<Node> > &Childs) :
      m_NodeIt(Childs.begin()),
      m_NodeEnd(Childs.end()) {}

    /// Destructor
    /** Destroys an instance of CNodeEnumerator
    */
    virtual ~NodeEnumerator() {}

    /// Cycle through nodes
    /** Returns the current node while advancing to the next one

        @return The next cycled node
    */
    const shared_ptr<Node> &cycle() {
      static const shared_ptr<Node> spEmpty;
      
      if(m_NodeIt != m_NodeEnd) {
        const shared_ptr<Node> &spNode = m_NodeIt->second;
        m_NodeIt++;
        return spNode;
      } else {
        return spEmpty;
      }
    }

  private:
    std::map<string, shared_ptr<Node> >::const_iterator m_NodeIt; ///< Current node
    std::map<string, shared_ptr<Node> >::const_iterator m_NodeEnd; ///< Last node
};

}

#if 0
// ####################################################################### //
// # Nuclex::INode::getNodePath()                                           # // 
// ####################################################################### //
/** Returns the absolute path of this node.

    @param  pNode  Node whose part to retrieve
    @return The absolute node path
*/
string INode::getNodePath(INode *pNode) {
  string sPath("/");

  pNode->incRef();
  try {
    while(INode *pParent = pNode->getParent()) {
      try {
        sPath = string("/") + pNode->getName() + sPath;
        pNode->decRef();
        pNode = pParent;
      }
      catch(const Exception &) {
        pParent->decRef();
        throw;
      }
    }
  }
  catch(const Exception &) {
    pNode->decRef();
    throw;
  }

  return sPath;
}

// ####################################################################### //
// # Nuclex::INode::getNodeByPath()                                         # // 
// ####################################################################### //
/** Looks for a node with the specified path. If the path is absolute
    (beginning with a /), the search will begin on the farthest root
    node. If the requested node is not found, NULL is returned. The
    method performs no reference counter manipulation on the returned
    node.

    @param  pNode  Node from which to start
    @param  sPath  Path to node to retrieve
    @return The node at the specified location
*/
INode *INode::getNodeByPath(INode *pNode, const string &sPath) {
  string               sTemp(sPath), sNode;
  string::size_type    Pos;
  CObjectPointer<INode> pCurNode(pNode, true);

  // Does the it begin with a backslash ?
  if(sTemp.operator[](0) == '/') {
    // Go to the farthest root node
    CObjectPointer<INode> pParent;
    while(pParent = pCurNode->getParent()) {
      pCurNode = pParent;

      #ifdef _DEBUG
      // If this exception hits, we went further than 256 nodes upwards,
      // where we make the assumption that the tree is ill-formed,
      // eg. there's a node somehow linked as one of its own child nodes.
      //int nDepth++;
      //NCXASSERT(nDepth < 256);
      #endif
    }

    // Cut trailing slash
    sTemp = sTemp.substr(1);
  }

  // Traverse node path
  do {
    // If the path contains slashes, cut it at the first slash
    Pos = sTemp.find('/');

    if(Pos == string::npos) {
      sNode = sTemp;
    } else {
      sNode = sTemp.substr(0, Pos);
      sTemp = sTemp.substr(Pos + 1);
    }

    // Look for the extracted path component
    if(sNode == "..")
      pCurNode = CObjectPointer<INode>(pCurNode->getParent());
    else if(sNode.length() && sNode != ".")
      pCurNode = CObjectPointer<INode>(pCurNode->getChild(sNode));

    // Node not found
    if(!pCurNode)
      return NULL;
  } while(Pos != string::npos);

  // We reached the end of the path string and no
  // errors occured -> pNode is the requested path
  pCurNode->incRef();
  return pCurNode;

  return NULL;
}
#endif

// ####################################################################### //
// # Nuclex::CNode::getChild()                                              # // 
// ####################################################################### //
/** Looks for a direct child node with the specified name. If the
    requested child is not found, NULL is returned. The method performs
    no reference counter manipulation on the returned node.

    @param  sName  Name of child node to retrieve
    @return The child node, if found, or NULL
*/
const shared_ptr<Node> &Node::getChild(const string &sName) const {
  NodeMap::const_iterator ChildIt = m_Childs.find(sName);
  if(ChildIt != m_Childs.end())
    return ChildIt->second;
  else
    throw InvalidArgumentException("Nuclex::Node::getChild()",
                                   string("Item not found: '") + sName + "'");
}

// ####################################################################### //
// # Nuclex::Node::attachChild()                                            # // 
// ####################################################################### //
/** Add a child node to the node. The child will be AddRef()d and its
    parent node will be set to this node.

    @param  pNode  Node to add as child node
*/
void Node::attachChild(const string &sName, const shared_ptr<Node> &spNode) {
  NodeMap::iterator ChildIt = m_Childs.find(sName);
  if(ChildIt != m_Childs.end())
    ChildIt->second = spNode;
  else
    m_Childs.insert(NodeMap::value_type(sName, spNode));

  return;
}

// ####################################################################### //
// # Nuclex::Node::detachChild()                                            # // 
// ####################################################################### //
/** Remove a child node. If the child node exists, its parent will be
    set to NULL, then it will be Release()d. If the node does not exist,
    the method will silently return.
    Use the CNode::Node() to check whether a node exists.

    @param  sName  Name of the child to remove
*/
void Node::detachChild(const string &sName) {
  NodeMap::iterator ChildIt = m_Childs.find(sName);
  if(ChildIt != m_Childs.end())
    m_Childs.erase(ChildIt);
  else
    throw InvalidArgumentException("Nuclex::Node::detachChild()",
                                   "The specified node could not be found");
  
  return;
}

// ####################################################################### //
// # Nuclex::Node::detachAllChilds()                                        # // 
// ####################################################################### //
/** Empties the node by killing all of its child nodes.
    The child nodes will be release()d once, meaning if there are
    still active references, the nodes will only get disconnected.
*/
void Node::detachAllChilds() {
  m_Childs.clear();

  return;
}

// ####################################################################### //
// # Nuclex::Node::enumChilds()                                             # // 
// ####################################################################### //
/** Returns a new iterator which iterates over all childs of the node.
    The iterator is initially set to the first child node (if any), and
    cycles in undefined order through all existing direct childs.
    The iterator object has to be deleted when it is no more needed.

    @return A new iterator for the node's direct childs.
*/
shared_ptr<Node::NodeEnumerator> Node::enumChilds() const {
  return shared_ptr<Node::NodeEnumerator>(new ::NodeEnumerator(m_Childs));
}
