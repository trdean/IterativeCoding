#include <stdio.h>

#include "graph.h"

int main()
{
    int i;

    VariableNode *var1 = new VariableNode();
    VariableNode *var2 = new VariableNode();
    VariableNode *var3 = new VariableNode();

    CheckNode *check = new CheckNode();

    var1->PushReference( check );
    var2->PushReference( check );
    var3->PushReference( check );

    check->PushReference( var1 );
    check->PushReference( var2 );
    check->PushReference( var3 );

    var1->SetSigma( 1.0 );
    var2->SetSigma( 1.0 );
    var3->SetSigma( 1.0 );

    var1->SetValueFromReal( -1.0 );
    var2->SetValueFromReal( -1.2 );
    var3->SetValueFromReal( 0.1 );

    printf( "Value 1: %f\n", var1->value );
    printf( "Value 2: %f\n", var2->value );
    printf( "Value 3: %f\n", var3->value );

    printf( "Hard 1: %d\n", var1->GetHardValue() );
    printf( "Hard 2: %d\n", var2->GetHardValue() );
    printf( "Hard 3: %d\n\n", var3->GetHardValue() );

    for( i = 0; i < 5; i++) {
        printf( "Round %d\n", i+1 );
        printf( "-------------------------\n\n" );
        check->Update();

        printf( "Check value: %f\n\n", check->value );

        var1->Update();
        var2->Update();
        var3->Update();

        printf( "After updating variables:\n" );

        printf( "Value 1: %f\n", var1->value );
        printf( "Value 2: %f\n", var2->value );
        printf( "Value 3: %f\n", var3->value );

        printf( "Hard 1: %d\n", var1->GetHardValue() );
        printf( "Hard 2: %d\n", var2->GetHardValue() );
        printf( "Hard 3: %d\n\n\n", var3->GetHardValue() );
    }
}
