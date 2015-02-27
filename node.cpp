#include <math.h>
#include <stdio.h>
#include "node.h"

#define _USE_MATH_DEFINES

int VariableNode::varCount = 0;
int CheckNode::checkCount = 0;

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
    messages.push_back( 0.0 );
    degree++;
}

void Node::PopReference()
{
    references.pop_back();
    messages.pop_back();
    degree--;
}

void Node::InsertReference( Node *ref, int index )
{
    references.insert( references.begin() + index, ref );
    messages.insert( messages.begin() + index, 0.0 );
    degree++;
}

void Node::RemoveReference( int index )
{
    references.erase( references.begin() + index );
    messages.erase( messages.begin() + index );
    degree--;
}

double Node::GetMessage( Node *ref )
{
    unsigned i;

    for ( i = 0; i < references.size(); i++ ) {
        if ( references[i] == ref )
            return messages[i];
    }

    //Shouldn't be here...maybe raise assertion
    return 0.0;
}

VariableNode::VariableNode() :
    Node()
{
    index = varCount;
    varCount++;
}

bool VariableNode::Update()
{
    int i;
    bool updated = false;

    //Update messages to check nodes
    for ( i = 0; i < degree; i++ )
        updated |= Update( i );

    //Update my believed value
    for ( i = 0; i < degree; i++ )
        //value += references[i]->messages[index];
        value += references[i]->GetMessage( this );

    return updated;
}

bool VariableNode::Update( int i )
{
    double initialValue;
    double sum;
    int j;

    initialValue = value;
    sum = initialValue;

    for ( j = 0; j < degree; j++ ) {
        if ( j == i )
            continue;
        //sum += references[j]->messages[index];
        sum += references[j]->GetMessage( this );
    }

    if (sum > MAX_LL)
        sum = MAX_LL;
    else if (sum < MIN_LL)
        sum = MIN_LL;


    messages[i] = sum;
    if( sum == initialValue)
        return false;
    else
        return true;
}

void VariableNode::SetValueFromReal( double real_value )
{
    int i;

    value = calc_ll( real_value, sigma );

    for ( i = 0; i < degree; i++ )
        messages[i] = value;
}

void VariableNode::SetValueFromLL( double ll )
{
    int i;

    value = ll;

    for ( i = 0; i < degree; i++ )
        messages[i] = value;
}

int VariableNode::GetHardValue()
{
    if ( value >= 0 )
        return 0;
    else    
        return 1;
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
    return -2 * val / (sigma * sigma);
}

void VariableNode::Debug()
{
    int i;

    printf( "Variable node %i\n", index );
    printf( "----------------\n" );

    printf("Value: %f\n", value);
    printf("Messages: \n");

    for ( i = 0; i < degree; i++ )
        printf( "  %f\n", messages[i] );

    printf("\n");
}

/*
double VariableNode::q_func( double x, double y, double sigma )
{
    double norm, expon;
    norm = 1 / ( sqrt(2 * M_PI) * sigma );
    expon = ( -1 * (y - x) * (y - x) / (2*sigma*sigma) );
    return (double) norm * exp( expon );
}
*/

CheckNode::CheckNode() :
    Node()
{
    index = checkCount;
    checkCount++;
}

bool CheckNode::Update()
{
    int i;
    bool updated = false;

    for ( i = 0; i < degree; i++ )
        updated |= Update( i );

    return updated;
}

bool CheckNode::Update( int i ) 
{
    double initialValue;
    double tmpVal;
    double product;
    int j;

    initialValue = messages[i];
    product = 1;

    for ( j = 0; j < degree; j++ ) {
        if ( i == j )
            continue;

        //tmpVal = references[j]->messages[index];
        tmpVal = references[j]->GetMessage( this );
        product *= tanh( tmpVal / 2 );
    }

    messages[i] = 2*atanh(product);

    if ( messages[i] > MAX_LL )
        messages[i] = MAX_LL;
    if ( messages[i] < MIN_LL )
        messages[i] = MIN_LL;

    if ( messages[i] == initialValue )
        return false;

    return true;
}

void CheckNode::Debug()
{
    int i;

    printf( "Check node %i\n", index );
    printf( "----------------\n" );

    printf("Messages: \n");

    for ( i = 0; i < degree; i++ )
        printf( "  %f\n", messages[i] );

    printf("\n");
}
