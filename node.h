#include <vector>

#define MAX_LL 100000
#define MIN_LL -100000

class Node
{
    public:
        Node();
        //~Node();
        Node( const Node& other );
        Node& operator=( const Node& rhs ); 


        virtual bool Update() = 0;
        
        void PushReference( Node *ref );
        void PopReference();
        void InsertReference( Node *ref, int index );
        void RemoveReference( int index );
        int GetDegree();

        double value;

    protected:
        int degree;
        std::vector<Node *> references;

};

class VariableNode : public Node
{
    public:
        bool Update();
        void SetValueFromReal( double real_value );
        void SetValueFromLL( double ll );
        int GetHardValue();
        void SetSigma( double dSigma );
        double GetSigma();

    private:
        double calc_ll( double val, double sigma );
        //double q_func( double x, double y, double sigma );
        double sigma;
};

class CheckNode : public Node
{
    public:
        bool Update();
};
