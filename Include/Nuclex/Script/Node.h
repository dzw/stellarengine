//  //
// #   #  ###  #   #              -= Nuclex Library =-                   //
// ##  # #   # ## ## Node.h - Nuclex tree node                           //
// ### # #      ###                                                      //
// # ### #      ###  Base class for objects which can be integrated      //
// #  ## #   # ## ## into nuclex' hierarchical object tree               //
// #   #  ###  #   # R1             (C)2002 Markus Ewald -> License.txt  //
//  //
#ifndef NCX_NODE_H
#define NCX_NODE_H

#include "Nuclex/Nuclex.h"
#include "Nuclex/Support/Exception.h"
#include "Nuclex/Support/String.h"

#include <map>

namespace Nuclex {

//  //
//  Nuclex::Node                                                            //
//  //
/// Nuclex tree node
/** Nodes are tree entries in the hierarchical object management system.
    The tree can be used for any number of purposes and provides a way
    to make classes and data easily accessible to scripts.

    The CNode class supports scripting and persistency. Any object derived
    from CNode which implements its own persistency methods should properly
    call CNode::Load() and CNode::Save() on its child nodes as well as
    scripting methods should call CNode::Methods() and CNode::Invoke() of
    their base classes if the method is not supported by that node.

    Node can be created on its own to provide a node which simply
    serves as a folder in the naming hierarchy.
*/
class Node {
  public:
    /// Node enumerator
    class NodeEnumerator;

    /// Destructor
    /** Destroys an instance of a Node
    */
    NUCLEX_API inline Node(const string &sName) :
     m_sName(sName) {}

    /// Destructor
    /** Destroys an instance of a Node
    */
    NUCLEX_API inline virtual ~Node() {}

    /// Get full path to node
    //NUCLEX_API static string getNodePath(const shared_ptr<Node> &spNode);
    /// Get node by relative or absolute path
    //NUCLEX_API static const shared_ptr<Node> &getNodeByPath(shared_ptr<Node> &spNode, const string &sPath);

  //
  // Node implementation
  //
  public:
    // General services
    //
    /// Retrieve node name
    /** Retrieves the node's current name

        @return The node's name
    */
    NUCLEX_API const string &getName() const { return m_sName; }
    /// Set node name
    /** Changes the node's name
      
        @param  sName  New node name
    */
    NUCLEX_API void setName(const string &sName) { m_sName = sName; }

    /// Get child node by name
    /** Retrieve a direct child node by its name

        @param  sName  Name of the child to retrieve
        @return The child node, if found, or NULL
    */
    NUCLEX_API const shared_ptr<Node> &getChild(const string &sName) const;

    /// Attach child node
    /** Attaches another node as child to this node.

        @param  sName    Name for the child
        @param  spChild  Child node to attach
    */
    NUCLEX_API void attachChild(const string &sName, const shared_ptr<Node> &spChild);
    /// Remove child node
    /** Removes an attached child node from the node

        @param  sName  Name of the node to detach
    */
    NUCLEX_API void detachChild(const string &sName);
    /// Remove all child nodes
    /** Detaches all child nodes from this node
    */
    NUCLEX_API void detachAllChilds();

    /// Get node child iterator
    /** Returns a new iterator which can be used to iterate
        over all child nodes of this node
    */
    NUCLEX_API shared_ptr<NodeEnumerator> enumChilds() const;

  private:
    typedef std::map<string, shared_ptr<Node> > NodeMap;

    string  m_sName;
    NodeMap m_Childs;
};

//  //
//  Nuclex::Node::NodeEnumerator                                            //
//  //
/** Enumerates a list of nodes
*/
class Node::NodeEnumerator {
  public:
    /// Destructor
    /** Destroys an instance of NodeEnumerator
    */
    NUCLEX_API virtual ~NodeEnumerator() {}

  //
  // NodeEnumerator implementation
  //
  public:
    /// Cycle through nodes
    /** Returns the current node being enumerated and advances
        to the next. If no more nodes are remaining, NULL is returned

        @return The currently enumerated renderer
    */
    NUCLEX_API virtual const shared_ptr<Node> &cycle() = 0;
};

} // namespace Nuclex

#endif NCX_NODE_H
