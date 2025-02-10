#ifndef _JOJ_NODE_TYPE_H
#define _JOJ_NODE_TYPE_H

namespace joj
{
    enum class NodeType
    {
        UNKNOWN,
        STATEMENT,
        EXPR,
        CONSTANT,
        IDENTIFIER,
        ASSIGNMENT,
        VAR_DECL,
        VAR_INIT,
        ACCESS,
        LOG,
        REL,
        ARI,
        UNARY,
        SEQ,
        IF_STATEMENT,
        WHILE_STATEMENT,
        LOOP_STATEMENT
    };

    inline const char* nodetype_to_string(const NodeType ntype)
    {
        switch (ntype)
        {
        case NodeType::UNKNOWN:         return "UNKNOWN";
        case NodeType::STATEMENT:       return "STATEMENT";
        case NodeType::EXPR:            return "EXPR";
        case NodeType::CONSTANT:        return "CONSTANT";
        case NodeType::IDENTIFIER:      return "IDENTIFIER";
        case NodeType::ASSIGNMENT:      return "ASSIGNMENT";
        case NodeType::VAR_DECL:        return "VAR_DECL";
        case NodeType::VAR_INIT:        return "VAR_INIT";
        case NodeType::ACCESS:          return "ACCESS";
        case NodeType::LOG:             return "LOG";
        case NodeType::REL:             return "REL";
        case NodeType::ARI:             return "ARI";
        case NodeType::UNARY:           return "UNARY";
        case NodeType::SEQ:             return "SEQ";
        case NodeType::IF_STATEMENT:    return "IF_STATEMENT";
        case NodeType::WHILE_STATEMENT: return "WHILE_STATEMENT";
        case NodeType::LOOP_STATEMENT:  return "LOOP_STATEMENT";
        }
    }
}

#endif // _JOJ_NODE_TYPE_H