#include <math.h>
#include "node.h"

#define _USE_MATH_DEFINES

Node::Node() : 
    value ( 0 )
{

}

Node::Node( const Node& other )
{
    int i;

    value = other.value;

    for (i = 0; i < degree; i++)
        PushReference( other.references[i] );
}

Node& Node::operator=( const Node& rhs )
{
    int i;
    this->value = rhs.value;

    for (i = 0; i < rhs.degree; i++)
        this->PushReference( rhs.references[i] );

    return *this;
}

void Node::PushReference( Node *ref )
{
    references.push_back( ref );
    degree++;
}

void Node::PopReference()
{
    references.pop_back();
    degree--;
}

void Node::InsertReference( Node *ref, int index )
{
    references.insert( references.begin() + index, ref );
    degree++;
}

void Node::RemoveReference( int index )
{
    references.erase( references.begin() + index );
    degree--;
}

bool VariableNode::Update()
{
    double initialValue;
    double sum;
    int i;

    initialValue = value;
    sum = initialValue;

    for ( i = 0; i < degree; i++ ) {
        //sum += fabs ( references[i]->value );
        sum += references[i]->value;
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

void VariableNode::SetValueFromReal( double real_value )
{
    value = calc_ll( real_value, sigma );
}

void VariableNode::SetValueFromLL( double ll )
{
    value = ll;
}

int VariableNode::GetHardValue()
{
    if ( value >= 0 )
        return 1;
    else    
        return 0;
}

void VariableNode::SetSigma( double dSigma )
{
    sigma = dSigma;
}

double VariableNode::GetSigma()
{
    return sigma;
}

double VariableNode::calc_ll( double val, double sigma )
{
    if ( val == 0 )
        return 0;

    return log( q_func( val, 1, sigma ) / q_func( val, -1, sigma ) );
}

double VariableNode::q_func( double x, double y, double sigma )
{
    double norm, expon;
    norm = 1 / sqrt(2 * M_PI) * sigma;
    expon = ( -1 * (y - x) * (y -x ) / (2*sigma*sigma) );
    return (double) norm * exp( expon );
}

bool CheckNode::Update() 
{
    double initialValue;
    double tmpVal;
    double product;
    int i;

    initialValue = value;
    product = 1;

    for ( i = 0; i < degree; i++ ) {
        tmpVal = references[i]->value;
        if ( tmpVal != 0 )
            product *= tanh( tmpVal / 2 );
        //    product *= tanh( fabs(tmpVal) / 2 );
        //if ( tmpVal < 0 )
        //    product *= -1;
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
