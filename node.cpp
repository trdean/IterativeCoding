#include <math.h>
#include "node.h"

Node::Node() : 
    value ( 0 )
{

}

Node::Node( const Node& other )
{
    int i;

    value = other.value;

    for (i = 0; i < degree; i++)
        PushReference( &other.reference[i] );
}

Node& Node::operator=( const Node& rhs )
{
    Node temp( rhs );

    Swap( temp ); 

    return *this;
}

void Node::Swap( Node& other ) throw ()
{
    int i;
    this.value = other.value;

    for (i = 0; i < other.degree; i++)
        this.PushReference( &other.reference[i] );
}

void PushReference( Node *ref )
{
    references.push_back( ref );
    degree++;
}

void PopReference()
{
    references.pop_back();
    degree--;
}

void InsertReference( Node *ref, int index )
{
    references.insert( index, ref );
    degree++;
}

void RemoveReference( int index )
{
    references.remove( index );
    degree--;
}

bool VariableNode::Update()
{
    double initalValue;
    double sum;
    int i;

    initialValue = this->value;
    sum = initialValue;

    for ( i = 0; i < this->degree; i++ ) {
        sum += fabs ( this->references[i]->value );
    }

    if (sum > MAX_LL)
        sum = MAX_LL;
    else if (sum < MIN_LL)
        sum = MIN_LL;

    if( sum == initialValue)
        return false;

    this->value = sum;
    return true;
}

bool CheckNode::Update() {
    double initialValue;
    double tmpVal;
    double product;
    int i;

    initialValue = this->value;
    product = 1;

    for ( int i = 0; i < this->degree; i++ ) {
        tmpVal = this->references[i]->value;
        if ( tmpVal != 0 )
            product *= tanh( tmpVal / 2 );
    }

    this->value = 2*atanh(product);

    if ( this->value > MAX_LL )
        this->value = MAX_LL;
    if ( this->value < MIN_LL )
        this->value = MIN_LL;

    if ( this->value == initialValue )
        return false;

    return true;
}
