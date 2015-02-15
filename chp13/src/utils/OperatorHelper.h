#ifndef OPERFATORHELPER_20120416_H_
#define  OPERFATORHELPER_20120416_H_


#define OPERATOR_DEF(Type, tag)\
    inline bool operator tag (const Type& right) const \
    { \
        return compare(right) tag 0;\
    }\
    /* OPERATOR_DEF END */

#define OPERATOR_DEF_LIST(Type)\
    OPERATOR_DEF(Type, >);\
    OPERATOR_DEF(Type, >=);\
    OPERATOR_DEF(Type, ==);\
    OPERATOR_DEF(Type, <);\
    OPERATOR_DEF(Type, <=);\
    OPERATOR_DEF(Type, !=);\
    /* OPERATOR_DEF_LIST */

#endif //OPERFATORHELPER_20120416_H_