#pragma once

#include <vector>

#define MAX_LL 100000
#define MIN_LL -100000

//! Abstract base class for nodes
/*!
  This class is inherited by CheckNode and VariableNode and contains the common
  functionality, mainly manipulating references and retreiving messages
 */

class Node
{
    public:
        Node();      ///< generic constructor inherited by all children 
        virtual ~Node();  ///< Vitual destructor doesn't to any fancy 
        Node( const Node& other );  ///< Copy constructor
        Node& operator=( const Node& rhs );   ///< Assignment operator 

        virtual void Update() = 0;  ///< Virtual update function
        virtual void Debug() = 0;   ///< Virtual debug function 
        
        void PushReference( Node *ref );    ///< Adds a pointer to ref to the back of the reference list
        void PopReference();                ///< Removes the last reference 
        void InsertReference( Node *ref, int index );  ///< Inserts a pointer to ref at position index
        void RemoveReference( int index );     ///< Removes reference at index
        bool IsReference( Node *other );    ///< Checks if node with index is a reference 

        double GetMessage( Node *ref ) const;  ///< Gets the message for node *ref
        int GetDegree() const;                 ///< Returns the number of references
        //int GetIndex() const;                  ///< Returns the index of the node
        Node *GetReference( int index );       ///< Returns reference to reference at index

        double value;

    protected:

        int index;      ///< I don't think this is used any more?
        int degree;     ///< Number of references
        std::vector<Node *> references;  //Pointer to other nodes connected
        std::vector<double> messages;    //Vector of outgoing messages

};

//! Variables Node class
/*!
    Some descriptive comment about this class.
 */

class VariableNode : public Node
{
    using Node::Node;

    public:
        VariableNode();
        void Update();
        void Update( int i );
        void SetValueFromReal( double real_value );
        void SetValueFromLL( double ll );
        int GetHardValue();
        void SetSigma( double dSigma );
        double GetSigma();

        void Debug();

    private:
        double calc_ll( double val, double sigma );
        //double q_func( double x, double y, double sigma );
        double sigma;
        static int varCount;
};

class CheckNode : public Node
{
    using Node::Node;

    public:
        CheckNode();
        void Update();
        void Update( int i );
        bool Syndrome();
        void Debug();

    private:
        static int checkCount;
};
