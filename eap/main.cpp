#include <cassert>
#include <iostream>

namespace eap {

struct Expr {
    enum class Type {
        UNDEFINED = 0,
        LITERAL,
        BINARY
    };

    Expr() = default;
    Expr(Type const type)
        : m_type{ type }
    {
    }

    Type type() const noexcept
    {
        return m_type;
    }

    Type m_type{ Type::UNDEFINED };
};

struct LiteralExpr : public Expr {
    explicit LiteralExpr(int const value)
        : Expr{ Type::LITERAL }
        , m_value{ value }
    {
    }

    int m_value{ 0 };
};

struct BinaryExpression : public Expr {
    explicit BinaryExpression(Expr* left, char const op, Expr* right)
        : Expr{ Type::BINARY }
        , m_left{ left }
        , m_right{ right }
        , m_op{ op }
    {
    }

    Expr* m_left{ nullptr };
    Expr* m_right{ nullptr };
    char m_op{ ' ' };
};

class Interpreter {
public:
    Interpreter() = default;
    ~Interpreter() = default;

    void execute(Expr* expr)
    {
        switch (expr->type())
        {
        case Expr::Type::LITERAL:
            break;
        case Expr::Type::BINARY:
            execute_binary_expr(reinterpret_cast<BinaryExpression*>(expr));
            break;
        case Expr::Type::UNDEFINED:
        default:
            break;
        }
    }

private:
    void execute_binary_expr(BinaryExpression* expr)
    {
        Expr* left = expr->m_left;
        Expr* right = expr->m_right;
        int left_operand = 0;
        int right_operand = 0;
        switch (expr->m_op)
        {
        case '+': {
            switch (left->type())
            {
            case Expr::Type::LITERAL:
                left_operand = reinterpret_cast<LiteralExpr*>(left)->m_value;
                break;
            default:
                assert(false);
                break;
            }

            switch (right->type())
            {
            case Expr::Type::LITERAL:
                right_operand = reinterpret_cast<LiteralExpr*>(right)->m_value;
                break;
            default:
                assert(false);
                break;
            }

            LiteralExpr res{ left_operand + right_operand };
            printf("%d", res.m_value);
        }
        break;

        case '-': {
            switch (left->type())
            {
            case Expr::Type::LITERAL:
                left_operand = reinterpret_cast<LiteralExpr*>(left)->m_value;
                break;
            default:
                assert(false);
                break;
            }

            switch (right->type())
            {
            case Expr::Type::LITERAL:
                right_operand = reinterpret_cast<LiteralExpr*>(right)->m_value;
                break;
            default:
                assert(false);
                break;
            }

            LiteralExpr res{ left_operand - right_operand };
            printf("%d", res.m_value);
        }
        break;

        default:
            assert(false);
            break;
        }
    }
};

} // namespace eap

int main(int argc, char** argv)
{

    // 5 + 7
    {
        eap::LiteralExpr left{ 5 };
        eap::LiteralExpr right{ 7 };
        char op = '+';
        auto binary_expr = eap::BinaryExpression{ &left, op, &right };

        eap::Interpreter interpreter;
        interpreter.execute(&binary_expr);
        printf("\n");
    }

    return 0;
}
