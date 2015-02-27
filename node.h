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

        virtual void Update() = 0;
        virtual void Debug() = 0;
        
        void PushReference( Node *ref );
        void PopReference();
        void InsertReference( Node *ref, int index );
        void RemoveReference( int index );
        double GetMessage( Node *ref );
        int GetDegree();

        double value;

    protected:
        int index;
        int degree;
        std::vector<Node *> references;
        std::vector<double> messages;

};

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
