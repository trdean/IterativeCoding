#pragma once

class LinearCode
{
    public:
        LinearCode() { }
        virtual ~LinearCode() { }

        virtual void SetVariablesFromLL( double *ll ) = 0;
        virtual bool Decode( int maxIterations ) = 0;
        virtual void DecodeRound() = 0;
        virtual void GetValues( double *values ) = 0;
        virtual int GetVariableLength() const = 0;
        virtual bool CheckSyndrome() = 0;
};
